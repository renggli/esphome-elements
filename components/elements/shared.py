import esphome.codegen as cg

# Elements namespace
elements_ns = cg.esphome_ns.namespace("elements")

# ESPHome display namespace and related types
display_ns = cg.esphome_ns.namespace("display")

DisplayRef = display_ns.class_("Display").operator("ref")

text_align_ns = display_ns.namespace("TextAlign")
TextAlign = text_align_ns.enum("TextAlign")

image_align_ns = display_ns.namespace("ImageAlign")
ImageAlign = image_align_ns.enum("ImageAlign")

# Other types
nullptr = cg.esphome_ns.class_("nullptr")
nullopt = cg.esphome_ns.class_("nullopt")
