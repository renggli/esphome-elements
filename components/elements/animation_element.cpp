#include "animation_element.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <numbers>

#include "color.h"
#include "esphome/core/color.h"
#include "esphome/core/helpers.h"

namespace esphome::elements {

namespace {
const float PI_F = std::numbers::pi_v<float>;
const float TWO_PI_F = 2.0f * PI_F;

constexpr Color DEFAULT_COLORS[] PROGMEM = {{0x00, 0x00, 0x00, 0x00},
                                            {0xff, 0xff, 0xff, 0x00}};
const ColorScheme DEFAULT_SCHEME = ColorScheme(DEFAULT_COLORS, 2);

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

float fract(float v) { return v - std::floor(v); }
}  // namespace

// ---------------------------------------------------------------------------
// AnimationElement
// ---------------------------------------------------------------------------

void AnimationElement::draw(display::Display& display) {
  if (color_scheme_ == nullptr) set_color_scheme(&DEFAULT_SCHEME);
  float time =
      ((millis() - start_time_) / 1000.0f) * speed_ * get_time_scale_();
  draw(display, display.get_width(), display.get_height(), time);
}

void AnimationElement::on_show() {
  start_time_ = millis();
  Element::on_show();
}

// ---------------------------------------------------------------------------
// MetaballsAnimationElement
// ---------------------------------------------------------------------------

void MetaballsAnimationElement::draw(display::Display& display, int width,
                                     int height, float time) {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float val = 0;
      for (int i = 0; i < count_; i++) {
        float bx = 0.25f + 0.5f * noise(i, 5, start_time_);
        float by = 0.25f + 0.5f * noise(i, 6, start_time_);
        float cx =
            width * bx + std::sin(time * TWO_PI_F + i * 2.1f) * (width / 3.5f);
        float cy = height * by +
                   std::cos(time * TWO_PI_F + i * 3.3f) * (height / 3.5f);
        float dx = x - cx;
        float dy = y - cy;
        float dist2 = dx * dx + dy * dy;
        val += 80.0f / (dist2 + 1.0f);
      }
      val = std::min(val / 5.5f, 0.85f);
      display.draw_pixel_at(x, y, get_color_(val));
    }
  }
}

// ---------------------------------------------------------------------------
// PlasmaAnimationElement
// ---------------------------------------------------------------------------

void PlasmaAnimationElement::draw(display::Display& display, int width,
                                  int height, float time) {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float nx = x / (float)width;
      float ny = y / (float)height;
      float val = std::sin(nx * 10.0f + time * TWO_PI_F);
      val += std::sin(10.0f * (nx * std::sin(time * PI_F) +
                               ny * std::cos(time * TWO_PI_F * 0.5f)) +
                      time * TWO_PI_F);
      float cx = nx + 0.5f * std::sin(time * TWO_PI_F * 0.33f);
      float cy = ny + 0.5f * std::cos(time * TWO_PI_F * 0.5f);
      val += std::sin(std::sqrt(128.0f * (cx * cx + cy * cy) + 1.0f) +
                      time * TWO_PI_F);
      val = (val / 3.0f + 1.0f) / 2.0f;
      display.draw_pixel_at(x, y, get_color_(val));
    }
  }
}

// ---------------------------------------------------------------------------
// RipplesAnimationElement
// ---------------------------------------------------------------------------

void RipplesAnimationElement::draw(display::Display& display, int width,
                                   int height, float time) {
  float max_radius = std::max(width, height) * 0.6f;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float val = 0.0f;
      for (int i = 0; i < count_; i++) {
        float burst_t = fract(time * 0.2f + i * (1.0f / count_));
        float cx = (0.1f + 0.8f * noise(i, 0, start_time_)) * width;
        float cy = (0.1f + 0.8f * noise(i, 1, start_time_)) * height;
        float radius = burst_t * max_radius;
        float fade = 1.0f - burst_t;
        float dx = x - cx;
        float dy = y - cy;
        float dist = std::sqrt(dx * dx + dy * dy);
        val += std::exp(-std::pow((dist - radius) / 1.5f, 2.0f)) * fade;
      }
      if (val < 0.05f) {
        val = 0.0f;
      } else {
        val = (val - 0.05f) / 0.95f;
      }
      display.draw_pixel_at(x, y, get_color_(std::min(val, 1.0f)));
    }
  }
}

// ---------------------------------------------------------------------------
// SpiralAnimationElement
// ---------------------------------------------------------------------------

void SpiralAnimationElement::draw(display::Display& display, int width,
                                  int height, float time) {
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
      float val = std::fmod(angle / TWO_PI_F + dist / 8.0f - time, 1.0f);
      if (val < 0) {
        val += 1.0f;
      }
      display.draw_pixel_at(x, y, get_color_(val));
    }
  }
}

// ---------------------------------------------------------------------------
// VoronoiAnimationElement
// ---------------------------------------------------------------------------

void VoronoiAnimationElement::draw(display::Display& display, int width,
                                   int height, float time) {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float min_dist = 1000.0f;
      for (int i = 0; i < count_; i++) {
        float base_x = noise(i, 0, start_time_);
        float base_y = noise(i, 1, start_time_);
        float orbit_r_x = width / 4.0f * (0.5f + noise(i, 2, start_time_));
        float orbit_r_y = height / 4.0f * (0.5f + noise(i, 3, start_time_));
        float phase = i * 1.0472f;
        float px =
            width * base_x + std::sin(time * TWO_PI_F + phase) * orbit_r_x;
        float py = height * base_y +
                   std::cos(time * TWO_PI_F + phase * 1.618f) * orbit_r_y;
        float dx = x - px;
        float dy = y - py;
        float dist = std::sqrt(dx * dx + dy * dy);
        min_dist = std::min(dist, min_dist);
      }
      float val =
          1.0f - std::min(std::pow(min_dist / (width / 2.5f), 1.5f), 1.0f);
      display.draw_pixel_at(x, y, get_color_(val));
    }
  }
}

// ---------------------------------------------------------------------------
// InterferenceAnimationElement
// ---------------------------------------------------------------------------

void InterferenceAnimationElement::draw(display::Display& display, int width,
                                        int height, float time) {
  const float k = 0.55f;
  const float omega = time * TWO_PI_F * 1.5f;
  const float cos0 = 1.0f, sin0 = 0.0f;
  const float cos1 = -0.5f, sin1 = 0.8660254f;
  const float cos2 = -0.5f, sin2 = -0.8660254f;

  for (int y = 0; y < height; ++y) {
    float fy = (float)y - (float)height * 0.5f;
    for (int x = 0; x < width; ++x) {
      float fx = (float)x - (float)width * 0.5f;
      float w1 = std::cos((fx * cos0 + fy * sin0) * k - omega);
      float w2 = std::cos((fx * cos1 + fy * sin1) * k - omega);
      float w3 = std::cos((fx * cos2 + fy * sin2) * k + omega * 0.7f);
      float sum = (w1 + w2 + w3) / 3.0f;
      float intensity = sum * sum;
      if (intensity < 0.20f) {
        intensity = 0.0f;
      }
      display.draw_pixel_at(x, y, get_color_(intensity));
    }
  }
}

