import esphome.config_validation as cv
import esphome.codegen as cg
import esphome.automation as automation
from esphome.const import CONF_ID

from . import shared
from . import element
from . import element_registry

CONF_ELEMENTS = "elements"
CONF_ACTIVE_MODE = "active_mode"

# Active Mode

ActiveMode = shared.elements_ns.enum("ActiveMode", is_class=True)

ACTIVE_MODE = {
    "ALWAYS": ActiveMode.ALWAYS,
    "ANY": ActiveMode.ANY,
    "ALL": ActiveMode.ALL,
    "NEVER": ActiveMode.NEVER,
}

# Abstract Container Element

ContainerElement = shared.elements_ns.class_("ContainerElement", element.Element)

CONTAINER_ELEMENT_SCHEMA = element.ELEMENT_SCHEMA.extend(
    {
        cv.Required(CONF_ELEMENTS): cv.All(
            cv.ensure_list(element_registry.TYPED_ELEMENT_SCHEMA),
            cv.Length(min=1),
        ),
        cv.Optional(CONF_ACTIVE_MODE): cv.enum(ACTIVE_MODE, upper=True, space="_"),
    }
)


async def container_element_to_code(config, component, parent):
    var = await element.element_to_code(config, component, parent)
    if elements_configs := config.get(CONF_ELEMENTS):
        for conf in elements_configs:
            element_var = await element_registry.typed_element_to_code(
                conf, component, var
            )
            cg.add(var.add_element(element_var))
    if conf := config.get(CONF_ACTIVE_MODE):
        cg.add(var.set_active_mode(conf))
    return var


# Horizontal Element

HorizontalElement = shared.elements_ns.class_("HorizontalElement", ContainerElement)

HORIZONTAL_ELEMENT_SCHEMA = CONTAINER_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(HorizontalElement),
    }
)

element_registry.register_element(
    "horizontal", HORIZONTAL_ELEMENT_SCHEMA, container_element_to_code
)

# Overlay Element

OverlayElement = shared.elements_ns.class_("OverlayElement", ContainerElement)

OVERLAY_ELEMENT_SCHEMA = CONTAINER_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(OverlayElement),
    }
)

element_registry.register_element(
    "overlay", OVERLAY_ELEMENT_SCHEMA, container_element_to_code
)

# Priority Element

PriorityElement = shared.elements_ns.class_("PriorityElement", ContainerElement)

PRIORITY_ELEMENT_SCHEMA = CONTAINER_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(PriorityElement),
    }
)

element_registry.register_element(
    "priority", PRIORITY_ELEMENT_SCHEMA, container_element_to_code
)

# Random Element

RandomElement = shared.elements_ns.class_("RandomElement", ContainerElement)

RANDOM_ELEMENT_SCHEMA = CONTAINER_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(RandomElement),
    }
)

element_registry.register_element(
    "random", RANDOM_ELEMENT_SCHEMA, container_element_to_code
)


@automation.register_action(
    "elements.random.prev",
    shared.elements_ns.class_("RandomPrevAction", automation.Action),
    automation.maybe_simple_id({cv.Required(CONF_ID): cv.use_id(RandomElement)}),
)
async def elements_random_prev_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)


@automation.register_action(
    "elements.random.next",
    shared.elements_ns.class_("RandomNextAction", automation.Action),
    automation.maybe_simple_id({cv.Required(CONF_ID): cv.use_id(RandomElement)}),
)
async def elements_random_next_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)


# Sequence Element

SequenceElement = shared.elements_ns.class_("SequenceElement", ContainerElement)

SEQUENCE_ELEMENT_SCHEMA = CONTAINER_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(SequenceElement),
    }
)

element_registry.register_element(
    "sequence", SEQUENCE_ELEMENT_SCHEMA, container_element_to_code
)


@automation.register_action(
    "elements.sequence.prev",
    shared.elements_ns.class_("SequencePrevAction", automation.Action),
    automation.maybe_simple_id({cv.Required(CONF_ID): cv.use_id(SequenceElement)}),
)
async def elements_sequence_prev_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)


@automation.register_action(
    "elements.sequence.next",
    shared.elements_ns.class_("SequenceNextAction", automation.Action),
    automation.maybe_simple_id({cv.Required(CONF_ID): cv.use_id(SequenceElement)}),
)
async def elements_sequence_next_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)


# Vertical Element

VerticalElement = shared.elements_ns.class_("VerticalElement", ContainerElement)

VERTICAL_ELEMENT_SCHEMA = CONTAINER_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(VerticalElement),
    }
)

element_registry.register_element(
    "vertical", VERTICAL_ELEMENT_SCHEMA, container_element_to_code
)
