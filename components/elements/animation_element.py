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
CONF_SHAPE = "shape"
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
import esphome.core as core

core.CORE.id_classes.pop(str(SolidAnimationElement), None)

SHAPE_DATA = {
    "tetrahedron": {
        "verts": [
            [0.0, 1.0, 0.0],
            [0.942809, -0.333333, 0.0],
            [-0.471405, -0.333333, 0.816497],
            [-0.471405, -0.333333, -0.816497],
        ],
        "edges": [[0, 1], [0, 2], [0, 3], [1, 2], [1, 3], [2, 3]],
    },
    "cube": {
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
    "sphere": {
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
