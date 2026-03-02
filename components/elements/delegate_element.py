import esphome.codegen as cg
import esphome.config_validation as cv

from . import shared
from . import element
from . import element_registry

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
