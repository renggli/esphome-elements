#include "text_element.h"

#include "esphome/core/log.h"

namespace esphome::elements {

const char* TEXT_ELEMENT_TAG = "elements.text";

using display::TextAlign;

const int TEXT_ALIGN_X = (int)TextAlign::LEFT |
                         (int)TextAlign::CENTER_HORIZONTAL |
                         (int)TextAlign::RIGHT;
const int TEXT_ALIGN_Y = (int)TextAlign::TOP | (int)TextAlign::CENTER_VERTICAL |
                         (int)TextAlign::BASELINE | (int)TextAlign::BOTTOM;

void TextElement::set_text(std::string text) {
  text_ = text;
  lambda_ = nullopt;
  scroll_offset_ = 0.0;
  request_measurement_ = true;
}

void TextElement::set_lambda(
    std::function<std::string(const Context&)> lambda) {
  text_ = nullopt;
  lambda_ = lambda;
  scroll_offset_ = 0.0;
  request_measurement_ = true;
}

void TextElement::draw(display::Display& display) {
  // Update the text, if necessary.
  if (lambda_.has_value()) {
    if (!text_.has_value() || update_timer_.check(get_context().current_ms)) {
      std::string text = (*lambda_)(get_context());
      if (!text_.has_value() || *text_ != text) {
        ESP_LOGD(TEXT_ELEMENT_TAG, "Text has changed: %s", text.c_str());
        text_ = text;
        request_measurement_ = true;
      }
      last_update_ms_ = get_context().current_ms;
    }
  }

  // Only continue if we have text.
  if (!text_.has_value()) {
    ESP_LOGW(TEXT_ELEMENT_TAG, "Missing text");
    return;
  }

  // Compute the placement of the text.
  int x = position_x_ * display.get_width();
  int y = position_y_ * display.get_height();

  // Update the placement, if we scroll.
  if (scroll_mode_ != ScrollMode::NONE) {
    scroll_offset_ += get_context().delta_ms * scroll_speed_ / 1000.0;
    switch (scroll_mode_) {
      case ScrollMode::LEFT_TO_RIGHT:
        x -= scroll_offset_;
        break;
      case ScrollMode::RIGHT_TO_LEFT:
        x += scroll_offset_;
        break;
      case ScrollMode::BOTTOM_TO_TOP:
        y -= scroll_offset_;
        break;
      case ScrollMode::TOP_TO_BOTTOM:
        y += scroll_offset_;
        break;
    }
    request_measurement_ = true;
  }

  // Measure the text, if necessary.
  if (request_measurement_) {
    display.get_text_bounds(x, y, text_->c_str(), font_, align_, &bounds_x_,
                            &bounds_y_, &bounds_w_, &bounds_h_);
    request_measurement_ = false;
  }

  // Reset the scrolling, if necessary.
  if (scroll_mode_ != ScrollMode::NONE) {
    if (bounds_x_ + bounds_w_ < 0 || display.get_width() < bounds_x_ ||
        bounds_y_ + bounds_h_ < 0 || display.get_height() < bounds_y_) {
      scroll_offset_ = 0.0;
      on_next();
    }
  }

  // Draw the text.
  display.print(x, y, font_, color_, align_, text_->c_str(), background_color_);
}

void TextElement::on_show() {
  // Clear the cached text, if we have a lambda.
  if (lambda_.has_value()) text_ = nullopt;

  // Reset drawing params, if we scroll.
  if (scroll_mode_ != ScrollMode::NONE) {
    switch (scroll_mode_) {
      case ScrollMode::LEFT_TO_RIGHT:
        position_x_ = 1.0;
        align_ =
            (TextAlign)(((int)align_ & ~TEXT_ALIGN_X) | (int)TextAlign::LEFT);
        break;
      case ScrollMode::RIGHT_TO_LEFT:
        position_x_ = 0.0;
        align_ =
            (TextAlign)(((int)align_ & ~TEXT_ALIGN_X) | (int)TextAlign::RIGHT);
        break;
      case ScrollMode::BOTTOM_TO_TOP:
        position_y_ = 1.0;
        align_ =
            (TextAlign)(((int)align_ & ~TEXT_ALIGN_Y) | (int)TextAlign::TOP);
        break;
      case ScrollMode::TOP_TO_BOTTOM:
        position_y_ = 0.0;
        align_ =
            (TextAlign)(((int)align_ & ~TEXT_ALIGN_Y) | (int)TextAlign::BOTTOM);
        break;
    }
    scroll_offset_ = 0.0;
  }
}

}  // namespace esphome::elements
