import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import image
from esphome.const import CONF_ID
from .element import Element, elements_ns, ELEMENT_SCHEMA
from .geometry import anchor_schema
from .shared import ImageAlign

ImageElement = elements_ns.class_("ImageElement", Element)

CONF_IMAGE = "image"

CONF_ALIGN = "align"
CONF_ANCHOR = "anchor"

IMAGE_ALIGN = {
    "TOP": ImageAlign.TOP,
    "CENTER_VERTICAL": ImageAlign.CENTER_VERTICAL,
    "BOTTOM": ImageAlign.BOTTOM,
    "LEFT": ImageAlign.LEFT,
    "CENTER_HORIZONTAL": ImageAlign.CENTER_HORIZONTAL,
    "RIGHT": ImageAlign.RIGHT,
    "TOP_LEFT": ImageAlign.TOP_LEFT,
    "TOP_CENTER": ImageAlign.TOP_CENTER,
    "TOP_RIGHT": ImageAlign.TOP_RIGHT,
    "CENTER_LEFT": ImageAlign.CENTER_LEFT,
    "CENTER": ImageAlign.CENTER,
    "CENTER_RIGHT": ImageAlign.CENTER_RIGHT,
    "BOTTOM_LEFT": ImageAlign.BOTTOM_LEFT,
    "BOTTOM_CENTER": ImageAlign.BOTTOM_CENTER,
    "BOTTOM_RIGHT": ImageAlign.BOTTOM_RIGHT,
}

IMAGE_ELEMENT_SCHEMA = ELEMENT_SCHEMA.extend(
    {
        cv.Optional(CONF_IMAGE): cv.use_id(image.Image_),
        cv.Optional(CONF_ANCHOR, default={}): anchor_schema(),
        cv.Optional(CONF_ALIGN, default="CENTER"): cv.enum(
            IMAGE_ALIGN, upper=True, space="_"
        ),
    }
)

TYPED_IMAGE_ELEMENT_SCHEMAS = {
    CONF_IMAGE: IMAGE_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(ImageElement),
        }
    ),
}
