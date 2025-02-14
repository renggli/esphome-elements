import esphome.codegen as cg
import esphome.config_validation as cv
import esphome.core as core
from esphome.components import color, display, font, image, time
from esphome.const import (CONF_BACKGROUND_COLOR, CONF_COLOR, CONF_DISPLAY,
                           CONF_DURATION, CONF_FONT, CONF_FORMAT, CONF_ID,
                           CONF_LAMBDA, CONF_TIME, CONF_TYPE,
                           CONF_UPDATE_INTERVAL, CONF_VISIBLE)

AUTO_LOAD = ["display", "image"]
CODEOWNERS = ['@renggli']
DEPENDENCIES = ['display']
MULTI_CONF = True

# conf names
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# properties
CONF_ALIGN = 'align'
CONF_ANCHOR = 'anchor'
CONF_ELEMENT = 'element'
CONF_ELEMENTS = 'elements'
CONF_END = 'end'
CONF_HOUR_HAND = 'hour_hand'
CONF_HOUR_MARKERS = 'hour_markers'
CONF_MINUTE_HAND = 'minute_hand'
CONF_MINUTE_MARKERS = 'minute_markers'
CONF_QUARTER_MARKERS = 'quarter_markers'
CONF_SCROLL_MODE = 'scroll_mode'
CONF_SCROLL_SPEED = 'scroll_speed'
CONF_SECOND_HAND = 'second_hand'
CONF_SMOOTH = 'smooth'
CONF_START = 'start'

# types
CONF_ANALOG_CLOCK = 'analog_clock'
CONF_DIGITAL_CLOCK = 'digital_clock'
CONF_HORIZONTAL = 'horizontal'
CONF_IMAGE = 'image'
CONF_OVERLAY = 'overlay'
CONF_SEQUENCE = 'sequence'
CONF_TEXT = 'text'
CONF_VERTICAL = 'vertical'

# classes
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# elements

elements_ns = cg.esphome_ns.namespace('elements')
ElementComponent = elements_ns.class_('ElementComponent', cg.Component)

Context = elements_ns.class_('Context')
ContextConstRef = Context.operator('ref').operator('const')

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

TextElement = elements_ns.class_('TextElement', Element)
ImageElement = elements_ns.class_('ImageElement', Element)

# color struct

def color_validation(value):
    if not isinstance(value, str) or value[0] != '#':
        raise cv.Invalid('Invalid value for hex color')
    value = value[1:]
    try:
        if len(value) == 8:
            return (int(value[0:2], 16), int(value[2:4], 16),
                    int(value[4:6], 16), int(value[6:8], 16))
        elif len(value) == 6:
            return (int(value[0:2], 16), int(value[2:4], 16),
                    int(value[4:6], 16), 0)
        elif len(value) == 4:
            return (17 * int(value[0], 16), 17 * int(value[1], 16),
                    17 * int(value[2], 16), 17 * int(value[3], 16))
        elif len(value) == 3:
            return (17 * int(value[0], 16), 17 * int(value[1], 16),
                    17 * int(value[2], 16), 0)
    except ValueError as exc:
        raise cv.Invalid('Color must be hexadecimal') from exc

async def color_to_code(config):
    if isinstance(config, core.ID):
        return await cg.get_variable(config)
    else:
        r, g, b, w = config
        return cg.ArrayInitializer(r, g, b, w)

COLOR_SCHEMA = cv.Any(
    cv.use_id(color.ColorStruct),
    color_validation,
)

# text and image align enums

display_ns = cg.esphome_ns.namespace('display')

text_align_ns = display_ns.namespace('TextAlign')
TextAlign = text_align_ns.enum('TextAlign')

TEXT_ALIGN = {
    'TOP': TextAlign.TOP,
    'CENTER_VERTICAL': TextAlign.CENTER_VERTICAL,
    'BASELINE': TextAlign.BASELINE,
    'BOTTOM': TextAlign.BOTTOM,
    'LEFT': TextAlign.LEFT,
    'CENTER_HORIZONTAL': TextAlign.CENTER_HORIZONTAL,
    'RIGHT': TextAlign.RIGHT,
    'TOP_LEFT': TextAlign.TOP_LEFT,
    'TOP_CENTER': TextAlign.TOP_CENTER,
    'TOP_RIGHT': TextAlign.TOP_RIGHT,
    'CENTER_LEFT': TextAlign.CENTER_LEFT,
    'CENTER': TextAlign.CENTER,
    'CENTER_RIGHT': TextAlign.CENTER_RIGHT,
    'BASELINE_LEFT': TextAlign.BASELINE_LEFT,
    'BASELINE_CENTER': TextAlign.BASELINE_CENTER,
    'BASELINE_RIGHT': TextAlign.BASELINE_RIGHT,
    'BOTTOM_LEFT': TextAlign.BOTTOM_LEFT,
    'BOTTOM_CENTER': TextAlign.BOTTOM_CENTER,
    'BOTTOM_RIGHT': TextAlign.BOTTOM_RIGHT,
}

image_align_ns = display_ns.namespace('ImageAlign')
ImageAlign = image_align_ns.enum('ImageAlign')

IMAGE_ALIGN = {
    'TOP': ImageAlign.TOP,
    'CENTER_VERTICAL': ImageAlign.CENTER_VERTICAL,
    'BOTTOM': ImageAlign.BOTTOM,
    'LEFT': ImageAlign.LEFT,
    'CENTER_HORIZONTAL': ImageAlign.CENTER_HORIZONTAL,
    'RIGHT': ImageAlign.RIGHT,
    'TOP_LEFT': ImageAlign.TOP_LEFT,
    'TOP_CENTER': ImageAlign.TOP_CENTER,
    'TOP_RIGHT': ImageAlign.TOP_RIGHT,
    'CENTER_LEFT': ImageAlign.CENTER_LEFT,
    'CENTER': ImageAlign.CENTER,
    'CENTER_RIGHT': ImageAlign.CENTER_RIGHT,
    'BOTTOM_LEFT': ImageAlign.BOTTOM_LEFT,
    'BOTTOM_CENTER': ImageAlign.BOTTOM_CENTER,
    'BOTTOM_RIGHT': ImageAlign.BOTTOM_RIGHT,
}

