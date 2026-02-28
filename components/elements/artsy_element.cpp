#include "artsy_element.h"
#include <algorithm>
#include <cmath>
#include <algorithm>

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

Color ArtsyElement::get_gradient_color_(float p) const {
  float h = this->base_h_;
  float s = this->base_s_;
  float v = this->base_v_;

  switch (this->color_scheme_) {
    // Monochromatic: shade/tint variations of the same hue.
    case ArtsyColorScheme::MONOCHROMATIC:
      v = this->base_v_ * (this->min_brightness_ + (1.0f - this->min_brightness_) * p);
      s = this->base_s_ * (1.0f - 0.4f * p);
      break;
    // Analogous: colors adjacent on the color wheel (±30° sweep).
    case ArtsyColorScheme::ANALOGOUS:
      h = std::fmod(this->base_h_ + 60.0f * p - 30.0f + 360.0f, 360.0f);
      v = this->base_v_ * (this->min_brightness_ + (1.0f - this->min_brightness_) * (1.0f - std::cos(p * PI_F)) / 2.0f);
      break;
    // Complementary: smooth sweep from base hue to its complement and back.
    case ArtsyColorScheme::COMPLEMENTARY:
      h = std::fmod(this->base_h_ + p * 180.0f, 360.0f);
      v = this->base_v_ * (this->min_brightness_ + (1.0f - this->min_brightness_) * (1.0f - std::cos(p * PI_F)) / 2.0f);
      break;
    // Triadic: full hue wheel sweep — passes equally through all three triadic hues.
    case ArtsyColorScheme::TRIADIC:
      h = std::fmod(this->base_h_ + 360.0f * p, 360.0f);
      v = this->base_v_ * (this->min_brightness_ + (1.0f - this->min_brightness_) * (1.0f - std::cos(p * PI_F)) / 2.0f);
      break;
    // Split-Complementary: smooth cosine blend between base, +150°, and +210°.
    case ArtsyColorScheme::SPLIT_COMPLEMENTARY: {
      float w0 = std::max(0.0f, std::cos((p - 0.0f) * TWO_PI_F * 1.5f));
      float w1 = std::max(0.0f, std::cos((p - 0.33f) * TWO_PI_F * 1.5f));
      float w2 = std::max(0.0f, std::cos((p - 0.67f) * TWO_PI_F * 1.5f));
      float total = w0 + w1 + w2 + 1e-6f;
      float h0 = this->base_h_;
      float h1 = std::fmod(this->base_h_ + 150.0f, 360.0f);
      float h2 = std::fmod(this->base_h_ + 210.0f, 360.0f);
      // Weighted circular mean of hue angles
      float sx =
          w0 * std::cos(h0 * PI_F / 180.0f) + w1 * std::cos(h1 * PI_F / 180.0f) + w2 * std::cos(h2 * PI_F / 180.0f);
      float sy =
          w0 * std::sin(h0 * PI_F / 180.0f) + w1 * std::sin(h1 * PI_F / 180.0f) + w2 * std::sin(h2 * PI_F / 180.0f);
      h = std::fmod(std::atan2(sy / total, sx / total) * 180.0f / PI_F + 360.0f, 360.0f);
      v = this->base_v_ * (this->min_brightness_ + (1.0f - this->min_brightness_) * (1.0f - std::cos(p * PI_F)) / 2.0f);
      break;
    }
    // Dual-Complementary (Tetradic): smooth cosine blend between four square hues.
    case ArtsyColorScheme::DUAL_COMPLEMENTARY: {
      float w0 = std::max(0.0f, std::cos((p - 0.00f) * TWO_PI_F * 2.0f));
      float w1 = std::max(0.0f, std::cos((p - 0.25f) * TWO_PI_F * 2.0f));
      float w2 = std::max(0.0f, std::cos((p - 0.50f) * TWO_PI_F * 2.0f));
      float w3 = std::max(0.0f, std::cos((p - 0.75f) * TWO_PI_F * 2.0f));
      float total = w0 + w1 + w2 + w3 + 1e-6f;
      float h0 = this->base_h_;
      float h1 = std::fmod(this->base_h_ + 90.0f, 360.0f);
      float h2 = std::fmod(this->base_h_ + 180.0f, 360.0f);
      float h3 = std::fmod(this->base_h_ + 270.0f, 360.0f);
      float sx = w0 * std::cos(h0 * PI_F / 180.0f) + w1 * std::cos(h1 * PI_F / 180.0f) +
                 w2 * std::cos(h2 * PI_F / 180.0f) + w3 * std::cos(h3 * PI_F / 180.0f);
      float sy = w0 * std::sin(h0 * PI_F / 180.0f) + w1 * std::sin(h1 * PI_F / 180.0f) +
                 w2 * std::sin(h2 * PI_F / 180.0f) + w3 * std::sin(h3 * PI_F / 180.0f);
      h = std::fmod(std::atan2(sy / total, sx / total) * 180.0f / PI_F + 360.0f, 360.0f);
      v = this->base_v_ * (this->min_brightness_ + (1.0f - this->min_brightness_) * (1.0f - std::cos(p * PI_F)) / 2.0f);
      break;
    }
    // Rainbow: full-spectrum hue rotation, ignoring the base color.
    case ArtsyColorScheme::RAINBOW:
      h = std::fmod(p * 360.0f, 360.0f);
      s = 1.0f;
      v = this->base_v_;
      break;
  }

  float r, g, b;
  hsv_to_rgb(h, s, v, r, g, b);
  return Color(0xff * r, 0xff * g, 0xff * b);
}

