import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display, font, image, time
from esphome.const import (CONF_BACKGROUND_COLOR, CONF_COLOR, CONF_DISPLAY,
                           CONF_DURATION, CONF_FONT, CONF_FORMAT, CONF_ID,
                           CONF_LAMBDA, CONF_TEXT, CONF_TIME)
from . import color as color_module
from .color import (COLOR_SCHEMA, COLOR_SCHEME_SCHEMA, CONF_COLOR_SCHEME,
                    color_scheme_schema, color_scheme_to_code, color_to_code)
from .element import Element, ElementComponent, elements_ns
from .geometry import anchor_schema, anchor_to_code
from .container_element import (
    ContainerElement, HorizontalElement, OverlayElement,
    PriorityElement, RandomElement, SequenceElement, VerticalElement,
    CONF_ACTIVE_MODE, CONF_ELEMENTS, ACTIVE_MODE,
)
from .delegate_element import (
    DelegateElement, DelayElement, TimeoutElement,
    CONF_COUNT, CONF_ELEMENT,
)
from .text_element import (
    TextElement, StaticTextElement, DynamicTextElement, TimeTextElement,
    CONF_ALIGN, CONF_ANCHOR, CONF_SCROLL_MODE, CONF_SCROLL_SPEED,
    TEXT_ALIGN, SCROLL_MODE, TEXT_ELEMENT_SCHEMA,
)
from .image_element import ImageElement, IMAGE_ALIGN, IMAGE_ELEMENT_SCHEMA
from .animation_element import (
    CONF_DENSITY, CONF_LENGTH, CONF_SPEED, CONF_STRENGTH,
    ANIMATION_ELEMENT_SCHEMA,
)
from .clock_element import (
    ClockElement, AnalogClockOptions,
    CONF_END, CONF_HOUR_HAND, CONF_HOUR_MARKERS,
    CONF_MINUTE_HAND, CONF_MINUTE_MARKERS, CONF_QUARTER_MARKERS,
    CONF_SECOND_HAND, CONF_SMOOTH, CONF_START,
    analog_clock_options_schema, analog_clock_options_to_code,
    CLOCK_ELEMENT_SCHEMA,
)
from .custom_element import (
    CustomElement,
    CONF_LAMBDA_DRAW, CONF_LAMBDA_IS_ACTIVE, CONF_LAMBDA_ON_HIDE,
    CONF_LAMBDA_ON_PREV, CONF_LAMBDA_ON_NEXT, CONF_LAMBDA_ON_SHOW,
    CUSTOM_ELEMENT_SCHEMA,
)

AUTO_LOAD = ['display', 'image']
CODEOWNERS = ['@renggli']
DEPENDENCIES = ['display']
MULTI_CONF = True

# conf names
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# types
CONF_ANIMATION = 'animation'
CONF_CLOCK = 'clock'
CONF_CUSTOM = 'custom'
CONF_DELAY = 'delay'
CONF_DYNAMIC_TEXT = 'dynamic_text'
CONF_HORIZONTAL = 'horizontal'
CONF_IMAGE = 'image'
CONF_OVERLAY = 'overlay'
CONF_PRIORITY = 'priority'
CONF_RANDOM = 'random'
CONF_SEQUENCE = 'sequence'
CONF_STATIC_TEXT = 'static_text'
CONF_TIME_TEXT = 'time_text'
CONF_TIMEOUT = 'timeout'
CONF_VERTICAL = 'vertical'

# classes
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

display_ns = cg.esphome_ns.namespace('display')
DisplayRef = display_ns.class_('Display').operator("ref")

# other

nullptr = cg.esphome_ns.class_('nullptr')
nullopt = cg.esphome_ns.class_('nullopt')

# elements

def element_schema(value):            # to avoid recursion
    return ELEMENT_SCHEMA(value)


BASE_ELEMENT_SCHEMA = cv.Schema({})

CONTAINER_ELEMENT_SCHEMA = BASE_ELEMENT_SCHEMA.extend({
    cv.Required(CONF_ELEMENTS): cv.All(
        cv.ensure_list(element_schema),
        cv.Length(min=1),
    ),
    cv.Optional(CONF_ACTIVE_MODE): cv.enum(ACTIVE_MODE, upper=True, space='_'),
})

DELEGATE_ELEMENT_SCHEMA = BASE_ELEMENT_SCHEMA.extend({
    cv.Required(CONF_ELEMENT): element_schema,
})