// ---------------------------------------------------------------------------
// JuliaAnimationElement
// ---------------------------------------------------------------------------

void JuliaAnimationElement::draw(display::Display& display, int width,
                                 int height, float time) {
  float off_x = 0.2f * std::sin(time * TWO_PI_F * 0.11f);
  float off_y = 0.2f * std::cos(time * TWO_PI_F * 0.13f);
  float zoom = 2.0f + 1.5f * std::sin(time * TWO_PI_F * 0.17f);
  float cx = -0.4f + 0.6f * std::sin(time * TWO_PI_F * 0.33f);
  float cy = 0.618f * std::cos(time * TWO_PI_F * 0.41f);

  float zoom_width_factor = 0.5f * zoom * width;
  float zoom_height_factor = 0.5f * zoom * height;

  for (int y = 0; y < height; ++y) {
    float zy_init = 1.0f * (y - height / 2.0f) / zoom_height_factor + off_y;
    for (int x = 0; x < width; ++x) {
      float zx = 1.5f * (x - width / 2.0f) / zoom_width_factor + off_x;
      float zy = zy_init;
      int i = 0;
      while (zx * zx + zy * zy < 4 && i < 16) {
        float tmp = zx * zx - zy * zy + cx;
        zy = 2.0f * zx * zy + cy;
        zx = tmp;
        i++;
      }
      float val = i / 16.0f;
      display.draw_pixel_at(x, y, get_color_(val));
    }
  }
}

// ---------------------------------------------------------------------------
// MatrixAnimationElement
// ---------------------------------------------------------------------------

void MatrixAnimationElement::draw(display::Display& display, int width,
                                  int height, float time) {
  for (int x = 0; x < width; ++x) {
    if (noise(x, 0, 123) > (1.0f - density_)) {
      uint32_t column_seed = hash(x + start_time_);
      float speed_mult = 0.5f + 1.0f * (noise(x, 1, column_seed));
      float drop_y = std::fmod(
          time * height * 2.0f * speed_mult + noise(x, 2, column_seed) * height,
          (float)height * 2.0f);
      float gradient_offset = std::fmod(
          noise(x, 3, column_seed) + time * 0.05f * noise(x, 4, column_seed),
          1.0f);
      for (int y = 0; y < height; ++y) {
        float dist = (float)y - drop_y;
        if (dist < 0 && dist > -length_) {
          float fade = 1.0f - (std::abs(dist) / length_);
          float val = std::fmod(fade * 0.8f + gradient_offset, 1.0f);
          display.draw_pixel_at(x, y, get_color_(val));
        }
      }
    }
  }
}

// ---------------------------------------------------------------------------
// FireAnimationElement
// ---------------------------------------------------------------------------

void FireAnimationElement::on_hide() {
  heat_buffer_.clear();
  next_heat_.clear();
  Element::on_hide();
}

void FireAnimationElement::draw(display::Display& display, int width,
                                int height, float time) {
  int size = width * height;
  if (heat_buffer_.size() != size) {
    heat_buffer_.assign(size, 0.0f);
  }
  // Use a second buffer for the next state to avoid using already-cooled pixels
  // in the same pass
  if (next_heat_.size() != size) {
    next_heat_.assign(size, 0.0f);
  }
  // Seed the bottom row with noise
  for (int x = 0; x < width; x++) {
    // Seed with full [0, 1] range, but keep it hot overall
    next_heat_[(height - 1) * width + x] =
        std::pow(noise(x, (int)time, start_time_), 0.5f);
  }
  // Propagate heat upwards with diffusion and cooling
  for (int y = 0; y < height - 1; y++) {
    for (int x = 0; x < width; x++) {
      // Average 3 cells below to diffuse upwards
      float h = 0;
      h += heat_buffer_[(y + 1) * width + std::max(0, x - 1)];
      h += heat_buffer_[(y + 1) * width + x];
      h += heat_buffer_[(y + 1) * width + std::min(width - 1, x + 1)];
      h = (h / 3.0f) * strength_;
      // Random jitter for the cooling factor makes it look "wispy"
      float jitter = 0.8f + 0.4f * noise(x, y, (int)(time * 2.0f));
      // Cooling factor increases with height to ensure it hits black at the top
      float height_factor = 1.0f + (float)(height - 1 - y) / height;
      next_heat_[y * width + x] =
          std::clamp(h - (cooling_ * jitter * height_factor), 0.0f, 1.0f);
    }
  }
  heat_buffer_ = next_heat_;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      // Apply a gamma-like curve to "stretch" the lower heat values
      // (reds/oranges)
      float heat = std::pow(heat_buffer_[y * width + x], 0.8f);
      display.draw_pixel_at(x, y, get_color_(heat));
    }
  }
}

// ---------------------------------------------------------------------------
// TunnelAnimationElement
// ---------------------------------------------------------------------------

void TunnelAnimationElement::draw(display::Display& display, int width,
                                  int height, float time) {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float dx = x - width / 2.0f;
      float dy = y - height / 2.0f;
      float dist = std::sqrt(dx * dx + dy * dy) + 0.1f;
      float angle = fract(std::atan2(dy, dx) / TWO_PI_F);
      float depth = 10.0f / dist;
      float stripe = fract(depth - time * 2.0f);
      float twist = fract(angle + depth * 0.25f);
      float val = fract(stripe + twist);
      display.draw_pixel_at(x, y, get_color_(val));
    }
  }
}

// ---------------------------------------------------------------------------
// WaveAnimationElement
// ---------------------------------------------------------------------------

void WaveAnimationElement::draw(display::Display& display, int width,
                                int height, float time) {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float nx = x / (float)width;
      float ny = y / (float)height;
      float v = std::sin(nx * 15.0f + time * TWO_PI_F);
      v += std::sin(ny * 10.0f - time * TWO_PI_F * 0.5f);
      float val = (v / 2.0f + 1.0f) / 2.0f;
      display.draw_pixel_at(x, y, get_color_(val));
    }
  }
}

// ---------------------------------------------------------------------------
// StarsAnimationElement
// ---------------------------------------------------------------------------

