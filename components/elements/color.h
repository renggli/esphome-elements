#pragma once

#include <initializer_list>
#include <vector>

#include "esphome/core/color.h"

namespace esphome::elements {

// Precomputed lookup table of ESPHome Colors.
class ColorScheme {
 public:
  void set_colors(std::initializer_list<Color> colors) { lut_ = colors; }

  Color get_color(float p) const {
    const auto n = static_cast<uint32_t>(lut_.size());
    if (n == 0) return Color::BLACK;
    const auto idx =
        static_cast<uint32_t>(p * static_cast<float>(n - 1) + 0.5f);
    return lut_[idx < n ? idx : n - 1];
  }

 private:
  std::vector<Color> lut_;
};

}  // namespace esphome::elements
