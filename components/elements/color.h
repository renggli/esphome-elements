#pragma once

#include <algorithm>
#include <cmath>

#include "esphome/core/color.h"

namespace esphome::elements {

// Precomputed color lookup table.
class ColorScheme {
 public:
  ColorScheme(const Color* colors, const size_t size)
      : colors_(colors), size_(size) {}

  Color get_color(float p) const {
    const size_t index = std::lround(p * (size_ - 1));
    return colors_[std::clamp<size_t>(index, 0, size_ - 1)];
  }

 private:
  const Color* colors_;
  const size_t size_;
};

}  // namespace esphome::elements
