import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID

from . import shared
from . import element
from . import element_registry

CONF_DRAW = "draw"
CONF_IS_ACTIVE = "is_active"
CONF_ON_HIDE = "on_hide"
CONF_ON_PREV = "on_prev"
CONF_ON_NEXT = "on_next"
CONF_ON_SHOW = "on_show"
CONF_ELEMENT = "element"
CONF_DISPLAY = "display"

# Custom Element

CustomElement = shared.elements_ns.class_("CustomElement", element.Element)

CUSTOM_ELEMENT_SCHEMA = element.ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(CustomElement),
        cv.Optional(CONF_DRAW): cv.lambda_,
        cv.Optional(CONF_ON_SHOW): cv.lambda_,
        cv.Optional(CONF_ON_HIDE): cv.lambda_,
        cv.Optional(CONF_ON_PREV): cv.lambda_,
        cv.Optional(CONF_ON_NEXT): cv.lambda_,
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
    for name in [CONF_ON_SHOW, CONF_ON_HIDE, CONF_ON_PREV, CONF_ON_NEXT]:
        if conf := config.get(name):
            value = await cg.process_lambda(
                conf,
                [(shared.ElementRef, CONF_ELEMENT)],
                return_type=cg.void,
            )
            cg.add(getattr(var, "set_" + name)(value))
    if conf := config.get(CONF_IS_ACTIVE):
        value = await cg.process_lambda(
            conf,
            [(shared.ElementRef, CONF_ELEMENT)],
            return_type=cg.bool_,
        )
        cg.add(var.set_is_active(value))
    return var


element_registry.register_element(
    "custom", CUSTOM_ELEMENT_SCHEMA, custom_element_to_code
)
