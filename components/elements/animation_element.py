import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

from . import shared
from . import color
from . import element
from . import element_registry

CONF_BETA = "beta"
CONF_COLOR_SCHEME = "color_scheme"
CONF_COOLING = "cooling"
CONF_COUNT = "count"
CONF_DENSITY = "density"
CONF_FADE_STEPS = "fade_steps"
CONF_LAYERS = "layers"
CONF_LENGTH = "length"
CONF_RHO = "rho"
CONF_SHAPE = "shape"
CONF_SIGMA = "sigma"
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

# Solid Animation

SolidAnimationElement = shared.elements_ns.class_(
    "SolidAnimationElement", AnimationElement
)

SHAPE_DATA = {
    # Platonic solids
    "tetrahedron": {
        "url": "https://en.wikipedia.org/wiki/Tetrahedron",
        "verts": [
            [0.0, 1.0, 0.0],
            [0.942809, -0.333333, 0.0],
            [-0.471405, -0.333333, 0.816497],
            [-0.471405, -0.333333, -0.816497],
        ],
        "edges": [[0, 1], [0, 2], [0, 3], [1, 2], [1, 3], [2, 3]],
    },
    "cube": {
        "url": "https://en.wikipedia.org/wiki/Cube",
        "verts": [
            [-0.57735, -0.57735, -0.57735],
            [0.57735, -0.57735, -0.57735],
            [0.57735, 0.57735, -0.57735],
            [-0.57735, 0.57735, -0.57735],
            [-0.57735, -0.57735, 0.57735],
            [0.57735, -0.57735, 0.57735],
            [0.57735, 0.57735, 0.57735],
            [-0.57735, 0.57735, 0.57735],
        ],
        "edges": [
            [0, 1],
            [1, 2],
            [2, 3],
            [3, 0],
            [4, 5],
            [5, 6],
            [6, 7],
            [7, 4],
            [0, 4],
            [1, 5],
            [2, 6],
            [3, 7],
        ],
    },
    "octahedron": {
        "url": "https://en.wikipedia.org/wiki/Octahedron",
        "verts": [
            [1.0, 0.0, 0.0],
            [-1.0, 0.0, 0.0],
            [0.0, 1.0, 0.0],
            [0.0, -1.0, 0.0],
            [0.0, 0.0, 1.0],
            [0.0, 0.0, -1.0],
        ],
        "edges": [
            [0, 2],
            [0, 3],
            [0, 4],
            [0, 5],
            [1, 2],
            [1, 3],
            [1, 4],
            [1, 5],
            [2, 4],
            [2, 5],
            [3, 4],
            [3, 5],
        ],
    },
    "icosahedron": {
        "url": "https://en.wikipedia.org/wiki/Icosahedron",
        "verts": [
            [0.0, 0.525731, 0.850651],
            [0.0, -0.525731, 0.850651],
            [0.0, 0.525731, -0.850651],
            [0.0, -0.525731, -0.850651],
            [0.525731, 0.850651, 0.0],
            [-0.525731, 0.850651, 0.0],
            [0.525731, -0.850651, 0.0],
            [-0.525731, -0.850651, 0.0],
            [0.850651, 0.0, 0.525731],
            [-0.850651, 0.0, 0.525731],
            [0.850651, 0.0, -0.525731],
            [-0.850651, 0.0, -0.525731],
        ],
        "edges": [
            [0, 1],
            [0, 4],
            [0, 5],
            [0, 8],
            [0, 9],
            [1, 6],
            [1, 7],
            [1, 8],
            [1, 9],
            [2, 3],
            [2, 4],
            [2, 5],
            [2, 10],
            [2, 11],
            [3, 6],
            [3, 7],
            [3, 10],
            [3, 11],
            [4, 5],
            [4, 8],
            [4, 10],
            [5, 9],
            [5, 11],
            [6, 7],
            [6, 8],
            [6, 10],
            [7, 9],
            [7, 11],
            [8, 10],
            [9, 11],
        ],
    },
    "dodecahedron": {
        "url": "https://en.wikipedia.org/wiki/Dodecahedron",
        "verts": [
            [0.57735, 0.57735, 0.57735],
            [0.57735, 0.57735, -0.57735],
            [0.57735, -0.57735, 0.57735],
            [0.57735, -0.57735, -0.57735],
            [-0.57735, 0.57735, 0.57735],
            [-0.57735, 0.57735, -0.57735],
            [-0.57735, -0.57735, 0.57735],
            [-0.57735, -0.57735, -0.57735],
            [0.0, 0.356822, 0.934172],
            [0.0, 0.356822, -0.934172],
            [0.0, -0.356822, 0.934172],
            [0.0, -0.356822, -0.934172],
            [0.356822, 0.934172, 0.0],
            [-0.356822, 0.934172, 0.0],
            [0.356822, -0.934172, 0.0],
            [-0.356822, -0.934172, 0.0],
            [0.934172, 0.0, 0.356822],
            [0.934172, 0.0, -0.356822],
            [-0.934172, 0.0, 0.356822],
            [-0.934172, 0.0, -0.356822],
        ],
        "edges": [
            [0, 8],
            [0, 12],
            [0, 16],
            [1, 9],
            [1, 12],
            [1, 17],
            [2, 10],
            [2, 14],
            [2, 16],
            [3, 11],
            [3, 14],
            [3, 17],
            [4, 8],
            [4, 13],
            [4, 18],
            [5, 9],
            [5, 13],
            [5, 19],
            [6, 10],
            [6, 15],
            [6, 18],
            [7, 11],
            [7, 15],
            [7, 19],
            [8, 10],
            [9, 11],
            [16, 17],
            [18, 19],
            [12, 13],
            [14, 15],
        ],
    },
    # Archimedean solids
    "truncated_tetrahedron": {
        "url": "https://en.wikipedia.org/wiki/Truncated_tetrahedron",
        "verts": [
            [0.301511, 0.301511, 0.904534],
            [0.301511, 0.904534, 0.301511],
            [0.904534, 0.301511, 0.301511],
            [0.301511, -0.301511, -0.904534],
            [-0.301511, -0.904534, 0.301511],
            [-0.904534, 0.301511, -0.301511],
            [-0.301511, 0.301511, -0.904534],
            [0.301511, -0.904534, -0.301511],
            [-0.904534, -0.301511, 0.301511],
            [-0.301511, -0.301511, 0.904534],
            [-0.301511, 0.904534, -0.301511],
            [0.904534, -0.301511, -0.301511],
        ],
        "edges": [
            [0, 1],
            [0, 2],
            [0, 9],
            [1, 2],
            [1, 10],
            [2, 11],
            [3, 6],
            [3, 7],
            [3, 11],
            [4, 7],
            [4, 8],
            [4, 9],
            [5, 6],
            [5, 8],
            [5, 10],
            [6, 10],
            [7, 11],
            [8, 9],
        ],
    },
    "cuboctahedron": {
        "url": "https://en.wikipedia.org/wiki/Cuboctahedron",
        "verts": [
            [0.000000, -0.707107, -0.707107],
            [0.707107, 0.000000, 0.707107],
            [0.707107, 0.707107, 0.000000],
            [-0.707107, 0.000000, 0.707107],
            [0.707107, 0.000000, -0.707107],
            [-0.707107, 0.000000, -0.707107],
            [0.000000, -0.707107, 0.707107],
            [-0.707107, 0.707107, 0.000000],
            [0.707107, -0.707107, 0.000000],
            [-0.707107, -0.707107, 0.000000],
            [0.000000, 0.707107, -0.707107],
            [0.000000, 0.707107, 0.707107],
        ],
        "edges": [
            [0, 4],
            [0, 5],
            [0, 8],
            [0, 9],
            [1, 2],
            [1, 6],
            [1, 8],
            [1, 11],
            [2, 4],
            [2, 10],
            [2, 11],
            [3, 6],
            [3, 7],
            [3, 9],
            [3, 11],
            [4, 8],
            [4, 10],
            [5, 7],
            [5, 9],
            [5, 10],
            [6, 8],
            [6, 9],
            [7, 10],
            [7, 11],
        ],
    },
    "truncated_cube": {
        "url": "https://en.wikipedia.org/wiki/Truncated_cube",
        "verts": [
            [-0.281085, 0.678598, 0.678598],
            [0.678598, -0.281085, 0.678598],
            [0.678598, -0.281085, -0.678598],
            [-0.678598, 0.678598, 0.281085],
            [-0.678598, -0.678598, 0.281085],
            [-0.678598, -0.678598, -0.281085],
            [0.281085, -0.678598, -0.678598],
            [0.281085, -0.678598, 0.678598],
            [-0.678598, -0.281085, -0.678598],
            [-0.281085, -0.678598, -0.678598],
            [-0.678598, -0.281085, 0.678598],
            [-0.281085, -0.678598, 0.678598],
            [0.678598, 0.281085, -0.678598],
            [0.678598, 0.281085, 0.678598],
            [-0.678598, 0.281085, -0.678598],
            [-0.678598, 0.281085, 0.678598],
            [0.281085, 0.678598, -0.678598],
            [0.678598, 0.678598, -0.281085],
            [0.281085, 0.678598, 0.678598],
            [0.678598, 0.678598, 0.281085],
            [-0.678598, 0.678598, -0.281085],
            [0.678598, -0.678598, -0.281085],
            [0.678598, -0.678598, 0.281085],
            [-0.281085, 0.678598, -0.678598],
        ],
        "edges": [
            [0, 3],
            [0, 15],
            [1, 7],
            [1, 22],
            [2, 6],
            [2, 21],
            [3, 15],
            [4, 10],
            [4, 11],
            [5, 8],
            [5, 9],
            [6, 21],
            [7, 22],
            [8, 9],
            [10, 11],
            [12, 16],
            [12, 17],
            [13, 18],
            [13, 19],
            [14, 20],
            [14, 23],
            [16, 17],
            [18, 19],
            [20, 23],
            [0, 18],
            [1, 13],
            [2, 12],
            [3, 20],
            [4, 5],
            [6, 9],
            [7, 11],
            [8, 14],
            [10, 15],
            [16, 23],
            [17, 19],
            [21, 22],
        ],
    },
    "truncated_octahedron": {
        "url": "https://en.wikipedia.org/wiki/Truncated_octahedron",
        "verts": [
            [0.000000, -0.447214, -0.894427],
            [0.000000, -0.894427, -0.447214],
            [0.000000, -0.894427, 0.447214],
            [0.894427, 0.447214, 0.000000],
            [-0.447214, 0.000000, 0.894427],
            [0.000000, 0.894427, -0.447214],
            [0.000000, 0.894427, 0.447214],
            [0.447214, 0.000000, -0.894427],
            [0.894427, 0.000000, -0.447214],
            [0.894427, 0.000000, 0.447214],
            [-0.894427, 0.447214, 0.000000],
            [0.000000, 0.447214, 0.894427],
            [0.447214, -0.894427, 0.000000],
            [0.894427, -0.447214, 0.000000],
            [-0.894427, 0.000000, 0.447214],
            [-0.447214, 0.000000, -0.894427],
            [-0.894427, 0.000000, -0.447214],
            [-0.447214, -0.894427, 0.000000],
            [0.000000, 0.447214, -0.894427],
            [-0.894427, -0.447214, 0.000000],
            [0.000000, -0.447214, 0.894427],
            [0.447214, 0.894427, 0.000000],
            [-0.447214, 0.894427, 0.000000],
            [0.447214, 0.000000, 0.894427],
        ],
        "edges": [
            [0, 1],
            [0, 7],
            [0, 15],
            [1, 12],
            [1, 17],
            [2, 12],
            [2, 17],
            [2, 20],
            [3, 8],
            [3, 9],
            [3, 21],
            [4, 11],
            [4, 14],
            [4, 20],
            [5, 18],
            [5, 21],
            [5, 22],
            [6, 11],
            [6, 21],
            [6, 22],
            [7, 8],
            [7, 18],
            [8, 13],
            [9, 13],
            [9, 23],
            [10, 14],
            [10, 16],
            [10, 22],
            [11, 23],
            [12, 13],
            [14, 19],
            [15, 16],
            [15, 18],
            [16, 19],
            [17, 19],
            [20, 23],
        ],
    },
    "rhombicuboctahedron": {
        "url": "https://en.wikipedia.org/wiki/Rhombicuboctahedron",
        "verts": [
            [-0.357407, 0.357407, 0.862856],
            [0.357407, -0.357407, 0.862856],
            [0.357407, 0.862856, 0.357407],
            [0.357407, 0.862856, -0.357407],
            [0.357407, -0.862856, -0.357407],
            [0.357407, -0.862856, 0.357407],
            [-0.357407, 0.862856, -0.357407],
            [-0.357407, -0.357407, 0.862856],
            [-0.357407, 0.862856, 0.357407],
            [0.862856, 0.357407, -0.357407],
            [-0.862856, 0.357407, -0.357407],
            [0.862856, 0.357407, 0.357407],
            [-0.862856, 0.357407, 0.357407],
            [-0.357407, -0.862856, 0.357407],
            [-0.357407, -0.862856, -0.357407],
            [0.862856, -0.357407, -0.357407],
            [0.357407, 0.357407, -0.862856],
            [0.862856, -0.357407, 0.357407],
            [-0.357407, 0.357407, -0.862856],
            [0.357407, -0.357407, -0.862856],
            [-0.862856, -0.357407, -0.357407],
            [-0.862856, -0.357407, 0.357407],
            [0.357407, 0.357407, 0.862856],
            [-0.357407, -0.357407, -0.862856],
        ],
        "edges": [
            [0, 7],
            [0, 8],
            [0, 12],
            [0, 22],
            [1, 5],
            [1, 7],
            [1, 17],
            [1, 22],
            [2, 3],
            [2, 8],
            [2, 11],
            [2, 22],
            [3, 6],
            [3, 9],
            [3, 16],
            [4, 5],
            [4, 14],
            [4, 15],
            [4, 19],
            [5, 13],
            [5, 17],
            [6, 8],
            [6, 10],
            [6, 18],
            [7, 13],
            [7, 21],
            [8, 12],
            [9, 11],
            [9, 15],
            [9, 16],
            [10, 12],
            [10, 18],
            [10, 20],
            [11, 17],
            [11, 22],
            [12, 21],
            [13, 14],
            [13, 21],
            [14, 20],
            [14, 23],
            [15, 17],
            [15, 19],
            [16, 18],
            [16, 19],
            [18, 23],
            [19, 23],
            [20, 21],
            [20, 23],
        ],
    },
    "truncated_cuboctahedron": {
        "url": "https://en.wikipedia.org/wiki/Truncated_cuboctahedron",
        "verts": [
            [-0.825943, 0.215739, -0.520841],
            [-0.825943, -0.520841, -0.215739],
            [-0.215739, 0.825943, -0.520841],
            [-0.825943, -0.520841, 0.215739],
            [0.215739, -0.825943, -0.520841],
            [0.825943, 0.215739, -0.520841],
            [-0.215739, -0.825943, -0.520841],
            [-0.520841, -0.215739, -0.825943],
            [-0.520841, -0.215739, 0.825943],
            [0.215739, 0.825943, 0.520841],
            [-0.825943, 0.215739, 0.520841],
            [0.215739, -0.825943, 0.520841],
            [0.825943, 0.215739, 0.520841],
            [-0.215739, 0.825943, 0.520841],
            [-0.825943, -0.215739, -0.520841],
            [0.825943, -0.520841, 0.215739],
            [0.825943, -0.215739, -0.520841],
            [-0.215739, -0.825943, 0.520841],
            [0.215739, 0.520841, -0.825943],
            [0.215739, -0.520841, -0.825943],
            [0.215739, 0.520841, 0.825943],
            [-0.215739, 0.520841, -0.825943],
            [0.215739, -0.520841, 0.825943],
            [-0.215739, 0.520841, 0.825943],
            [0.520841, 0.215739, -0.825943],
            [0.520841, -0.825943, -0.215739],
            [-0.520841, 0.215739, -0.825943],
            [0.520841, 0.825943, -0.215739],
            [0.520841, -0.825943, 0.215739],
            [-0.825943, 0.520841, -0.215739],
            [0.520841, 0.825943, 0.215739],
            [-0.825943, -0.215739, 0.520841],
            [0.520841, 0.215739, 0.825943],
            [-0.825943, 0.520841, 0.215739],
            [-0.215739, -0.520841, -0.825943],
            [-0.520841, 0.215739, 0.825943],
            [0.825943, -0.215739, 0.520841],
            [0.825943, 0.520841, -0.215739],
            [-0.215739, -0.520841, 0.825943],
            [0.520841, -0.215739, -0.825943],
            [-0.520841, -0.825943, -0.215739],
            [0.825943, -0.520841, -0.215739],
            [-0.520841, 0.825943, -0.215739],
            [-0.520841, -0.825943, 0.215739],
            [0.825943, 0.520841, 0.215739],
            [0.215739, 0.825943, -0.520841],
            [-0.520841, 0.825943, 0.215739],
            [0.520841, -0.215739, 0.825943],
        ],
        "edges": [
            [0, 14],
            [0, 29],
            [1, 3],
            [1, 14],
            [2, 42],
            [2, 45],
            [3, 31],
            [4, 6],
            [4, 25],
            [5, 16],
            [5, 37],
            [6, 40],
            [7, 26],
            [7, 34],
            [8, 35],
            [8, 38],
            [9, 13],
            [9, 30],
            [10, 31],
            [10, 33],
            [11, 17],
            [11, 28],
            [12, 36],
            [12, 44],
            [13, 46],
            [15, 36],
            [15, 41],
            [16, 41],
            [17, 43],
            [18, 21],
            [18, 24],
            [19, 34],
            [19, 39],
            [20, 23],
            [20, 32],
            [21, 26],
            [22, 38],
            [22, 47],
            [23, 35],
            [24, 39],
            [25, 28],
            [27, 30],
            [27, 45],
            [29, 33],
            [32, 47],
            [37, 44],
            [40, 43],
            [42, 46],
            [0, 26],
            [1, 40],
            [2, 21],
            [3, 43],
            [4, 19],
            [5, 24],
            [6, 34],
            [7, 14],
            [8, 31],
            [9, 20],
            [10, 35],
            [11, 22],
            [12, 32],
            [13, 23],
            [15, 28],
            [16, 39],
            [17, 38],
            [18, 45],
            [25, 41],
            [27, 37],
            [29, 42],
            [30, 44],
            [33, 46],
            [36, 47],
        ],
    },
    "snub_cube": {
        "url": "https://en.wikipedia.org/wiki/Snub_cube",
        "verts": [
            [0.462321, 0.850340, 0.251359],
            [0.462321, -0.850340, -0.251359],
            [-0.462321, -0.850340, 0.251359],
            [-0.462321, 0.850340, -0.251359],
            [-0.850340, -0.251359, 0.462321],
            [0.850340, -0.251359, -0.462321],
            [-0.850340, 0.251359, -0.462321],
            [0.850340, 0.251359, 0.462321],
            [0.251359, -0.462321, -0.850340],
            [-0.251359, -0.462321, 0.850340],
            [0.251359, 0.462321, 0.850340],
            [-0.251359, 0.462321, -0.850340],
            [-0.462321, -0.251359, -0.850340],
            [0.462321, 0.251359, -0.850340],
            [0.462321, -0.251359, 0.850340],
            [-0.462321, 0.251359, 0.850340],
            [-0.251359, 0.850340, 0.462321],
            [-0.251359, -0.850340, -0.462321],
            [0.251359, -0.850340, 0.462321],
            [0.251359, 0.850340, -0.462321],
            [0.850340, -0.462321, 0.251359],
            [-0.850340, -0.462321, -0.251359],
            [-0.850340, 0.462321, 0.251359],
            [0.850340, 0.462321, -0.251359],
        ],
        "edges": [
            [0, 7],
            [0, 10],
            [1, 5],
            [1, 8],
            [2, 4],
            [2, 9],
            [3, 6],
            [3, 11],
            [4, 9],
            [5, 8],
            [6, 11],
            [7, 10],
            [12, 17],
            [12, 21],
            [13, 19],
            [13, 23],
            [14, 18],
            [14, 20],
            [15, 16],
            [15, 22],
            [16, 22],
            [17, 21],
            [18, 20],
            [19, 23],
            [0, 16],
            [0, 19],
            [0, 23],
            [1, 17],
            [1, 18],
            [1, 20],
            [2, 17],
            [2, 18],
            [2, 21],
            [3, 16],
            [3, 19],
            [3, 22],
            [4, 15],
            [4, 21],
            [4, 22],
            [5, 13],
            [5, 20],
            [5, 23],
            [6, 12],
            [6, 21],
            [6, 22],
            [7, 14],
            [7, 20],
            [7, 23],
            [8, 12],
            [8, 13],
            [8, 17],
            [9, 14],
            [9, 15],
            [9, 18],
            [10, 14],
            [10, 15],
            [10, 16],
            [11, 12],
            [11, 13],
            [11, 19],
        ],
    },
    "icosidodecahedron": {
        "url": "https://en.wikipedia.org/wiki/Icosidodecahedron",
        "verts": [
            [0.000000, 0.000000, 1.000000],
            [1.000000, 0.000000, 0.000000],
            [0.000000, 1.000000, 0.000000],
            [-1.000000, 0.000000, 0.000000],
            [0.000000, -1.000000, 0.000000],
            [0.000000, 0.000000, -1.000000],
            [0.809017, -0.309017, -0.500000],
            [0.309017, -0.500000, 0.809017],
            [-0.809017, 0.309017, 0.500000],
            [-0.809017, 0.309017, -0.500000],
            [0.809017, 0.309017, 0.500000],
            [0.309017, 0.500000, -0.809017],
            [0.500000, -0.809017, -0.309017],
            [0.809017, 0.309017, -0.500000],
            [-0.309017, -0.500000, -0.809017],
            [-0.500000, 0.809017, 0.309017],
            [0.309017, 0.500000, 0.809017],
            [-0.309017, 0.500000, -0.809017],
            [-0.309017, -0.500000, 0.809017],
            [-0.809017, -0.309017, 0.500000],
            [0.500000, 0.809017, -0.309017],
            [0.500000, 0.809017, 0.309017],
            [-0.309017, 0.500000, 0.809017],
            [-0.500000, -0.809017, 0.309017],
            [-0.500000, -0.809017, -0.309017],
            [0.309017, -0.500000, -0.809017],
            [-0.809017, -0.309017, -0.500000],
            [0.809017, -0.309017, 0.500000],
            [0.500000, -0.809017, 0.309017],
            [-0.500000, 0.809017, -0.309017],
        ],
        "edges": [
            [0, 7],
            [0, 16],
            [0, 18],
            [0, 22],
            [1, 6],
            [1, 10],
            [1, 13],
            [1, 27],
            [2, 15],
            [2, 20],
            [2, 21],
            [2, 29],
            [3, 8],
            [3, 9],
            [3, 19],
            [3, 26],
            [4, 12],
            [4, 23],
            [4, 24],
            [4, 28],
            [5, 11],
            [5, 14],
            [5, 17],
            [5, 25],
            [6, 13],
            [7, 18],
            [8, 19],
            [9, 26],
            [10, 27],
            [11, 17],
            [12, 28],
            [14, 25],
            [15, 29],
            [16, 22],
            [20, 21],
            [23, 24],
            [6, 12],
            [6, 25],
            [7, 27],
            [7, 28],
            [8, 15],
            [8, 22],
            [9, 17],
            [9, 29],
            [10, 16],
            [10, 21],
            [11, 13],
            [11, 20],
            [12, 25],
            [13, 20],
            [14, 24],
            [14, 26],
            [15, 22],
            [16, 21],
            [17, 29],
            [18, 19],
            [18, 23],
            [19, 23],
            [24, 26],
            [27, 28],
        ],
    },
    "truncated_dodecahedron": {
        "url": "https://en.wikipedia.org/wiki/Truncated_dodecahedron",
        "verts": [
            [0.985722, 0.000000, -0.168381],
            [0.168381, -0.985722, 0.000000],
            [0.168381, 0.985722, 0.000000],
            [-0.168381, -0.985722, 0.000000],
            [-0.985722, 0.000000, -0.168381],
            [0.000000, -0.168381, -0.985722],
            [0.985722, 0.000000, 0.168381],
            [0.000000, 0.168381, -0.985722],
            [-0.985722, 0.000000, 0.168381],
            [0.000000, -0.168381, 0.985722],
            [-0.168381, 0.985722, 0.000000],
            [0.000000, 0.168381, 0.985722],
            [0.440828, 0.881656, 0.168381],
            [0.168381, -0.440828, -0.881656],
            [0.440828, -0.881656, -0.168381],
            [-0.440828, -0.881656, 0.168381],
            [-0.881656, -0.168381, -0.440828],
            [-0.881656, -0.168381, 0.440828],
            [0.168381, 0.440828, -0.881656],
            [-0.168381, 0.440828, -0.881656],
            [-0.168381, -0.440828, -0.881656],
            [-0.440828, 0.881656, 0.168381],
            [0.881656, -0.168381, -0.440828],
            [0.881656, -0.168381, 0.440828],
            [0.440828, 0.881656, -0.168381],
            [-0.440828, -0.881656, -0.168381],
            [0.168381, -0.440828, 0.881656],
            [0.440828, -0.881656, 0.168381],
            [-0.881656, 0.168381, -0.440828],
            [-0.881656, 0.168381, 0.440828],
            [0.168381, 0.440828, 0.881656],
            [-0.440828, 0.881656, -0.168381],
            [-0.168381, 0.440828, 0.881656],
            [-0.168381, -0.440828, 0.881656],
            [0.881656, 0.168381, -0.440828],
            [0.881656, 0.168381, 0.440828],
            [-0.440828, -0.544894, -0.713275],
            [0.544894, -0.713275, -0.440828],
            [0.544894, 0.713275, -0.440828],
            [0.440828, -0.544894, 0.713275],
            [0.544894, -0.713275, 0.440828],
            [0.544894, 0.713275, 0.440828],
            [-0.544894, -0.713275, -0.440828],
            [-0.544894, 0.713275, -0.440828],
            [-0.440828, 0.544894, -0.713275],
            [-0.544894, -0.713275, 0.440828],
            [-0.544894, 0.713275, 0.440828],
            [0.440828, 0.544894, 0.713275],
            [0.713275, -0.440828, 0.544894],
            [-0.440828, -0.544894, 0.713275],
            [-0.713275, -0.440828, 0.544894],
            [-0.713275, 0.440828, 0.544894],
            [0.713275, 0.440828, 0.544894],
            [0.440828, -0.544894, -0.713275],
            [-0.440828, 0.544894, 0.713275],
            [0.713275, -0.440828, -0.544894],
            [-0.713275, 0.440828, -0.544894],
            [0.440828, 0.544894, -0.713275],
            [0.713275, 0.440828, -0.544894],
            [-0.713275, -0.440828, -0.544894],
        ],
        "edges": [
            [0, 6],
            [0, 22],
            [0, 34],
            [1, 3],
            [1, 14],
            [1, 27],
            [2, 10],
            [2, 12],
            [2, 24],
            [3, 15],
            [3, 25],
            [4, 8],
            [4, 16],
            [4, 28],
            [5, 7],
            [5, 13],
            [5, 20],
            [6, 23],
            [6, 35],
            [7, 18],
            [7, 19],
            [8, 17],
            [8, 29],
            [9, 11],
            [9, 26],
            [9, 33],
            [10, 21],
            [10, 31],
            [11, 30],
            [11, 32],
            [12, 24],
            [12, 41],
            [13, 20],
            [13, 53],
            [14, 27],
            [14, 37],
            [15, 25],
            [15, 45],
            [16, 28],
            [16, 59],
            [17, 29],
            [17, 50],
            [18, 19],
            [18, 57],
            [19, 44],
            [20, 36],
            [21, 31],
            [21, 46],
            [22, 34],
            [22, 55],
            [23, 35],
            [23, 48],
            [24, 38],
            [25, 42],
            [26, 33],
            [26, 39],
            [27, 40],
            [28, 56],
            [29, 51],
            [30, 32],
            [30, 47],
            [31, 43],
            [32, 54],
            [33, 49],
            [34, 58],
            [35, 52],
            [36, 42],
            [36, 59],
            [37, 53],
            [37, 55],
            [38, 57],
            [38, 58],
            [39, 40],
            [39, 48],
            [40, 48],
            [41, 47],
            [41, 52],
            [42, 59],
            [43, 44],
            [43, 56],
            [44, 56],
            [45, 49],
            [45, 50],
            [46, 51],
            [46, 54],
            [47, 52],
            [49, 50],
            [51, 54],
            [53, 55],
            [57, 58],
        ],
    },
    "truncated_icosahedron": {
        "url": "https://en.wikipedia.org/wiki/Truncated_icosahedron",
        "verts": [
            [-0.979432, 0.000000, 0.201774],
            [0.000000, -0.201774, -0.979432],
            [0.979432, 0.000000, -0.201774],
            [0.201774, 0.979432, 0.000000],
            [0.979432, 0.000000, 0.201774],
            [-0.979432, 0.000000, -0.201774],
            [0.000000, -0.201774, 0.979432],
            [0.000000, 0.201774, 0.979432],
            [0.201774, -0.979432, 0.000000],
            [-0.201774, -0.979432, 0.000000],
            [-0.201774, 0.979432, 0.000000],
            [0.000000, 0.201774, -0.979432],
            [-0.326477, 0.403548, -0.854729],
            [-0.403548, 0.854729, 0.326477],
            [0.854729, -0.326477, -0.403548],
            [0.403548, -0.854729, -0.326477],
            [0.326477, -0.403548, 0.854729],
            [-0.403548, 0.854729, -0.326477],
            [0.403548, -0.854729, 0.326477],
            [0.326477, -0.403548, -0.854729],
            [-0.403548, -0.854729, -0.326477],
            [0.326477, 0.403548, 0.854729],
            [-0.403548, -0.854729, 0.326477],
            [0.854729, 0.326477, 0.403548],
            [-0.854729, -0.326477, 0.403548],
            [-0.326477, -0.403548, 0.854729],
            [0.326477, 0.403548, -0.854729],
            [-0.854729, 0.326477, 0.403548],
            [0.403548, 0.854729, -0.326477],
            [0.854729, -0.326477, 0.403548],
            [-0.326477, -0.403548, -0.854729],
            [0.403548, 0.854729, 0.326477],
            [-0.326477, 0.403548, 0.854729],
            [0.854729, 0.326477, -0.403548],
            [-0.854729, 0.326477, -0.403548],
            [-0.854729, -0.326477, -0.403548],
            [0.201774, 0.730026, 0.652955],
            [-0.201774, -0.730026, 0.652955],
            [-0.730026, 0.652955, 0.201774],
            [-0.730026, 0.652955, -0.201774],
            [0.652955, -0.201774, -0.730026],
            [0.201774, -0.730026, -0.652955],
            [-0.201774, 0.730026, 0.652955],
            [0.730026, -0.652955, -0.201774],
            [0.730026, -0.652955, 0.201774],
            [-0.652955, 0.201774, 0.730026],
            [0.201774, 0.730026, -0.652955],
            [-0.201774, -0.730026, -0.652955],
            [-0.652955, -0.201774, 0.730026],
            [0.730026, 0.652955, -0.201774],
            [0.730026, 0.652955, 0.201774],
            [-0.652955, 0.201774, -0.730026],
            [0.652955, 0.201774, 0.730026],
            [-0.201774, 0.730026, -0.652955],
            [-0.652955, -0.201774, -0.730026],
            [0.201774, -0.730026, 0.652955],
            [-0.730026, -0.652955, 0.201774],
            [-0.730026, -0.652955, -0.201774],
            [0.652955, 0.201774, -0.730026],
            [0.652955, -0.201774, 0.730026],
        ],
        "edges": [
            [0, 5],
            [0, 24],
            [0, 27],
            [1, 11],
            [1, 19],
            [1, 30],
            [2, 4],
            [2, 14],
            [2, 33],
            [3, 10],
            [3, 28],
            [3, 31],
            [4, 23],
            [4, 29],
            [5, 34],
            [5, 35],
            [6, 7],
            [6, 16],
            [6, 25],
            [7, 21],
            [7, 32],
            [8, 9],
            [8, 15],
            [8, 18],
            [9, 20],
            [9, 22],
            [10, 13],
            [10, 17],
            [11, 12],
            [11, 26],
            [12, 51],
            [12, 53],
            [13, 38],
            [13, 42],
            [14, 40],
            [14, 43],
            [15, 41],
            [15, 43],
            [16, 55],
            [16, 59],
            [17, 39],
            [17, 53],
            [18, 44],
            [18, 55],
            [19, 40],
            [19, 41],
            [20, 47],
            [20, 57],
            [21, 36],
            [21, 52],
            [22, 37],
            [22, 56],
            [23, 50],
            [23, 52],
            [24, 48],
            [24, 56],
            [25, 37],
            [25, 48],
            [26, 46],
            [26, 58],
            [27, 38],
            [27, 45],
            [28, 46],
            [28, 49],
            [29, 44],
            [29, 59],
            [30, 47],
            [30, 54],
            [31, 36],
            [31, 50],
            [32, 42],
            [32, 45],
            [33, 49],
            [33, 58],
            [34, 39],
            [34, 51],
            [35, 54],
            [35, 57],
            [36, 42],
            [37, 55],
            [38, 39],
            [40, 58],
            [41, 47],
            [43, 44],
            [45, 48],
            [46, 53],
            [49, 50],
            [51, 54],
            [52, 59],
            [56, 57],
        ],
    },
    "rhombicosidodecahedron": {
        "url": "https://en.wikipedia.org/wiki/Rhombicosidodecahedron",
        "verts": [
            [0.948536, 0.223919, 0.223919],
            [0.223919, 0.948536, -0.223919],
            [0.948536, 0.223919, -0.223919],
            [-0.948536, 0.223919, -0.223919],
            [0.223919, 0.948536, 0.223919],
            [-0.948536, 0.223919, 0.223919],
            [-0.223919, -0.223919, 0.948536],
            [-0.223919, 0.948536, -0.223919],
            [-0.223919, 0.948536, 0.223919],
            [-0.223919, -0.223919, -0.948536],
            [0.948536, -0.223919, 0.223919],
            [0.948536, -0.223919, -0.223919],
            [-0.948536, -0.223919, 0.223919],
            [-0.948536, -0.223919, -0.223919],
            [0.223919, -0.948536, -0.223919],
            [0.223919, -0.948536, 0.223919],
            [-0.223919, -0.948536, -0.223919],
            [0.223919, 0.223919, 0.948536],
            [-0.223919, -0.948536, 0.223919],
            [-0.223919, 0.223919, 0.948536],
            [0.223919, -0.223919, 0.948536],
            [0.223919, 0.223919, -0.948536],
            [-0.223919, 0.223919, -0.948536],
            [0.223919, -0.223919, -0.948536],
            [-0.586227, -0.362309, 0.724617],
            [0.362309, 0.724617, -0.586227],
            [0.362309, -0.724617, 0.586227],
            [-0.586227, 0.362309, 0.724617],
            [0.586227, 0.362309, 0.724617],
            [-0.362309, -0.724617, -0.586227],
            [-0.724617, -0.586227, -0.362309],
            [-0.724617, 0.586227, -0.362309],
            [-0.724617, -0.586227, 0.362309],
            [-0.724617, 0.586227, 0.362309],
            [0.586227, -0.362309, -0.724617],
            [-0.362309, 0.724617, -0.586227],
            [0.362309, 0.724617, 0.586227],
            [-0.586227, -0.362309, -0.724617],
            [-0.586227, 0.362309, -0.724617],
            [-0.362309, -0.724617, 0.586227],
            [0.586227, 0.362309, -0.724617],
            [0.362309, -0.724617, -0.586227],
            [0.724617, -0.586227, -0.362309],
            [0.724617, 0.586227, -0.362309],
            [0.724617, -0.586227, 0.362309],
            [0.724617, 0.586227, 0.362309],
            [-0.362309, 0.724617, 0.586227],
            [0.586227, -0.362309, 0.724617],
            [0.000000, -0.586227, 0.810146],
            [-0.810146, 0.000000, 0.586227],
            [0.586227, 0.810146, 0.000000],
            [0.810146, 0.000000, -0.586227],
            [-0.586227, -0.810146, 0.000000],
            [0.000000, 0.586227, -0.810146],
            [0.586227, -0.810146, 0.000000],
            [-0.810146, 0.000000, -0.586227],
            [0.000000, 0.586227, 0.810146],
            [0.000000, -0.586227, -0.810146],
            [-0.586227, 0.810146, 0.000000],
            [0.810146, 0.000000, 0.586227],
        ],
        "edges": [
            [0, 2],
            [0, 10],
            [0, 45],
            [0, 59],
            [1, 4],
            [1, 7],
            [1, 25],
            [1, 50],
            [2, 11],
            [2, 43],
            [2, 51],
            [3, 5],
            [3, 13],
            [3, 31],
            [3, 55],
            [4, 8],
            [4, 36],
            [4, 50],
            [5, 12],
            [5, 33],
            [5, 49],
            [6, 19],
            [6, 20],
            [6, 24],
            [6, 48],
            [7, 8],
            [7, 35],
            [7, 58],
            [8, 46],
            [8, 58],
            [9, 22],
            [9, 23],
            [9, 37],
            [9, 57],
            [10, 11],
            [10, 44],
            [10, 59],
            [11, 42],
            [11, 51],
            [12, 13],
            [12, 32],
            [12, 49],
            [13, 30],
            [13, 55],
            [14, 15],
            [14, 16],
            [14, 41],
            [14, 54],
            [15, 18],
            [15, 26],
            [15, 54],
            [16, 18],
            [16, 29],
            [16, 52],
            [17, 19],
            [17, 20],
            [17, 28],
            [17, 56],
            [18, 39],
            [18, 52],
            [19, 27],
            [19, 56],
            [20, 47],
            [20, 48],
            [21, 22],
            [21, 23],
            [21, 40],
            [21, 53],
            [22, 38],
            [22, 53],
            [23, 34],
            [23, 57],
            [24, 32],
            [24, 39],
            [24, 49],
            [25, 40],
            [25, 43],
            [25, 53],
            [26, 44],
            [26, 47],
            [26, 48],
            [27, 33],
            [27, 46],
            [27, 49],
            [28, 36],
            [28, 45],
            [28, 59],
            [29, 30],
            [29, 37],
            [29, 57],
            [30, 37],
            [30, 52],
            [31, 35],
            [31, 38],
            [31, 58],
            [32, 39],
            [32, 52],
            [33, 46],
            [33, 58],
            [34, 41],
            [34, 42],
            [34, 51],
            [35, 38],
            [35, 53],
            [36, 45],
            [36, 56],
            [37, 55],
            [38, 55],
            [39, 48],
            [40, 43],
            [40, 51],
            [41, 42],
            [41, 57],
            [42, 54],
            [43, 50],
            [44, 47],
            [44, 54],
            [45, 50],
            [46, 56],
            [47, 59],
        ],
    },
    "truncated_icosidodecahedron": {
        "url": "https://en.wikipedia.org/wiki/Truncated_icosidodecahedron",
        "verts": [
            [-0.131496, 0.982557, -0.131496],
            [0.131496, 0.131496, -0.982557],
            [0.131496, -0.982557, 0.131496],
            [-0.982557, 0.131496, -0.131496],
            [0.131496, 0.131496, 0.982557],
            [0.982557, -0.131496, -0.131496],
            [-0.131496, 0.131496, -0.982557],
            [0.131496, 0.982557, 0.131496],
            [-0.982557, -0.131496, 0.131496],
            [-0.131496, -0.982557, 0.131496],
            [0.982557, 0.131496, 0.131496],
            [-0.131496, 0.131496, 0.982557],
            [0.131496, -0.982557, -0.131496],
            [-0.131496, 0.982557, 0.131496],
            [-0.982557, 0.131496, 0.131496],
            [0.131496, -0.131496, -0.982557],
            [-0.982557, -0.131496, -0.131496],
            [-0.131496, -0.131496, -0.982557],
            [0.131496, 0.982557, -0.131496],
            [0.982557, 0.131496, -0.131496],
            [-0.131496, -0.982557, -0.131496],
            [0.131496, -0.131496, 0.982557],
            [0.982557, -0.131496, 0.131496],
            [-0.131496, -0.131496, 0.982557],
            [-0.344261, 0.901288, -0.262992],
            [0.344261, -0.901288, -0.262992],
            [0.901288, 0.262992, 0.344261],
            [-0.344261, -0.901288, 0.262992],
            [-0.901288, 0.262992, 0.344261],
            [0.344261, 0.901288, -0.262992],
            [0.901288, -0.262992, -0.344261],
            [-0.344261, 0.901288, 0.262992],
            [0.344261, -0.901288, 0.262992],
            [-0.901288, -0.262992, -0.344261],
            [-0.344261, -0.901288, -0.262992],
            [-0.262992, 0.344261, 0.901288],
            [-0.262992, -0.344261, 0.901288],
            [0.901288, -0.262992, 0.344261],
            [-0.901288, -0.262992, 0.344261],
            [0.262992, 0.344261, 0.901288],
            [0.262992, -0.344261, 0.901288],
            [-0.262992, 0.344261, -0.901288],
            [-0.262992, -0.344261, -0.901288],
            [0.262992, 0.344261, -0.901288],
            [0.262992, -0.344261, -0.901288],
            [0.344261, 0.901288, 0.262992],
            [-0.901288, 0.262992, -0.344261],
            [0.901288, 0.262992, -0.344261],
            [0.131496, 0.557026, 0.820019],
            [-0.557026, -0.820019, -0.131496],
            [-0.557026, 0.820019, -0.131496],
            [-0.131496, -0.557026, 0.820019],
            [0.131496, -0.557026, 0.820019],
            [0.820019, -0.131496, 0.557026],
            [-0.131496, 0.557026, 0.820019],
            [0.820019, 0.131496, -0.557026],
            [-0.820019, -0.131496, -0.557026],
            [0.557026, -0.820019, 0.131496],
            [-0.820019, 0.131496, 0.557026],
            [0.557026, 0.820019, 0.131496],
            [-0.557026, -0.820019, 0.131496],
            [-0.557026, 0.820019, 0.131496],
            [-0.820019, -0.131496, 0.557026],
            [0.820019, 0.131496, 0.557026],
            [0.131496, 0.557026, -0.820019],
            [0.557026, -0.820019, -0.131496],
            [0.820019, -0.131496, -0.557026],
            [-0.131496, -0.557026, -0.820019],
            [0.131496, -0.557026, -0.820019],
            [-0.820019, 0.131496, -0.557026],
            [0.557026, 0.820019, -0.131496],
            [-0.131496, 0.557026, -0.820019],
            [0.425530, -0.769792, -0.475757],
            [-0.769792, -0.475757, 0.425530],
            [-0.475757, 0.425530, -0.769792],
            [0.425530, -0.769792, 0.475757],
            [-0.425530, -0.769792, -0.475757],
            [0.425530, 0.769792, -0.475757],
            [0.475757, -0.425530, 0.769792],
            [-0.769792, 0.475757, 0.425530],
            [-0.425530, 0.769792, -0.475757],
            [-0.425530, -0.769792, 0.475757],
            [0.769792, -0.475757, 0.425530],
            [-0.769792, -0.475757, -0.425530],
            [0.425530, 0.769792, 0.475757],
            [0.475757, -0.425530, -0.769792],
            [0.475757, 0.425530, 0.769792],
            [-0.425530, 0.769792, 0.475757],
            [0.769792, 0.475757, 0.425530],
            [-0.769792, 0.475757, -0.425530],
            [0.475757, 0.425530, -0.769792],
            [0.769792, -0.475757, -0.425530],
            [-0.475757, -0.425530, 0.769792],
            [-0.475757, 0.425530, 0.769792],
            [0.769792, 0.475757, -0.425530],
            [-0.475757, -0.425530, -0.769792],
            [-0.344261, -0.638295, -0.688522],
            [0.344261, -0.638295, 0.688522],
            [-0.638295, 0.688522, 0.344261],
            [-0.344261, 0.638295, -0.688522],
            [-0.688522, 0.344261, -0.638295],
            [-0.688522, 0.344261, 0.638295],
            [-0.688522, -0.344261, -0.638295],
            [-0.688522, -0.344261, 0.638295],
            [-0.638295, -0.688522, -0.344261],
            [0.344261, 0.638295, 0.688522],
            [-0.638295, -0.688522, 0.344261],
            [0.638295, -0.688522, -0.344261],
            [0.344261, -0.638295, -0.688522],
            [0.688522, -0.344261, -0.638295],
            [-0.344261, -0.638295, 0.688522],
            [0.638295, -0.688522, 0.344261],
            [0.688522, 0.344261, -0.638295],
            [0.688522, 0.344261, 0.638295],
            [0.688522, -0.344261, 0.638295],
            [-0.344261, 0.638295, 0.688522],
            [-0.638295, 0.688522, -0.344261],
            [0.638295, 0.688522, -0.344261],
            [0.344261, 0.638295, -0.688522],
            [0.638295, 0.688522, 0.344261],
        ],
        "edges": [
            [0, 13],
            [0, 18],
            [1, 6],
            [1, 15],
            [2, 9],
            [2, 12],
            [3, 14],
            [3, 16],
            [4, 11],
            [4, 21],
            [5, 19],
            [5, 22],
            [6, 17],
            [7, 13],
            [7, 18],
            [8, 14],
            [8, 16],
            [9, 20],
            [10, 19],
            [10, 22],
            [11, 23],
            [12, 20],
            [15, 17],
            [21, 23],
            [48, 54],
            [49, 60],
            [50, 61],
            [51, 52],
            [53, 63],
            [55, 66],
            [56, 69],
            [57, 65],
            [58, 62],
            [59, 70],
            [64, 71],
            [67, 68],
            [73, 103],
            [74, 99],
            [78, 97],
            [79, 101],
            [82, 114],
            [83, 102],
            [85, 108],
            [86, 105],
            [88, 113],
            [89, 100],
            [90, 118],
            [91, 109],
            [92, 110],
            [93, 115],
            [94, 112],
            [95, 96],
            [24, 50],
            [25, 65],
            [26, 63],
            [27, 60],
            [28, 58],
            [29, 70],
            [30, 66],
            [31, 61],
            [32, 57],
            [33, 56],
            [34, 49],
            [35, 54],
            [36, 51],
            [37, 53],
            [38, 62],
            [39, 48],
            [40, 52],
            [41, 71],
            [42, 67],
            [43, 64],
            [44, 68],
            [45, 59],
            [46, 69],
            [47, 55],
            [72, 107],
            [75, 111],
            [76, 104],
            [77, 117],
            [80, 116],
            [81, 106],
            [84, 119],
            [87, 98],
            [0, 24],
            [1, 43],
            [2, 32],
            [3, 46],
            [4, 39],
            [5, 30],
            [6, 41],
            [7, 45],
            [8, 38],
            [9, 27],
            [10, 26],
            [11, 35],
            [12, 25],
            [13, 31],
            [14, 28],
            [15, 44],
            [16, 33],
            [17, 42],
            [18, 29],
            [19, 47],
            [20, 34],
            [21, 40],
            [22, 37],
            [23, 36],
            [26, 88],
            [28, 79],
            [30, 91],
            [33, 83],
            [35, 93],
            [36, 92],
            [37, 82],
            [38, 73],
            [39, 86],
            [40, 78],
            [41, 74],
            [42, 95],
            [43, 90],
            [44, 85],
            [46, 89],
            [47, 94],
            [48, 105],
            [49, 104],
            [50, 116],
            [51, 110],
            [52, 97],
            [53, 114],
            [54, 115],
            [55, 112],
            [56, 102],
            [57, 111],
            [58, 101],
            [59, 119],
            [60, 106],
            [61, 98],
            [62, 103],
            [63, 113],
            [64, 118],
            [65, 107],
            [66, 109],
            [67, 96],
            [68, 108],
            [69, 100],
            [70, 117],
            [71, 99],
            [72, 108],
            [73, 106],
            [74, 100],
            [75, 97],
            [76, 96],
            [77, 118],
            [78, 114],
            [79, 98],
            [80, 99],
            [81, 110],
            [82, 111],
            [83, 104],
            [84, 105],
            [85, 109],
            [86, 113],
            [87, 115],
            [88, 119],
            [89, 116],
            [90, 112],
            [91, 107],
            [92, 103],
            [93, 101],
            [94, 117],
            [95, 102],
            [24, 80],
            [25, 72],
            [27, 81],
            [29, 77],
            [31, 87],
            [32, 75],
            [34, 76],
            [45, 84],
        ],
    },
    "snub_dodecahedron": {
        "url": "https://en.wikipedia.org/wiki/Snub_dodecahedron",
        "verts": [
            [-0.153509, 0.173813, 0.972740],
            [-0.674425, -0.337878, 0.656498],
            [-0.263309, -0.917429, 0.298316],
            [0.511691, -0.763920, 0.393189],
            [0.579551, -0.089496, 0.810007],
            [0.298316, 0.263309, 0.917429],
            [0.337878, 0.656498, 0.674425],
            [-0.089496, 0.810007, 0.579551],
            [-0.393189, 0.511691, 0.763920],
            [0.763920, -0.393189, 0.511691],
            [0.674425, -0.337878, -0.656498],
            [-0.298316, 0.263309, -0.917429],
            [-0.810007, 0.579551, 0.089496],
            [-0.917429, -0.298316, 0.263309],
            [-0.579551, -0.089496, -0.810007],
            [0.393189, 0.511691, -0.763920],
            [0.656498, 0.674425, 0.337878],
            [0.089496, -0.810007, 0.579551],
            [0.656498, -0.674425, -0.337878],
            [0.763920, 0.393189, -0.511691],
            [0.263309, 0.917429, 0.298316],
            [-0.511691, 0.763920, 0.393189],
            [-0.917429, 0.298316, -0.263309],
            [-0.810007, -0.579551, -0.089496],
            [-0.337878, -0.656498, 0.674425],
            [0.393189, -0.511691, 0.763920],
            [0.674425, 0.337878, 0.656498],
            [-0.579551, 0.089496, 0.810007],
            [-0.298316, -0.263309, 0.917429],
            [-0.763920, -0.393189, -0.511691],
            [0.917429, -0.298316, -0.263309],
            [-0.173813, -0.972740, -0.153509],
            [0.972740, 0.153509, -0.173813],
            [-0.972740, -0.153509, -0.173813],
            [0.173813, -0.972740, 0.153509],
            [0.810007, 0.579551, -0.089496],
            [-0.656498, 0.674425, -0.337878],
            [0.972740, -0.153509, 0.173813],
            [0.173813, 0.972740, -0.153509],
            [-0.173813, 0.972740, 0.153509],
            [-0.972740, 0.153509, 0.173813],
            [0.263309, -0.917429, -0.298316],
            [-0.337878, 0.656498, -0.674425],
            [-0.511691, -0.763920, -0.393189],
            [0.089496, 0.810007, -0.579551],
            [0.153509, -0.173813, 0.972740],
            [0.337878, -0.656498, -0.674425],
            [-0.089496, -0.810007, -0.579551],
            [0.810007, -0.579551, 0.089496],
            [-0.656498, -0.674425, 0.337878],
            [0.511691, 0.763920, -0.393189],
            [-0.263309, 0.917429, -0.298316],
            [0.917429, 0.298316, 0.263309],
            [-0.763920, 0.393189, 0.511691],
            [0.153509, 0.173813, -0.972740],
            [0.579551, 0.089496, -0.810007],
            [-0.393189, -0.511691, -0.763920],
            [0.298316, -0.263309, -0.917429],
            [-0.674425, 0.337878, -0.656498],
            [-0.153509, -0.173813, -0.972740],
        ],
        "edges": [
            [12, 40],
            [3, 25],
            [12, 22],
            [2, 34],
            [22, 40],
            [35, 52],
            [36, 58],
            [2, 17],
            [18, 46],
            [20, 39],
            [31, 47],
            [13, 33],
            [23, 33],
            [36, 42],
            [54, 57],
            [0, 27],
            [1, 24],
            [1, 49],
            [4, 45],
            [5, 45],
            [8, 21],
            [9, 25],
            [10, 18],
            [11, 59],
            [15, 19],
            [21, 53],
            [24, 49],
            [37, 48],
            [42, 58],
            [43, 56],
            [0, 28],
            [7, 39],
            [11, 14],
            [27, 28],
            [41, 47],
            [54, 55],
            [3, 9],
            [4, 5],
            [6, 26],
            [13, 23],
            [14, 59],
            [8, 53],
            [16, 26],
            [17, 34],
            [19, 50],
            [29, 43],
            [29, 56],
            [30, 48],
            [38, 51],
            [10, 46],
            [31, 41],
            [32, 52],
            [55, 57],
            [7, 20],
            [32, 35],
            [6, 16],
            [15, 50],
            [38, 44],
            [44, 51],
            [30, 37],
            [31, 34],
            [2, 24],
            [1, 28],
            [18, 30],
            [23, 29],
            [3, 17],
            [41, 46],
            [43, 47],
            [13, 49],
            [16, 52],
            [8, 27],
            [0, 45],
            [12, 53],
            [15, 55],
            [32, 37],
            [4, 25],
            [7, 21],
            [9, 48],
            [33, 40],
            [19, 35],
            [54, 59],
            [5, 26],
            [10, 57],
            [11, 58],
            [22, 36],
            [14, 56],
            [38, 39],
            [6, 20],
            [44, 50],
            [42, 51],
            [10, 30],
            [38, 50],
            [11, 42],
            [15, 44],
            [20, 38],
            [5, 6],
            [19, 55],
            [26, 52],
            [34, 41],
            [9, 37],
            [15, 54],
            [16, 35],
            [22, 58],
            [36, 51],
            [39, 51],
            [40, 53],
            [4, 9],
            [7, 8],
            [17, 25],
            [21, 39],
            [30, 32],
            [31, 43],
            [42, 44],
            [57, 59],
            [0, 8],
            [17, 24],
            [18, 41],
            [18, 48],
            [23, 43],
            [29, 33],
            [0, 5],
            [1, 27],
            [4, 26],
            [6, 7],
            [10, 55],
            [14, 58],
            [16, 20],
            [23, 49],
            [35, 50],
            [37, 52],
            [47, 56],
            [56, 59],
            [11, 54],
            [14, 29],
            [22, 33],
            [24, 28],
            [27, 53],
            [46, 47],
            [1, 13],
            [2, 31],
            [25, 45],
            [2, 49],
            [3, 48],
            [19, 32],
            [28, 45],
            [12, 21],
            [12, 36],
            [46, 57],
            [13, 40],
            [3, 34],
        ],
    },
    # Others
    "sphere": {
        "url": "https://en.wikipedia.org/wiki/Sphere",
        "verts": [
            [0.0, 0.0, 1.0],
            [0.0, 0.0, -1.0],
            [0.224144, 0.12941, 0.965926],
            [0.25, 0.433013, 0.866025],
            [0.0, 0.707107, 0.707107],
            [-0.433013, 0.75, 0.5],
            [-0.836516, 0.482963, 0.258819],
            [-1.0, 0.0, 0.0],
            [-0.836516, -0.482963, -0.258819],
            [-0.433013, -0.75, -0.5],
            [-0.0, -0.707107, -0.707107],
            [0.25, -0.433013, -0.866025],
            [0.224144, -0.12941, -0.965926],
            [-0.053812, 0.253163, 0.965926],
            [-0.334565, 0.371572, 0.866025],
            [-0.672499, 0.218508, 0.707107],
            [-0.847101, -0.180057, 0.5],
            [-0.717823, -0.646331, 0.258819],
            [-0.309017, -0.951057, 0.0],
            [0.200827, -0.944818, -0.258819],
            [0.579484, -0.643582, -0.5],
            [0.672499, -0.218508, -0.707107],
            [0.489074, 0.103956, -0.866025],
            [0.19234, 0.173184, -0.965926],
            [-0.257401, 0.027054, 0.965926],
            [-0.456773, -0.203368, 0.866025],
            [-0.415627, -0.572061, 0.707107],
            [-0.090524, -0.861281, 0.5],
            [0.392877, -0.882417, 0.258819],
            [0.809017, -0.587785, 0.0],
            [0.960634, -0.100967, -0.258819],
            [0.791154, 0.352244, -0.5],
            [0.415627, 0.572061, -0.707107],
            [0.052264, 0.497261, -0.866025],
            [-0.105271, 0.236443, -0.965926],
            [-0.105271, -0.236443, 0.965926],
            [0.052264, -0.497261, 0.866025],
            [0.415627, -0.572061, 0.707107],
            [0.791154, -0.352244, 0.5],
            [0.960634, 0.100967, 0.258819],
            [0.809017, 0.587785, 0.0],
            [0.392877, 0.882417, -0.258819],
            [-0.090524, 0.861281, -0.5],
            [-0.415627, 0.572061, -0.707107],
            [-0.456773, 0.203368, -0.866025],
            [-0.257401, -0.027054, -0.965926],
            [0.19234, -0.173184, 0.965926],
            [0.489074, -0.103956, 0.866025],
            [0.672499, 0.218508, 0.707107],
            [0.579484, 0.643582, 0.5],
            [0.200827, 0.944818, 0.258819],
            [-0.309017, 0.951057, 0.0],
            [-0.717823, 0.646331, -0.258819],
            [-0.847101, 0.180057, -0.5],
            [-0.672499, -0.218508, -0.707107],
            [-0.334565, -0.371572, -0.866025],
            [-0.053812, -0.253163, -0.965926],
        ],
        "edges": [
            [0, 2],
            [2, 3],
            [3, 4],
            [4, 5],
            [5, 6],
            [6, 7],
            [7, 8],
            [8, 9],
            [9, 10],
            [10, 11],
            [11, 12],
            [12, 1],
            [0, 13],
            [13, 14],
            [14, 15],
            [15, 16],
            [16, 17],
            [17, 18],
            [18, 19],
            [19, 20],
            [20, 21],
            [21, 22],
            [22, 23],
            [23, 1],
            [0, 24],
            [24, 25],
            [25, 26],
            [26, 27],
            [27, 28],
            [28, 29],
            [29, 30],
            [30, 31],
            [31, 32],
            [32, 33],
            [33, 34],
            [34, 1],
            [0, 35],
            [35, 36],
            [36, 37],
            [37, 38],
            [38, 39],
            [39, 40],
            [40, 41],
            [41, 42],
            [42, 43],
            [43, 44],
            [44, 45],
            [45, 1],
            [0, 46],
            [46, 47],
            [47, 48],
            [48, 49],
            [49, 50],
            [50, 51],
            [51, 52],
            [52, 53],
            [53, 54],
            [54, 55],
            [55, 56],
            [56, 1],
        ],
    },
}

