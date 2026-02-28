import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_DURATION
from .element import Element, elements_ns

DelegateElement = elements_ns.class_('DelegateElement', Element)
DelayElement = elements_ns.class_('DelayElement', DelegateElement)
TimeoutElement = elements_ns.class_('TimeoutElement', DelegateElement)

# conf names
CONF_COUNT = 'count'
CONF_ELEMENT = 'element'

DELEGATE_ELEMENT_SCHEMA_EXTENSION = {}
