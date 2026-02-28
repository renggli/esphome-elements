import esphome.config_validation as cv
from esphome.const import CONF_ID
from .element import Element, elements_ns, ELEMENT_SCHEMA

CustomElement = elements_ns.class_("CustomElement", Element)

CONF_CUSTOM = "custom"
CONF_LAMBDA_DRAW = "draw"
CONF_LAMBDA_IS_ACTIVE = "is_active"
CONF_LAMBDA_ON_HIDE = "on_hide"
CONF_LAMBDA_ON_PREV = "on_prev"
CONF_LAMBDA_ON_NEXT = "on_next"
CONF_LAMBDA_ON_SHOW = "on_show"

CUSTOM_ELEMENT_SCHEMA = ELEMENT_SCHEMA.extend(
    {
        cv.Optional(CONF_LAMBDA_DRAW): cv.lambda_,
        cv.Optional(CONF_LAMBDA_ON_SHOW): cv.lambda_,
        cv.Optional(CONF_LAMBDA_ON_HIDE): cv.lambda_,
        cv.Optional(CONF_LAMBDA_ON_PREV): cv.lambda_,
        cv.Optional(CONF_LAMBDA_ON_NEXT): cv.lambda_,
        cv.Optional(CONF_LAMBDA_IS_ACTIVE): cv.returning_lambda,
    }
)

TYPED_CUSTOM_ELEMENT_SCHEMAS = {
    CONF_CUSTOM: CUSTOM_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(CustomElement),
        }
    ),
}