void ArtsyElement::on_show() {
  float r_f = this->color_.r / 255.0f;
  float g_f = this->color_.g / 255.0f;
  float b_f = this->color_.b / 255.0f;
  int h;
  rgb_to_hsv(r_f, g_f, b_f, h, this->base_s_, this->base_v_);
  base_h_ = h;
}

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
    case ArtsyPattern::LISSAJOUS:
      this->draw_lissajous_(display, width, height, current_ms);
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
      float v1 = std::sin(x / 5.0f + t * TWO_PI_F);
      float v2 = std::sin(y / 4.0f + t * TWO_PI_F);
      float v3 = std::sin((x + y) / 6.0f + t * TWO_PI_F);
      float v4 = std::sin(
          std::sqrt((x - width / 2.0f) * (x - width / 2.0f) + (y - height / 2.0f) * (y - height / 2.0f)) / 4.0f +
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
      // Invert: cell interiors are bright, borders are dark
      float val = 1.0f - std::min(min_dist / (width / 3.0f), 1.0f);
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
      float cx = -0.4f + 0.4f * std::sin(t * TWO_PI_F);  // wider orbit for more shapes
      float cy = 0.4f * std::cos(t * TWO_PI_F);
      int i = 0;
      while (zx * zx + zy * zy < 4 && i < 20) {
        float tmp = zx * zx - zy * zy + cx;
        zy = 2.0f * zx * zy + cy;
        zx = tmp;
        i++;
      }
      float val = i / 20.0f;
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void ArtsyElement::draw_matrix_(display::Display &display, int width, int height, uint32_t current_ms) {
  float t = current_ms / 5000.0f * this->speed_;
  // Use a slowly-evolving seed so drop positions drift over time
  uint32_t drop_seed = (uint32_t) (t * 0.2f);
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float drop_y = (t * height * 2.0f) - noise(x, 0, drop_seed) * height;
      float dist = y - std::fmod(drop_y, (float) height * 1.5f);
      float val = 0.0f;
      if (dist < 0 && dist > -5) {
        val = 1.0f - (std::abs(dist) / 5.0f);
      }
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void ArtsyElement::draw_lissajous_(display::Display &display, int width, int height, uint32_t current_ms) {
  float t = current_ms / 5000.0f * this->speed_;
  display.clear();
  float a = 3.0f;
  // b must be an integer for sin(a*u+δ), sin(b*u) to close at u=2π.
  // Round to nearest integer from a slow oscillator → 2, 3, or 4.
  float b = std::round(3.0f + std::sin(t * TWO_PI_F * 0.07f));
  float delta = t * TWO_PI_F;  // continuously morphs shape within the closed family
  int steps = 800;
  for (int i = 0; i < steps; ++i) {
    float u = (float) i / steps * TWO_PI_F;
    float px = (width / 2.0f) + (width / 2.0f - 1) * std::sin(a * u + delta);
    float py = (height / 2.0f) + (height / 2.0f - 1) * std::sin(b * u);
    float p = (float) i / steps;
    int ix = (int) px, iy = (int) py;
    if (ix >= 0 && ix < width && iy >= 0 && iy < height) {
      display.draw_pixel_at(ix, iy, this->get_gradient_color_(p));
    }
  }
}

void ArtsyElement::draw_fire_(display::Display &display, int width, int height, uint32_t current_ms) {
  static float heat[64][64] = {};
  float t = current_ms / 500.0f * this->speed_;
  // Seed the bottom 3 rows with reliably hot values
  for (int x = 0; x < width && x < 64; ++x) {
    heat[height - 1][x] = 0.8f + 0.2f * noise(x, (int) (t * 3.0f), (uint32_t) (t * 73));
    if (height >= 2) {
      heat[height - 2][x] = 0.5f + 0.3f * noise(x + 1, (int) (t * 2.0f), (uint32_t) (t * 97));
    }
    if (height >= 3) {
      heat[height - 3][x] = 0.2f + 0.3f * noise(x + 2, (int) (t * 1.5f), (uint32_t) (t * 137));
    }
  }
  // Propagate heat upward with diffusion and stronger cooling
  // 0.90 per frame gives a clear gradient on 32 rows: top ≈ 3%, bottom ≈ 90%
  int seed_rows = std::min(3, height);
  for (int y = 0; y < height - seed_rows; ++y) {
    for (int x = 0; x < width && x < 64; ++x) {
      float left = heat[y + 1][std::max(x - 1, 0)];
      float mid = heat[y + 1][x];
      float right = heat[y + 1][std::min(x + 1, width - 1)];
      heat[y][x] = (left + mid + mid + right) / 4.0f * 0.95f;
    }
  }
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      display.draw_pixel_at(x, y, this->get_gradient_color_(heat[y][x]));
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
      float val = (base_brightness > (1.0f - this->density_ * 0.1f)) ? base_brightness * twinkle : 0.0f;
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

}  // namespace esphome::elements