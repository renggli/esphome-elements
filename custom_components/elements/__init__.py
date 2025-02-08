import esphome.codegen as cg
import esphome.config_validation as cv
import esphome.core as core
from esphome.components import color, display, font, time
from esphome.const import (CONF_COLOR, CONF_DISPLAY, CONF_DURATION, CONF_FONT,
                           CONF_FORMAT, CONF_ID, CONF_TIME, CONF_TYPE,
                           CONF_VISIBLE)

# conf names

CONF_ELEMENT = "element"
CONF_ELEMENTS = "elements"

CONF_ANALOG_CLOCK = "analog_clock"
CONF_DIGITAL_CLOCK = "digital_clock"
CONF_ERROR = "error"
CONF_FLOW = "flow"
CONF_HORIZONTAL = "horizontal"
CONF_OVERLAY = "overlay"
CONF_SCHEDULER = "scheduler"
CONF_SEQUENCE = "sequence"
CONF_VERTICAL = "vertical"

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

ContainerElement = elements_ns.class_('ContainerElement', Element)
FlowElement = elements_ns.class_('FlowElement', ContainerElement)
HorizontalElement = elements_ns.class_('HorizontalElement', ContainerElement)
OverlayElement = elements_ns.class_('OverlayElement', ContainerElement)
SequenceElement = elements_ns.class_('SequenceElement', ContainerElement)
VerticalElement = elements_ns.class_('VerticalElement', ContainerElement)

ErrorElement = elements_ns.class_('ErrorElement', Element)
SchedulerElement = elements_ns.class_('SchedulerElement', Element)

# color struct

def color_validation(value):
    if not isinstance(value, str) or value[0] != '#':
        raise cv.Invalid("Invalid value for hex color")
    return color.hex_color(value[1:])

async def color_to_code(config):
    if isinstance(config, core.ID):
        return await cg.get_variable(config)
    else:
        r, g, b = config
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

async def analog_clock_options_to_code(config):
    return cg.StructInitializer(
        AnalogClockOptions,
        (CONF_START, config.get(CONF_START)),
        (CONF_END, config.get(CONF_END)),
        (CONF_COLOR, await color_to_code(config.get(CONF_COLOR))),
        (CONF_VISIBLE, config.get(CONF_VISIBLE)),
        (CONF_SMOOTH, config.get(CONF_SMOOTH)),
    )

# elements

def element_schema(value):            # to avoid recursion
    return ELEMENT_SCHEMA(value)

BASE_ELEMENT_SCHEMA = cv.Schema({})

CLOCK_ELEMENT_SCHEMA = cv.Schema({
    cv.Required(CONF_TIME): cv.use_id(time.RealTimeClock),
})

CONTAINER_ELEMENT_SCHEMA = cv.Schema({
    cv.Required(CONF_ELEMENTS): cv.All(
        cv.ensure_list(element_schema),
        cv.Length(min=1),
    )
})

ELEMENT_SCHEMA = cv.typed_schema({
    CONF_ANALOG_CLOCK: CLOCK_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(AnalogClockElement),
        cv.Optional(CONF_MINUTE_MARKERS, default={}):
            analog_clock_options_schema(0.9, 1.0, '#0000ff', visible=False),
        cv.Optional(CONF_HOUR_MARKERS, default={}):
            analog_clock_options_schema(0.9, 1.0, '#0000dd'),
        cv.Optional(CONF_QUARTER_MARKERS, default={}):
            analog_clock_options_schema(0.8, 1.0, '#0000ff'),
        cv.Optional(CONF_SECOND_HAND, default={}):
            analog_clock_options_schema(0.0, 0.8, '#ff0000', smooth=False),
        cv.Optional(CONF_MINUTE_HAND, default={}):
            analog_clock_options_schema(0.0, 1.0, '#ffffff', smooth=False),
        cv.Optional(CONF_HOUR_HAND, default={}):
            analog_clock_options_schema(0.0, 0.6, '#ffffff', smooth=True),
    }),
    CONF_DIGITAL_CLOCK: CLOCK_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(DigitalClockElement),
        cv.Required(CONF_FONT): cv.use_id(font.Font),
        cv.Optional(CONF_COLOR): COLOR_SCHEMA,
        cv.Optional(CONF_FORMAT): cv.string,
    }),
    CONF_OVERLAY: CONTAINER_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(OverlayElement),
    }),
    CONF_HORIZONTAL: CONTAINER_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(HorizontalElement),
    }),
    CONF_VERTICAL: CONTAINER_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(VerticalElement),
    }),
    CONF_SEQUENCE: CONTAINER_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(SequenceElement),
        cv.Optional(CONF_DURATION, default="5s"): cv.positive_time_period_milliseconds,
    }),
    CONF_ERROR: BASE_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(ErrorElement),
    }),
    CONF_SCHEDULER: BASE_ELEMENT_SCHEMA.extend({
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

    # literals
    for name in [CONF_DURATION, CONF_FORMAT]:
        if value := config.get(name):
            cg.add(getattr(var, "set_" + name)(value))

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

    # elements
    if conf := config.get(CONF_ELEMENTS):
        for conf_item in conf:
            value = await element_to_code(conf_item)
            cg.add(var.add_element(value))

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
