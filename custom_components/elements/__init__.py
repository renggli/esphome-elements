import esphome.codegen as cg
import esphome.config_validation as cv
import esphome.core as core
from esphome.components import color, display, font, time
from esphome.const import (CONF_COLOR, CONF_DISPLAY, CONF_FONT, CONF_FORMAT,
                           CONF_ID, CONF_TIME, CONF_VISIBLE)

# conf names

CONF_ELEMENT = "element"

CONF_ANALOG_CLOCK = "analog_clock"
CONF_DIGITAL_CLOCK = "digital_clock"
CONF_ERROR = "error"
CONF_SCHEDULER = "scheduler"

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

def validate_color(value):
    if not isinstance(value, str) or value[0] != '#':
        raise cv.Invalid("Invalid value for hex color")
    return color.hex_color(value[1:])

async def generate_color(value):
    if isinstance(value, core.ID):
        return await cg.get_variable(value)
    else:
        r, g, b = value
        return cg.ArrayInitializer(r, g, b)

COLOR_SCHEMA = cv.Any(
    cv.use_id(color.ColorStruct),
    validate_color,
)

# elements

ELEMENT_BASE_SCHEMA = cv.Schema({})

ELEMENT_CLOCK_SCHEMA = cv.Schema({
    cv.Required(CONF_TIME): cv.use_id(time.RealTimeClock),
})

ELEMENT_SCHEMA = cv.typed_schema({
    CONF_ANALOG_CLOCK: ELEMENT_CLOCK_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(AnalogClockElement),
        cv.Optional('hour_marks_start'): cv.float_range(0, 1),
        cv.Optional('hour_marks_end'): cv.float_range(0, 1),
        cv.Optional('hour_marks_color'): COLOR_SCHEMA,
        cv.Optional('hour_marks_visible'): cv.boolean,
        cv.Optional('trihoral_marks_start'): cv.float_range(0, 1),
        cv.Optional('trihoral_marks_end'): cv.float_range(0, 1),
        cv.Optional('trihoral_marks_color'): COLOR_SCHEMA,
        cv.Optional('trihoral_marks_visible'): cv.boolean,
        cv.Optional('second_arm_start'): cv.float_range(-1, 1),
        cv.Optional('second_arm_end'): cv.float_range(-1, 1),
        cv.Optional('second_arm_color'): COLOR_SCHEMA,
        cv.Optional('second_arm_visible'): cv.boolean,
        cv.Optional('second_arm_smooth'): cv.boolean,
        cv.Optional('minute_arm_start'): cv.float_range(-1, 1),
        cv.Optional('minute_arm_end'): cv.float_range(-1, 1),
        cv.Optional('minute_arm_color'): COLOR_SCHEMA,
        cv.Optional('minute_arm_visible'): cv.boolean,
        cv.Optional('minute_arm_smooth'): cv.boolean,
        cv.Optional('hour_arm_start'): cv.float_range(-1, 1),
        cv.Optional('hour_arm_end'): cv.float_range(-1, 1),
        cv.Optional('hour_arm_color'): COLOR_SCHEMA,
        cv.Optional('hour_arm_visible'): cv.boolean,
        cv.Optional('hour_arm_smooth'): cv.boolean,
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
    for name in [CONF_COLOR, 'hour_marks_color', 'trihoral_marks_color', 'second_arm_color', 'minute_arm_color', 'hour_arm_color']:
        if conf := config.get(name):
            value = await generate_color(conf)
            cg.add(getattr(var, "set_" + name)(value))

    # literals
    for name in ['hour_marks_start', 'hour_marks_end', 'trihoral_marks_start', 'trihoral_marks_end', 'second_arm_start', 'second_arm_end', 'minute_arm_start', 'minute_arm_end', 'hour_arm_start', 'hour_arm_end', 'hour_marks_visible', 'trihoral_marks_visible', 'second_arm_visible', 'second_arm_smooth', 'minute_arm_visible', 'minute_arm_smooth', 'hour_arm_visible', 'hour_arm_smooth']:
        value = config.get(name)
        if value != None:
            cg.add(getattr(var, 'set_' + name)(value))

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
