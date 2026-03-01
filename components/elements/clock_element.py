import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import time
from esphome.const import CONF_ID

from . import shared
from . import element
from . import element_registry
from . import color

CONF_COLOR = "color"
CONF_END = "end"
CONF_HOUR_HAND = "hour_hand"
CONF_HOUR_MARKERS = "hour_markers"
CONF_MINUTE_HAND = "minute_hand"
CONF_MINUTE_MARKERS = "minute_markers"
CONF_QUARTER_MARKERS = "quarter_markers"
CONF_SECOND_HAND = "second_hand"
CONF_SMOOTH = "smooth"
CONF_START = "start"
CONF_TIME = "time"
CONF_VISIBLE = "visible"

# Clock options

ClockOptions = shared.elements_ns.class_("ClockOptions")


def clock_options_schema(start, end, default_color, visible=True, smooth=None):
    schema = cv.Schema(
        {
            cv.Optional(CONF_START, default=start): cv.float_range(-1, 1),
            cv.Optional(CONF_END, default=end): cv.float_range(-1, 1),
            cv.Optional(CONF_COLOR, default=default_color): color.COLOR_SCHEMA,
            cv.Optional(CONF_VISIBLE, default=visible): cv.boolean,
        }
    )
    if smooth is not None:
        schema = schema.extend(
            {
                cv.Optional(CONF_SMOOTH, default=smooth): cv.boolean,
            }
        )
    return schema


async def clock_options_to_code(config):
    return cg.StructInitializer(
        ClockOptions,
        (CONF_START, config.get(CONF_START)),
        (CONF_END, config.get(CONF_END)),
        (CONF_COLOR, await color.color_to_code(config.get(CONF_COLOR))),
        (CONF_VISIBLE, config.get(CONF_VISIBLE)),
        (CONF_SMOOTH, config.get(CONF_SMOOTH) or False),
    )


# Clock Element

ClockElement = shared.elements_ns.class_("ClockElement", element.Element)

CLOCK_ELEMENT_SCHEMA = element.ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(ClockElement),
        cv.Required(CONF_TIME): cv.use_id(time.RealTimeClock),
        cv.Optional(CONF_MINUTE_MARKERS, default={}): clock_options_schema(
            0.95, 1.00, "#0000ff", visible=False
        ),
        cv.Optional(CONF_HOUR_MARKERS, default={}): clock_options_schema(
            0.90, 1.00, "#0000ff"
        ),
        cv.Optional(CONF_QUARTER_MARKERS, default={}): clock_options_schema(
            0.75, 1.00, "#0000ff"
        ),
        cv.Optional(CONF_SECOND_HAND, default={}): clock_options_schema(
            0.00, 0.75, "#ff0000", smooth=True
        ),
        cv.Optional(CONF_MINUTE_HAND, default={}): clock_options_schema(
            0.00, 0.95, "#ffffff", smooth=False
        ),
        cv.Optional(CONF_HOUR_HAND, default={}): clock_options_schema(
            0.00, 0.66, "#ffffff", smooth=True
        ),
    }
)


async def clock_element_to_code(config, component, parent):
    var = await element.element_to_code(config, component, parent)
    if conf := config.get(CONF_TIME):
        value = await cg.get_variable(conf)
        cg.add(var.set_time(value))
    for key in [
        CONF_MINUTE_MARKERS,
        CONF_HOUR_MARKERS,
        CONF_QUARTER_MARKERS,
        CONF_SECOND_HAND,
        CONF_MINUTE_HAND,
        CONF_HOUR_HAND,
    ]:
        if conf := config.get(key):
            value = await clock_options_to_code(conf)
            cg.add(getattr(var, "set_" + key)(value))
    return var


element_registry.register_element("clock", CLOCK_ELEMENT_SCHEMA, clock_element_to_code)
