import esphome.codegen as cg
import esphome.config_validation as cv
from .element import elements_ns

Point = elements_ns.class_('Point')
Anchor = elements_ns.class_('Anchor')


def point_schema(x, y, validation):
    return cv.Schema({
        cv.Optional('x', default=x): validation,
        cv.Optional('y', default=y): validation,
    })


async def point_to_code(config):
    return cg.ArrayInitializer(config.get('x'), config.get('y'))


def anchor_schema(offset_x=0, offset_y=0, fraction_x=0.5, fraction_y=0.5):
    return cv.Schema({
        cv.Optional('offset', default={}): point_schema(
            x=offset_x,
            y=offset_y,
            validation=cv.int_,
        ),
        cv.Optional('fraction', default={}): point_schema(
            x=fraction_x,
            y=fraction_y,
            validation=cv.float_range(min=0.0, max=1.0),
        ),
    })


async def anchor_to_code(config):
    return cg.StructInitializer(
        Anchor,
        ('offset', await point_to_code(config.get('offset'))),
        ('fraction', await point_to_code(config.get('fraction'))),
    )
