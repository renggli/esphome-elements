import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display
from esphome.const import (
    CONF_BACKGROUND_COLOR,
    CONF_COLOR,
    CONF_DISPLAY,
    CONF_DURATION,
    CONF_FONT,
    CONF_FORMAT,
    CONF_ID,
    CONF_LAMBDA,
    CONF_TEXT,
    CONF_TIME,
)
from .color import (
    CONF_COLOR_SCHEME,
    color_scheme_to_code,
    color_to_code,
)
from .shared import nullptr, DisplayRef
from .geometry import anchor_to_code
from .container_element import (
    TYPED_CONTAINER_ELEMENT_SCHEMAS,
    CONF_ACTIVE_MODE,
    CONF_ELEMENTS,
)
from .delegate_element import (
    TYPED_DELEGATE_ELEMENT_SCHEMAS,
    CONF_ELEMENT,
    CONF_COUNT,
)
from .text_element import (
    CONF_ALIGN,
    CONF_ANCHOR,
    CONF_SCROLL_MODE,
    CONF_SCROLL_SPEED,
    TYPED_TEXT_ELEMENT_SCHEMAS,
)
from .image_element import (
    CONF_IMAGE,
    TYPED_IMAGE_ELEMENT_SCHEMAS,
)
from .animation_element import (
    CONF_DENSITY,
    CONF_LENGTH,
    CONF_SPEED,
    CONF_STRENGTH,
    TYPED_ANIMATION_ELEMENT_SCHEMAS,
)
from .clock_element import (
    CONF_HOUR_HAND,
    CONF_HOUR_MARKERS,
    CONF_MINUTE_HAND,
    CONF_MINUTE_MARKERS,
    CONF_QUARTER_MARKERS,
    CONF_SECOND_HAND,
    analog_clock_options_to_code,
    TYPED_CLOCK_ELEMENT_SCHEMAS,
)
from .custom_element import (
    CONF_LAMBDA_DRAW,
    CONF_LAMBDA_IS_ACTIVE,
    CONF_LAMBDA_ON_HIDE,
    CONF_LAMBDA_ON_PREV,
    CONF_LAMBDA_ON_NEXT,
    CONF_LAMBDA_ON_SHOW,
    TYPED_CUSTOM_ELEMENT_SCHEMAS,
)
from .component import ElementComponent

# configuration of the component
AUTO_LOAD = ["display", "image"]
CODEOWNERS = ["@renggli"]
DEPENDENCIES = ["display"]
MULTI_CONF = True

# typed schema of all elements
TYPED_ELEMENT_SCHEMA = cv.typed_schema(
    {
        **TYPED_ANIMATION_ELEMENT_SCHEMAS,
        **TYPED_CONTAINER_ELEMENT_SCHEMAS,
        **TYPED_DELEGATE_ELEMENT_SCHEMAS,
        **TYPED_CLOCK_ELEMENT_SCHEMAS,
        **TYPED_CUSTOM_ELEMENT_SCHEMAS,
        **TYPED_IMAGE_ELEMENT_SCHEMAS,
        **TYPED_TEXT_ELEMENT_SCHEMAS,
    }
)

# schema to configure the component
CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(ElementComponent),
        cv.Optional(CONF_DISPLAY): cv.use_id(display.Display),
        cv.Required(CONF_ELEMENT): TYPED_ELEMENT_SCHEMA,
    }
)


# recursively convert element to code
async def element_to_code(config, component, parent=nullptr):
    var = cg.new_Pvariable(config[CONF_ID], component, parent)

    # literals
    for name in [
        CONF_DURATION,
        CONF_FORMAT,
        CONF_TEXT,
        CONF_ALIGN,
        CONF_SCROLL_MODE,
        CONF_SCROLL_SPEED,
        CONF_ACTIVE_MODE,
        CONF_COUNT,
    ]:
        if value := config.get(name):
            cg.add(getattr(var, "set_" + name)(value))

    # color scheme - requires object instantiation
    if color_scheme_config := config.get(CONF_COLOR_SCHEME):
        scheme = await color_scheme_to_code(color_scheme_config)
        cg.add(var.set_color_scheme(scheme))

    # floats
    for name in [CONF_SPEED, CONF_DENSITY, CONF_LENGTH, CONF_STRENGTH]:
        if (value := config.get(name)) is not None:
            cg.add(getattr(var, "set_" + name)(value))

    # references
    for name in [CONF_TIME, CONF_FONT, CONF_IMAGE]:
        if conf := config.get(name):
            value = await cg.get_variable(conf)
            cg.add(getattr(var, "set_" + name)(value))

    # colors
    for name in [CONF_COLOR, CONF_BACKGROUND_COLOR]:
        if conf := config.get(name):
            value = await color_to_code(conf)
            cg.add(getattr(var, "set_" + name)(value))

    # lambdas
    ElementRef = config["id"].type.operator("ref")
    for name in [CONF_LAMBDA]:
        if conf := config.get(name):
            value = await cg.process_lambda(
                conf, [(ElementRef, "element")], return_type=cg.std_string
            )
            cg.add(getattr(var, "set_" + name)(value))
    for name in [CONF_LAMBDA_IS_ACTIVE]:
        if conf := config.get(name):
            value = await cg.process_lambda(
                conf, [(ElementRef, "element")], return_type=cg.bool_
            )
            cg.add(getattr(var, "set_" + name)(value))
    for name in [
        CONF_LAMBDA_ON_SHOW,
        CONF_LAMBDA_ON_HIDE,
        CONF_LAMBDA_ON_PREV,
        CONF_LAMBDA_ON_NEXT,
    ]:
        if conf := config.get(name):
            value = await cg.process_lambda(
                conf, [(ElementRef, "element")], return_type=cg.void
            )
            cg.add(getattr(var, "set_" + name)(value))
    for name in [CONF_LAMBDA_DRAW]:
        if conf := config.get(name):
            value = await cg.process_lambda(
                conf,
                [(ElementRef, "element"), (DisplayRef, "display")],
                return_type=cg.void,
            )
            cg.add(getattr(var, "set_" + name)(value))

    # elements
    for name in [CONF_ELEMENT]:
        if conf := config.get(name):
            value = await element_to_code(conf, component, var)
            cg.add(getattr(var, "set_" + name)(value))
    if conf := config.get(CONF_ELEMENTS):
        for conf_item in conf:
            value = await element_to_code(conf_item, component, var)
            cg.add(var.add_element(value))

    # clock
    for name in [
        CONF_MINUTE_MARKERS,
        CONF_HOUR_MARKERS,
        CONF_QUARTER_MARKERS,
        CONF_SECOND_HAND,
        CONF_MINUTE_HAND,
        CONF_HOUR_HAND,
    ]:
        if conf := config.get(name):
            value = await analog_clock_options_to_code(conf)
            cg.add(getattr(var, "set_" + name)(value))

    # anchor
    for name in [CONF_ANCHOR]:
        if conf := config.get(name):
            value = await anchor_to_code(conf)
            cg.add(getattr(var, "set_" + name)(value))

    return var


# convert component to code
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if display_config := config.get(CONF_DISPLAY):
        drawing_display = await cg.get_variable(display_config)
        cg.add(var.set_display(drawing_display))

    if root_config := config.get(CONF_ELEMENT):
        root_element = await element_to_code(root_config, var)
        cg.add(var.set_root(root_element))