SOLID_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(SolidAnimationElement),
        cv.Optional(CONF_SHAPE, default="icosahedron"): cv.one_of(
            *SHAPE_DATA.keys(), lower=True
        ),
    }
)


async def solid_animation_to_code(config, component, parent):
    shape = SHAPE_DATA[config[CONF_SHAPE]]
    num_verts = len(shape["verts"])
    num_edges = len(shape["edges"])
    config[CONF_ID].type = SolidAnimationElement.template(num_verts, num_edges)

    var = await animation_element_to_code(config, component, parent)
    cg.add(
        var.set_points(
            cg.RawExpression(
                f"std::array<esphome::elements::Vec3, {num_verts}>{{ {{"
                + ", ".join([f"{{{v[0]}f, {v[1]}f, {v[2]}f}}" for v in shape["verts"]])
                + "} }"
            )
        )
    )
    cg.add(
        var.set_edges(
            cg.RawExpression(
                f"std::array<esphome::elements::Edge, {num_edges}>{{ {{"
                + ", ".join([f"{{{e[0]}, {e[1]}}}" for e in shape["edges"]])
                + "} }"
            )
        )
    )
    return var


element_registry.register_element(
    "solid_animation",
    SOLID_ANIMATION_ELEMENT_SCHEMA,
    solid_animation_to_code,
)


