import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID

from . import shared
from . import element
from . import element_registry

CONF_DRAW = "draw"
CONF_IS_ACTIVE = "is_active"
CONF_ELEMENT = "element"
CONF_DISPLAY = "display"

# Custom Element

CustomElement = shared.elements_ns.class_("CustomElement", element.Element)

CUSTOM_ELEMENT_SCHEMA = element.ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(CustomElement),
        cv.Optional(CONF_DRAW): cv.lambda_,
        cv.Optional(CONF_IS_ACTIVE): cv.returning_lambda,
    }
)


async def custom_element_to_code(config, component, parent):
    var = await element.element_to_code(config, component, parent)
    if conf := config.get(CONF_DRAW):
        value = await cg.process_lambda(
            conf,
            [(shared.ElementRef, CONF_ELEMENT), (shared.DisplayRef, CONF_DISPLAY)],
            return_type=cg.void,
        )
        cg.add(var.set_draw(value))
    if conf := config.get(CONF_IS_ACTIVE):
        value = await cg.process_lambda(
            conf,
            [(shared.ElementConstRef, CONF_ELEMENT)],
            return_type=cg.bool_,
        )
        cg.add(var.set_is_active(value))
    return var


element_registry.register_element(
    "custom", CUSTOM_ELEMENT_SCHEMA, custom_element_to_code
)
