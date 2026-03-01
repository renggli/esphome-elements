import esphome.codegen as cg
import esphome.config_validation as cv

from . import shared

CONF_ALIGN = "align"
CONF_ANCHOR = "anchor"
CONF_OFFSET = "offset"
CONF_FRACTION = "fraction"
CONF_POINT_X = "x"
CONF_POINT_Y = "y"

# Point

Point = shared.elements_ns.class_("Point")


def point_schema(x, y, validation):
    return cv.Schema(
        {
            cv.Optional(CONF_POINT_X, default=x): validation,
            cv.Optional(CONF_POINT_Y, default=y): validation,
        }
    )


async def point_to_code(config, type_):
    return cg.StructInitializer(
        Point.template(type_),
        (CONF_POINT_X, config.get(CONF_POINT_X)),
        (CONF_POINT_Y, config.get(CONF_POINT_Y)),
    )


# Anchor

Anchor = shared.elements_ns.class_("Anchor")


def anchor_schema(offset_x=0, offset_y=0, fraction_x=0.5, fraction_y=0.5):
    return cv.Schema(
        {
            cv.Optional(CONF_OFFSET, default={}): point_schema(
                x=offset_x,
                y=offset_y,
                validation=cv.int_,
            ),
            cv.Optional(CONF_FRACTION, default={}): point_schema(
                x=fraction_x,
                y=fraction_y,
                validation=cv.float_range(min=0.0, max=1.0),
            ),
        }
    )


async def anchor_to_code(config):
    return cg.StructInitializer(
        Anchor,
        (CONF_OFFSET, await point_to_code(config.get(CONF_OFFSET), cg.int_)),
        (CONF_FRACTION, await point_to_code(config.get(CONF_FRACTION), cg.float_)),
    )
