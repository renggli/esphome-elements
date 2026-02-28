import esphome.codegen as cg
import esphome.config_validation as cv
from .element import Element, ElementComponent, elements_ns

ContainerElement = elements_ns.class_('ContainerElement', Element)
HorizontalElement = elements_ns.class_('HorizontalElement', ContainerElement)
OverlayElement = elements_ns.class_('OverlayElement', ContainerElement)
PriorityElement = elements_ns.class_('PriorityElement', ContainerElement)
RandomElement = elements_ns.class_('RandomElement', ContainerElement)
SequenceElement = elements_ns.class_('SequenceElement', ContainerElement)
VerticalElement = elements_ns.class_('VerticalElement', ContainerElement)

# conf names
CONF_ACTIVE_MODE = 'active_mode'
CONF_ELEMENTS = 'elements'

# active mode enum

active_mode_ns = elements_ns.namespace('ActiveMode')
ActiveMode = active_mode_ns.enum('ActiveMode')

ACTIVE_MODE = {
    'ALWAYS': ActiveMode.ALWAYS,
    'ANY': ActiveMode.ANY,
    'ALL': ActiveMode.ALL,
    'NEVER': ActiveMode.NEVER,
}

CONTAINER_ELEMENT_SCHEMA_EXTENSION = {
    cv.Optional(CONF_ACTIVE_MODE): cv.enum(ACTIVE_MODE, upper=True, space='_'),
}
