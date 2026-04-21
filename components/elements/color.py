"""Color scheme support for the elements component.

All color schemes are fully resolved in Python at code-generation time and
emitted as a precomputed ``Color[]`` lookup table (``ColorScheme``).  No HSV
maths or virtual dispatch is needed in the generated C++; the runtime just
reads from the table with optional linear interpolation.
"""

import colorsys

import esphome.codegen as cg
import esphome.config_validation as cv
import esphome.core as core
from esphome.components import color
from esphome.const import CONF_COLOR
from . import shared

CONF_CS_FROM = "from"
CONF_CS_HUE_OFFSET = "hue_offset"
CONF_CS_MIN_BRIGHTNESS = "min_brightness"
CONF_CS_SATURATION_SCALE = "saturation_scale"
CONF_CS_SCHEME = "scheme"
CONF_CS_SCHEMES = "schemes"
CONF_CS_STEPS = "steps"
CONF_CS_SWEEP = "sweep"
CONF_CS_TO = "to"
CONF_CS_VALUE_SCALE = "value_scale"
CONF_CS_FRACTION = "fraction"

# ---------------------------------------------------------------------------
# Color parsing
# ---------------------------------------------------------------------------


def _color_validation(value):
    if not isinstance(value, str) or value[0] != "#":
        raise cv.Invalid("Invalid value for hex color")
    value = value[1:]
    try:
        if len(value) == 8:
            return (
                int(value[0:2], 16),
                int(value[2:4], 16),
                int(value[4:6], 16),
                int(value[6:8], 16),
            )
        elif len(value) == 6:
            return (
                int(value[0:2], 16),
                int(value[2:4], 16),
                int(value[4:6], 16),
                0,
            )
        elif len(value) == 4:
            return (
                17 * int(value[0], 16),
                17 * int(value[1], 16),
                17 * int(value[2], 16),
                17 * int(value[3], 16),
            )
        elif len(value) == 3:
            return (
                17 * int(value[0], 16),
                17 * int(value[1], 16),
                17 * int(value[2], 16),
                0,
            )
    except ValueError as exc:
        raise cv.Invalid("Color must be hexadecimal") from exc


_RGB_SCHEMA = cv.Schema(
    {
        cv.Required("r"): cv.int_range(min=0, max=255),
        cv.Required("g"): cv.int_range(min=0, max=255),
        cv.Required("b"): cv.int_range(min=0, max=255),
        cv.Optional("w", default=0): cv.int_range(min=0, max=255),
    }
)


def _rgb_validation(value):
    value = _RGB_SCHEMA(value)
    return (value["r"], value["g"], value["b"], value["w"])


_HSV_SCHEMA = cv.Schema(
    {
        cv.Required("h"): cv.float_range(min=0.0, max=360.0),
        cv.Required("s"): cv.float_range(min=0.0, max=1.0),
        cv.Required("v"): cv.float_range(min=0.0, max=1.0),
    }
)


def _hsv_validation(value):
    value = _HSV_SCHEMA(value)
    r, g, b = colorsys.hsv_to_rgb(value["h"] / 360.0, value["s"], value["v"])
    return (round(r * 255), round(g * 255), round(b * 255), 0)


COLOR_SCHEMA = cv.Any(
    cv.use_id(color.ColorStruct),
    _color_validation,
    _rgb_validation,
    _hsv_validation,
)


async def color_to_code(config):
    if isinstance(config, core.ID):
        return await cg.get_variable(config)
    else:
        r, g, b, w = config
        return cg.ArrayInitializer(r, g, b, w)


# ---------------------------------------------------------------------------
# Internal helpers – high-precision Python colour math
# ---------------------------------------------------------------------------


def _parse_color_to_rgb(config) -> tuple[float, float, float]:
    """Return (r, g, b) ∈ [0, 1] from a COLOR_SCHEMA config value."""
    if isinstance(config, tuple):
        r, g, b, _ = config
        return r / 255.0, g / 255.0, b / 255.0
    # ESPHome ColorStruct ID – can't resolve at config time; treat as white.
    return 1.0, 1.0, 1.0


def _parse_color_to_hsv(config) -> tuple[float, float, float]:
    """Return (h_deg, s, v) from a COLOR_SCHEMA config value."""
    r, g, b = _parse_color_to_rgb(config)
    h, s, v = colorsys.rgb_to_hsv(r, g, b)
    return h * 360.0, s, v


def _lerp_hue(from_h: float, to_h: float, t: float) -> float:
    """Interpolate hue along the shortest arc (degrees)."""
    dh = (to_h - from_h + 180.0) % 360.0 - 180.0
    return (from_h + dh * t) % 360.0


def _hsv_to_rgb8(h_deg: float, s: float, v: float) -> tuple[int, int, int]:
    """Convert HSV (h in degrees) to rounded 8-bit RGB."""
    r, g, b = colorsys.hsv_to_rgb(h_deg / 360.0, s, v)
    return (round(r * 255), round(g * 255), round(b * 255))


