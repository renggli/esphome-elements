#include "color.h"

#include <algorithm>
#include <cmath>

namespace esphome::elements {

// ----------------------------------------------------------------------------
// Utility implementations
// ----------------------------------------------------------------------------

Color hsv_to_color(float h, float s, float v) {
  float r, g, b;
  if (s <= 0.0f) {
    r = g = b = v;
  } else {
    float hh = std::fmod(h, 360.0f) / 60.0f;
    int i = (int) hh;
    float f = hh - i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));
    switch (i % 6) {
      case 0: {
        r = v;
        g = t;
        b = p;
        break;
      }
      case 1: {
        r = q;
        g = v;
        b = p;
        break;
      }
      case 2: {
        r = p;
        g = v;
        b = t;
        break;
      }
      case 3: {
        r = p;
        g = q;
        b = v;
        break;
      }
      case 4: {
        r = t;
        g = p;
        b = v;
        break;
      }
      default: {
        r = v;
        g = p;
        b = q;
        break;
      }
    }
  }
  return Color((uint8_t) (r * 255.0f), (uint8_t) (g * 255.0f), (uint8_t) (b * 255.0f));
}

void color_to_hsv(Color c, float &h, float &s, float &v) {
  float r = c.r / 255.0f, g = c.g / 255.0f, b = c.b / 255.0f;
  float cmax = std::max({r, g, b}), cmin = std::min({r, g, b});
  float delta = cmax - cmin;
  v = cmax;
  s = (cmax > 0.0f) ? delta / cmax : 0.0f;
  if (delta < 1e-6f) {
    h = 0.0f;
    return;
  }
  if (cmax == r) {
    h = 60.0f * std::fmod((g - b) / delta, 6.0f);
  } else if (cmax == g) {
    h = 60.0f * ((b - r) / delta + 2.0f);
  } else {
    h = 60.0f * ((r - g) / delta + 4.0f);
  }
  if (h < 0.0f) {
    h += 360.0f;
  }
}

float lerp_hue(float from_h, float to_h, float t) {
  float dh = to_h - from_h;
  if (dh > 180.0f)
    dh -= 360.0f;
  if (dh < -180.0f)
    dh += 360.0f;
  return std::fmod(from_h + dh * t + 360.0f, 360.0f);
}

// ColorScheme base

Color ColorScheme::get_color(float p) {
  float h, s, v;
  get_hsv(std::clamp(p, 0.0f, 1.0f), h, s, v);
  return hsv_to_color(h, s, v);
}

// StaticColorScheme

void StaticColorScheme::set_hsv(float h, float s, float v) {
  h_ = h;
  s_ = s;
  v_ = v;
}

void StaticColorScheme::set_color(Color c) { color_to_hsv(c, h_, s_, v_); }

void StaticColorScheme::get_hsv(float /*p*/, float &h, float &s, float &v) {
  h = h_;
  s = s_;
  v = v_;
}

// GradientColorScheme

void GradientColorScheme::set_from_hsv(float h, float s, float v) {
  from_h_ = h;
  from_s_ = s;
  from_v_ = v;
}

void GradientColorScheme::set_to_hsv(float h, float s, float v) {
  to_h_ = h;
  to_s_ = s;
  to_v_ = v;
}

void GradientColorScheme::set_from(Color c) { color_to_hsv(c, from_h_, from_s_, from_v_); }

void GradientColorScheme::set_to(Color c) { color_to_hsv(c, to_h_, to_s_, to_v_); }

void GradientColorScheme::get_hsv(float p, float &h, float &s, float &v) {
  p = std::clamp(p, 0.0f, 1.0f);
  h = lerp_hue(from_h_, to_h_, p);
  s = from_s_ + (to_s_ - from_s_) * p;
  v = from_v_ + (to_v_ - from_v_) * p;
}

// MirrorColorScheme

void MirrorColorScheme::set_scheme(ColorScheme *scheme) { scheme_ = scheme; }

void MirrorColorScheme::get_hsv(float p, float &h, float &s, float &v) {
  if (scheme_ == nullptr) {
    h = s = v = 0;
    return;
  }
  float mirrored = (p < 0.5f) ? (2.0f * p) : (2.0f * (1.0f - p));
  scheme_->get_hsv(mirrored, h, s, v);
}

// InverseColorScheme

void InverseColorScheme::set_scheme(ColorScheme *scheme) { scheme_ = scheme; }

void InverseColorScheme::get_hsv(float p, float &h, float &s, float &v) {
  if (scheme_ == nullptr) {
    h = s = v = 0;
    return;
  }
  scheme_->get_hsv(1.0f - std::clamp(p, 0.0f, 1.0f), h, s, v);
}

// SequenceColorScheme

void SequenceColorScheme::add_scheme(ColorScheme *scheme) { schemes_.push_back(scheme); }

void SequenceColorScheme::get_hsv(float p, float &h, float &s, float &v) {
  if (schemes_.empty()) {
    h = s = v = 0;
    return;
  }
  p = std::clamp(p, 0.0f, 1.0f);
  float scaled = p * (float) schemes_.size();
  int idx = std::clamp((int) scaled, 0, (int) schemes_.size() - 1);
  float local_p = scaled - (float) idx;
  schemes_[idx]->get_hsv(local_p, h, s, v);
}

// Palette factory implementations

static ColorScheme *hue_segment(float from_h, float s, float from_v, float to_h, float to_v) {
  auto *scheme = new GradientColorScheme();
  scheme->set_from_hsv(from_h, s, from_v);
  scheme->set_to_hsv(to_h, s, to_v);
  return scheme;
}

ColorScheme *make_monochromatic(float h, float s, float v, float min_v) { return hue_segment(h, s, min_v, h, v); }

ColorScheme *make_analogous(float h, float s, float v, float sweep_deg) {
  return hue_segment(h - sweep_deg, s, v, h + sweep_deg, v);
}

ColorScheme *make_complementary(float h, float s, float v, float sweep_deg) {
  auto *seq = new SequenceColorScheme();
  seq->add_scheme(hue_segment(h - sweep_deg, s, v, h + sweep_deg, v));
  seq->add_scheme(hue_segment(h + 180.0f - sweep_deg, s, v, h + 180.0f + sweep_deg, v));
  return (ColorScheme *) seq;
}

ColorScheme *make_split_complementary(float h, float s, float v, float sweep_deg) {
  auto *seq = new SequenceColorScheme();
  seq->add_scheme(hue_segment(h - sweep_deg, s, v, h + sweep_deg, v));
  seq->add_scheme(hue_segment(h + 150.0f - sweep_deg, s, v, h + 150.0f + sweep_deg, v));
  seq->add_scheme(hue_segment(h + 210.0f - sweep_deg, s, v, h + 210.0f + sweep_deg, v));
  return (ColorScheme *) seq;
}

ColorScheme *make_triadic(float h, float s, float v, float sweep_deg) {
  auto *seq = new SequenceColorScheme();
  for (float offset : {0.0f, 120.0f, 240.0f})
    seq->add_scheme(hue_segment(h + offset - sweep_deg, s, v, h + offset + sweep_deg, v));
  return (ColorScheme *) seq;
}

ColorScheme *make_square(float h, float s, float v, float sweep_deg) {
  auto *seq = new SequenceColorScheme();
  for (float offset : {0.0f, 90.0f, 180.0f, 270.0f})
    seq->add_scheme(hue_segment(h + offset - sweep_deg, s, v, h + offset + sweep_deg, v));
  return (ColorScheme *) seq;
}

}  // namespace esphome::elements
