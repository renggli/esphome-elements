import esphome.codegen as cg

elements_ns = cg.esphome_ns.namespace('elements')

ElementComponent = elements_ns.class_('ElementComponent', cg.Component)
Element = elements_ns.class_('Element')
