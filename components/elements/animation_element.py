import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from .element import Element, elements_ns
from .color import COLOR_SCHEME_SCHEMA, CONF_COLOR_SCHEME, color_scheme_schema

AnimationElement = elements_ns.class_('AnimationElement', Element)

MetaballsElement = elements_ns.class_('MetaballsElement', AnimationElement)
AuroraElement = elements_ns.class_('AuroraElement', AnimationElement)
KaleidoscopeElement = elements_ns.class_('KaleidoscopeElement', AnimationElement)
PlasmaElement = elements_ns.class_('PlasmaElement', AnimationElement)
RipplesElement = elements_ns.class_('RipplesElement', AnimationElement)
SpiralElement = elements_ns.class_('SpiralElement', AnimationElement)
VoronoiElement = elements_ns.class_('VoronoiElement', AnimationElement)
InterferenceElement = elements_ns.class_('InterferenceElement', AnimationElement)
JuliaElement = elements_ns.class_('JuliaElement', AnimationElement)
MatrixElement = elements_ns.class_('MatrixElement', AnimationElement)
GradientElement = elements_ns.class_('GradientElement', AnimationElement)
FireElement = elements_ns.class_('FireElement', AnimationElement)
TunnelElement = elements_ns.class_('TunnelElement', AnimationElement)
WaveElement = elements_ns.class_('WaveElement', AnimationElement)
StarsElement = elements_ns.class_('StarsElement', AnimationElement)

# conf names
CONF_DENSITY = 'density'
CONF_LENGTH = 'length'
CONF_SPEED = 'speed'
CONF_STRENGTH = 'strength'

def _base_schema(cpp_class):
    return cv.Schema({
        cv.GenerateID(CONF_ID): cv.declare_id(cpp_class),
        cv.Required(CONF_COLOR_SCHEME): color_scheme_schema,
        cv.Optional(CONF_SPEED, default=1.0): cv.float_range(min=0.01, max=10.0),
    })

ANIMATION_ELEMENT_SCHEMA = cv.typed_schema({
    'metaballs':    _base_schema(MetaballsElement),
    'aurora':       _base_schema(AuroraElement),
    'kaleidoscope': _base_schema(KaleidoscopeElement),
    'plasma':       _base_schema(PlasmaElement),
    'ripples':      _base_schema(RipplesElement),
    'spiral':       _base_schema(SpiralElement),
    'voronoi':      _base_schema(VoronoiElement),
    'interference': _base_schema(InterferenceElement),
    'julia':        _base_schema(JuliaElement),
    'matrix':       _base_schema(MatrixElement).extend({
        cv.Optional(CONF_LENGTH, default=10.0): cv.float_range(min=1.0),
        cv.Optional(CONF_DENSITY, default=0.5): cv.float_range(min=0.0, max=1.0),
    }),
    'gradient':     _base_schema(GradientElement),
    'fire':         _base_schema(FireElement).extend({
        cv.Optional(CONF_STRENGTH, default=2.0): cv.float_range(min=0.1, max=10.0),
    }),
    'tunnel':       _base_schema(TunnelElement),
    'wave':         _base_schema(WaveElement),
    'stars':        _base_schema(StarsElement).extend({
        cv.Optional(CONF_DENSITY, default=0.02): cv.float_range(min=0.0, max=1.0),
    }),
}, key='pattern', lower=True)
