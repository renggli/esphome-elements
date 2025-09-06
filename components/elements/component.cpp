#include "component.h"

#include "element.h"

#include <cstdint>

namespace esphome::elements {

static const char *const ELEMENT_COMPONENT_TAG = "elements.component";

void ElementComponent::setup() {
  // If we have a display, setup automatic drawing. Otherwise users need to
  // manually call `Elements::draw(Display&)` with the desired display as
  // argument.
  if (display_ != nullptr) {
    display::display_writer_t writer = [this](display::Display &it) {
      display_->clear();
      draw(*display_);
    };
    display_->show_page(new display::DisplayPage(writer));
  } else {
    ESP_LOGW(ELEMENT_COMPONENT_TAG, "No display setup, drawing needs to be manually called.");
  }

#ifdef USE_WEBSERVER
  // If we have a webserver, register the element display as an image.
  auto *web_server_base = web_server_base::global_web_server_base;
  if (web_server_base != nullptr) {
    web_server_base->add_handler(new ElementComponentHandler(this));
  }
#endif
}

void ElementComponent::dump_config() {
  ESP_LOGCONFIG(ELEMENT_COMPONENT_TAG, "Elements Component");
  if (root_ != nullptr) {
    root_->dump_config(0);
  }
}

void ElementComponent::set_root(Element *root) {
  root_ = root;
  request_on_show_ = true;
}

void ElementComponent::draw(display::Display &display) {
  // Check the preconditions.
  if (root_ == nullptr) {
    ESP_LOGE(ELEMENT_COMPONENT_TAG, "Unable to draw without a root element.");
    return;
  }

  // Check if the display buffer has the right size.
  canvas_.resize(display.get_width(), display.get_height());
  if (!canvas_.is_valid()) {
    ESP_LOGE(ELEMENT_COMPONENT_TAG, "Unable to allocate display buffer.");
    return;
  }

  // Update the time.
  uint32_t now = millis();
  delta_ms_ = now - current_ms_;
  current_ms_ = now;

  // Call on-show the first time.
  if (request_on_show_) {
    root_->on_show();
    request_on_show_ = false;
  }

  // Draw the update.
  canvas_.clear();
  root_->draw(canvas_);
  canvas_.copy_to(display);
}

#ifdef USE_WEBSERVER
bool ElementComponentHandler::canHandle(AsyncWebServerRequest *request) const {
  if (request->method() == HTTP_GET) {
    if (request->url() == "/icon.bmp") {
      return true;
    }
  }
  return false;
}

// BMP File Header (14 bytes)
#pragma pack(push, 1)
typedef struct {
  uint8_t bfType[2];     // "BM"
  uint32_t bfSize;       // File size in bytes
  uint16_t bfReserved1;  // Reserved, should be 0
  uint16_t bfReserved2;  // Reserved, should be 0
  uint32_t bfOffBits;    // Offset to pixel data
} BMPFileHeader;

// BMP Info Header (40 bytes for BITMAPINFOHEADER)
typedef struct {
  uint32_t biSize;          // Size of this header (40 bytes)
  int32_t biWidth;          // Image width in pixels
  int32_t biHeight;         // Image height in pixels
  uint16_t biPlanes;        // Number of color planes (always 1)
  uint16_t biBitCount;      // Bits per pixel (24 for true color)
  uint32_t biCompression;   // Compression method (0 for BI_RGB)
  uint32_t biSizeImage;     // Image size in bytes (can be 0 for uncompressed)
  int32_t biXPelsPerMeter;  // Horizontal resolution (pixels/meter)
  int32_t biYPelsPerMeter;  // Vertical resolution (pixels/meter)
  uint32_t biClrUsed;       // Number of colors in palette (0 for 24-bit)
  uint32_t biClrImportant;  // Number of important colors (0 for all)
} BMPInfoHeader;
#pragma pack(pop)

void ElementComponentHandler::handleRequest(AsyncWebServerRequest *request) {
  // Fetch display dimensions of the component.
  int width = 0;
  int height = 0;

  display::Display *display_component = component_->display_;
  if (display_component != nullptr) {
    width = display_component->get_width();
    height = display_component->get_height();
  }

  if (request->hasParam("width")) {
    width = static_cast<int>(parse_number<uint32_t>(request->getParam("width")->value().c_str()).value_or(0));
  }
  if (request->hasParam("height")) {
    height = static_cast<int>(parse_number<uint32_t>(request->getParam("height")->value().c_str()).value_or(0));
  }

  if (height <= 0 || width <= 0) {
    request->send(500, "text/plain", "Invalid display dimensions.");
    return;
  }

  // Create a display buffer to draw the current state on.
  ImageDisplay display_buffer(width, height);
  component_->draw(display_buffer);

  if (display_buffer.is_valid()) {
    request->send(500, "text/plain", "Unable to allocate display buffer.");
  }

  // Calculate BMP properties
  int bytes_per_pixel = 3;  // RGB (24-bit)
  int row_data_bytes = width * bytes_per_pixel;
  int row_padding_bytes = (4 - (row_data_bytes % 4)) % 4;
  int row_stride_bytes = row_data_bytes + row_padding_bytes;
  int image_data_size = row_stride_bytes * height;

  // Populate headers
  BMPFileHeader fileHeader = {
      .bfType = {'B', 'M'},
      .bfSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + image_data_size,
      .bfReserved1 = 0,
      .bfReserved2 = 0,
      .bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader),
  };
  BMPInfoHeader infoHeader = {
      .biSize = sizeof(BMPInfoHeader),
      .biWidth = width,
      .biHeight = height,
      .biPlanes = 1,
      .biBitCount = 24,
      .biCompression = 0,
      .biSizeImage = static_cast<uint32_t>(image_data_size),
      .biXPelsPerMeter = 0,
      .biYPelsPerMeter = 0,
      .biClrUsed = 0,
      .biClrImportant = 0,
  };

  // Allocate memory
  RAMAllocator<uint8_t> allocator;
  uint8_t *data = allocator.allocate(fileHeader.bfSize);
  if (!data) {
    request->send(500, "text/plain", "Unable to allocate response buffer.");
    return;
  }
  uint8_t *ptr = data;

  // Write headers
  memcpy(ptr, &fileHeader, sizeof(BMPFileHeader));
  ptr += sizeof(BMPFileHeader);
  memcpy(ptr, &infoHeader, sizeof(BMPInfoHeader));
  ptr += sizeof(BMPInfoHeader);

  // Write pixel data
  for (int y = height - 1; y >= 0; --y) {
    for (int x = 0; x < width; ++x) {
      Color color = display_buffer.get_pixel(x, y);
      *ptr++ = color.b;
      *ptr++ = color.g;
      *ptr++ = color.r;
    }
    for (int i = 0; i < row_padding_bytes; ++i) {
      *ptr++ = 0;
    }
  }

  // Server the data.
  AsyncWebServerResponse *response = request->beginResponse(200, "image/bmp", data, fileHeader.bfSize);
  request->send(response);

  // Free the memory.
  allocator.deallocate(data, fileHeader.bfSize);
}
#endif

}  // namespace esphome::elements
