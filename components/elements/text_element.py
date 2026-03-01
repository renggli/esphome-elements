import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import font, time
from esphome.const import CONF_ID

from . import shared
from . import element
from . import element_registry
from . import color
from . import geometry

CONF_ALIGN = "align"
CONF_ANCHOR = "anchor"
CONF_BACKGROUND_COLOR = "background_color"
CONF_COLOR = "color"
CONF_FONT = "font"
CONF_FORMAT = "format"
CONF_LAMBDA = "lambda"
CONF_SCROLL_MODE = "scroll_mode"
CONF_SCROLL_SPEED = "scroll_speed"
CONF_TEXT = "text"
CONF_TIME = "time"

# Scroll Mode

ScrollMode = shared.elements_ns.enum("ScrollMode", is_class=True)

SCROLL_MODE = {
    "NONE": ScrollMode.NONE,
    "LEFT_TO_RIGHT": ScrollMode.LEFT_TO_RIGHT,
    "RIGHT_TO_LEFT": ScrollMode.RIGHT_TO_LEFT,
    "BOTTOM_TO_TOP": ScrollMode.BOTTOM_TO_TOP,
    "TOP_TO_BOTTOM": ScrollMode.TOP_TO_BOTTOM,
}

# Text Align

TextAlign = shared.display_ns.enum("TextAlign", is_class=True)

TEXT_ALIGN = {
    "TOP": TextAlign.TOP,
    "CENTER_VERTICAL": TextAlign.CENTER_VERTICAL,
    "BASELINE": TextAlign.BASELINE,
    "BOTTOM": TextAlign.BOTTOM,
    "LEFT": TextAlign.LEFT,
    "CENTER_HORIZONTAL": TextAlign.CENTER_HORIZONTAL,
    "RIGHT": TextAlign.RIGHT,
    "TOP_LEFT": TextAlign.TOP_LEFT,
    "TOP_CENTER": TextAlign.TOP_CENTER,
    "TOP_RIGHT": TextAlign.TOP_RIGHT,
    "CENTER_LEFT": TextAlign.CENTER_LEFT,
    "CENTER": TextAlign.CENTER,
    "CENTER_RIGHT": TextAlign.CENTER_RIGHT,
    "BASELINE_LEFT": TextAlign.BASELINE_LEFT,
    "BASELINE_CENTER": TextAlign.BASELINE_CENTER,
    "BASELINE_RIGHT": TextAlign.BASELINE_RIGHT,
    "BOTTOM_LEFT": TextAlign.BOTTOM_LEFT,
    "BOTTOM_CENTER": TextAlign.BOTTOM_CENTER,
    "BOTTOM_RIGHT": TextAlign.BOTTOM_RIGHT,
}

# Abstract Text Element

TextElement = shared.elements_ns.class_("TextElement", element.Element)

TEXT_ELEMENT_SCHEMA = element.ELEMENT_SCHEMA.extend(
    {
        cv.Required(CONF_FONT): cv.use_id(font.Font),
        cv.Optional(CONF_COLOR, default="#ffffff"): color.COLOR_SCHEMA,
        cv.Optional(CONF_BACKGROUND_COLOR): color.COLOR_SCHEMA,
        cv.Optional(CONF_ANCHOR, default={}): geometry.anchor_schema(),
        cv.Optional(CONF_ALIGN, default="CENTER"): cv.enum(
            TEXT_ALIGN, upper=True, space="_"
        ),
        cv.Optional(CONF_SCROLL_MODE): cv.enum(SCROLL_MODE, upper=True, space="_"),
        cv.Optional(CONF_SCROLL_SPEED): cv.float_range(min=0),
    }
)


async def text_element_to_code(config, component, parent):
    var = await element.element_to_code(config, component, parent)
    if conf := config.get(CONF_FONT):
        value = await cg.get_variable(conf)
        cg.add(var.set_font(value))
    if conf := config.get(CONF_COLOR):
        value = await color.color_to_code(conf)
        cg.add(var.set_color(value))
    if conf := config.get(CONF_BACKGROUND_COLOR):
        value = await color.color_to_code(conf)
        cg.add(var.set_background_color(value))
    if conf := config.get(CONF_ANCHOR):
        value = await geometry.anchor_to_code(conf)
        cg.add(var.set_anchor(value))
    if conf := config.get(CONF_ALIGN):
        cg.add(var.set_align(conf))
    if conf := config.get(CONF_SCROLL_MODE):
        cg.add(var.set_scroll_mode(conf))
    if conf := config.get(CONF_SCROLL_SPEED):
        cg.add(var.set_scroll_speed(conf))
    return var


# Static Text Element

StaticTextElement = shared.elements_ns.class_("StaticTextElement", TextElement)

STATIC_TEXT_ELEMENT_SCHEMA = TEXT_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(StaticTextElement),
        cv.Optional(CONF_TEXT): cv.string,
    }
)


async def static_text_element_to_code(config, component, parent):
    var = await text_element_to_code(config, component, parent)
    if conf := config.get(CONF_TEXT):
        cg.add(var.set_text(conf))
    return var


element_registry.register_element(
    "static_text", STATIC_TEXT_ELEMENT_SCHEMA, static_text_element_to_code
)

# Dynamic Text Element

DynamicTextElement = shared.elements_ns.class_("DynamicTextElement", TextElement)
DynamicTextRef = DynamicTextElement.operator("ref")

DYNAMIC_TEXT_ELEMENT_SCHEMA = TEXT_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(DynamicTextElement),
        cv.Required(CONF_LAMBDA): cv.returning_lambda,
    }
)


async def dynamic_text_element_to_code(config, component, parent):
    var = await text_element_to_code(config, component, parent)
    if conf := config.get(CONF_LAMBDA):
        value = await cg.process_lambda(
            conf,
            [(DynamicTextRef, "element")],
            return_type=cg.std_string,
        )
        cg.add(var.set_lambda(value))
    return var


element_registry.register_element(
    "dynamic_text", DYNAMIC_TEXT_ELEMENT_SCHEMA, dynamic_text_element_to_code
)

# Time Text Element

TimeTextElement = shared.elements_ns.class_("TimeTextElement", TextElement)

TIME_TEXT_ELEMENT_SCHEMA = TEXT_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(TimeTextElement),
        cv.Required(CONF_TIME): cv.use_id(time.RealTimeClock),
        cv.Optional(CONF_FORMAT, default="%H:%M:%S"): cv.string,
    }
)


async def time_text_element_to_code(config, component, parent):
    var = await text_element_to_code(config, component, parent)
    if conf := config.get(CONF_TIME):
        value = await cg.get_variable(conf)
        cg.add(var.set_time(value))
    if conf := config.get(CONF_FORMAT):
        cg.add(var.set_format(conf))
    return var


element_registry.register_element(
    "time_text", TIME_TEXT_ELEMENT_SCHEMA, time_text_element_to_code
)
