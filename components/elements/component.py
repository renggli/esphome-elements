import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.components import display

from . import shared
from . import element_registry

CONF_ELEMENT = "element"
CONF_DISPLAY = "display"

# Element Component

ElementComponent = shared.elements_ns.class_("ElementComponent", cg.Component)

ELEMENT_COMPONENT_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(ElementComponent),
        cv.Optional(CONF_DISPLAY): cv.use_id(display.Display),
        cv.Required(CONF_ELEMENT): element_registry.TYPED_ELEMENT_SCHEMA,
    }
)


async def element_component_to_code(config):
    var = cg.new_Pvariable(config.get(CONF_ID))
    await cg.register_component(var, config)

    if conf := config.get(CONF_DISPLAY):
        value = await cg.get_variable(conf)
        cg.add(var.set_display(value))
    if conf := config.get(CONF_ELEMENT):
        value = await element_registry.typed_element_to_code(conf, var, shared.nullptr)
        cg.add(var.set_root(value))
