from . import component
from . import (
    animation_element,
    clock_element,
    container_element,
    custom_element,
    delegate_element,
    image_element,
    select_element,
    text_element,
)

AUTO_LOAD = ["display", "image", "time"]
CODEOWNERS = ["@renggli"]
DEPENDENCIES = []
MULTI_CONF = True

CONFIG_SCHEMA = component.ELEMENT_COMPONENT_SCHEMA


async def to_code(config):
    await component.element_component_to_code(config)
