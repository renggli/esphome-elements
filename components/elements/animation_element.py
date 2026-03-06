import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

from . import shared
from . import color
from . import element
from . import element_registry

CONF_COLOR_SCHEME = "color_scheme"
CONF_COOLING = "cooling"
CONF_COUNT = "count"
CONF_DENSITY = "density"
CONF_FADE_STEPS = "fade_steps"
CONF_LENGTH = "length"
CONF_SPEED = "speed"
CONF_STRENGTH = "strength"


# Animation Element

AnimationElement = shared.elements_ns.class_("AnimationElement", element.Element)

ANIMATION_ELEMENT_SCHEMA = element.ELEMENT_SCHEMA.extend(
    {
        cv.Optional(CONF_COLOR_SCHEME): color.COLOR_SCHEME_SCHEMA,
        cv.Optional(CONF_SPEED, default=1.0): cv.float_range(min=0.0),
    }
)


async def animation_element_to_code(config, component, parent):
    var = await element.element_to_code(config, component, parent)
    if conf := config.get(CONF_COLOR_SCHEME):
        value = await color.color_scheme_to_code(conf)
        cg.add(var.set_color_scheme(value))
    if conf := config.get(CONF_SPEED):
        cg.add(var.set_speed(conf))
    return var


# Metaballs Animation

MetaballsAnimationElement = shared.elements_ns.class_(
    "MetaballsAnimationElement", AnimationElement
)

MEATBALLS_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(MetaballsAnimationElement),
        cv.Optional(CONF_COUNT, default=3): cv.int_range(min=1),
    }
)


async def metaballs_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    if conf := config.get(CONF_COUNT):
        cg.add(var.set_count(conf))
    return var


element_registry.register_element(
    "meatballs_animation",
    MEATBALLS_ANIMATION_ELEMENT_SCHEMA,
    metaballs_animation_to_code,
)

# Aurora Animation

AuroraAnimationElement = shared.elements_ns.class_(
    "AuroraAnimationElement", AnimationElement
)

AURORA_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(AuroraAnimationElement),
    }
)


async def aurora_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    return var


element_registry.register_element(
    "aurora_animation",
    AURORA_ANIMATION_ELEMENT_SCHEMA,
    aurora_animation_to_code,
)

# Kaleidoscope Animation

KaleidoscopeAnimationElement = shared.elements_ns.class_(
    "KaleidoscopeAnimationElement", AnimationElement
)

KALEIDOSCOPE_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(KaleidoscopeAnimationElement),
    }
)


async def kaleidoscope_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    return var


element_registry.register_element(
    "kaleidoscope_animation",
    KALEIDOSCOPE_ANIMATION_ELEMENT_SCHEMA,
    kaleidoscope_animation_to_code,
)

# Plasma Animation

PlasmaAnimationElement = shared.elements_ns.class_(
    "PlasmaAnimationElement", AnimationElement
)

PLASMA_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(PlasmaAnimationElement),
    }
)


async def plasma_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    return var


element_registry.register_element(
    "plasma_animation",
    PLASMA_ANIMATION_ELEMENT_SCHEMA,
    plasma_animation_to_code,
)

# Ripples Animation

RipplesAnimationElement = shared.elements_ns.class_(
    "RipplesAnimationElement", AnimationElement
)

RIPPLES_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(RipplesAnimationElement),
        cv.Optional(CONF_COUNT, default=8): cv.int_range(min=1),
    }
)


async def ripples_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    if conf := config.get(CONF_COUNT):
        cg.add(var.set_count(conf))
    return var


element_registry.register_element(
    "ripples_animation",
    RIPPLES_ANIMATION_ELEMENT_SCHEMA,
    ripples_animation_to_code,
)

# Spiral Animation

SpiralAnimationElement = shared.elements_ns.class_(
    "SpiralAnimationElement", AnimationElement
)

SPIRAL_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(SpiralAnimationElement),
    }
)


async def spiral_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    return var


element_registry.register_element(
    "spiral_animation",
    SPIRAL_ANIMATION_ELEMENT_SCHEMA,
    spiral_animation_to_code,
)

# Voronoi Animation

VoronoiAnimationElement = shared.elements_ns.class_(
    "VoronoiAnimationElement", AnimationElement
)

VORONOI_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(VoronoiAnimationElement),
        cv.Optional(CONF_COUNT, default=5): cv.int_range(min=1),
    }
)


async def voronoi_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    if conf := config.get(CONF_COUNT):
        cg.add(var.set_count(conf))
    return var


element_registry.register_element(
    "voronoi_animation",
    VORONOI_ANIMATION_ELEMENT_SCHEMA,
    voronoi_animation_to_code,
)

# Interference Animation

InterferenceAnimationElement = shared.elements_ns.class_(
    "InterferenceAnimationElement", AnimationElement
)

INTERFERENCE_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(InterferenceAnimationElement),
        cv.Optional(CONF_COUNT, default=3): cv.int_range(min=1),
    }
)


async def interference_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    if conf := config.get(CONF_COUNT):
        cg.add(var.set_count(conf))
    return var


element_registry.register_element(
    "interference_animation",
    INTERFERENCE_ANIMATION_ELEMENT_SCHEMA,
    interference_animation_to_code,
)

# Julia Animation

JuliaAnimationElement = shared.elements_ns.class_(
    "JuliaAnimationElement", AnimationElement
)

JULIA_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(JuliaAnimationElement),
    }
)


async def julia_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    return var


