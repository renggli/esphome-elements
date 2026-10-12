import esphome.codegen as cg
import esphome.core as core
import esphome.config_validation as cv
from esphome.const import CONF_ID

from . import shared
from . import color
from . import element
from . import element_registry
from . import animation_data

CONF_BETA = "beta"
CONF_COLOR_SCHEME = "color_scheme"
CONF_COOLING = "cooling"
CONF_COUNT = "count"
CONF_DENSITY = "density"
CONF_DISPLAY = "display"
CONF_DRAW = "draw"
CONF_ELEMENT = "element"
CONF_FADE_STEPS = "fade_steps"
CONF_IS_ACTIVE = "is_active"
CONF_LENGTH = "length"
CONF_MODE = "mode"
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

SHAPE_DATA = animation_data.SHAPE_DATA

SolidMode = shared.elements_ns.enum("SolidMode", is_class=True)

# 3D solid animation rendering modes:
# - wire: Wireframe rendering showing all edges without occlusion (transparent/see-through).
# - hidden: Wireframe rendering with occluded/back edges hidden behind front faces.
# - solid: Solid filled faces with directional lighting mapped to the color palette.
SOLID_MODE = {
    "WIRE": SolidMode.WIRE,
    "HIDDEN": SolidMode.HIDDEN,
    "SOLID": SolidMode.SOLID,
}

SOLID_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(SolidAnimationElement),
        cv.Optional(CONF_SHAPE, default="icosahedron"): cv.one_of(
            *SHAPE_DATA.keys(), lower=True
        ),
        cv.Optional(CONF_MODE, default="wire"): cv.enum(
            SOLID_MODE, upper=True, space="_"
        ),
    }
)

Vec3Struct = shared.elements_ns.struct("Vec3")
EdgeStruct = shared.elements_ns.struct("Edge")
FaceStruct = shared.elements_ns.struct("Face")


async def solid_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    shape = SHAPE_DATA[config[CONF_SHAPE]]

    verts = [[float(value) for value in vert] for vert in shape["verts"]]
    verts_id = core.ID(
        f"{config[CONF_ID]}_verts",
        is_declaration=True,
        type=Vec3Struct,
    )
    verts_array = cg.progmem_array(verts_id, verts)
    cg.add(var.set_points(verts_array, len(verts)))

    edges = [[int(value) for value in edge] for edge in shape["edges"]]
    edges_id = core.ID(
        f"{config[CONF_ID]}_edges",
        is_declaration=True,
        type=EdgeStruct,
    )
    edges_array = cg.progmem_array(edges_id, edges)
    cg.add(var.set_edges(edges_array, len(edges)))

    faces = [[int(value) for value in face] for face in shape["faces"]]
    faces_id = core.ID(
        f"{config[CONF_ID]}_faces",
        is_declaration=True,
        type=FaceStruct,
    )
    faces_array = cg.progmem_array(faces_id, faces)

    normals = [[float(value) for value in normal] for normal in shape["normals"]]
    normals_id = core.ID(
        f"{config[CONF_ID]}_normals",
        is_declaration=True,
        type=Vec3Struct,
    )
    normals_array = cg.progmem_array(normals_id, normals)

    cg.add(var.set_faces(faces_array, normals_array, len(shape["faces"])))
    cg.add(var.set_mode(config[CONF_MODE]))

    return var


element_registry.register_element(
    "solid_animation",
    SOLID_ANIMATION_ELEMENT_SCHEMA,
    solid_animation_to_code,
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


# Custom Animation Element

CustomAnimationElement = shared.elements_ns.class_(
    "CustomAnimationElement", AnimationElement
)

CUSTOM_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(CustomAnimationElement),
        cv.Optional(CONF_DRAW): cv.lambda_,
        cv.Optional(CONF_IS_ACTIVE): cv.returning_lambda,
    }
)


async def custom_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    if conf := config.get(CONF_DRAW):
        value = await cg.process_lambda(
            conf,
            [
                (shared.ElementRef, CONF_ELEMENT),
                (shared.DisplayRef, CONF_DISPLAY),
                (cg.int_, "width"),
                (cg.int_, "height"),
                (cg.float_, "time"),
            ],
            return_type=cg.void,
        )
        cg.add(var.set_draw(value))
    if conf := config.get(CONF_IS_ACTIVE):
        value = await cg.process_lambda(
            conf,
            [(shared.ElementConstRef, CONF_ELEMENT)],
            return_type=cg.bool_,
        )
        cg.add(var.set_is_active(value))
    return var


