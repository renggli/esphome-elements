import esphome.codegen as cg
import esphome.config_validation as cv
import esphome.core as core
from esphome.components import color, display, font, time
from esphome.const import (CONF_COLOR, CONF_DISPLAY, CONF_FONT, CONF_FORMAT,
                           CONF_ID, CONF_TIME, CONF_TYPE, CONF_VISIBLE)

# conf names

CONF_ELEMENT = "element"

CONF_ANALOG_CLOCK = "analog_clock"
CONF_DIGITAL_CLOCK = "digital_clock"
CONF_ERROR = "error"
CONF_SCHEDULER = "scheduler"

CONF_START = "start"
CONF_END = "end"
CONF_SMOOTH = "smooth"

CONF_MINUTE_MARKERS = "minute_markers"
CONF_HOUR_MARKERS = "hour_markers"
CONF_QUARTER_MARKERS = "quarter_markers"
CONF_SECOND_HAND = "second_hand"
CONF_MINUTE_HAND = "minute_hand"
CONF_HOUR_HAND = "hour_hand"

# classes

elements_ns = cg.esphome_ns.namespace('elements')
ElementComponent = elements_ns.class_('ElementComponent', cg.Component)

Element = elements_ns.class_('Element')
ClockElement = elements_ns.class_('ClockElement', Element)
AnalogClockElement = elements_ns.class_('AnalogClockElement', ClockElement)
DigitalClockElement = elements_ns.class_('DigitalClockElement', ClockElement)
ErrorElement = elements_ns.class_('ErrorElement', Element)
SchedulerElement = elements_ns.class_('SchedulerElement', Element)

# color struct

def color_validation(value):
    if not isinstance(value, str) or value[0] != '#':
        raise cv.Invalid("Invalid value for hex color")
    return color.hex_color(value[1:])

async def color_to_code(value):
    if isinstance(value, core.ID):
        return await cg.get_variable(value)
    else:
        r, g, b = value
        return cg.ArrayInitializer(r, g, b, 0)

COLOR_SCHEMA = cv.Any(
    cv.use_id(color.ColorStruct),
    color_validation,
)

# analog clock options


AnalogClockOptions = elements_ns.class_('AnalogClockOptions')

def analog_clock_options_schema(start, end, color, visible=True, smooth=None):
    return cv.Schema({
        cv.Optional(CONF_START, default=start): cv.float_range(-1, 1),
        cv.Optional(CONF_END, default=end): cv.float_range(-1, 1),
        cv.Optional(CONF_COLOR, default=color): COLOR_SCHEMA,
        cv.Optional(CONF_VISIBLE, default=visible): cv.boolean,
    } | ({
        cv.Optional(CONF_SMOOTH, default=smooth): cv.boolean,
    } if smooth is not None else {}))

async def analog_clock_options_to_code(value):
    return cg.StructInitializer(
        AnalogClockOptions,
        (CONF_START, value.get(CONF_START)),
        (CONF_END, value.get(CONF_END)),
        (CONF_COLOR, await color_to_code(value.get(CONF_COLOR))),
        (CONF_VISIBLE, value.get(CONF_VISIBLE)),
        (CONF_SMOOTH, value.get(CONF_SMOOTH)),
    )

# elements

ELEMENT_BASE_SCHEMA = cv.Schema({})

ELEMENT_CLOCK_SCHEMA = cv.Schema({
    cv.Required(CONF_TIME): cv.use_id(time.RealTimeClock),
})

ELEMENT_SCHEMA = cv.typed_schema({
    CONF_ANALOG_CLOCK: ELEMENT_CLOCK_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(AnalogClockElement),
        cv.Optional(CONF_MINUTE_MARKERS, default={}):
            analog_clock_options_schema(0.9, 1.0, '#0000ff', visible=False),
        cv.Optional(CONF_HOUR_MARKERS, default={}):
            analog_clock_options_schema(0.9, 1.0, '#0000dd'),
        cv.Optional(CONF_QUARTER_MARKERS, default={}):
            analog_clock_options_schema(0.8, 1.0, '#0000ff'),
        cv.Optional(CONF_SECOND_HAND, default={}):
            analog_clock_options_schema(0.0, 0.8, '#ff0000', smooth=True),
        cv.Optional(CONF_MINUTE_HAND, default={}):
            analog_clock_options_schema(0.0, 1.0, '#ffffff', smooth=False),
        cv.Optional(CONF_HOUR_HAND, default={}):
            analog_clock_options_schema(0.0, 0.6, '#ffffff', smooth=True),
    }),
    CONF_DIGITAL_CLOCK: ELEMENT_CLOCK_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(DigitalClockElement),
        cv.Required(CONF_FONT): cv.use_id(font.Font),
        cv.Optional(CONF_COLOR): COLOR_SCHEMA,
        cv.Optional(CONF_FORMAT): cv.string,
    }),
    CONF_ERROR: ELEMENT_BASE_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(ErrorElement),
    }),
    CONF_SCHEDULER: ELEMENT_BASE_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(SchedulerElement),
    }),
})

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ElementComponent),
    cv.Optional(CONF_DISPLAY): cv.use_id(display.Display),
    cv.Optional(CONF_ELEMENT): ELEMENT_SCHEMA,
}).extend(cv.COMPONENT_SCHEMA)

async def element_to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    # references
    for name in [CONF_TIME, CONF_FONT]:
        if conf := config.get(name):
            value = await cg.get_variable(conf)
            cg.add(getattr(var, "set_" + name)(value))

    # colors
    for name in [CONF_COLOR]:
        if conf := config.get(name):
            value = await color_to_code(conf)
            cg.add(getattr(var, "set_" + name)(value))

    # analog clock
    if config[CONF_TYPE] == CONF_ANALOG_CLOCK:
        for name in [CONF_MINUTE_MARKERS, CONF_HOUR_MARKERS,
                     CONF_QUARTER_MARKERS, CONF_SECOND_HAND, CONF_MINUTE_HAND,
                     CONF_HOUR_HAND]:
            value = await analog_clock_options_to_code(config.get(name))
            cg.add(getattr(var, "set_" + name)(value))

    return var

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    if display_config := config.get(CONF_DISPLAY):
        drawing_display = await cg.get_variable(display_config)
        cg.add(var.set_display(drawing_display))

    if root_config := config.get(CONF_ELEMENT):
        root_element = await element_to_code(root_config)
        cg.add(var.set_root(root_element))

    await cg.register_component(var, config)
