import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import color, display, font, time
from esphome.const import (CONF_COLOR, CONF_DISPLAY, CONF_FONT, CONF_FORMAT,
                           CONF_ID, CONF_MESSAGE, CONF_TIME)

CONF_ELEMENT = "element"

CONF_ANALOG_CLOCK = "analog_clock"
CONF_DIGITAL_CLOCK = "digital_clock"
CONF_ERROR = "error"
CONF_SCHEDULER = "scheduler"

elements_ns = cg.esphome_ns.namespace('elements')
ElementComponent = elements_ns.class_('ElementComponent', cg.Component)

Element = elements_ns.class_('Element', cg.Component)
ClockElement = elements_ns.class_('ClockElement', Element)
AnalogClockElement = elements_ns.class_('AnalogClockElement', ClockElement)
DigitalClockElement = elements_ns.class_('DigitalClockElement', ClockElement)
ErrorElement = elements_ns.class_('ErrorElement', Element)
SchedulerElement = elements_ns.class_('SchedulerElement', Element)

ELEMENT_BASE_SCHEMA = cv.Schema({})

ELEMENT_CLOCK_SCHEMA = cv.Schema({
    cv.Required(CONF_TIME): cv.use_id(time.RealTimeClock),
})

ELEMENT_SCHEMA = cv.typed_schema({
    CONF_ANALOG_CLOCK: ELEMENT_CLOCK_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(AnalogClockElement),
        # hour_marks
        # trihoral_marks
        # second_arm
        # minute_arm
        # hour_arm
    }),
    CONF_DIGITAL_CLOCK: ELEMENT_CLOCK_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(DigitalClockElement),
        cv.Required(CONF_FONT): cv.use_id(font.Font),
        cv.Optional(CONF_COLOR): cv.use_id(color.ColorStruct),
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

    if conf_time := config.get(CONF_TIME):
        var_time = await cg.get_variable(conf_time)
        cg.add(var.set_time(var_time))

    if conf_font := config.get(CONF_FONT):
        var_font = await cg.get_variable(conf_font)
        cg.add(var.set_font(var_font))

    if conf_color := config.get(CONF_COLOR):
        var_color = await cg.get_variable(conf_color)
        cg.add(var.set_color(var_color))

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
