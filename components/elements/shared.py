import esphome.codegen as cg

# Elements namespace
elements_ns = cg.esphome_ns.namespace("elements")
ElementRef = elements_ns.class_("Element").operator("ref")
ElementConstRef = elements_ns.class_("Element").operator("const").operator("ref")

# Display namespace
display_ns = cg.esphome_ns.namespace("display")
DisplayRef = display_ns.class_("Display").operator("ref")

# Other types
nullptr = cg.esphome_ns.class_("nullptr")
nullopt = cg.esphome_ns.class_("nullopt")
