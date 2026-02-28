#include "animation_element.h"
#include "color.h"
#include <algorithm>
#include <cmath>

namespace esphome::elements {

namespace {
const float PI_F = (float) M_PI;
const float TWO_PI_F = 2.0f * PI_F;

uint32_t hash(uint32_t v) {
  v ^= v >> 16;
  v *= 0x85ebca6b;
  v ^= v >> 13;
  v *= 0xc2b2ae35;
  v ^= v >> 16;
  return v;
}

float noise(int x, int y, uint32_t seed) {
  uint32_t val = hash(seed + (y * 10000) + x);
  return (val % 1000) / 1000.0f;
}
}  // namespace

void AnimationElement::draw(display::Display &display) {
  this->draw(display, display.get_width(), display.get_height(), this->get_component().get_current_ms() * this->speed_);
}

Color AnimationElement::get_gradient_color_(float p) {
  if (color_scheme_ != nullptr) {
    return color_scheme_->get_color(std::clamp(p, 0.0f, 1.0f));
  }
  return Color(0, 0, 0);
}

void MetaballsAnimationElement::draw(display::Display &display, int width, int height, uint32_t time) {
  float t = time / 5000.0f;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float val = 0;
      for (int i = 0; i < this->count_; i++) {
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

void AuroraAnimationElement::draw(display::Display &display, int width, int height, uint32_t time) {
  float t = time / 8000.0f;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float nx = x / (float) width;
      float ny = y / (float) height;
      // Increased spatial frequencies and band variation for more dynamic patterns
      float b1 = std::exp(-std::pow((ny - (0.25f + 0.15f * std::sin(nx * 6.0f + t * TWO_PI_F))) / 0.12f, 2.0f));
      float b2 =
          std::exp(-std::pow((ny - (0.50f + 0.12f * std::sin(nx * 4.5f - t * TWO_PI_F * 0.7f + 1.5f))) / 0.10f, 2.0f));
      float b3 =
          std::exp(-std::pow((ny - (0.75f + 0.10f * std::sin(nx * 7.5f + t * TWO_PI_F * 1.3f + 3.0f))) / 0.08f, 2.0f));
      float val = std::min(b1 + b2 * 0.8f + b3 * 0.6f, 1.0f);
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void KaleidoscopeAnimationElement::draw(display::Display &display, int width, int height, uint32_t time) {
  float t = time / 8000.0f;
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

void PlasmaAnimationElement::draw(display::Display &display, int width, int height, uint32_t time) {
  float t = time / 5000.0f;
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

void RipplesAnimationElement::draw(display::Display &display, int width, int height, uint32_t time) {
  float t = time / 5000.0f;
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

void SpiralAnimationElement::draw(display::Display &display, int width, int height, uint32_t time) {
  float t = time / 5000.0f;
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
      float val = std::fmod(angle / TWO_PI_F + dist / 8.0f - t, 1.0f);
      if (val < 0) {
        val += 1.0f;
      }
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void VoronoiAnimationElement::draw(display::Display &display, int width, int height, uint32_t time) {
  float t = time / 5000.0f;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float min_dist = 1000.0f;
      for (int i = 0; i < this->count_; i++) {
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

void InterferenceAnimationElement::draw(display::Display &display, int width, int height, uint32_t time) {
  float t = time / 5000.0f;
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

void JuliaAnimationElement::draw(display::Display &display, int width, int height, uint32_t time) {
  float t = time / 5000.0f;
  // Slowly orbit the viewport center to reveal different regions of the set
  float off_x = 0.2f * std::sin(t * TWO_PI_F * 0.11f);
  float off_y = 0.2f * std::cos(t * TWO_PI_F * 0.13f);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float zoom = 2.0f + 1.5f * std::sin(t * TWO_PI_F * 0.17f);
      float zx = 1.5f * (x - width / 2.0f) / (0.5f * zoom * width) + off_x;
      float zy = 1.0f * (y - height / 2.0f) / (0.5f * zoom * height) + off_y;
      float cx = -0.4f + 0.6f * std::sin(t * TWO_PI_F * 0.33f);
      float cy = 0.618f * std::cos(t * TWO_PI_F * 0.41f);
      int i = 0;
      while (zx * zx + zy * zy < 4 && i < 16) {
        float tmp = zx * zx - zy * zy + cx;
        zy = 2.0f * zx * zy + cy;
        zx = tmp;
        i++;
      }
      // Normalize i to 0..1 to ensure we use the full palette range
      float val = i / 16.0f;
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void MatrixAnimationElement::draw(display::Display &display, int width, int height, uint32_t time) {
  float t = time / 2500.0f;
  display.clear();
  for (int x = 0; x < width; ++x) {
    // Use density to decide if this column has a drop
    if (noise(x, 0, 123) > (1.0f - this->density_)) {
      uint32_t column_seed = hash(x + 42);
      float speed_mult = 0.5f + 1.0f * (noise(x, 1, column_seed));
      float drop_y =
          std::fmod(t * height * 2.0f * speed_mult + noise(x, 2, column_seed) * height, (float) height * 2.0f);
      // Each column drifts through the gradient at its own speed and phase
      float gradient_offset = std::fmod(noise(x, 3, column_seed) + t * 0.05f * noise(x, 4, column_seed), 1.0f);
      for (int y = 0; y < height; ++y) {
        float dist = (float) y - drop_y;
        if (dist < 0 && dist > -this->length_) {
          float fade = 1.0f - (std::abs(dist) / this->length_);
          float val = std::fmod(fade * 0.8f + gradient_offset, 1.0f);
          display.draw_pixel_at(x, y, this->get_gradient_color_(val));
        }
      }
    }
  }
}

void GradientAnimationElement::draw(display::Display &display, int width, int height, uint32_t time) {
  float t = time / 2000.0f;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float p = std::fmod((float) x / width + t, 1.0f);
      display.draw_pixel_at(x, y, this->get_gradient_color_(p));
    }
  }
}

void FireAnimationElement::draw(display::Display &display, int width, int height, uint32_t time) {
  if (this->heat_buffer_.size() != width * height) {
    this->heat_buffer_.assign(width * height, 0.0f);
  }

  // Cooling factor: how quickly the fire dies down
  float cooling = this->cooling_;
  // Strength: how much heat is carried upward
  float strength = this->strength_;

  // Propagate heat upward with diffusion and cooling
  for (int y = 0; y < height - 2; ++y) {
    for (int x = 0; x < width; ++x) {
      int idx = y * width + x;
      // Average the heat from below (diffusion)
      float h1 = this->heat_buffer_[(y + 1) * width + std::max(0, x - 1)];
      float h2 = this->heat_buffer_[(y + 1) * width + x];
      float h3 = this->heat_buffer_[(y + 1) * width + std::min(width - 1, x + 1)];
      float h4 = this->heat_buffer_[(y + 2) * width + x];

      float h = (h1 + h2 + h3 + h4) / 4.0f * strength;
      this->heat_buffer_[idx] = std::max(0.0f, h - cooling);

      // Draw with a slight curve to push the hottest colors to the very bottom
      display.draw_pixel_at(x, y, this->get_gradient_color_(std::pow(this->heat_buffer_[idx], 1.2f)));
    }
  }

  // Seed bottom rows with noise
  float t = time / 400.0f;
  for (int y = height - 2; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int idx = y * width + x;
      this->heat_buffer_[idx] = 0.6f + 0.4f * noise(x, y + (int) t, (uint32_t) (t * 100));
      display.draw_pixel_at(x, y, this->get_gradient_color_(this->heat_buffer_[idx]));
    }
  }
}

void TunnelAnimationElement::draw(display::Display &display, int width, int height, uint32_t time) {
  float t = time / 2000.0f;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float dx = x - width / 2.0f;
      float dy = y - height / 2.0f;
      // Minimum dist of 1.0 prevents huge depth values at center, but 1.5 was too flat
      float dist = std::sqrt(dx * dx + dy * dy) + 1.2f;
      float angle = std::atan2(dy, dx) / TWO_PI_F;
      // Increased depth constant for more rings/perspective
      float depth = 8.0f / dist;
      float stripe = std::fmod(depth - t * 2.0f, 1.0f);
      float twist = std::fmod(angle + depth * 0.25f, 1.0f);
      float val = std::fmod(std::abs(stripe) + std::abs(twist), 1.0f);
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void WaveAnimationElement::draw(display::Display &display, int width, int height, uint32_t time) {
  float t = time / 4000.0f;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float nx = x / (float) width - 0.5f;
      float ny = y / (float) height - 0.5f;
      // Increased spatial frequencies to show more cycles on 32x32
      float v = 0;
      v += std::sin(nx * 12.0f + t * TWO_PI_F);
      v += std::sin(ny * 10.0f + t * TWO_PI_F * 0.7f);
      v += std::sin((nx + ny) * 8.0f + t * TWO_PI_F * 1.3f);
      v += std::sin(std::sqrt(nx * nx + ny * ny) * 15.0f - t * TWO_PI_F);
      float val = (v / 4.0f + 1.0f) / 2.0f;
      display.draw_pixel_at(x, y, this->get_gradient_color_(val));
    }
  }
}

void StarsAnimationElement::draw(display::Display &display, int width, int height, uint32_t time) {
  float t = time / 5000.0f;
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
