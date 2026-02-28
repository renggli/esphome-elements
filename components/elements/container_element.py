import esphome.config_validation as cv
from esphome.const import CONF_ID
from .element import Element, elements_ns, ELEMENT_SCHEMA, TYPED_ELEMENT_SCHEMA

ContainerElement = elements_ns.class_("ContainerElement", Element)

HorizontalElement = elements_ns.class_("HorizontalElement", ContainerElement)
OverlayElement = elements_ns.class_("OverlayElement", ContainerElement)
PriorityElement = elements_ns.class_("PriorityElement", ContainerElement)
RandomElement = elements_ns.class_("RandomElement", ContainerElement)
SequenceElement = elements_ns.class_("SequenceElement", ContainerElement)
VerticalElement = elements_ns.class_("VerticalElement", ContainerElement)

CONF_HORIZONTAL = "horizontal"
CONF_OVERLAY = "overlay"
CONF_PRIORITY = "priority"
CONF_RANDOM = "random"
CONF_SEQUENCE = "sequence"
CONF_VERTICAL = "vertical"

CONF_ACTIVE_MODE = "active_mode"
CONF_ELEMENTS = "elements"

active_mode_ns = elements_ns.namespace("ActiveMode")
ActiveMode = active_mode_ns.enum("ActiveMode")

ACTIVE_MODE = {
    "ALWAYS": ActiveMode.ALWAYS,
    "ANY": ActiveMode.ANY,
    "ALL": ActiveMode.ALL,
    "NEVER": ActiveMode.NEVER,
}

CONTAINER_ELEMENT_SCHEMA = ELEMENT_SCHEMA.extend(
    {
        cv.Required(CONF_ELEMENTS): cv.All(
            cv.ensure_list(TYPED_ELEMENT_SCHEMA),
            cv.Length(min=1),
        ),
        cv.Optional(CONF_ACTIVE_MODE): cv.enum(ACTIVE_MODE, upper=True, space="_"),
    }
)

TYPED_CONTAINER_ELEMENT_SCHEMAS = {
    CONF_HORIZONTAL: CONTAINER_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(HorizontalElement),
        }
    ),
    CONF_OVERLAY: CONTAINER_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(OverlayElement),
        }
    ),
    CONF_PRIORITY: CONTAINER_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(PriorityElement),
        }
    ),
    CONF_RANDOM: CONTAINER_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(RandomElement),
        }
    ),
    CONF_SEQUENCE: CONTAINER_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(SequenceElement),
        }
    ),
    CONF_VERTICAL: CONTAINER_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(VerticalElement),
        }
    ),
}
