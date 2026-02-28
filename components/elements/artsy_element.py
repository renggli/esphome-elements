import esphome.codegen as cg
import esphome.config_validation as cv
from .element import Element, elements_ns
from .color import COLOR_SCHEME_SCHEMA, CONF_COLOR_SCHEME, color_scheme_schema

ArtsyElement = elements_ns.class_('ArtsyElement', Element)

artsy_pattern_ns = elements_ns.namespace('ArtsyPattern')
ArtsyPattern = artsy_pattern_ns.enum('ArtsyPattern')

# conf names
CONF_DENSITY = 'density'
CONF_PATTERN = 'pattern'
CONF_SPEED = 'speed'
CONF_STRENGTH = 'strength'

ARTSY_PATTERN = {
    'METABALLS': ArtsyPattern.METABALLS,
    'AURORA': ArtsyPattern.AURORA,
    'KALEIDOSCOPE': ArtsyPattern.KALEIDOSCOPE,
    'PLASMA': ArtsyPattern.PLASMA,
    'RIPPLES': ArtsyPattern.RIPPLES,
    'SPIRAL': ArtsyPattern.SPIRAL,
    'VORONOI': ArtsyPattern.VORONOI,
    'INTERFERENCE': ArtsyPattern.INTERFERENCE,
    'JULIA': ArtsyPattern.JULIA,
    'MATRIX': ArtsyPattern.MATRIX,
    'GRADIENT': ArtsyPattern.GRADIENT,
    'FIRE': ArtsyPattern.FIRE,
    'TUNNEL': ArtsyPattern.TUNNEL,
    'WAVE': ArtsyPattern.WAVE,
    'STARS': ArtsyPattern.STARS,
}

ARTSY_ELEMENT_SCHEMA = cv.Schema({
    cv.Required(CONF_COLOR_SCHEME): color_scheme_schema,
    cv.Optional(CONF_PATTERN, default='METABALLS'):
        cv.enum(ARTSY_PATTERN, upper=True, space='_'),
    cv.Optional(CONF_SPEED, default=1.0): cv.float_range(min=0.01, max=10.0),
    cv.Optional(CONF_DENSITY, default=1.0): cv.float_range(min=0.0, max=10.0),
    cv.Optional(CONF_STRENGTH, default=1.0): cv.float_range(min=0.1, max=10.0),
})