# scroll mode enum

scroll_mode_ns = elements_ns.namespace('ScrollMode')
ScrollMode = scroll_mode_ns.enum('ScrollMode')

SCROLL_MODE = {
    'NONE': ScrollMode.NONE,
    'LEFT_TO_RIGHT': ScrollMode.LEFT_TO_RIGHT,
    'RIGHT_TO_LEFT': ScrollMode.RIGHT_TO_LEFT,
    'BOTTOM_TO_TOP': ScrollMode.BOTTOM_TO_TOP,
    'TOP_TO_BOTTOM': ScrollMode.TOP_TO_BOTTOM,
}

# other

nullptr = cg.esphome_ns.class_('nullptr')
nullopt = cg.esphome_ns.class_('nullopt')

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

# anchor

Point = elements_ns.class_('Point')

def point_schema(x, y, validation):
    return cv.Schema({
        cv.Optional('x', default=x): validation,
        cv.Optional('y', default=y): validation,
    })

async def point_to_code(config):
    return cg.ArrayInitializer(config.get('x'), config.get('y'))

Anchor = elements_ns.class_('Anchor')

def anchor_schema(offset_x=0, offset_y=0, fraction_x=0.5, fraction_y=0.5):
    return cv.Schema({
        cv.Optional('offset', default={}): point_schema(x=offset_x, y=offset_y, validation=cv.int_),
        cv.Optional('fraction', default={}): point_schema(x=fraction_x, y=fraction_y, validation=cv.float_range(min=0.0, max=1.0)),
    })

async def anchor_to_code(config):
    return cg.StructInitializer(
        Anchor,
        ('offset', await point_to_code(config.get('offset'))),
        ('fraction', await point_to_code(config.get('fraction'))),
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
    CONF_HORIZONTAL: CONTAINER_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(HorizontalElement),
    }),
    CONF_IMAGE: BASE_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(ImageElement),
        cv.Required(CONF_IMAGE): cv.use_id(image.Image_),
        cv.Optional(CONF_ANCHOR, default={}): anchor_schema(),
        cv.Optional(CONF_ALIGN, default='CENTER'): cv.enum(IMAGE_ALIGN, upper=True, space='_'),
    }),
    CONF_OVERLAY: CONTAINER_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(OverlayElement),
    }),
    CONF_SEQUENCE: CONTAINER_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(SequenceElement),
        cv.Optional(CONF_DURATION): cv.positive_time_period_milliseconds,
    }),
    CONF_TEXT: BASE_ELEMENT_SCHEMA.extend({
        cv.GenerateID(CONF_ID): cv.declare_id(TextElement),
        cv.Required(CONF_FONT): cv.use_id(font.Font),
        cv.Optional(CONF_COLOR, default='#ffffff'): COLOR_SCHEMA,
        cv.Optional(CONF_BACKGROUND_COLOR): COLOR_SCHEMA,
        cv.Optional(CONF_ANCHOR, default={}): anchor_schema(),
        cv.Optional(CONF_ALIGN, default='CENTER'): cv.enum(TEXT_ALIGN, upper=True, space='_'),
        cv.Optional(CONF_SCROLL_MODE): cv.enum(SCROLL_MODE, upper=True, space='_'),
        cv.Optional(CONF_SCROLL_SPEED): cv.float_range(min=0),
        cv.Optional(CONF_UPDATE_INTERVAL): cv.positive_time_period_milliseconds,
        cv.Exclusive(CONF_LAMBDA, 'text'): cv.returning_lambda,
        cv.Exclusive(CONF_TEXT, 'text'): cv.string,
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
                 CONF_SCROLL_MODE, CONF_SCROLL_SPEED, CONF_UPDATE_INTERVAL]:
        if value := config.get(name):
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
    for name in [CONF_LAMBDA]:
        if conf := config.get(name):
            value = await cg.process_lambda(
                conf,
                [(ContextConstRef, 'context')],
                return_type=cg.std_string,
            )
            cg.add(getattr(var, 'set_' + name)(value))

    # elements
    if conf := config.get(CONF_ELEMENTS):
        for conf_item in conf:
            value = await element_to_code(conf_item, component, var)
            cg.add(var.add_element(value))

    # analog clock
    if config[CONF_TYPE] == CONF_ANALOG_CLOCK:
        for name in [CONF_MINUTE_MARKERS, CONF_HOUR_MARKERS,
                     CONF_QUARTER_MARKERS, CONF_SECOND_HAND, CONF_MINUTE_HAND,
                     CONF_HOUR_HAND]:
            value = await analog_clock_options_to_code(config.get(name))
            cg.add(getattr(var, 'set_' + name)(value))

    # anchor
    for name in [CONF_ANCHOR]:
        if conf := config.get(name):
            value = await anchor_to_code(conf)
            cg.add(getattr(var, 'set_' + name)(value))

    return var

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    if display_config := config.get(CONF_DISPLAY):
        drawing_display = await cg.get_variable(display_config)
        cg.add(var.set_display(drawing_display))

    if root_config := config.get(CONF_ELEMENT):
        root_element = await element_to_code(root_config, var)
        cg.add(var.set_root(root_element))

    await cg.register_component(var, config)