void StarsAnimationElement::draw(display::Display& display, int width,
                                 int height, float time) {
  float threshold = 1.0f - density_;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      uint32_t seed = start_time_ + y * width + x;
      float base_brightness = noise(x, y, seed);
      if (base_brightness > threshold) {
        // Each star has its own twinkle frequency and phase
        float freq = 0.5f + noise(x, y, seed + 1) * 2.0f;
        float phase = noise(x, y, seed + 2) * TWO_PI_F;
        float twinkle =
            (std::sin(time * freq * TWO_PI_F + phase) + 1.0f) / 2.0f;
        float val = base_brightness * twinkle;
        display.draw_pixel_at(x, y, get_color_(val));
      } else {
        display.draw_pixel_at(x, y, get_color_(0.0f));
      }
    }
  }
}

// ---------------------------------------------------------------------------
// SolidAnimationElement
// ---------------------------------------------------------------------------

void SolidAnimationElement::draw(display::Display& display, int width,
                                 int height, float time) {
  if (width <= 0 || height <= 0 || points_ == nullptr || num_points_ == 0) {
    return;
  }

  const float ax = time * TWO_PI_F * 0.397f;
  const float ay = time * TWO_PI_F;
  const float cx_r = std::cos(ax), sx_r = std::sin(ax);
  const float cy_r = std::cos(ay), sy_r = std::sin(ay);
  const float scale = std::min(width, height) * 0.42f;
  const float ox = (width - 1) * 0.5f, oy = (height - 1) * 0.5f;

  static std::vector<float> rz;
  static std::vector<int> px;
  static std::vector<int> py;
  static std::vector<uint16_t> pz;
  if (px.size() < num_points_) {
    px.resize(num_points_);
    py.resize(num_points_);
    pz.resize(num_points_);
    rz.resize(num_points_);
  }

  // Transform and project points.
  for (size_t i = 0; i < num_points_; i++) {
    const Vec3& vert = points_[i];
    const float y1 = vert.y * cx_r - vert.z * sx_r;
    const float z1 = vert.y * sx_r + vert.z * cx_r;
    const float x2 = vert.x * cy_r + z1 * sy_r;
    rz[i] = -vert.x * sy_r + z1 * cy_r;
    px[i] =
        std::clamp(static_cast<int>(std::round(ox + x2 * scale)), 0, width - 1);
    py[i] = std::clamp(static_cast<int>(std::round(oy - y1 * scale)), 0,
                       height - 1);
    const float z_norm = std::clamp((rz[i] + 1.5f) / 3.0f, 0.0f, 1.0f);
    pz[i] = static_cast<uint16_t>(z_norm * 65535.0f);
  }

  if (mode_ == SolidMode::WIRE || faces_ == nullptr || num_faces_ == 0) {
    // Draw background lines first
    for (size_t i = 0; i < num_edges_; i++) {
      const Edge& edge = edges_[i];
      if (edge.a >= num_points_ || edge.b >= num_points_) {
        continue;
      }
      const float depth = ((rz[edge.a] + rz[edge.b]) * 0.5f + 1.0f) * 0.5f;
      if (depth < 0.5f) {
        display.line(px[edge.a], py[edge.a], px[edge.b], py[edge.b],
                     get_color_(std::clamp(depth, 0.0f, 1.0f)));
      }
    }
    // Draw foreground lines on top
    for (size_t i = 0; i < num_edges_; i++) {
      const Edge& edge = edges_[i];
      if (edge.a >= num_points_ || edge.b >= num_points_) {
        continue;
      }
      const float depth = ((rz[edge.a] + rz[edge.b]) * 0.5f + 1.0f) * 0.5f;
      if (depth >= 0.5f) {
        display.line(px[edge.a], py[edge.a], px[edge.b], py[edge.b],
                     get_color_(std::clamp(depth, 0.0f, 1.0f)));
      }
    }
    return;
  }

  static std::vector<uint16_t> depth_buffer;
  const size_t buffer_size =
      static_cast<size_t>(width) * static_cast<size_t>(height);
  if (depth_buffer.size() < buffer_size) {
    depth_buffer.resize(buffer_size);
  }
  std::memset(depth_buffer.data(), 0, buffer_size * sizeof(uint16_t));

  static std::vector<bool> face_front;
  if (face_front.size() < num_faces_) {
    face_front.resize(num_faces_);
  }
  std::fill_n(face_front.begin(), num_faces_, false);

  static std::vector<bool> edge_front;
  if (edge_front.size() < num_edges_) {
    edge_front.resize(num_edges_);
  }
  std::fill_n(edge_front.begin(), num_edges_, false);

  constexpr float LX = -0.577350269f;
  constexpr float LY = 0.577350269f;
  constexpr float LZ = 0.577350269f;

  for (size_t i = 0; i < num_faces_; i++) {
    const Vec3& normal = normals_[i];
    const float ny1 = normal.y * cx_r - normal.z * sx_r;
    const float nz1 = normal.y * sx_r + normal.z * cx_r;
    const float nx2 = normal.x * cy_r + nz1 * sy_r;
    const float nz2 = -normal.x * sy_r + nz1 * cy_r;

    if (nz2 <= 0.0f) {
      continue;
    }
    face_front[i] = true;

    const Face& face = faces_[i];
    if (face.a >= num_points_ || face.b >= num_points_ ||
        face.c >= num_points_) {
      continue;
    }

    if (mode_ == SolidMode::HIDDEN) {
      rasterize_triangle_(display, depth_buffer.data(), width, height,
                          px[face.a], py[face.a], pz[face.a], px[face.b],
                          py[face.b], pz[face.b], px[face.c], py[face.c],
                          pz[face.c], false, Color{});
    } else if (mode_ == SolidMode::SOLID) {
      const float dot_key = nx2 * LX + ny1 * LY + nz2 * LZ;
      const float dot_fill = nx2 * (-LX) + ny1 * (-LY) + nz2 * LZ;
      const float diff_key = std::max(0.0f, dot_key);
      const float diff_fill = std::max(0.0f, dot_fill);
      const float intensity = 0.10f + 0.65f * diff_key + 0.15f * diff_fill;
      const Color color = get_color_(std::clamp(intensity, 0.0f, 0.85f));

      rasterize_triangle_(display, depth_buffer.data(), width, height,
                          px[face.a], py[face.a], pz[face.a], px[face.b],
                          py[face.b], pz[face.b], px[face.c], py[face.c],
                          pz[face.c], true, color);
    }

    // Mark edges belonging to this front-facing face
    for (size_t e = 0; e < num_edges_; ++e) {
      const Edge& edge = edges_[e];
      if ((face.a == edge.a && face.b == edge.b) ||
          (face.a == edge.b && face.b == edge.a) ||
          (face.b == edge.a && face.c == edge.b) ||
          (face.b == edge.b && face.c == edge.a) ||
          (face.c == edge.a && face.a == edge.b) ||
          (face.c == edge.b && face.a == edge.a)) {
        edge_front[e] = true;
      }
    }
  }

  constexpr uint16_t BIAS = 2048;

  if (mode_ == SolidMode::HIDDEN) {
    for (size_t i = 0; i < num_edges_; i++) {
      if (!edge_front[i]) {
        continue;
      }
      const Edge& edge = edges_[i];
      if (edge.a >= num_points_ || edge.b >= num_points_) {
        continue;
      }
      const float depth = ((rz[edge.a] + rz[edge.b]) * 0.5f + 1.0f) * 0.5f;
      const Color color = get_color_(std::clamp(depth, 0.0f, 1.0f));
      draw_line_depth_(display, depth_buffer.data(), width, height, px[edge.a],
                       py[edge.a], pz[edge.a], px[edge.b], py[edge.b],
                       pz[edge.b], BIAS, color);
    }
  } else if (mode_ == SolidMode::SOLID) {
    const Color edge_color = get_color_(1.0f);
    for (size_t i = 0; i < num_edges_; i++) {
      if (!edge_front[i]) {
        continue;
      }
      const Edge& edge = edges_[i];
      if (edge.a >= num_points_ || edge.b >= num_points_) {
        continue;
      }
      draw_line_depth_(display, depth_buffer.data(), width, height, px[edge.a],
                       py[edge.a], pz[edge.a], px[edge.b], py[edge.b],
                       pz[edge.b], BIAS, edge_color);
    }
  }
}

