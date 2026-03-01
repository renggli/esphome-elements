import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

from . import shared
from . import element
from . import element_registry

CONF_COUNT = "count"
CONF_DURATION = "duration"
CONF_ELEMENT = "element"

# Abstract Delegate Element

DelegateElement = shared.elements_ns.class_("DelegateElement", element.Element)

DELEGATE_ELEMENT_SCHEMA = element.ELEMENT_SCHEMA.extend(
    {
        cv.Required(CONF_ELEMENT): element_registry.TYPED_ELEMENT_SCHEMA,
    }
)


async def delegate_element_to_code(config, component, parent):
    var = await element.element_to_code(config, component, parent)
    if conf := config.get(CONF_ELEMENT):
        typed_element = await element_registry.typed_element_to_code(
            conf, component, var
        )
        cg.add(var.set_element(typed_element))
    return var


# Delay Element

DelayElement = shared.elements_ns.class_("DelayElement", DelegateElement)

DELAY_ELEMENT_SCHEMA = DELEGATE_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(DelayElement),
        cv.Optional(CONF_COUNT): cv.positive_int,
    }
)


async def delay_element_to_code(config, component, parent):
    var = await delegate_element_to_code(config, component, parent)
    if conf := config.get(CONF_COUNT):
        cg.add(var.set_count(conf))
    return var


element_registry.register_element("delay", DELAY_ELEMENT_SCHEMA, delay_element_to_code)

# Timeout Element

TimeoutElement = shared.elements_ns.class_("TimeoutElement", DelegateElement)


TIMEOUT_ELEMENT_SCHEMA = DELEGATE_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(TimeoutElement),
        cv.Optional(CONF_DURATION): cv.positive_time_period_milliseconds,
    }
)


async def timeout_element_to_code(config, component, parent):
    var = await delegate_element_to_code(config, component, parent)
    if conf := config.get(CONF_DURATION):
        cg.add(var.set_duration(conf))
    return var


element_registry.register_element(
    "timeout", TIMEOUT_ELEMENT_SCHEMA, timeout_element_to_code
)
