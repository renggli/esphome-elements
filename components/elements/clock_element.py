import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import time
from esphome.const import CONF_COLOR, CONF_VISIBLE
from .element import Element, elements_ns
from .color import COLOR_SCHEMA, color_to_code

ClockElement = elements_ns.class_('ClockElement', Element)
AnalogClockOptions = elements_ns.class_('AnalogClockOptions')

# conf names
CONF_END = 'end'
CONF_HOUR_HAND = 'hour_hand'
CONF_HOUR_MARKERS = 'hour_markers'
CONF_MINUTE_HAND = 'minute_hand'
CONF_MINUTE_MARKERS = 'minute_markers'
CONF_QUARTER_MARKERS = 'quarter_markers'
CONF_SECOND_HAND = 'second_hand'
CONF_SMOOTH = 'smooth'
CONF_START = 'start'


def analog_clock_options_schema(start, end, color, visible=True, smooth=None):
    return cv.Schema({
        cv.Optional(CONF_START, default=start): cv.float_range(-1, 1),
        cv.Optional(CONF_END, default=end): cv.float_range(-1, 1),
        cv.Optional(CONF_COLOR, default=color): COLOR_SCHEMA,
        cv.Optional(CONF_VISIBLE, default=visible): cv.boolean,
    } | ({
        cv.Optional(CONF_SMOOTH, default=smooth): cv.boolean,
    } if smooth is not None else {}))


async def analog_clock_options_to_code(config):
    return cg.StructInitializer(
        AnalogClockOptions,
        (CONF_START, config.get(CONF_START)),
        (CONF_END, config.get(CONF_END)),
        (CONF_COLOR, await color_to_code(config.get(CONF_COLOR))),
        (CONF_VISIBLE, config.get(CONF_VISIBLE)),
        (CONF_SMOOTH, config.get(CONF_SMOOTH)),
    )


CLOCK_ELEMENT_SCHEMA = cv.Schema({
    cv.Required('time'): cv.use_id(time.RealTimeClock),
    cv.Optional(CONF_MINUTE_MARKERS, default={}):\
        analog_clock_options_schema(0.95, 1.00, '#0000ff', visible=False),
    cv.Optional(CONF_HOUR_MARKERS, default={}):\
        analog_clock_options_schema(0.90, 1.00, '#0000ff'),
    cv.Optional(CONF_QUARTER_MARKERS, default={}):\
        analog_clock_options_schema(0.75, 1.00, '#0000ff'),
    cv.Optional(CONF_SECOND_HAND, default={}):\
        analog_clock_options_schema(0.00, 0.75, '#ff0000', smooth=True),
    cv.Optional(CONF_MINUTE_HAND, default={}):\
        analog_clock_options_schema(0.00, 0.95, '#ffffff', smooth=False),
    cv.Optional(CONF_HOUR_HAND, default={}):\
        analog_clock_options_schema(0.00, 0.66, '#ffffff', smooth=True),
})