void SolidAnimationElement::rasterize_triangle_(
    display::Display& display, uint16_t* depth_buffer, int width, int height,
    int x0, int y0, uint16_t z0, int x1, int y1, uint16_t z1, int x2, int y2,
    uint16_t z2, bool write_color, Color color) {
  const float det =
      static_cast<float>((x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0));
  if (std::abs(det) < 0.001f) {
    return;
  }

  // Sort vertices by Y ascending: a <= b <= c
  int ax = x0, ay = y0;
  uint16_t az = z0;
  int bx = x1, by = y1;
  uint16_t bz = z1;
  int cx = x2, cy = y2;
  uint16_t cz = z2;

  if (ay > by) {
    std::swap(ax, bx);
    std::swap(ay, by);
    std::swap(az, bz);
  }
  if (ay > cy) {
    std::swap(ax, cx);
    std::swap(ay, cy);
    std::swap(az, cz);
  }
  if (by > cy) {
    std::swap(bx, cx);
    std::swap(by, cy);
    std::swap(bz, cz);
  }

  if (ay == cy) {
    return;
  }

  const float z0_f = static_cast<float>(z0);
  const float z1_f = static_cast<float>(z1);
  const float z2_f = static_cast<float>(z2);
  const float min_z = std::min({z0_f, z1_f, z2_f});
  const float max_z = std::max({z0_f, z1_f, z2_f});
  const float inv_det = 1.0f / det;
  const float dz_dx = ((z1_f - z0_f) * static_cast<float>(y2 - y0) -
                       (z2_f - z0_f) * static_cast<float>(y1 - y0)) *
                      inv_det;
  const float dz_dy = (static_cast<float>(x1 - x0) * (z2_f - z0_f) -
                       static_cast<float>(x2 - x0) * (z1_f - z0_f)) *
                      inv_det;

  // Slope-dependent polygon depth offset prevents depth fighting with edges
  const float poly_offset =
      std::min(12000.0f, 2.0f * (std::abs(dz_dx) + std::abs(dz_dy)) + 1024.0f);

  const int y_start = std::max(ay, 0);
  const int y_end = std::min(cy, height - 1);

  const float inv_cy_ay = 1.0f / static_cast<float>(cy - ay);
  const float inv_by_ay =
      (by != ay) ? (1.0f / static_cast<float>(by - ay)) : 0.0f;
  const float inv_cy_by =
      (cy != by) ? (1.0f / static_cast<float>(cy - by)) : 0.0f;

  for (int y = y_start; y <= y_end; ++y) {
    const float y_ay = static_cast<float>(y - ay);
    const float x_ac =
        static_cast<float>(ax) + static_cast<float>(cx - ax) * y_ay * inv_cy_ay;
    float x_other;
    if (y < by) {
      x_other = static_cast<float>(ax) +
                static_cast<float>(bx - ax) * y_ay * inv_by_ay;
    } else {
      const float y_by = static_cast<float>(y - by);
      x_other = (cy != by) ? (static_cast<float>(bx) +
                              static_cast<float>(cx - bx) * y_by * inv_cy_by)
                           : static_cast<float>(bx);
    }

    const int min_x = static_cast<int>(std::round(std::min(x_ac, x_other)));
    const int max_x = static_cast<int>(std::round(std::max(x_ac, x_other)));

    const int xs = std::max(min_x, 0);
    const int xe = std::min(max_x, width - 1);

    if (xs > xe) {
      continue;
    }

    const size_t row_idx = static_cast<size_t>(y) * width;
    float z_curr = z0_f + static_cast<float>(xs - x0) * dz_dx +
                   static_cast<float>(y - y0) * dz_dy;

    for (int x = xs; x <= xe; ++x, z_curr += dz_dx) {
      const float z_clamped = std::clamp(z_curr, min_z, max_z);
      const float z_write = std::max(0.0f, z_clamped - poly_offset);
      const uint16_t z_val =
          static_cast<uint16_t>(std::clamp(z_write, 0.0f, 65535.0f));
      const size_t idx = row_idx + x;
      if (z_val > depth_buffer[idx]) {
        depth_buffer[idx] = z_val;
        if (write_color) {
          display.draw_pixel_at(x, y, color);
        }
      }
    }
  }
}

