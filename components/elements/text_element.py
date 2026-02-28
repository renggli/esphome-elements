import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import font, time
from esphome.const import (CONF_BACKGROUND_COLOR, CONF_COLOR, CONF_FONT,
                           CONF_FORMAT, CONF_TEXT)
from .element import Element, elements_ns
from .color import COLOR_SCHEMA, color_to_code
from .geometry import anchor_schema, anchor_to_code

display_ns = cg.esphome_ns.namespace('display')
text_align_ns = display_ns.namespace('TextAlign')
TextAlign = text_align_ns.enum('TextAlign')

image_align_ns = display_ns.namespace('ImageAlign')
ImageAlign = image_align_ns.enum('ImageAlign')

TextElement = elements_ns.class_('TextElement', Element)
StaticTextElement = elements_ns.class_('StaticTextElement', TextElement)
DynamicTextElement = elements_ns.class_('DynamicTextElement', TextElement)
TimeTextElement = elements_ns.class_('TimeTextElement', TextElement)

# conf names
CONF_ALIGN = 'align'
CONF_ANCHOR = 'anchor'
CONF_SCROLL_MODE = 'scroll_mode'
CONF_SCROLL_SPEED = 'scroll_speed'

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

TEXT_ELEMENT_SCHEMA = cv.Schema({
    cv.Required(CONF_FONT): cv.use_id(font.Font),
    cv.Optional(CONF_COLOR, default='#ffffff'): COLOR_SCHEMA,
    cv.Optional(CONF_BACKGROUND_COLOR): COLOR_SCHEMA,
    cv.Optional(CONF_ANCHOR, default={}): anchor_schema(),
    cv.Optional(CONF_ALIGN, default='CENTER'): cv.enum(TEXT_ALIGN, upper=True, space='_'),
    cv.Optional(CONF_SCROLL_MODE): cv.enum(SCROLL_MODE, upper=True, space='_'),
    cv.Optional(CONF_SCROLL_SPEED): cv.float_range(min=0),
})
