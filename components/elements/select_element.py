import esphome.config_validation as cv
import esphome.codegen as cg
import esphome.automation as automation
from esphome.const import CONF_CLEAR, CONF_ID, CONF_INDEX, CONF_TRIGGER_ID

from . import shared
from . import element
from . import element_registry

CONF_ON_CHANGE = "on_change"
CONF_ELEMENTS = "elements"
CONF_ELEMENT = "element"
CONF_ADVANCE = "advance"

# Abstract Select Element

SelectElement = shared.elements_ns.class_("SelectElement", element.Element)
SelectElementRef = SelectElement.operator("ref")
SelectElementChangeTrigger = shared.elements_ns.class_(
    "SelectElementChangeTrigger", automation.Trigger.template(cg.int_, cg.int_)
)

SELECT_ELEMENT_SCHEMA = element.ELEMENT_SCHEMA.extend(
    {
        cv.Required(CONF_ELEMENTS): cv.All(
            cv.ensure_list(element_registry.TYPED_ELEMENT_SCHEMA),
            cv.Length(min=1),
        ),
        cv.Optional(CONF_ON_CHANGE): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                    SelectElementChangeTrigger
                ),
            }
        ),
    }
)


async def select_element_to_code(config, component, parent):
    var = await element.element_to_code(config, component, parent)
    if elements_configs := config.get(CONF_ELEMENTS):
        for conf in elements_configs:
            element_var = await element_registry.typed_element_to_code(
                conf, component, var
            )
            cg.add(var.add_element(element_var))

    for conf in config.get(CONF_ON_CHANGE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(
            trigger, [(cg.int_, "from_index"), (cg.int_, "to_index")], conf
        )
    return var


@automation.register_action(
    "elements.next",
    shared.elements_ns.class_("SelectNextAction", automation.Action),
    automation.maybe_simple_id({cv.Required(CONF_ID): cv.use_id(SelectElement)}),
    synchronous=True,
)
async def elements_next_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)


@automation.register_action(
    "elements.prev",
    shared.elements_ns.class_("SelectPrevAction", automation.Action),
    automation.maybe_simple_id({cv.Required(CONF_ID): cv.use_id(SelectElement)}),
    synchronous=True,
)
async def elements_prev_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)


SELECT_GO_TO_ACTION_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.Required(CONF_ID): cv.use_id(SelectElement),
            cv.Optional(CONF_INDEX): cv.templatable(cv.int_),
            cv.Optional(CONF_ELEMENT): cv.use_id(element.Element),
        }
    ),
    cv.has_exactly_one_key(CONF_INDEX, CONF_ELEMENT),
)


@automation.register_action(
    "elements.go_to",
    shared.elements_ns.class_("SelectGoToAction", automation.Action),
    SELECT_GO_TO_ACTION_SCHEMA,
    synchronous=True,
)
async def elements_go_to_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    if CONF_INDEX in config:
        template_ = await cg.templatable(config[CONF_INDEX], args, cg.int_)
        cg.add(var.set_index(template_))
    if CONF_ELEMENT in config:
        elem = await cg.get_variable(config[CONF_ELEMENT])
        cg.add(var.set_element(elem))
    return var


# Priority Element

PriorityElement = shared.elements_ns.class_("PriorityElement", SelectElement)

PRIORITY_ELEMENT_SCHEMA = SELECT_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(PriorityElement),
    }
)

element_registry.register_element(
    "priority", PRIORITY_ELEMENT_SCHEMA, select_element_to_code
)

# Random Element

RandomElement = shared.elements_ns.class_("RandomElement", SelectElement)

RANDOM_ELEMENT_SCHEMA = SELECT_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(RandomElement),
    }
)

element_registry.register_element(
    "random", RANDOM_ELEMENT_SCHEMA, select_element_to_code
)

RANDOM_PUSH_ACTION_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_ID): cv.use_id(RandomElement),
        cv.Optional(CONF_CLEAR, default=False): cv.boolean,
        cv.Optional(CONF_ADVANCE, default=False): cv.boolean,
        cv.Optional(CONF_ELEMENT): cv.use_id(element.Element),
        cv.Optional(CONF_ELEMENTS, default=[]): cv.ensure_list(
            cv.use_id(element.Element)
        ),
    }
)


@automation.register_action(
    "elements.push",
    shared.elements_ns.class_("RandomPushAction", automation.Action),
    RANDOM_PUSH_ACTION_SCHEMA,
    synchronous=True,
)
async def elements_push_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    if config[CONF_CLEAR]:
        cg.add(var.set_clear(True))
    if config[CONF_ADVANCE]:
        cg.add(var.set_advance(True))
    if CONF_ELEMENT in config:
        elem = await cg.get_variable(config[CONF_ELEMENT])
        cg.add(var.add_element(elem))
    for elem_id in config.get(CONF_ELEMENTS, []):
        elem = await cg.get_variable(elem_id)
        cg.add(var.add_element(elem))
    return var


RANDOM_CLEAR_QUEUE_ACTION_SCHEMA = automation.maybe_simple_id(
    {
        cv.Required(CONF_ID): cv.use_id(RandomElement),
    }
)


@automation.register_action(
    "elements.clear_queue",
    shared.elements_ns.class_("RandomClearQueueAction", automation.Action),
    RANDOM_CLEAR_QUEUE_ACTION_SCHEMA,
    synchronous=True,
)
async def elements_clear_queue_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)

# Sequence Element

SequenceElement = shared.elements_ns.class_("SequenceElement", SelectElement)

SEQUENCE_ELEMENT_SCHEMA = SELECT_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(SequenceElement),
    }
)

element_registry.register_element(
    "sequence", SEQUENCE_ELEMENT_SCHEMA, select_element_to_code
)