# ---------------------------------------------------------------------------
# Colour-gradient samplers
#
# Each sampler is a callable  p ∈ [0, 1] → (h_deg, s, v)
# ---------------------------------------------------------------------------


def _sampler_static(h_deg, s, v):
    return lambda p: (h_deg, s, v)


def _sampler_gradient(from_h, from_s, from_v, to_h, to_s, to_v):
    def sample(p):
        h = _lerp_hue(from_h, to_h, p)
        s = from_s + (to_s - from_s) * p
        v = from_v + (to_v - from_v) * p
        return (h, s, v)

    return sample


def _sampler_mirror(inner):
    def sample(p):
        mirrored = 2.0 * p if p < 0.5 else 2.0 * (1.0 - p)
        return inner(mirrored)

    return sample


def _sampler_inverse(inner):
    return lambda p: inner(1.0 - p)


def _sampler_sequence(items):
    """items: list of (weight, sampler) – weights need not sum to 1."""
    total = sum(w for w, _ in items)
    cumulative = []
    acc = 0.0
    for w, s in items:
        acc += w / total
        cumulative.append((acc, s))

    def sample(p):
        p = max(0.0, min(1.0, p))
        prev = 0.0
        for cum, s in cumulative:
            if p <= cum:
                span = cum - prev
                local_p = (p - prev) / span if span > 1e-9 else 0.0
                return s(local_p)
            prev = cum
        return cumulative[-1][1](1.0)

    return sample


def _hue_segment_sampler(from_h, s, from_v, to_h, to_v):
    return _sampler_gradient(from_h, s, from_v, to_h, s, to_v)


# ---------------------------------------------------------------------------
# Build a sampler from a COLOR_SCHEME_SCHEMA config dict
# ---------------------------------------------------------------------------


def _scheme_config_to_sampler(config):
    scheme_type = config.get("type")

    if scheme_type == "static":
        h, s, v = _parse_color_to_hsv(config[CONF_COLOR])
        return _sampler_static(h, s, v)

    if scheme_type == "gradient":
        fh, fs, fv = _parse_color_to_hsv(config[CONF_CS_FROM])
        th, ts, tv = _parse_color_to_hsv(config[CONF_CS_TO])
        return _sampler_gradient(fh, fs, fv, th, ts, tv)

    if scheme_type == "mirror":
        return _sampler_mirror(_scheme_config_to_sampler(config[CONF_CS_SCHEME]))

    if scheme_type == "inverse":
        return _sampler_inverse(_scheme_config_to_sampler(config[CONF_CS_SCHEME]))

    if scheme_type == "sequence":
        items = [
            (child.get(CONF_CS_FRACTION, 1.0), _scheme_config_to_sampler(child))
            for child in config[CONF_CS_SCHEMES]
        ]
        return _sampler_sequence(items)

    # Palette shorthands
    h, s, v = _parse_color_to_hsv(config[CONF_COLOR])

    if scheme_type == "monochromatic":
        min_v = config[CONF_CS_MIN_BRIGHTNESS]
        return _hue_segment_sampler(h, s, min_v, h, v)

    if scheme_type == "analogous":
        sweep = config[CONF_CS_SWEEP]
        return _hue_segment_sampler(h - sweep, s, v, h + sweep, v)

    if scheme_type == "complementary":
        sweep = config[CONF_CS_SWEEP]
        return _sampler_sequence(
            [
                (
                    1.0,
                    _hue_segment_sampler(
                        h + offset - sweep, s, v, h + offset + sweep, v
                    ),
                )
                for offset in (0.0, 180.0)
            ]
        )

    if scheme_type == "split_complementary":
        sweep = config[CONF_CS_SWEEP]
        return _sampler_sequence(
            [
                (
                    1.0,
                    _hue_segment_sampler(
                        h + offset - sweep, s, v, h + offset + sweep, v
                    ),
                )
                for offset in (0.0, 150.0, 210.0)
            ]
        )

    if scheme_type == "triadic":
        sweep = config[CONF_CS_SWEEP]
        return _sampler_sequence(
            [
                (
                    1.0,
                    _hue_segment_sampler(
                        h + offset - sweep, s, v, h + offset + sweep, v
                    ),
                )
                for offset in (0.0, 120.0, 240.0)
            ]
        )

    if scheme_type == "square":
        sweep = config[CONF_CS_SWEEP]
        return _sampler_sequence(
            [
                (
                    1.0,
                    _hue_segment_sampler(
                        h + offset - sweep, s, v, h + offset + sweep, v
                    ),
                )
                for offset in (0.0, 90.0, 180.0, 270.0)
            ]
        )

    raise ValueError(f"Unknown color scheme type: {scheme_type!r}")


# ---------------------------------------------------------------------------
# Color Scheme C++ class references (for generated code only)
# ---------------------------------------------------------------------------

ColorScheme = shared.elements_ns.class_("ColorScheme")


# ---------------------------------------------------------------------------
# Schema definitions
# ---------------------------------------------------------------------------


def color_scheme_schema(value):  # forward-declare for recursion
    return COLOR_SCHEME_SCHEMA(value)


