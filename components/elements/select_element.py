import esphome.config_validation as cv
import esphome.codegen as cg
import esphome.automation as automation
from esphome.const import CONF_ID

from . import shared
from . import element_registry
from .container_element import (
    ContainerElement,
    CONTAINER_ELEMENT_SCHEMA,
    container_element_to_code,
)

# Abstract Select Element

SelectElement = shared.elements_ns.class_("SelectElement", ContainerElement)

# Priority Element

PriorityElement = shared.elements_ns.class_("PriorityElement", SelectElement)

PRIORITY_ELEMENT_SCHEMA = CONTAINER_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(PriorityElement),
    }
)

element_registry.register_element(
    "priority", PRIORITY_ELEMENT_SCHEMA, container_element_to_code
)

# Random Element

RandomElement = shared.elements_ns.class_("RandomElement", SelectElement)

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

SequenceElement = shared.elements_ns.class_("SequenceElement", SelectElement)

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