void SolidAnimationElement::draw_line_depth_(display::Display& display,
                                             const uint16_t* depth_buffer,
                                             int width, int height, int x0,
                                             int y0, uint16_t z0, int x1,
                                             int y1, uint16_t z1, uint16_t bias,
                                             Color color) {
  if (width <= 0 || height <= 0) {
    return;
  }
  const int dx = std::abs(x1 - x0);
  const int dy = std::abs(y1 - y0);
  const int steps = std::max(dx, dy);

  if (steps == 0) {
    if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height) {
      const size_t idx = static_cast<size_t>(y0) * width + x0;
      if (static_cast<uint32_t>(z0) + bias >= depth_buffer[idx]) {
        display.draw_pixel_at(x0, y0, color);
      }
    }
    return;
  }

  // Pre-allocated static vectors for zero heap allocations in steady-state loop
  static std::vector<int> line_x;
  static std::vector<int> line_y;
  static std::vector<bool> line_pass;
  const size_t total_points = static_cast<size_t>(steps + 1);
  if (line_x.size() < total_points) {
    line_x.resize(total_points);
    line_y.resize(total_points);
    line_pass.resize(total_points);
  }

  const int sx = (x0 < x1) ? 1 : -1;
  const int sy = (y0 < y1) ? 1 : -1;
  int err = dx - dy;
  float z_curr = static_cast<float>(z0);
  const float dz = (static_cast<float>(z1) - static_cast<float>(z0)) /
                   static_cast<float>(steps);

  int cur_x = x0;
  int cur_y = y0;
  int pass_count = 0;

  for (size_t i = 0; i < total_points; ++i) {
    line_x[i] = cur_x;
    line_y[i] = cur_y;

    if (cur_x >= 0 && cur_x < width && cur_y >= 0 && cur_y < height) {
      const size_t idx = static_cast<size_t>(cur_y) * width + cur_x;
      const uint16_t z_val =
          static_cast<uint16_t>(std::clamp(z_curr, 0.0f, 65535.0f));
      bool pass = (static_cast<uint32_t>(z_val) + bias >= depth_buffer[idx]);
      line_pass[i] = pass;
      if (pass) {
        pass_count++;
      }
    } else {
      line_pass[i] = false;
    }

    if (i < static_cast<size_t>(steps)) {
      const int e2 = 2 * err;
      if (e2 > -dy) {
        err -= dy;
        cur_x += sx;
      }
      if (e2 < dx) {
        err += dx;
        cur_y += sy;
      }
      z_curr += dz;
    }
  }

  // Fill up to 2-pixel gaps caused by rasterization/slope depth fighting
  for (size_t i = 1; i + 1 < total_points; ++i) {
    if (!line_pass[i] && line_pass[i - 1] && line_pass[i + 1]) {
      line_pass[i] = true;
      pass_count++;
    }
  }
  for (size_t i = 1; i + 2 < total_points; ++i) {
    if (!line_pass[i] && !line_pass[i + 1] && line_pass[i - 1] &&
        line_pass[i + 2]) {
      line_pass[i] = true;
      line_pass[i + 1] = true;
      pass_count += 2;
    }
  }

  // If both endpoints pass: edge connects visible vertices, render unbroken
  // continuous line to prevent low-resolution rasterization gaps.
  if (line_pass[0] && line_pass[steps]) {
    for (size_t i = 0; i < total_points; ++i) {
      display.draw_pixel_at(line_x[i], line_y[i], color);
    }
    return;
  }

  // Otherwise draw all visible pixels (including stubs and runs on partially
  // occluded edges)
  for (size_t i = 0; i < total_points; ++i) {
    if (line_pass[i]) {
      display.draw_pixel_at(line_x[i], line_y[i], color);
    }
  }
}

// ---------------------------------------------------------------------------
// LorenzAnimationElement
// ---------------------------------------------------------------------------

void LorenzAnimationElement::step_() {
  float dt = 0.012f;
  float dx = sigma_ * (y_ - x_) * dt;
  float dy = (x_ * (rho_ - z_) - y_) * dt;
  float dz = (x_ * y_ - beta_ * z_) * dt;
  x_ += dx;
  y_ += dy;
  z_ += dz;
}

void LorenzAnimationElement::on_show() {
  AnimationElement::on_show();
  points_.clear();
  x_ = (noise(1, 0, start_time_) - 0.5f) * 10.0f;
  y_ = (noise(2, 0, start_time_) - 0.5f) * 10.0f;
  z_ = noise(3, 0, start_time_) * 20.0f + 10.0f;
  for (int i = 0; i < 150; i++) {
    this->step_();
  }
}

void LorenzAnimationElement::draw(display::Display& display, int width,
                                  int height, float time) {
  for (int i = 0; i < 2; i++) {
    this->step_();
    points_.push_back({x_, y_, z_});
  }
  while (points_.size() > (size_t)length_) {
    points_.pop_front();
  }
  if (points_.size() < 2) return;
  float ax = time * TWO_PI_F * 0.2f;
  float ay = time * TWO_PI_F * 0.3f;
  float cx_r = std::cos(ax), sx_r = std::sin(ax);
  float cy_r = std::cos(ay), sy_r = std::sin(ay);
  float scale = std::min(width, height) * 0.015f;
  float ox = width * 0.5f, oy = height * 0.5f;
  for (size_t i = 1; i < points_.size(); i++) {
    Vec3 p1 = points_[i - 1];
    Vec3 p2 = points_[i];
    p1.z -= 24.0f;
    p2.z -= 24.0f;
    float y1_1 = p1.y * cx_r - p1.z * sx_r;
    float z1_1 = p1.y * sx_r + p1.z * cx_r;
    float x2_1 = p1.x * cy_r + z1_1 * sy_r;
    int px1 = (int)(ox + x2_1 * scale);
    int py1 = (int)(oy - y1_1 * scale);
    float y1_2 = p2.y * cx_r - p2.z * sx_r;
    float z1_2 = p2.y * sx_r + p2.z * cx_r;
    float x2_2 = p2.x * cy_r + z1_2 * sy_r;
    int px2 = (int)(ox + x2_2 * scale);
    int py2 = (int)(oy - y1_2 * scale);
    float p = (float)i / points_.size();
    display.line(px1, py1, px2, py2, get_color_(p));
  }
}

// ---------------------------------------------------------------------------
// CustomAnimationElement
// ---------------------------------------------------------------------------

void CustomAnimationElement::draw(display::Display& display, int width,
                                  int height, float time) {
  if (draw_) {
    draw_(*this, display, width, height, time);
  }
}

bool CustomAnimationElement::is_active() {
  if (is_active_) {
    return is_active_(*this);
  }
  return AnimationElement::is_active();
}

void CustomAnimationElement::test_draw_(CustomAnimationElement& element,
                                        display::Display& display, int width,
                                        int height, float time) {
  int center_x = width / 2;
  int center_y = height / 2;
  float delta = std::fmod(time / 2.0f, TWO_PI_F);
  int x1 = center_x;
  int y1 = (center_y * std::sin(delta)) + center_y;
  for (int i = 1; i <= 0xff; i++) {
    float t = TWO_PI_F * i / 255.0f;
    float r;
    float g;
    float b;
    hsv_to_rgb(360 * i / 0xff, 1.0f, 1.0f, r, g, b);
    int x2 = (center_x * std::sin(2.0f * t)) + center_x;
    int y2 = (center_y * std::sin((3.0f * t) + delta)) + center_y;
    display.line(x1, y1, x2, y2, Color(0xff * r, 0xff * g, 0xff * b));
    x1 = x2;
    y1 = y2;
  }
}

// ---------------------------------------------------------------------------
// CausticsAnimationElement
// ---------------------------------------------------------------------------

