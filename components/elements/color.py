import colorsys
import random
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
CONF_CS_SWEEP = "sweep"
CONF_CS_TO = "to"
CONF_CS_VALUE_SCALE = "value_scale"
CONF_CS_FRACTION = "fraction"

# Color


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


# Color Scheme

ColorScheme = shared.elements_ns.class_("ColorScheme")
StaticColorScheme = shared.elements_ns.class_("StaticColorScheme")
GradientColorScheme = shared.elements_ns.class_("GradientColorScheme")
SequenceColorScheme = shared.elements_ns.class_("SequenceColorScheme")
MirrorColorScheme = shared.elements_ns.class_("MirrorColorScheme")
InverseColorScheme = shared.elements_ns.class_("InverseColorScheme")

make_monochromatic = shared.elements_ns.namespace("").operation("make_monochromatic")
make_analogous = shared.elements_ns.namespace("").operation("make_analogous")
make_complementary = shared.elements_ns.namespace("").operation("make_complementary")
make_split_complementary = shared.elements_ns.namespace("").operation(
    "make_split_complementary"
)
make_triadic = shared.elements_ns.namespace("").operation("make_triadic")
make_square = shared.elements_ns.namespace("").operation("make_square")


def color_scheme_schema(value):  # forward-declare for recursion
    return COLOR_SCHEME_SCHEMA(value)


CS_BASE_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_CS_FRACTION, default=1.0): cv.float_range(min=0.001),
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


def _new_id(prefix):
    return f"{prefix}_{random.randint(0, 0xFFFFFF):06x}"


async def _hsv_from_color(config):
    """Extract H, S, V floats from a COLOR_SCHEMA config value."""
    r, g, b, _ = (
        config
        if isinstance(config, tuple)
        else (config.r / 255.0, config.g / 255.0, config.b / 255.0, 0)
    )
    if isinstance(config, tuple):
        r, g, b = r / 255.0, g / 255.0, b / 255.0
    h, s, v = colorsys.rgb_to_hsv(r, g, b)
    return h * 360.0, s, v


async def color_scheme_to_code(config):
    scheme_type = config.get("type")

    # -- Primitives --
    if scheme_type == "static":
        var = cg.new_Pvariable(core.ID(_new_id("cs_static"), True, StaticColorScheme))
        cg.add(var.set_color(await color_to_code(config.get(CONF_COLOR))))
        return var
    elif scheme_type == "gradient":
        var = cg.new_Pvariable(core.ID(_new_id("cs_grad"), True, GradientColorScheme))
        cg.add(var.set_from(await color_to_code(config.get(CONF_CS_FROM))))
        cg.add(var.set_to(await color_to_code(config.get(CONF_CS_TO))))
        return var
    elif scheme_type == "mirror":
        var = cg.new_Pvariable(core.ID(_new_id("cs_mirror"), True, MirrorColorScheme))
        child = await color_scheme_to_code(config.get(CONF_CS_SCHEME))
        cg.add(var.set_scheme(child))
        return var
    elif scheme_type == "inverse":
        var = cg.new_Pvariable(core.ID(_new_id("cs_inverse"), True, InverseColorScheme))
        child = await color_scheme_to_code(config.get(CONF_CS_SCHEME))
        cg.add(var.set_scheme(child))
        return var
    elif scheme_type == "sequence":
        var = cg.new_Pvariable(core.ID(_new_id("cs_seq"), True, SequenceColorScheme))
        for child_config in config.get(CONF_CS_SCHEMES):
            child_fraction = child_config.get(CONF_CS_FRACTION, 1.0)
            child = await color_scheme_to_code(child_config)
            cg.add(var.add_scheme(child, child_fraction))
        return var

    # -- Palette shorthands (call C++ factory functions directly) --
    # We extract actual H/S/V values in Python so we can call the C++ free functions.
    h_deg, s_f, v_f = await _hsv_from_color(config.get(CONF_COLOR))

    if scheme_type == "monochromatic":
        min_v = config.get(CONF_CS_MIN_BRIGHTNESS)
        return cg.RawExpression(
            f"esphome::elements::make_monochromatic({h_deg:.3f}f, {s_f:.3f}f, {v_f:.3f}f, {min_v:.3f}f)"
        )

    elif scheme_type == "analogous":
        sweep = config.get(CONF_CS_SWEEP)
        return cg.RawExpression(
            f"esphome::elements::make_analogous({h_deg:.3f}f, {s_f:.3f}f, {v_f:.3f}f, {sweep:.3f}f)"
        )

    elif scheme_type == "complementary":
        sweep = config.get(CONF_CS_SWEEP)
        return cg.RawExpression(
            f"esphome::elements::make_complementary({h_deg:.3f}f, {s_f:.3f}f, {v_f:.3f}f, {sweep:.3f}f)"
        )

    elif scheme_type == "split_complementary":
        sweep = config.get(CONF_CS_SWEEP)
        return cg.RawExpression(
            f"esphome::elements::make_split_complementary({h_deg:.3f}f, {s_f:.3f}f, {v_f:.3f}f, {sweep:.3f}f)"
        )

    elif scheme_type == "triadic":
        sweep = config.get(CONF_CS_SWEEP)
        return cg.RawExpression(
            f"esphome::elements::make_triadic({h_deg:.3f}f, {s_f:.3f}f, {v_f:.3f}f, {sweep:.3f}f)"
        )

    elif scheme_type == "square":
        sweep = config.get(CONF_CS_SWEEP)
        return cg.RawExpression(
            f"esphome::elements::make_square({h_deg:.3f}f, {s_f:.3f}f, {v_f:.3f}f, {sweep:.3f}f)"
        )

    raise ValueError(f"Unknown color scheme type: {scheme_type}")
