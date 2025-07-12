#include "text_element.h"

namespace esphome::elements {

static const char *const TEXT_ELEMENT_TAG = "elements.text";

using display::TextAlign;

const int TEXT_ALIGN_X = (int) TextAlign::LEFT | (int) TextAlign::CENTER_HORIZONTAL | (int) TextAlign::RIGHT;
const int TEXT_ALIGN_Y =
    (int) TextAlign::TOP | (int) TextAlign::CENTER_VERTICAL | (int) TextAlign::BASELINE | (int) TextAlign::BOTTOM;

void TextElement::draw(display::Display &display) {
  // Update the text, if necessary.
  std::string new_text = get_text();
  if (text_ != new_text) {
    text_ = new_text;
    request_measurement_ = true;
  }

  // Skip, if no text.
  if (text_.empty()) {
    return;
  }

  // Compute the placement of the text.
  Point<int> point = anchor_.get(Point<int>::from_extent(display));

  // Update the placement, if we scroll.
  if (scroll_mode_ != ScrollMode::NONE) {
    scroll_offset_ += get_component().get_delta_ms() * scroll_speed_ / 1000.0;
    switch (scroll_mode_) {
      case ScrollMode::NONE:
        break;
      case ScrollMode::LEFT_TO_RIGHT:
        point.x -= scroll_offset_;
        break;
      case ScrollMode::RIGHT_TO_LEFT:
        point.x += scroll_offset_;
        break;
      case ScrollMode::BOTTOM_TO_TOP:
        point.y -= scroll_offset_;
        break;
      case ScrollMode::TOP_TO_BOTTOM:
        point.y += scroll_offset_;
        break;
    }
    request_measurement_ = true;
  }

  // Measure the text, if necessary.
  if (request_measurement_) {
    display.get_text_bounds(point.x, point.y, text_.c_str(), font_, align_, &bounds_x_, &bounds_y_, &bounds_w_,
                            &bounds_h_);
    request_measurement_ = false;
  }

  // Reset the scrolling, if necessary.
  if (scroll_mode_ != ScrollMode::NONE) {
    if (bounds_x_ + bounds_w_ < 0 || display.get_width() < bounds_x_ || bounds_y_ + bounds_h_ < 0 ||
        display.get_height() < bounds_y_) {
      scroll_offset_ = 0.0;
      on_next();
    }
  }

  // Draw the text.
  display.print(point.x, point.y, font_, color_, align_, text_.c_str(), background_color_);
}

void TextElement::on_show() {
  switch (scroll_mode_) {
    case ScrollMode::NONE:
      break;
    case ScrollMode::LEFT_TO_RIGHT:
      anchor_.fraction.x = 1.0;
      align_ = TextAlign((int(align_) & ~TEXT_ALIGN_X) | int(TextAlign::LEFT));
      break;
    case ScrollMode::RIGHT_TO_LEFT:
      anchor_.fraction.x = 0.0;
      align_ = TextAlign((int(align_) & ~TEXT_ALIGN_X) | int(TextAlign::RIGHT));
      break;
    case ScrollMode::BOTTOM_TO_TOP:
      anchor_.fraction.y = 1.0;
      align_ = TextAlign((int(align_) & ~TEXT_ALIGN_Y) | int(TextAlign::TOP));
      break;
    case ScrollMode::TOP_TO_BOTTOM:
      anchor_.fraction.y = 0.0;
      align_ = TextAlign((int(align_) & ~TEXT_ALIGN_Y) | int(TextAlign::BOTTOM));
      break;
  }
  scroll_offset_ = 0.0;
}

bool TextElement::is_active() { return !get_text().empty(); };

std::string StaticTextElement::get_text() { return text_; }

std::string DynamicTextElement::get_text() { return lambda_(*this); }

std::string TimeTextElement::get_text() { return time_->now().strftime(format_); }

}  // namespace esphome::elements
