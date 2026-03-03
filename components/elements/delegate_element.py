import esphome.codegen as cg
import esphome.config_validation as cv
import esphome.automation as automation
from esphome.const import CONF_ID, CONF_TRIGGER_ID

from . import shared
from . import element
from . import element_registry

CONF_DURATION = "duration"
CONF_ELEMENT = "element"
CONF_ON_COMPLETE = "on_complete"

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


# Timeout Element

TimeoutElement = shared.elements_ns.class_("TimeoutElement", DelegateElement)
TimeoutElementRef = TimeoutElement.operator("ref")
TimeoutElementCompleteTrigger = shared.elements_ns.class_(
    "TimeoutElementCompleteTrigger", automation.Trigger.template(TimeoutElementRef)
)


TIMEOUT_ELEMENT_SCHEMA = DELEGATE_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(TimeoutElement),
        cv.Optional(CONF_DURATION): cv.positive_time_period_milliseconds,
        cv.Optional(CONF_ON_COMPLETE): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                    TimeoutElementCompleteTrigger
                ),
            }
        ),
    }
)


async def timeout_element_to_code(config, component, parent):
    var = await delegate_element_to_code(config, component, parent)
    if CONF_DURATION in config:
        cg.add(var.set_duration(config[CONF_DURATION]))
    for conf in config.get(CONF_ON_COMPLETE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(
            trigger, [(TimeoutElementRef, "element")], conf
        )
    return var


element_registry.register_element(
    "timeout", TIMEOUT_ELEMENT_SCHEMA, timeout_element_to_code
)
