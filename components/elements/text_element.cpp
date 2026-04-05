#include "text_element.h"

namespace esphome::elements {

static const char* const TEXT_ELEMENT_TAG = "elements.text";

using display::TextAlign;

const int TEXT_ALIGN_X = (int)TextAlign::LEFT |
                         (int)TextAlign::CENTER_HORIZONTAL |
                         (int)TextAlign::RIGHT;
const int TEXT_ALIGN_Y = (int)TextAlign::TOP | (int)TextAlign::CENTER_VERTICAL |
                         (int)TextAlign::BASELINE | (int)TextAlign::BOTTOM;

void TextElement::draw(display::Display& display) {
  // Update the text, if necessary.
  std::string new_text = get_text();
  if (text_ != new_text) {
    text_ = new_text;
    request_measurement_ = true;
  }

  // If there is no text, signal completion once and skip drawing.
  if (text_.empty()) {
    if (!complete_) {
      complete_ = true;
      on_complete();
    }
    return;
  }

  // Compute the placement of the text.
  Point<int> point = anchor_.get(Point<int>::from_extent(display));

  // Measure the text, if necessary, using the un-scrolled base point.
  if (request_measurement_) {
    display.get_text_bounds(point.x, point.y, text_.c_str(), font_, align_,
                            &measured_position_.x, &measured_position_.y,
                            &measured_extent_.x, &measured_extent_.y);
    request_measurement_ = false;
  }

  // Update the placement, if we scroll.
  if (scroll_mode_ != ScrollMode::NONE) {
    uint32_t now = millis();
    if (last_scroll_time_ > 0) {
      scroll_offset_ += (now - last_scroll_time_) * scroll_speed_ / 1000.0f;
    }
    last_scroll_time_ = now;
    int dist = static_cast<int>(scroll_offset_);
    Point<int> delta = {
        .x = (scroll_mode_ == ScrollMode::RIGHT_TO_LEFT ? 1 : 0) -
             (scroll_mode_ == ScrollMode::LEFT_TO_RIGHT ? 1 : 0),
        .y = (scroll_mode_ == ScrollMode::TOP_TO_BOTTOM ? 1 : 0) -
             (scroll_mode_ == ScrollMode::BOTTOM_TO_TOP ? 1 : 0),
    };
    point.x += delta.x * dist;
    point.y += delta.y * dist;
    Point<int> current = {
        .x = measured_position_.x + delta.x * dist,
        .y = measured_position_.y + delta.y * dist,
    };
    if (current.x + measured_extent_.x < 0 || current.x > display.get_width() ||
        current.y + measured_extent_.y < 0 ||
        current.y > display.get_height()) {
      scroll_offset_ = 0.0f;
      complete_ = false;
      on_complete();
      return;
    }
  }

  // Draw the text.
  display.print(point.x, point.y, font_, color_, align_, text_.c_str(),
                background_color_);
}

void TextElement::on_show() {
  switch (scroll_mode_) {
    case ScrollMode::NONE:
      break;
    case ScrollMode::LEFT_TO_RIGHT:
      anchor_.fraction.x = 1.0f;
      align_ = TextAlign((int(align_) & ~TEXT_ALIGN_X) | int(TextAlign::LEFT));
      break;
    case ScrollMode::RIGHT_TO_LEFT:
      anchor_.fraction.x = 0.0f;
      align_ = TextAlign((int(align_) & ~TEXT_ALIGN_X) | int(TextAlign::RIGHT));
      break;
    case ScrollMode::BOTTOM_TO_TOP:
      anchor_.fraction.y = 1.0f;
      align_ = TextAlign((int(align_) & ~TEXT_ALIGN_Y) | int(TextAlign::TOP));
      break;
    case ScrollMode::TOP_TO_BOTTOM:
      anchor_.fraction.y = 0.0f;
      align_ =
          TextAlign((int(align_) & ~TEXT_ALIGN_Y) | int(TextAlign::BOTTOM));
      break;
  }
  scroll_offset_ = 0.0f;
  last_scroll_time_ = millis();
  complete_ = false;
  Element::on_show();
}

void TextElement::on_hide() {
  scroll_offset_ = 0.0f;
  last_scroll_time_ = 0;
  complete_ = false;
  Element::on_hide();
}

void TextElement::on_complete() {
  ESP_LOGI(TEXT_ELEMENT_TAG, "Triggering `on_complete` for %s (%p)",
           get_type_name(), this);
  on_complete_callbacks_.call(this);
}

bool TextElement::is_active() { return !get_text().empty(); }

std::string StaticTextElement::get_text() { return text_; }

std::string DynamicTextElement::get_text() { return lambda_(*this); }

std::string TimeTextElement::get_text() {
  return time_->now().strftime(format_);
}

}  // namespace esphome::elements
