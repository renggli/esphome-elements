import esphome.config_validation as cv
import esphome.codegen as cg
import esphome.automation as automation
from esphome.const import CONF_ID, CONF_TRIGGER_ID

from . import shared
from . import element
from . import element_registry

CONF_ON_CHANGE = "on_change"
CONF_ELEMENTS = "elements"

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