CS_BASE_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_CS_FRACTION, default=1.0): cv.float_range(min=0.001),
        cv.Optional(CONF_CS_STEPS, default=32): cv.int_range(min=1, max=1024),
    }
)

STATIC_CS_SCHEMA = CS_BASE_SCHEMA.extend(
    {
        cv.Required(CONF_COLOR): COLOR_SCHEMA,
    }
)

GRADIENT_CS_SCHEMA = CS_BASE_SCHEMA.extend(
    {
        cv.Required(CONF_CS_FROM): COLOR_SCHEMA,
        cv.Required(CONF_CS_TO): COLOR_SCHEMA,
    }
)

SEQUENCE_CS_SCHEMA = CS_BASE_SCHEMA.extend(
    {
        cv.Required(CONF_CS_SCHEMES): cv.All(
            cv.ensure_list(color_scheme_schema),
            cv.Length(min=1),
        ),
    }
)

MIRROR_CS_SCHEMA = CS_BASE_SCHEMA.extend(
    {
        cv.Required(CONF_CS_SCHEME): color_scheme_schema,
    }
)

INVERSE_CS_SCHEMA = CS_BASE_SCHEMA.extend(
    {
        cv.Required(CONF_CS_SCHEME): color_scheme_schema,
    }
)

MONOCHROMATIC_CS_SCHEMA = CS_BASE_SCHEMA.extend(
    {
        cv.Required(CONF_COLOR): COLOR_SCHEMA,
        cv.Optional(CONF_CS_MIN_BRIGHTNESS, default=0.2): cv.float_range(
            min=0.0, max=1.0
        ),
    }
)

ANALOGOUS_CS_SCHEMA = CS_BASE_SCHEMA.extend(
    {
        cv.Required(CONF_COLOR): COLOR_SCHEMA,
        cv.Optional(CONF_CS_SWEEP, default=30.0): cv.float_range(min=0.0, max=180.0),
    }
)

COMPLEMENTARY_CS_SCHEMA = CS_BASE_SCHEMA.extend(
    {
        cv.Required(CONF_COLOR): COLOR_SCHEMA,
        cv.Optional(CONF_CS_SWEEP, default=20.0): cv.float_range(min=0.0, max=90.0),
    }
)

SPLIT_COMPLEMENTARY_CS_SCHEMA = CS_BASE_SCHEMA.extend(
    {
        cv.Required(CONF_COLOR): COLOR_SCHEMA,
        cv.Optional(CONF_CS_SWEEP, default=20.0): cv.float_range(min=0.0, max=45.0),
    }
)

TRIADIC_CS_SCHEMA = CS_BASE_SCHEMA.extend(
    {
        cv.Required(CONF_COLOR): COLOR_SCHEMA,
        cv.Optional(CONF_CS_SWEEP, default=20.0): cv.float_range(min=0.0, max=45.0),
    }
)

SQUARE_CS_SCHEMA = CS_BASE_SCHEMA.extend(
    {
        cv.Required(CONF_COLOR): COLOR_SCHEMA,
        cv.Optional(CONF_CS_SWEEP, default=15.0): cv.float_range(min=0.0, max=45.0),
    }
)

COLOR_SCHEME_SCHEMA = cv.typed_schema(
    {
        "static": STATIC_CS_SCHEMA,
        "gradient": GRADIENT_CS_SCHEMA,
        "sequence": SEQUENCE_CS_SCHEMA,
        "mirror": MIRROR_CS_SCHEMA,
        "inverse": INVERSE_CS_SCHEMA,
        "monochromatic": MONOCHROMATIC_CS_SCHEMA,
        "analogous": ANALOGOUS_CS_SCHEMA,
        "complementary": COMPLEMENTARY_CS_SCHEMA,
        "split_complementary": SPLIT_COMPLEMENTARY_CS_SCHEMA,
        "triadic": TRIADIC_CS_SCHEMA,
        "square": SQUARE_CS_SCHEMA,
    },
    lower=True,
)


# ---------------------------------------------------------------------------
# Code-generation
# ---------------------------------------------------------------------------

_color_scheme_counter = 0


async def color_scheme_to_code(config):
    """Emit a ColorScheme* with a precomputed LUT from the Python sampler."""
    global _color_scheme_counter
    _color_scheme_counter += 1
    scheme_id = core.ID(
        f"color_scheme_{_color_scheme_counter}",
        is_declaration=True,
        type=ColorScheme,
    )

    steps = config.get(CONF_CS_STEPS, 32)
    sampler = _scheme_config_to_sampler(config)

    var = cg.new_Pvariable(scheme_id)

    points = [0.0] if steps == 1 else [i / (steps - 1) for i in range(steps)]
    entries = []
    for p in points:
        h, s, v = sampler(p)
        r, g, b = _hsv_to_rgb8(h, s, v)
        entries.append(f"Color({r}, {g}, {b})")

    colors_init = "{" + ", ".join(entries) + "}"
    cg.add(cg.RawExpression(f"{var}->set_colors({colors_init})"))

    return var
