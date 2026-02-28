import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_DURATION, CONF_ID
from .element import Element, elements_ns, ELEMENT_SCHEMA, TYPED_ELEMENT_SCHEMA

DelegateElement = elements_ns.class_("DelegateElement", Element)

DelayElement = elements_ns.class_("DelayElement", DelegateElement)
TimeoutElement = elements_ns.class_("TimeoutElement", DelegateElement)

CONF_DELAY = "delay"
CONF_TIMEOUT = "timeout"

CONF_COUNT = "count"
CONF_ELEMENT = "element"

DELEGATE_ELEMENT_SCHEMA = ELEMENT_SCHEMA.extend(
    {
        cv.Required(CONF_ELEMENT): TYPED_ELEMENT_SCHEMA,
    }
)

TYPED_DELEGATE_ELEMENT_SCHEMAS = {
    CONF_DELAY: DELEGATE_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(DelayElement),
            cv.Optional(CONF_COUNT): cv.positive_int,
        }
    ),
    CONF_TIMEOUT: DELEGATE_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(TimeoutElement),
            cv.Optional(CONF_DURATION): cv.positive_time_period_milliseconds,
        }
    ),
}
