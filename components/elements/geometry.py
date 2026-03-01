import esphome.codegen as cg
import esphome.config_validation as cv
from .element import elements_ns


CONF_ALIGN = "align"
CONF_ANCHOR = "anchor"

# Point

Point = elements_ns.class_("Point")

CONF_POINT_X = "x"
CONF_POINT_Y = "y"


def point_schema(x, y, validation):
    return cv.Schema(
        {
            cv.Optional(CONF_POINT_X, default=x): validation,
            cv.Optional(CONF_POINT_Y, default=y): validation,
        }
    )


async def point_to_code(config):
    return cg.StructInitializer(
        Point,
        (CONF_POINT_X, config.get(CONF_POINT_X)),
        (CONF_POINT_Y, config.get(CONF_POINT_Y)),
    )


# Anchor

Anchor = elements_ns.class_("Anchor")

CONF_ANCHOR_OFFSET = "offset"
CONF_ANCHOR_FRACTION = "fraction"


def anchor_schema(offset_x=0, offset_y=0, fraction_x=0.5, fraction_y=0.5):
    return cv.Schema(
        {
            cv.Optional(CONF_ANCHOR_OFFSET, default={}): point_schema(
                x=offset_x,
                y=offset_y,
                validation=cv.int_,
            ),
            cv.Optional(CONF_ANCHOR_FRACTION, default={}): point_schema(
                x=fraction_x,
                y=fraction_y,
                validation=cv.float_range(min=0.0, max=1.0),
            ),
        }
    )


async def anchor_to_code(config):
    return cg.StructInitializer(
        Anchor,
        (CONF_ANCHOR_OFFSET, await point_to_code(config.get(CONF_ANCHOR_OFFSET))),
        (CONF_ANCHOR_FRACTION, await point_to_code(config.get(CONF_ANCHOR_FRACTION))),
    )
