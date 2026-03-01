import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

from . import shared

# Abstract Element
Element = shared.elements_ns.class_("Element")

ELEMENT_SCHEMA = cv.Schema({})


async def element_to_code(config, component, parent):
    var = cg.new_Pvariable(config.get(CONF_ID), component, parent)
    return var