void CausticsAnimationElement::draw(display::Display& display, int width,
                                    int height, float time) {
  const float inv_w = 1.0f / (float)width;
  const float inv_h = 1.0f / (float)height;
  for (int y = 0; y < height; ++y) {
    float ny = ((float)y - (float)height * 0.5f) * inv_h * 8.0f;
    for (int x = 0; x < width; ++x) {
      float nx = ((float)x - (float)width * 0.5f) * inv_w * 8.0f;
      float u = nx;
      float v = ny;
      float sum = 0.0f;
      for (int i = 1; i <= 4; ++i) {
        float fi = (float)i;
        float u_next =
            u + (0.6f / fi) * std::sin(fi * v + time * 2.5f + fi * 0.4f);
        float v_next =
            v + (0.6f / fi) * std::cos(fi * u - time * 2.2f + fi * 0.6f);
        u = u_next;
        v = v_next;
        sum += std::abs(std::sin(u) * std::cos(v));
      }
      float intensity = std::pow(sum * 0.25f, 3.0f);
      float val = std::clamp(intensity, 0.0f, 1.0f);
      display.draw_pixel_at(x, y, get_color_(val));
    }
  }
}

// ---------------------------------------------------------------------------
// ClockworkAnimationElement
// ---------------------------------------------------------------------------

void ClockworkAnimationElement::draw(display::Display& display, int width,
                                     int height, float time) {
  const int gear_count = std::clamp(count_, 1, 3);
  const float cx0 =
      (float)width * (0.5f + 0.14f * std::sin(time * TWO_PI_F * 0.25f));
  const float cy0 =
      (float)height * (0.5f + 0.14f * std::cos(time * TWO_PI_F * 0.20f));
  const float base_r = (float)std::min(width, height) * 0.28f;
  const float omega0 = time * TWO_PI_F * 1.1f;

  struct Gear {
    float cx, cy;
    float r;
    int teeth;
    float omega;
    int spokes;
    float color_base;
  };
  Gear gears[3];
  gears[0] = {cx0, cy0, base_r, 12, omega0, 6, 0.35f};

  const float r1 = base_r * (8.0f / 12.0f);
  const float ang1 = 0.45f + time * 0.4f;
  const float d1 = base_r + r1;
  const float omega1 = -omega0 * (12.0f / 8.0f);
  gears[1] = {cx0 + d1 * std::cos(ang1),
              cy0 + d1 * std::sin(ang1),
              r1,
              8,
              omega1,
              4,
              0.65f};

  const float r2 = base_r * (10.0f / 12.0f);
  const float ang2 = 0.45f + PI_F * 0.95f + time * 0.35f;
  const float d2 = base_r + r2;
  const float omega2 = -omega0 * (12.0f / 10.0f);
  gears[2] = {cx0 + d2 * std::cos(ang2),
              cy0 + d2 * std::sin(ang2),
              r2,
              10,
              omega2,
              5,
              0.95f};

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float pixel_val = 0.0f;
      for (int g = 0; g < gear_count; ++g) {
        float dx = (float)x - gears[g].cx;
        float dy = (float)y - gears[g].cy;
        float dist = std::sqrt(dx * dx + dy * dy);
        float r = gears[g].r;
        float tooth_amp = 1.3f;
        if (dist > r + tooth_amp + 0.5f) continue;

        float angle = std::atan2(dy, dx);
        float teeth_wave =
            std::cos((float)gears[g].teeth * (angle - gears[g].omega));
        float outer_r = r + tooth_amp * teeth_wave;

        if (dist > outer_r) continue;

        float base = gears[g].color_base;
        if (dist <= 1.2f) {
          pixel_val = base + 0.04f;
        } else if (dist <= 2.2f) {
          pixel_val = base - 0.04f;
        } else if (dist >= r - tooth_amp - 1.2f) {
          pixel_val = base + 0.05f * teeth_wave;
        } else {
          float spoke_wave = std::abs(std::sin((float)gears[g].spokes * 0.5f *
                                               (angle - gears[g].omega)));
          if (spoke_wave < 0.22f) {
            pixel_val = base - 0.06f;
          }
        }
        if (pixel_val > 0.0f) break;
      }
      if (pixel_val > 0.0f) {
        display.draw_pixel_at(x, y,
                              get_color_(std::clamp(pixel_val, 0.05f, 1.0f)));
      }
    }
  }
}

// ---------------------------------------------------------------------------
// FrizzleAnimationElement
// ---------------------------------------------------------------------------

void FrizzleAnimationElement::draw(display::Display& display, int width,
                                   int height, float time) {
  const int count = std::clamp(count_, 1, 12);
  const uint32_t step = (uint32_t)(time * 18.0f);
  const float cx = (float)width * 0.5f;
  const float cy = (float)height * 0.5f;
  const float max_reach = (float)std::min(width, height);
  for (int i = 0; i < count; ++i) {
    uint32_t seed = step * 101u + (uint32_t)i * 37u;
    if ((hash(seed) % 100u) < 20u) continue;
    float angle = ((float)(hash(seed + 1u) % 1000u) / 1000.0f) * TWO_PI_F;
    float reach =
        (0.35f + 0.45f * ((float)(hash(seed + 2u) % 1000u) / 1000.0f)) *
        max_reach;
    float target_x = cx + std::cos(angle) * reach;
    float target_y = cy + std::sin(angle) * reach;
    float x0 = cx;
    float y0 = cy;
    const int segments = 5;
    for (int s = 1; s <= segments; ++s) {
      float t_seg = (float)s / (float)segments;
      float next_x = cx + (target_x - cx) * t_seg;
      float next_y = cy + (target_y - cy) * t_seg;
      if (s < segments) {
        float jitter_x =
            (((float)(hash(seed + (uint32_t)s * 13u) % 1000u) / 1000.0f) -
             0.5f) *
            5.0f;
        float jitter_y =
            (((float)(hash(seed + (uint32_t)s * 29u) % 1000u) / 1000.0f) -
             0.5f) *
            5.0f;
        next_x += jitter_x;
        next_y += jitter_y;
      }
      float brightness = 1.0f - (t_seg * 0.4f);
      display.line((int)x0, (int)y0, (int)next_x, (int)next_y,
                   get_color_(brightness));
      if (s == 2 && (hash(seed + 77u) % 100u) < 60u) {
        float bx =
            next_x +
            (((float)(hash(seed + 83u) % 1000u) / 1000.0f) - 0.5f) * 8.0f;
        float by =
            next_y +
            (((float)(hash(seed + 89u) % 1000u) / 1000.0f) - 0.5f) * 8.0f;
        display.line((int)next_x, (int)next_y, (int)bx, (int)by,
                     get_color_(0.55f));
      }
      x0 = next_x;
      y0 = next_y;
    }
  }
}