# Parallax Animation

ParallaxAnimationElement = shared.elements_ns.class_(
    "ParallaxAnimationElement", AnimationElement
)

PARALLAX_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(ParallaxAnimationElement),
        cv.Optional(CONF_LAYERS, default=3): cv.int_range(min=1, max=10),
    }
)


async def parallax_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    cg.add(var.set_num_layers(config[CONF_LAYERS]))
    return var


element_registry.register_element(
    "parallax_animation",
    PARALLAX_ANIMATION_ELEMENT_SCHEMA,
    parallax_animation_to_code,
)

# Lorenz Animation

LorenzAnimationElement = shared.elements_ns.class_(
    "LorenzAnimationElement", AnimationElement
)

LORENZ_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(LorenzAnimationElement),
        cv.Optional(CONF_LENGTH, default=300): cv.int_range(min=10, max=1000),
        cv.Optional(CONF_SIGMA, default=10.0): cv.float_range(min=0.1),
        cv.Optional(CONF_RHO, default=28.0): cv.float_range(min=0.1),
        cv.Optional(CONF_BETA, default=8.0 / 3.0): cv.float_range(min=0.1),
    }
)


async def lorenz_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    if conf := config.get(CONF_LENGTH):
        cg.add(var.set_length(conf))
    if conf := config.get(CONF_SIGMA):
        cg.add(var.set_sigma(conf))
    if conf := config.get(CONF_RHO):
        cg.add(var.set_rho(conf))
    if conf := config.get(CONF_BETA):
        cg.add(var.set_beta(conf))
    return var


element_registry.register_element(
    "lorenz_animation",
    LORENZ_ANIMATION_ELEMENT_SCHEMA,
    lorenz_animation_to_code,
)