element_registry.register_element(
    "julia_animation",
    JULIA_ANIMATION_ELEMENT_SCHEMA,
    julia_animation_to_code,
)

# Matrix Animation

MatrixAnimationElement = shared.elements_ns.class_(
    "MatrixAnimationElement", AnimationElement
)

MATRIX_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(MatrixAnimationElement),
        cv.Optional(CONF_LENGTH, default=10.0): cv.float_range(min=1.0),
        cv.Optional(CONF_DENSITY, default=0.5): cv.float_range(min=0.0, max=1.0),
    }
)


async def matrix_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    if conf := config.get(CONF_LENGTH):
        cg.add(var.set_length(conf))
    if conf := config.get(CONF_DENSITY):
        cg.add(var.set_density(conf))
    return var


element_registry.register_element(
    "matrix_animation",
    MATRIX_ANIMATION_ELEMENT_SCHEMA,
    matrix_animation_to_code,
)

# Gradient Animation

GradientAnimationElement = shared.elements_ns.class_(
    "GradientAnimationElement", AnimationElement
)

GRADIENT_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(GradientAnimationElement),
    }
)


async def gradient_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    return var


element_registry.register_element(
    "gradient_animation",
    GRADIENT_ANIMATION_ELEMENT_SCHEMA,
    gradient_animation_to_code,
)

# Fire Animation

FireAnimationElement = shared.elements_ns.class_(
    "FireAnimationElement", AnimationElement
)

FIRE_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(FireAnimationElement),
        cv.Optional(CONF_STRENGTH, default=0.98): cv.float_range(min=0.0, max=5.0),
        cv.Optional(CONF_COOLING, default=0.02): cv.float_range(min=0.0, max=1.0),
    }
)


async def fire_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    if conf := config.get(CONF_STRENGTH):
        cg.add(var.set_strength(conf))
    if conf := config.get(CONF_COOLING):
        cg.add(var.set_cooling(conf))
    return var


element_registry.register_element(
    "fire_animation",
    FIRE_ANIMATION_ELEMENT_SCHEMA,
    fire_animation_to_code,
)

# Tunnel Animation

TunnelAnimationElement = shared.elements_ns.class_(
    "TunnelAnimationElement", AnimationElement
)

TUNNEL_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(TunnelAnimationElement),
    }
)


async def tunnel_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    return var


element_registry.register_element(
    "tunnel_animation",
    TUNNEL_ANIMATION_ELEMENT_SCHEMA,
    tunnel_animation_to_code,
)

# Wave Animation

WaveAnimationElement = shared.elements_ns.class_(
    "WaveAnimationElement", AnimationElement
)

WAVE_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(WaveAnimationElement),
    }
)


async def wave_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    return var


element_registry.register_element(
    "wave_animation",
    WAVE_ANIMATION_ELEMENT_SCHEMA,
    wave_animation_to_code,
)

# Stars Animation

StarsAnimationElement = shared.elements_ns.class_(
    "StarsAnimationElement", AnimationElement
)

STARS_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(StarsAnimationElement),
        cv.Optional(CONF_DENSITY, default=0.05): cv.float_range(min=0.0, max=1.0),
    }
)


async def stars_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    if conf := config.get(CONF_DENSITY):
        cg.add(var.set_density(conf))
    return var


element_registry.register_element(
    "stars_animation",
    STARS_ANIMATION_ELEMENT_SCHEMA,
    stars_animation_to_code,
)

# Game of Life Animation

GameOfLifeAnimationElement = shared.elements_ns.class_(
    "GameOfLifeAnimationElement", AnimationElement
)

GAME_OF_LIFE_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(GameOfLifeAnimationElement),
        cv.Optional(CONF_DENSITY, default=0.25): cv.float_range(min=0.0, max=1.0),
        cv.Optional(CONF_FADE_STEPS, default=8): cv.int_range(min=0, max=15),
    }
)


async def game_of_life_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    if conf := config.get(CONF_DENSITY):
        cg.add(var.set_density(conf))
    if conf := config.get(CONF_FADE_STEPS):
        cg.add(var.set_fade_steps(conf))
    return var


element_registry.register_element(
    "game_of_life_animation",
    GAME_OF_LIFE_ANIMATION_ELEMENT_SCHEMA,
    game_of_life_animation_to_code,
)

# Platonic Solid Animation

PlatonicSolidAnimationElement = shared.elements_ns.class_(
    "PlatonicSolidAnimationElement", AnimationElement
)

PlatonicSolid = shared.elements_ns.enum("PlatonicSolid", is_class=True)

PLATONIC_SOLID_OPTIONS = {
    "tetrahedron": PlatonicSolid.TETRAHEDRON,
    "cube": PlatonicSolid.CUBE,
    "octahedron": PlatonicSolid.OCTAHEDRON,
    "icosahedron": PlatonicSolid.ICOSAHEDRON,
    "dodecahedron": PlatonicSolid.DODECAHEDRON,
}

CONF_SOLID = "solid"

PLATONIC_SOLID_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(PlatonicSolidAnimationElement),
        cv.Optional(CONF_SOLID, default="icosahedron"): cv.enum(
            PLATONIC_SOLID_OPTIONS, lower=True
        ),
    }
)


async def platonic_solid_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    cg.add(var.set_solid(config[CONF_SOLID]))
    return var


element_registry.register_element(
    "platonic_solid_animation",
    PLATONIC_SOLID_ANIMATION_ELEMENT_SCHEMA,
    platonic_solid_animation_to_code,
)