// ---------------------------------------------------------------------------
// SpectralAnimationElement
// ---------------------------------------------------------------------------

void SpectralAnimationElement::draw(display::Display& display, int width,
                                    int height, float time) {
  const int n_wisps = 3;
  for (int y = 0; y < height; ++y) {
    float fy = (float)y;
    float norm_y = fy / (float)height;
    for (int x = 0; x < width; ++x) {
      float fx = (float)x;
      float total_val = 0.0f;
      for (int w = 0; w < n_wisps; ++w) {
        float fw = (float)w;
        float base_x = (float)width * (0.22f + 0.28f * fw);
        float sway = 4.5f * std::sin(norm_y * 4.0f - time * 3.0f + fw * 2.1f) +
                     2.5f * std::cos(norm_y * 7.0f + time * 2.2f + fw * 1.5f);
        float wisp_x = base_x + sway;
        float dx = fx - wisp_x;
        float sigma = 1.3f + 0.8f * (1.0f - norm_y);
        float intensity = std::exp(-(dx * dx) / (2.0f * sigma * sigma)) *
                          (0.3f + 0.7f * norm_y);
        total_val += intensity;
      }
      if (total_val > 0.12f) {
        display.draw_pixel_at(
            x, y, get_color_(std::clamp(total_val * 0.85f, 0.0f, 1.0f)));
      }
    }
  }

  const int num_embers = 16;
  for (int i = 0; i < num_embers; ++i) {
    float fi = (float)i;
    float ember_t = fract(time * 0.8f + fi * (1.0f / (float)num_embers));
    float ey = (1.0f - ember_t) * (float)height;
    float ex = (float)width * (0.15f + 0.7f * noise(i, 1, 100)) +
               3.5f * std::sin(ember_t * 8.0f + fi * 1.7f);
    int px = (int)ex;
    int py = (int)ey;
    if (px >= 0 && px < width && py >= 0 && py < height) {
      float sparkle = 0.6f + 0.4f * std::sin(time * 12.0f + fi * 2.0f);
      display.draw_pixel_at(px, py, get_color_(sparkle));
    }
  }
}

// ---------------------------------------------------------------------------
// SplatterAnimationElement
// ---------------------------------------------------------------------------

void SplatterAnimationElement::on_show() {
  canvas_.clear();
  last_baked_splat_ = -1;
  activation_seed_ = random_uint32();
  AnimationElement::on_show();
}

void SplatterAnimationElement::on_hide() {
  canvas_.clear();
  last_baked_splat_ = -1;
  AnimationElement::on_hide();
}

void SplatterAnimationElement::render_splat_(display::Display* display,
                                             int width, int height,
                                             int splat_id, float tau,
                                             bool to_canvas) {
  uint32_t cycle = (uint32_t)splat_id;
  int k = splat_id % 7;
  uint32_t seed = hash(cycle * 0x9e3779b9u + activation_seed_);
  float cx = (float)width * (0.20f + 0.60f * noise(k, 1, seed));
  float cy = (float)height * (0.20f + 0.60f * noise(k, 2, seed));

  float color_offset = noise(13, 0, activation_seed_);
  float splat_color =
      fract(color_offset + 0.23f * (float)cycle + 0.11f * (float)k);
  Color color = get_color_(splat_color);

  float max_r = 2.4f + 1.4f * noise(k, 3, seed);
  float expand = std::min(1.0f, tau / 0.20f);
  float cur_r = max_r * expand;

  float p1 = noise(k, 4, seed) * TWO_PI_F;
  float p2 = noise(k, 5, seed) * TWO_PI_F;
  float p3 = noise(k, 6, seed) * TWO_PI_F;

  int min_x = std::max(0, (int)(cx - cur_r * 2.0f));
  int max_x = std::min(width - 1, (int)(cx + cur_r * 2.0f));
  int min_y = std::max(0, (int)(cy - cur_r * 2.0f));
  int max_y = std::min(height - 1, (int)(cy + cur_r * 2.0f));

  for (int y = min_y; y <= max_y; ++y) {
    for (int x = min_x; x <= max_x; ++x) {
      float dx = (float)x - cx;
      float dy = (float)y - cy;
      float dist = std::sqrt(dx * dx + dy * dy);
      float angle = std::atan2(dy, dx);
      float star = 1.0f + 0.35f * std::sin(3.0f * angle + p1) +
                   0.30f * std::cos(5.0f * angle + p2) +
                   0.20f * std::sin(7.0f * angle + p3);
      if (dist <= cur_r * star) {
        if (to_canvas) {
          canvas_[y * width + x] = color;
        } else if (display != nullptr) {
          display->draw_pixel_at(x, y, color);
        }
      }
    }
  }

  const int num_droplets = 3;
  for (int d = 0; d < num_droplets; ++d) {
    float fd = (float)d;
    float d_angle = fd * (TWO_PI_F / (float)num_droplets) +
                    0.8f * (noise(d, 7, seed) - 0.5f);
    float travel = 1.0f + 2.0f * noise(d, 8, seed);
    float d_dist = cur_r + travel * (1.0f - std::exp(-tau * 5.0f));
    int dx_p = (int)(cx + std::cos(d_angle) * d_dist);
    int dy_p = (int)(cy + std::sin(d_angle) * d_dist);
    if (dx_p >= 0 && dx_p < width && dy_p >= 0 && dy_p < height) {
      if (to_canvas) {
        canvas_[dy_p * width + dx_p] = color;
      } else if (display != nullptr) {
        display->draw_pixel_at(dx_p, dy_p, color);
      }
    }
  }
}

void SplatterAnimationElement::draw(display::Display& display, int width,
                                    int height, float time) {
  int size = width * height;
  if ((int)canvas_.size() != size) {
    canvas_.assign(size, Color(0, 0, 0, 0));
    last_baked_splat_ = -1;
  }

  const float splat_interval = 2.5f / (float)std::clamp(count_, 1, 8);
  int current_splat = (int)(time / splat_interval);

  // Bake any completed splats into canvas
  for (int s = last_baked_splat_ + 1; s < current_splat; ++s) {
    render_splat_(nullptr, width, height, s, 1.0f, true);
    last_baked_splat_ = s;
  }

  float tau = time - (float)current_splat * splat_interval;
  if (tau >= 0.25f && current_splat > last_baked_splat_) {
    render_splat_(nullptr, width, height, current_splat, 1.0f, true);
    last_baked_splat_ = current_splat;
  }

  // Draw accumulated canvas
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      Color c = canvas_[y * width + x];
      if (c.raw_32 != 0) {
        display.draw_pixel_at(x, y, c);
      }
    }
  }

  // Draw expanding active splat if not yet baked
  if (current_splat > last_baked_splat_) {
    render_splat_(&display, width, height, current_splat, tau, false);
  }
}