element_registry.register_element(
    "custom",
    CUSTOM_ANIMATION_ELEMENT_SCHEMA,
    custom_animation_to_code,
)

element_registry.register_element(
    "custom_animation",
    CUSTOM_ANIMATION_ELEMENT_SCHEMA,
    custom_animation_to_code,
)

# Caustics Animation (Caustics refraction)
CausticsAnimationElement = shared.elements_ns.class_(
    "CausticsAnimationElement", AnimationElement
)

CAUSTICS_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(CausticsAnimationElement),
    }
)


async def caustics_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    return var


element_registry.register_element(
    "caustics_animation",
    CAUSTICS_ANIMATION_ELEMENT_SCHEMA,
    caustics_animation_to_code,
)


# Clockwork Animation (Radial gear / clockwork rotation)
ClockworkAnimationElement = shared.elements_ns.class_(
    "ClockworkAnimationElement", AnimationElement
)

CLOCKWORK_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(ClockworkAnimationElement),
        cv.Optional(CONF_COUNT, default=3): cv.int_range(min=1),
    }
)


async def clockwork_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    if conf := config.get(CONF_COUNT):
        cg.add(var.set_count(conf))
    return var


element_registry.register_element(
    "clockwork_animation",
    CLOCKWORK_ANIMATION_ELEMENT_SCHEMA,
    clockwork_animation_to_code,
)

# Frizzle Animation (Electric spark discharge)
FrizzleAnimationElement = shared.elements_ns.class_(
    "FrizzleAnimationElement", AnimationElement
)

FRIZZLE_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(FrizzleAnimationElement),
        cv.Optional(CONF_COUNT, default=6): cv.int_range(min=1),
    }
)


async def frizzle_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    if conf := config.get(CONF_COUNT):
        cg.add(var.set_count(conf))
    return var


element_registry.register_element(
    "frizzle_animation",
    FRIZZLE_ANIMATION_ELEMENT_SCHEMA,
    frizzle_animation_to_code,
)

# Spectral Animation (Spectral flame / phantom trail)
SpectralAnimationElement = shared.elements_ns.class_(
    "SpectralAnimationElement", AnimationElement
)

SPECTRAL_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(SpectralAnimationElement),
        cv.Optional(CONF_LENGTH, default=12.0): cv.float_range(min=1.0),
    }
)


async def spectral_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    if conf := config.get(CONF_LENGTH):
        cg.add(var.set_length(conf))
    return var


element_registry.register_element(
    "spectral_animation",
    SPECTRAL_ANIMATION_ELEMENT_SCHEMA,
    spectral_animation_to_code,
)

# Splatter Animation (Splatter / droplet impact dispersion)
SplatterAnimationElement = shared.elements_ns.class_(
    "SplatterAnimationElement", AnimationElement
)

SPLATTER_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(SplatterAnimationElement),
        cv.Optional(CONF_COUNT, default=5): cv.int_range(min=1),
    }
)


async def splatter_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    if conf := config.get(CONF_COUNT):
        cg.add(var.set_count(conf))
    return var


element_registry.register_element(
    "splatter_animation",
    SPLATTER_ANIMATION_ELEMENT_SCHEMA,
    splatter_animation_to_code,
)

# Black Hole Animation (Gravitational black hole simulation)
BlackholeAnimationElement = shared.elements_ns.class_(
    "BlackholeAnimationElement", AnimationElement
)

BLACKHOLE_ANIMATION_ELEMENT_SCHEMA = ANIMATION_ELEMENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(BlackholeAnimationElement),
    }
)


async def blackhole_animation_to_code(config, component, parent):
    var = await animation_element_to_code(config, component, parent)
    return var


element_registry.register_element(
    "blackhole_animation",
    BLACKHOLE_ANIMATION_ELEMENT_SCHEMA,
    blackhole_animation_to_code,
)

