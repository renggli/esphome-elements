#include "artsy_element.h"
#include "color.cpp"
#include "display.h"
#include <algorithm>
#include <cmath>

namespace esphome::elements {

static const float PI_F = (float) M_PI;
static const float TWO_PI_F = 2.0f * PI_F;

static uint32_t hash(uint32_t v) {
  v ^= v >> 16;
  v *= 0x85ebca6b;
  v ^= v >> 13;
  v *= 0xc2b2ae35;
  v ^= v >> 16;
  return v;
}

static float noise(int x, int y, uint32_t seed) {
  uint32_t val = hash(seed + (y * 10000) + x);
  return (val % 1000) / 1000.0f;
}

Color ArtsyElement::get_gradient_color_(float p) {
  if (color_scheme_ != nullptr) {
    return color_scheme_->get_color(std::clamp(p, 0.0f, 1.0f));
  }
  return Color(0, 0, 0);
}

void ArtsyElement::on_show() {}

void ArtsyElement::draw(display::Display &display) {
  uint32_t current_ms = get_component().get_current_ms();
  int width = display.get_width(), height = display.get_height();
  switch (this->pattern_) {
    case ArtsyPattern::METABALLS:
      this->draw_metaballs_(display, width, height, current_ms);
      break;
    case ArtsyPattern::AURORA:
      this->draw_aurora_(display, width, height, current_ms);
      break;
    case ArtsyPattern::KALEIDOSCOPE:
      this->draw_kaleidoscope_(display, width, height, current_ms);
      break;
    case ArtsyPattern::PLASMA:
      this->draw_plasma_(display, width, height, current_ms);
      break;
    case ArtsyPattern::RIPPLES:
      this->draw_ripples_(display, width, height, current_ms);
      break;
    case ArtsyPattern::SPIRAL:
      this->draw_spiral_(display, width, height, current_ms);
      break;
    case ArtsyPattern::VORONOI:
      this->draw_voronoi_(display, width, height, current_ms);
      break;
    case ArtsyPattern::INTERFERENCE:
      this->draw_interference_(display, width, height, current_ms);
      break;
    case ArtsyPattern::JULIA:
      this->draw_julia_(display, width, height, current_ms);
      break;
    case ArtsyPattern::MATRIX:
      this->draw_matrix_(display, width, height, current_ms);
      break;
    case ArtsyPattern::GRADIENT:
      this->draw_gradient_(display, width, height, current_ms);
      break;
    case ArtsyPattern::FIRE:
      this->draw_fire_(display, width, height, current_ms);
      break;
    case ArtsyPattern::TUNNEL:
      this->draw_tunnel_(display, width, height, current_ms);
      break;
    case ArtsyPattern::WAVE:
      this->draw_wave_(display, width, height, current_ms);
      break;
    case ArtsyPattern::STARS:
      this->draw_stars_(display, width, height, current_ms);
      break;
  }
}

void ArtsyElement::draw_metaballs_(display::Display &display, int width, int height, uint32_t current_ms) {
  float t = current_ms / 5000.0f * this->speed_;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float val = 0;
      for (int i = 0; i < 3; i++) {
        // Each ball has a unique base position, not all orbiting the same point
        float bx = 0.25f + 0.5f * noise(i, 5, 99);
        float by = 0.25f + 0.5f * noise(i, 6, 99);
        float cx = width * bx + std::sin(t * TWO_PI_F + i * 2.1f) * (width / 3.5f);
        float cy = height * by + std::cos(t * TWO_PI_F + i * 3.3f) * (height / 3.5f);
        float dx = x - cx;
        float dy = y - cy;
        float dist2 = dx * dx + dy * dy;
        val += 80.0f / (dist2 + 1.0f);
      }
      // Scale to 0..0.85 so the brightness formula never hits its second zero
      val = std::min(val / 5.5f, 0.85f);
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void ArtsyElement::draw_aurora_(display::Display &display, int width, int height, uint32_t current_ms) {
  float t = current_ms / 8000.0f * this->speed_;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float nx = x / (float) width;
      float ny = y / (float) height;
      // Three aurora bands at different heights, drifting independently
      float b1 = std::exp(-std::pow((ny - (0.25f + 0.12f * std::sin(nx * 4.0f + t * TWO_PI_F))) / 0.14f, 2.0f));
      float b2 =
          std::exp(-std::pow((ny - (0.50f + 0.10f * std::sin(nx * 3.0f - t * TWO_PI_F * 0.7f + 1.0f))) / 0.11f, 2.0f));
      float b3 =
          std::exp(-std::pow((ny - (0.72f + 0.08f * std::sin(nx * 5.0f + t * TWO_PI_F * 1.3f + 2.0f))) / 0.09f, 2.0f));
      float val = std::min(b1 + b2 * 0.7f + b3 * 0.5f, 1.0f);
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void ArtsyElement::draw_kaleidoscope_(display::Display &display, int width, int height, uint32_t current_ms) {
  float t = current_ms / 8000.0f * this->speed_;
  const int segments = 6;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float dx = x - width / 2.0f;
      float dy = y - height / 2.0f;
      float angle = std::atan2(dy, dx) + t * TWO_PI_F;
      float dist = std::sqrt(dx * dx + dy * dy);
      // Fold into one mirrored segment
      float seg_angle = std::fmod(std::abs(angle), TWO_PI_F / segments);
      if (seg_angle > TWO_PI_F / (2 * segments)) {
        seg_angle = TWO_PI_F / segments - seg_angle;
      }
      float tx = dist * std::cos(seg_angle);
      float ty = dist * std::sin(seg_angle);
      float val = std::sin(tx / 3.0f + t * TWO_PI_F) * std::cos(ty / 3.0f - t * TWO_PI_F * 0.7f);
      val = (val + 1.0f) / 2.0f;
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void ArtsyElement::draw_plasma_(display::Display &display, int width, int height, uint32_t current_ms) {
  float t = current_ms / 5000.0f * this->speed_;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float v1 = std::sin(x / 10.0f + t * TWO_PI_F);
      float v2 = std::sin(y / 8.0f + t * TWO_PI_F);
      float v3 = std::sin((x + y) / 12.0f + t * TWO_PI_F);
      float v4 = std::sin(
          std::sqrt((x - width / 2.0f) * (x - width / 2.0f) + (y - height / 2.0f) * (y - height / 2.0f)) / 8.0f +
          t * TWO_PI_F);
      float val = (v1 + v2 + v3 + v4) / 4.0f;
      val = (val + 1.0f) / 2.0f;
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void ArtsyElement::draw_ripples_(display::Display &display, int width, int height, uint32_t current_ms) {
  float t = current_ms / 5000.0f * this->speed_;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      // Two sources orbiting in opposite directions — their interference creates rich patterns
      float cx1 = width / 2.0f + std::sin(t * TWO_PI_F) * (width / 4.0f);
      float cy1 = height / 2.0f + std::cos(t * TWO_PI_F) * (height / 4.0f);
      float cx2 = width / 2.0f + std::sin(t * TWO_PI_F + PI_F) * (width / 4.0f);
      float cy2 = height / 2.0f + std::cos(t * TWO_PI_F * 0.7f) * (height / 4.0f);
      float d1 = std::sqrt((x - cx1) * (x - cx1) + (y - cy1) * (y - cy1));
      float d2 = std::sqrt((x - cx2) * (x - cx2) + (y - cy2) * (y - cy2));
      float v1 = std::sin(d1 / 2.0f - t * 4.0f * PI_F);
      float v2 = std::sin(d2 / 2.0f - t * 4.0f * PI_F + 0.8f);
      float val = (v1 + v2) / 2.0f;
      val = (val + 1.0f) / 2.0f;
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void ArtsyElement::draw_spiral_(display::Display &display, int width, int height, uint32_t current_ms) {
  float t = current_ms / 5000.0f * this->speed_;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float cx = width / 2.0f;
      float cy = height / 2.0f;
      float dx = x - cx;
      float dy = y - cy;
      float angle = std::atan2(dy, dx);
      if (angle < 0) {
        angle += TWO_PI_F;
      }
      float dist = std::sqrt(dx * dx + dy * dy);
      float val = std::fmod(angle / TWO_PI_F + dist / (width * 1.5f) + t, 1.0f);
      // Fade brightness from center out to give depth
      float fade = std::max(0.0f, 1.0f - dist / (std::min(cx, cy) * 1.2f));
      val = val * (0.3f + 0.7f * fade) + (1.0f - fade) * 0.1f;
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void ArtsyElement::draw_voronoi_(display::Display &display, int width, int height, uint32_t current_ms) {
  float t = current_ms / 5000.0f * this->speed_;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float min_dist = 1000.0f;
      for (int i = 0; i < 6; i++) {
        // Each seed has a unique, well-spread base position and independent orbit
        float base_x = noise(i, 0, 42);
        float base_y = noise(i, 1, 42);
        float orbit_r_x = width / 4.0f * (0.5f + noise(i, 2, 42));
        float orbit_r_y = height / 4.0f * (0.5f + noise(i, 3, 42));
        float phase = i * 1.0472f;  // 60° apart in phase (TWO_PI / 6)
        float px = width * base_x + std::sin(t * TWO_PI_F + phase) * orbit_r_x;
        float py = height * base_y + std::cos(t * TWO_PI_F + phase * 1.618f) * orbit_r_y;
        float dx = x - px;
        float dy = y - py;
        float dist = std::sqrt(dx * dx + dy * dy);
        min_dist = std::min(dist, min_dist);
      }
      // Sharper falloff to differentiate from metaballs: cell interiors are bright, borders are dark
      float val = 1.0f - std::min(std::pow(min_dist / (width / 2.5f), 1.5f), 1.0f);
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void ArtsyElement::draw_interference_(display::Display &display, int width, int height, uint32_t current_ms) {
  float t = current_ms / 5000.0f * this->speed_;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      // Both sources orbit slowly so the interference pattern rotates and morphs
      float cx1 = width * (0.5f + 0.3f * std::sin(t * TWO_PI_F));
      float cy1 = height * (0.5f + 0.3f * std::cos(t * TWO_PI_F * 0.7f));
      float cx2 = width * (0.5f - 0.3f * std::sin(t * TWO_PI_F + 1.0f));
      float cy2 = height * (0.5f - 0.3f * std::cos(t * TWO_PI_F * 0.7f + 1.0f));
      float d1 = std::sqrt((x - cx1) * (x - cx1) + (y - cy1) * (y - cy1));
      float d2 = std::sqrt((x - cx2) * (x - cx2) + (y - cy2) * (y - cy2));
      float v1 = std::sin(d1 / 1.5f - t * TWO_PI_F);
      float v2 = std::sin(d2 / 1.5f - t * TWO_PI_F);
      float val = (v1 * v2 + 1.0f) / 2.0f;
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void ArtsyElement::draw_julia_(display::Display &display, int width, int height, uint32_t current_ms) {
  float t = current_ms / 5000.0f * this->speed_;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      // Zoom oscillates periodically — never accumulates unboundedly
      float zoom = 2.0f + 1.5f * std::sin(t * TWO_PI_F * 0.17f);
      float zx = 1.5f * (x - width / 2.0f) / (0.5f * zoom * width);
      float zy = 1.0f * (y - height / 2.0f) / (0.5f * zoom * height);
      float cx = -0.4f + 0.6f * std::sin(t * TWO_PI_F * 0.33f);
      float cy = 0.618f * std::cos(t * TWO_PI_F * 0.41f);
      int i = 0;
      while (zx * zx + zy * zy < 4 && i < 12) {
        float tmp = zx * zx - zy * zy + cx;
        zy = 2.0f * zx * zy + cy;
        zx = tmp;
        i++;
      }
      float val = i / 12.0f;
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void ArtsyElement::draw_matrix_(display::Display &display, int width, int height, uint32_t current_ms) {
  float t = current_ms / 5000.0f * this->speed_;
  float strike_length = 5.0f * this->strength_;
  display.clear();
  for (int x = 0; x < width; ++x) {
    // Use density to decide if this column has a drop
    if (noise(x, 0, 123) > (1.0f - 0.2f * this->density_)) {
      uint32_t column_seed = hash(x + 42);
      float speed_mult = 0.5f + 1.0f * (noise(x, 1, column_seed));
      float drop_y =
          std::fmod(t * height * 2.0f * speed_mult + noise(x, 2, column_seed) * height, (float) height * 2.0f);

      for (int y = 0; y < height; ++y) {
        float dist = (float) y - drop_y;
        if (dist < 0 && dist > -strike_length) {
          float val = 1.0f - (std::abs(dist) / strike_length);
          display.draw_pixel_at(x, y, this->get_gradient_color_(val));
        }
      }
    }
  }
}

void ArtsyElement::draw_gradient_(display::Display &display, int width, int height, uint32_t current_ms) {
  float t = current_ms / 2000.0f * this->speed_;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float p = std::fmod((float) x / width + t, 1.0f);
      display.draw_pixel_at(x, y, this->get_gradient_color_(p));
    }
  }
}

void ArtsyElement::draw_fire_(display::Display &display, int width, int height, uint32_t current_ms) {
  static float heat[32][32] = {};
  float t = current_ms / 500.0f * this->speed_;
  int w = std::min(width, 32);
  int h = std::min(height, 32);

  // Propagate heat upward
  for (int y = 0; y < h - 1; ++y) {
    for (int x = 0; x < w; ++x) {
      float left = heat[y + 1][std::max(0, x - 1)];
      float mid = heat[y + 1][x];
      float right = heat[y + 1][std::min(w - 1, x + 1)];
      heat[y][x] = (left + mid + mid + right) / 4.0f * (0.85f * this->strength_);
      display.draw_pixel_at(x, y, this->get_gradient_color_(std::max(0.0f, heat[y][x] - 0.05f)));
    }
  }

  // Seed bottom rows
  for (int x = 0; x < w; ++x) {
    heat[h - 1][x] = 0.7f + 0.3f * noise(x, (int) t, (uint32_t) (t * 100));
    display.draw_pixel_at(x, h - 1, this->get_gradient_color_(heat[h - 1][x]));
    if (h > 1) {
      heat[h - 2][x] = 0.4f + 0.4f * noise(x + 10, (int) t, (uint32_t) (t * 200));
      display.draw_pixel_at(x, h - 2, this->get_gradient_color_(heat[h - 2][x]));
    }
  }
}

void ArtsyElement::draw_tunnel_(display::Display &display, int width, int height, uint32_t current_ms) {
  float t = current_ms / 2000.0f * this->speed_;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float dx = x - width / 2.0f;
      float dy = y - height / 2.0f;
      // Minimum dist of 1.5 prevents huge depth values at center
      float dist = std::sqrt(dx * dx + dy * dy) + 1.5f;
      float angle = std::atan2(dy, dx) / TWO_PI_F;
      float depth = 3.0f / dist;
      float stripe = std::fmod(depth - t, 1.0f);
      float twist = std::fmod(angle + depth * 0.2f, 1.0f);
      float val = std::fmod(std::abs(stripe) + std::abs(twist), 1.0f);
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void ArtsyElement::draw_wave_(display::Display &display, int width, int height, uint32_t current_ms) {
  float t = current_ms / 4000.0f * this->speed_;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float nx = x / (float) width - 0.5f;
      float ny = y / (float) height - 0.5f;
      // Halved spatial frequencies for 32x32: gives ~4-6 visible cycles per axis
      float v = 0;
      v += std::sin(nx * 5.0f + t * TWO_PI_F);
      v += std::sin(ny * 4.0f + t * TWO_PI_F * 0.7f);
      v += std::sin((nx + ny) * 3.5f + t * TWO_PI_F * 1.3f);
      v += std::sin(std::sqrt(nx * nx + ny * ny) * 6.0f - t * TWO_PI_F);
      float val = (v / 4.0f + 1.0f) / 2.0f;
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void ArtsyElement::draw_stars_(display::Display &display, int width, int height, uint32_t current_ms) {
  float t = current_ms / 5000.0f * this->speed_;  // slow, gentle twinkling by default
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      uint32_t seed = y * width + x;
      float base_brightness = noise(x, y, seed);
      // Each star has its own twinkle frequency and phase
      float freq = 0.5f + noise(x, y, seed + 1) * 2.0f;
      float phase = noise(x, y, seed + 2) * TWO_PI_F;
      float twinkle = (std::sin(t * freq * TWO_PI_F + phase) + 1.0f) / 2.0f;
      // Only show pixels above a density threshold
      float threshold = 1.0f - 0.05f * this->density_;
      float val = (base_brightness > threshold) ? base_brightness * twinkle : 0.0f;
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

}  // namespace esphome::elements