ELEMENT_SCHEMA = cv.typed_schema({
    CONF_ANIMATION: ANIMATION_ELEMENT_SCHEMA,
    CONF_CLOCK: CLOCK_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(ClockElement),
    }),
    CONF_CUSTOM: CUSTOM_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(CustomElement),
    }),
    CONF_DELAY: DELEGATE_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(DelayElement),
        cv.Optional(CONF_COUNT): cv.positive_int,
    }),
    CONF_DYNAMIC_TEXT: TEXT_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(DynamicTextElement),
        cv.Required(CONF_LAMBDA): cv.returning_lambda,
    }),
    CONF_HORIZONTAL: CONTAINER_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(HorizontalElement),
    }),
    CONF_IMAGE: IMAGE_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(ImageElement),
    }),
    CONF_OVERLAY: CONTAINER_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(OverlayElement),
    }),
    CONF_PRIORITY: CONTAINER_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(PriorityElement),
    }),
    CONF_RANDOM: CONTAINER_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(RandomElement),
    }),
    CONF_SEQUENCE: CONTAINER_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(SequenceElement),
    }),
    CONF_STATIC_TEXT: TEXT_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(StaticTextElement),
        cv.Optional(CONF_TEXT): cv.string,
    }),
    CONF_TIME_TEXT: TEXT_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(TimeTextElement),
        cv.Required(CONF_TIME): cv.use_id(time.RealTimeClock),
        cv.Optional(CONF_FORMAT, default='%H:%M:%S'): cv.string,
    }),
    CONF_TIMEOUT: DELEGATE_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(TimeoutElement),
        cv.Optional(CONF_DURATION): cv.positive_time_period_milliseconds,
    }),
    CONF_VERTICAL: CONTAINER_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(VerticalElement),
    }),
})

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ElementComponent),
    cv.Optional(CONF_DISPLAY): cv.use_id(display.Display),
    cv.Required(CONF_ELEMENT): ELEMENT_SCHEMA,
}).extend(cv.COMPONENT_SCHEMA)

async def element_to_code(config, component, parent=nullptr):
    var = cg.new_Pvariable(config[CONF_ID], component, parent)

    # literals
    for name in [CONF_DURATION, CONF_FORMAT, CONF_TEXT, CONF_ALIGN,
                 CONF_SCROLL_MODE, CONF_SCROLL_SPEED, CONF_ACTIVE_MODE, CONF_COUNT]:
        if value := config.get(name):
            cg.add(getattr(var, 'set_' + name)(value))

    # color scheme - requires object instantiation
    if color_scheme_config := config.get(CONF_COLOR_SCHEME):
        scheme = await color_scheme_to_code(color_scheme_config)
        cg.add(var.set_color_scheme(scheme))

    # floats that may legitimately be 0.0 (falsy), checked explicitly
    for name in [CONF_SPEED, CONF_DENSITY, CONF_LENGTH, CONF_STRENGTH]:
        if (value := config.get(name)) is not None:
            cg.add(getattr(var, 'set_' + name)(value))

    # references
    for name in [CONF_TIME, CONF_FONT, CONF_IMAGE]:
        if conf := config.get(name):
            value = await cg.get_variable(conf)
            cg.add(getattr(var, 'set_' + name)(value))

    # colors
    for name in [CONF_COLOR, CONF_BACKGROUND_COLOR]:
        if conf := config.get(name):
            value = await color_to_code(conf)
            cg.add(getattr(var, 'set_' + name)(value))

    # lambdas
    ElementRef = config['id'].type.operator("ref")
    for name in [CONF_LAMBDA]:
        if conf := config.get(name):
            value = await cg.process_lambda(conf,
                                            [(ElementRef, "element")],
                                            return_type=cg.std_string)
            cg.add(getattr(var, 'set_' + name)(value))
    for name in [CONF_LAMBDA_IS_ACTIVE]:
        if conf := config.get(name):
            value = await cg.process_lambda(conf,
                                            [(ElementRef, "element")],
                                            return_type=cg.bool_)
            cg.add(getattr(var, 'set_' + name)(value))
    for name in [CONF_LAMBDA_ON_SHOW, CONF_LAMBDA_ON_HIDE,
                 CONF_LAMBDA_ON_PREV, CONF_LAMBDA_ON_NEXT]:
        if conf := config.get(name):
            value = await cg.process_lambda(conf,
                                            [(ElementRef, "element")],
                                            return_type=cg.void)
            cg.add(getattr(var, 'set_' + name)(value))
    for name in [CONF_LAMBDA_DRAW]:
        if conf := config.get(name):
            value = await cg.process_lambda(conf,
                                            [(ElementRef, "element"),
                                             (DisplayRef, "display")],
                                            return_type=cg.void)
            cg.add(getattr(var, 'set_' + name)(value))

    # elements
    for name in [CONF_ELEMENT]:
        if conf := config.get(name):
            value = await element_to_code(conf, component, var)
            cg.add(getattr(var, 'set_' + name)(value))
    if conf := config.get(CONF_ELEMENTS):
        for conf_item in conf:
            value = await element_to_code(conf_item, component, var)
            cg.add(var.add_element(value))

    # clock
    for name in [CONF_MINUTE_MARKERS, CONF_HOUR_MARKERS, CONF_QUARTER_MARKERS,
                 CONF_SECOND_HAND, CONF_MINUTE_HAND, CONF_HOUR_HAND]:
        if conf := config.get(name):
            value = await analog_clock_options_to_code(conf)
            cg.add(getattr(var, 'set_' + name)(value))

    # anchor
    for name in [CONF_ANCHOR]:
        if conf := config.get(name):
            value = await anchor_to_code(conf)
            cg.add(getattr(var, 'set_' + name)(value))

    return var

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if display_config := config.get(CONF_DISPLAY):
        drawing_display = await cg.get_variable(display_config)
        cg.add(var.set_display(drawing_display))

    if root_config := config.get(CONF_ELEMENT):
        root_element = await element_to_code(root_config, var)
        cg.add(var.set_root(root_element))