// ---------------------------------------------------------------------------
// BlackholeAnimationElement
// ---------------------------------------------------------------------------

void BlackholeAnimationElement::draw(display::Display& display, int width,
                                     int height, float time) {
  const int min_dim = std::min(width, height);
  const int step = std::max(1, min_dim / 32);

  const float orbit_angle = time * 0.85f;
  const float cam_dist = 3.6f;
  // Slowly vary elevation between ~10 deg and ~23 deg
  const float elev = 0.28f + 0.12f * std::sin(time * 0.50f);

  const float cx = cam_dist * std::cos(orbit_angle) * std::cos(elev);
  const float cy = cam_dist * std::sin(elev);
  const float cz = cam_dist * std::sin(orbit_angle) * std::cos(elev);

  const float fwd_x = -cx / cam_dist;
  const float fwd_y = -cy / cam_dist;
  const float fwd_z = -cz / cam_dist;

  const float rx = fwd_z;
  const float rz = -fwd_x;
  const float rlen = std::sqrt(rx * rx + rz * rz);
  const float inv_rlen = 1.0f / (rlen + 1e-6f);
  const float right_x = rx * inv_rlen;
  const float right_z = rz * inv_rlen;

  // true up = cross(right, fwd)
  const float up_x = -right_z * fwd_y;
  const float up_y = right_z * fwd_x - right_x * fwd_z;
  const float up_z = right_x * fwd_y;

  const float rs = 0.38f;
  const float rs_capture = rs * 1.05f;
  const float rs_capture_sq = rs_capture * rs_capture;
  const float r_in = 0.75f;
  const float r_out = 2.40f;
  const float r_in_sq = r_in * r_in;
  const float r_out_sq = r_out * r_out;
  const float inv_r_span = 1.0f / (r_out - r_in);

  const float dt = 0.18f;
  const int n_steps = 38;
  const float fov_scale = 0.85f;
  const float inv_half_dim = 1.0f / ((float)min_dim * 0.5f);

  for (int y = 0; y < height; y += step) {
    const float py =
        ((float)(height - 1) * 0.5f - (float)y) * inv_half_dim * fov_scale;
    for (int x = 0; x < width; x += step) {
      const float px =
          ((float)x - (float)(width - 1) * 0.5f) * inv_half_dim * fov_scale;

      float rdx = fwd_x + px * right_x + py * up_x;
      float rdy = fwd_y + py * up_y;
      float rdz = fwd_z + px * right_z + py * up_z;
      const float inv_len = 1.0f / std::sqrt(rdx * rdx + rdy * rdy + rdz * rdz);
      rdx *= inv_len;
      rdy *= inv_len;
      rdz *= inv_len;

      float pos_x = cx;
      float pos_y = cy;
      float pos_z = cz;

      float val = 0.0f;
      float opacity = 0.0f;
      float min_r = 100.0f;
      bool fell_in = false;

      for (int s = 0; s < n_steps; ++s) {
        const float r2 = pos_x * pos_x + pos_y * pos_y + pos_z * pos_z;
        if (r2 <= rs_capture_sq) {
          fell_in = true;
          break;
        }

        const float r = std::sqrt(r2);
        if (r < min_r) min_r = r;

        // Conserved angular momentum L = pos x rd
        const float lx = pos_y * rdz - pos_z * rdy;
        const float ly = pos_z * rdx - pos_x * rdz;
        const float lz = pos_x * rdy - pos_y * rdx;
        const float l2 = lx * lx + ly * ly + lz * lz;

        // Gravitational light deflection in Schwarzschild metric
        const float bend = 1.5f * rs * l2 / (r2 * r2 * r + 1e-4f);
        rdx -= pos_x * bend * dt;
        rdy -= pos_y * bend * dt;
        rdz -= pos_z * bend * dt;
        const float inv_rd =
            1.0f / std::sqrt(rdx * rdx + rdy * rdy + rdz * rdz);
        rdx *= inv_rd;
        rdy *= inv_rd;
        rdz *= inv_rd;

        const float next_y = pos_y + rdy * dt;

        // Check if ray crosses the equatorial accretion disk plane y = 0
        if ((pos_y > 0.0f && next_y <= 0.0f) ||
            (pos_y < 0.0f && next_y >= 0.0f)) {
          const float t_c = -pos_y / (next_y - pos_y + 1e-9f);
          const float cx_p = pos_x + rdx * dt * t_c;
          const float cz_p = pos_z + rdz * dt * t_c;
          const float r_disk_sq = cx_p * cx_p + cz_p * cz_p;

          if (r_disk_sq >= r_in_sq && r_disk_sq <= r_out_sq) {
            const float r_disk = std::sqrt(r_disk_sq);
            const float t_d = (r_disk - r_in) * inv_r_span;
            const float prof =
                std::sin(t_d * PI_F) * std::pow(r_in / r_disk, 0.65f);
            const float phi = std::atan2(cz_p, cx_p);
            const float omega =
                std::sqrt(rs / (r_disk_sq * r_disk + 0.1f)) * 3.2f;
            const float arm1 =
                std::sin(3.0f * (phi - omega * time) - r_disk * 4.0f);
            const float arm2 =
                std::sin(7.0f * (phi - 0.7f * omega * time) + r_disk * 3.0f);
            const float swirl = 0.45f + 0.35f * arm1 + 0.20f * arm2;
            const float v_rot = 0.65f * std::sqrt(rs / r_disk);
            const float v_proj = (cz_p * rdx - cx_p * rdz) / r_disk * v_rot;
            const float doppler =
                std::max(0.08f, std::pow(1.0f + v_proj, 3.2f));
            const float emission = prof * swirl * doppler;

            val += emission * (1.0f - opacity);
            opacity += 0.85f * (1.0f - opacity);
            if (opacity >= 0.95f) {
              break;
            }
          }
        }

        pos_x += rdx * dt;
        pos_y = next_y;
        pos_z += rdz * dt;
      }

      // Thin photon ring encircling the event horizon shadow
      if (!fell_in && min_r < rs * 2.0f) {
        const float d_ph = std::abs(min_r - rs * 1.5f);
        const float ring = std::exp(-(d_ph * d_ph) / 0.008f) * 0.45f;
        val += ring * (1.0f - opacity);
      }

      float p = 0.0f;
      if (val > 0.015f) {
        p = std::clamp(std::pow(val / 1.15f, 0.80f), 0.0f, 1.0f);
      }

      Color c = get_color_(p);
      if (step == 1) {
        display.draw_pixel_at(x, y, c);
      } else {
        display.filled_rectangle(x, y, step, step, c);
      }
    }
  }
}

}  // namespace esphome::elements
