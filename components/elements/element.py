from .shared import elements_ns
import esphome.config_validation as cv

# base class of all elements
Element = elements_ns.class_("Element")

# base schema of all elements
ELEMENT_SCHEMA = cv.Schema({})


# import the typed element schema lazily
def TYPED_ELEMENT_SCHEMA(value):
    from . import TYPED_ELEMENT_SCHEMA as TYPED_ELEMENT_SCHEMA_IMPL

    return TYPED_ELEMENT_SCHEMA_IMPL(value)
