import esphome.config_validation as cv

from esphome.const import CONF_TYPE

# Registry of all elements
_ELEMENT_REGISTRY = {}


# Registers a new element to the registry
def register_element(name, schema, to_code):
    _ELEMENT_REGISTRY[name] = (schema, to_code)


# The typed schema of all elements
def TYPED_ELEMENT_SCHEMA(value):
    return cv.typed_schema(
        {name.lower(): schema for name, (schema, _) in _ELEMENT_REGISTRY.items()},
        lower=True,
    )(value)


# Generates the code for a typed element
async def typed_element_to_code(config, component, parent):
    if CONF_TYPE not in config:
        raise ValueError(f"Missing element type: {config}")
    type = config.get(CONF_TYPE)
    if type not in _ELEMENT_REGISTRY:
        raise ValueError(f"Unknown element type: {type}")
    (_, to_code) = _ELEMENT_REGISTRY[type]
    return await to_code(config, component, parent)
