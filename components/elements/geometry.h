#pragma once

namespace esphome::elements {

/// A point in the x-y-plane.
template<typename T> struct Point {
  T x = 0;
  T y = 0;

  // Constructs an extent from the width and height of another object.
  template<typename C> static Point<T> from_extent(C &object) {
    return {.x = object.get_width(), .y = object.get_height()};
  }
};

/// An anchor with both absolute relative offsets.
struct Anchor {
  Point<int> offset = {};
  Point<float> fraction = {0.5f, 0.5f};

  Point<int> get(const Point<int> &extent) const {
    return {
        .x = offset.x + (int) (fraction.x * extent.x),
        .y = offset.y + (int) (fraction.y * extent.y),
    };
  }
};

}  // namespace esphome::elements
