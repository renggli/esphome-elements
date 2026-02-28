import esphome.config_validation as cv
from esphome.const import CONF_COUNT, CONF_ID
from .color import COLOR_SCHEME_SCHEMA, CONF_COLOR_SCHEME
from .element import Element, ELEMENT_SCHEMA
from .shared import elements_ns

AnimationElement = elements_ns.class_("AnimationElement", Element)

MetaballsAnimationElement = elements_ns.class_(
    "MetaballsAnimationElement", AnimationElement
)
AuroraAnimationElement = elements_ns.class_("AuroraAnimationElement", AnimationElement)
KaleidoscopeAnimationElement = elements_ns.class_(
    "KaleidoscopeAnimationElement", AnimationElement
)
PlasmaAnimationElement = elements_ns.class_("PlasmaAnimationElement", AnimationElement)
RipplesAnimationElement = elements_ns.class_(
    "RipplesAnimationElement", AnimationElement
)
SpiralAnimationElement = elements_ns.class_("SpiralAnimationElement", AnimationElement)
VoronoiAnimationElement = elements_ns.class_(
    "VoronoiAnimationElement", AnimationElement
)
InterferenceAnimationElement = elements_ns.class_(
    "InterferenceAnimationElement", AnimationElement
)
JuliaAnimationElement = elements_ns.class_("JuliaAnimationElement", AnimationElement)
MatrixAnimationElement = elements_ns.class_("MatrixAnimationElement", AnimationElement)
GradientAnimationElement = elements_ns.class_(
    "GradientAnimationElement", AnimationElement
)
FireAnimationElement = elements_ns.class_("FireAnimationElement", AnimationElement)
TunnelAnimationElement = elements_ns.class_("TunnelAnimationElement", AnimationElement)
WaveAnimationElement = elements_ns.class_("WaveAnimationElement", AnimationElement)
StarsAnimationElement = elements_ns.class_("StarsAnimationElement", AnimationElement)

CONF_METABALLS_ANIMATION = "meatballs_animation"
CONF_AURORA_ANIMATION = "aurora_animation"
CONF_KALEIDOSCOPE_ANIMATION = "kaleidoscope_animation"
CONF_PLASMA_ANIMATION = "plasma_animation"
CONF_RIPPLES_ANIMATION = "ripples_animation"
CONF_SPIRAL_ANIMATION = "spiral_animation"
CONF_VORONOI_ANIMATION = "voronoi_animation"
CONF_INTERFERENCE_ANIMATION = "interference_animation"
CONF_JULIA_ANIMATION = "julia_animation"
CONF_MATRIX_ANIMATION = "matrix_animation"
CONF_GRADIENT_ANIMATION = "gradient_animation"
CONF_FIRE_ANIMATION = "fire_animation"
CONF_TUNNEL_ANIMATION = "tunnel_animation"
CONF_WAVE_ANIMATION = "wave_animation"
CONF_STARS_ANIMATION = "stars_animation"

CONF_DENSITY = "density"
CONF_LENGTH = "length"
CONF_SPEED = "speed"
CONF_STRENGTH = "strength"
CONF_COOLING = "cooling"

ANIMATION_ELEMENT_SCHEMA = ELEMENT_SCHEMA.extend(
    {
        cv.Required(CONF_COLOR_SCHEME): COLOR_SCHEME_SCHEMA,
        cv.Optional(CONF_SPEED, default=1.0): cv.float_range(min=0.0),
    }
)

TYPED_ANIMATION_ELEMENT_SCHEMAS = {
    CONF_METABALLS_ANIMATION: ANIMATION_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(MetaballsAnimationElement),
            cv.Optional(CONF_COUNT, default=3): cv.int_range(min=1),
        }
    ),
    CONF_AURORA_ANIMATION: ANIMATION_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(AuroraAnimationElement),
        }
    ),
    CONF_KALEIDOSCOPE_ANIMATION: ANIMATION_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(KaleidoscopeAnimationElement),
        }
    ),
    CONF_PLASMA_ANIMATION: ANIMATION_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(PlasmaAnimationElement),
        }
    ),
    CONF_RIPPLES_ANIMATION: ANIMATION_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(RipplesAnimationElement),
        }
    ),
    CONF_SPIRAL_ANIMATION: ANIMATION_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(SpiralAnimationElement),
        }
    ),
    CONF_VORONOI_ANIMATION: ANIMATION_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(VoronoiAnimationElement),
            cv.Optional(CONF_COUNT, default=6): cv.int_range(min=1),
        }
    ),
    CONF_INTERFERENCE_ANIMATION: ANIMATION_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(InterferenceAnimationElement),
        }
    ),
    CONF_JULIA_ANIMATION: ANIMATION_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(JuliaAnimationElement),
        }
    ),
    CONF_MATRIX_ANIMATION: ANIMATION_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(MatrixAnimationElement),
            cv.Optional(CONF_LENGTH, default=10.0): cv.float_range(min=1.0),
            cv.Optional(CONF_DENSITY, default=0.5): cv.float_range(min=0.0, max=1.0),
        }
    ),
    CONF_GRADIENT_ANIMATION: ANIMATION_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(GradientAnimationElement),
        }
    ),
    CONF_FIRE_ANIMATION: ANIMATION_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(FireAnimationElement),
            cv.Optional(CONF_STRENGTH, default=0.9): cv.float_range(min=0.1, max=5.0),
            cv.Optional(CONF_COOLING, default=0.12): cv.float_range(min=0.01, max=1.0),
        }
    ),
    CONF_TUNNEL_ANIMATION: ANIMATION_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(TunnelAnimationElement),
        }
    ),
    CONF_WAVE_ANIMATION: ANIMATION_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(WaveAnimationElement),
        }
    ),
    CONF_STARS_ANIMATION: ANIMATION_ELEMENT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(StarsAnimationElement),
            cv.Optional(CONF_DENSITY, default=0.02): cv.float_range(min=0.0, max=1.0),
        }
    ),
}
