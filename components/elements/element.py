import esphome.codegen as cg
import esphome.config_validation as cv
import esphome.automation as automation
from esphome.const import CONF_ID, CONF_TRIGGER_ID

from . import shared

CONF_ON_SHOW = "on_show"
CONF_ON_HIDE = "on_hide"

# Abstract Element
Element = shared.elements_ns.class_("Element")

ElementShowTrigger = shared.elements_ns.class_(
    "ElementShowTrigger", automation.Trigger.template(shared.ElementRef)
)
ElementHideTrigger = shared.elements_ns.class_(
    "ElementHideTrigger", automation.Trigger.template(shared.ElementRef)
)

ELEMENT_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_ON_SHOW): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(ElementShowTrigger),
            }
        ),
        cv.Optional(CONF_ON_HIDE): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(ElementHideTrigger),
            }
        ),
    }
)


async def element_to_code(config, component, parent):
    var = cg.new_Pvariable(config.get(CONF_ID), component, parent)
    for conf in config.get(CONF_ON_SHOW, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(
            trigger, [(shared.ElementRef, "element")], conf
        )
    for conf in config.get(CONF_ON_HIDE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(
            trigger, [(shared.ElementRef, "element")], conf
        )
    return var
