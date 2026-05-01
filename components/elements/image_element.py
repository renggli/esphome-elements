import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import image
from esphome.const import CONF_ID

from . import shared
from . import element
from . import geometry
from . import element_registry

CONF_ALIGN = "align"
CONF_ANCHOR = "anchor"
CONF_IMAGE = "image"
CONF_SCALE = "scale"

# Image Align Enum

ImageAlign = shared.display_ns.enum("ImageAlign", is_class=True)

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

# Image Element Class

ImageElement = shared.elements_ns.class_("ImageElement", element.Element)

IMAGE_ELEMENT_SCHEMA = element.ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(ImageElement),
        cv.Optional(CONF_IMAGE): cv.use_id(image.Image_),
        cv.Optional(CONF_ANCHOR, default={}): geometry.anchor_schema(),
        cv.Optional(CONF_ALIGN, default="CENTER"): cv.enum(
            IMAGE_ALIGN, upper=True, space="_"
        ),
        cv.Optional(CONF_SCALE, default=1.0): cv.positive_float,
    }
)


async def image_element_to_code(config, component, parent):
    var = await element.element_to_code(config, component, parent)
    if conf := config.get(CONF_IMAGE):
        image = await cg.get_variable(conf)
        cg.add(var.set_image(image))
    if conf := config.get(CONF_ANCHOR):
        anchor = await geometry.anchor_to_code(conf)
        cg.add(var.set_anchor(anchor))
    if conf := config.get(CONF_ALIGN):
        cg.add(var.set_align(conf))
    if (scale := config.get(CONF_SCALE, 1.0)) != 1.0:
        cg.add(var.set_scale(scale))
    return var


element_registry.register_element("image", IMAGE_ELEMENT_SCHEMA, image_element_to_code)
