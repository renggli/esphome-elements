#include "text_element.h"

#include "esphome/core/log.h"

namespace esphome::elements {

void TextElement::draw(const Context& context, display::Display& display) {
  // Compute the placement of the text.
  int x = position_x_ * display.get_width();
  int y = position_y_ * display.get_height();

  // Update the text, if necessary.
  if (lambda_.has_value() &&
      (!text_.has_value() ||
       last_update_ms_ + update_every_ms_ <= context.current_ms)) {
    std::string text = (*lambda_)(context);
    if (!text_.has_value() || *text_ != text) {
      text_ = text;
      request_measurement_ = true;
    }
    last_update_ms_ = context.current_ms;
  }

  // Measure the text, if necessary.
  if (text_.has_value() && request_measurement_) {
    display.get_text_bounds(x, y, text_->c_str(), font_, align_, &bounds_x_,
                            &bounds_y_, &bounds_w_, &bounds_h_);
    request_measurement_ = false;
  }

  // Draw the text.
  if (text_.has_value()) {
    display.print(x, y, font_, color_, align_, text_->c_str(),
                  background_color_);
  }
}

void TextElement::on_show(const Context& context) {
  // Clear the cached text, if we have a lambda.
  if (lambda_.has_value()) text_ = nullopt;
}

}  // namespace esphome::elements
