#!/usr/bin/env python3
"""Export animation_data.py meshes, edges, faces, and normals to a Blender file (shapes.blend)
and render a transparent overview image (assets/shapes.png) grouped by classification.

Usage:
  blender --background --python bin/export_blender.py
  or:
  python3 bin/export_blender.py
"""

import math
import os
import shutil
import subprocess
import sys

# If invoked directly via python3, invoke blender in background mode
if "bpy" not in sys.modules:
    blender_bin = shutil.which("blender")
    if not blender_bin and os.path.exists("/Applications/Blender.app/Contents/MacOS/Blender"):
        blender_bin = "/Applications/Blender.app/Contents/MacOS/Blender"
    if not blender_bin:
        print("Error: Blender executable not found in PATH or /Applications.")
        sys.exit(1)

    cmd = [blender_bin, "--background", "--python", __file__]
    res = subprocess.run(cmd)
    sys.exit(res.returncode)

import bpy
from importlib.util import module_from_spec, spec_from_file_location

SHAPE_GROUPS = [
    (
        "Platonic Solids",
        [
            "tetrahedron",
            "cube",
            "octahedron",
            "icosahedron",
            "dodecahedron",
        ],
    ),
    (
        "Star Polyhedra",
        [
            "stella_octangula",
            "star",
            "triakis_tetrahedron",
            "small_stellated_dodecahedron",
            "great_stellated_dodecahedron",
        ],
    ),
    (
        "Archimedean Solids",
        [
            "truncated_tetrahedron",
            "cuboctahedron",
            "truncated_cube",
            "truncated_octahedron",
            "rhombicuboctahedron",
            "truncated_cuboctahedron",
            "snub_cube",
            "icosidodecahedron",
            "truncated_dodecahedron",
            "truncated_icosahedron",
            "rhombicosidodecahedron",
            "truncated_icosidodecahedron",
            "snub_dodecahedron",
        ],
    ),
    (
        "Primitives & Others",
        [
            "pyramid",
            "cylinder",
            "cone",
            "torus",
            "diamond",
            "rhombic_dodecahedron",
            "triangular_prism",
            "pentagonal_prism",
            "square_antiprism",
            "pentagonal_antiprism",
            "triangular_bipyramid",
            "square_bipyramid",
            "pentagonal_bipyramid",
            "sphere",
        ],
    ),
]


def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    project_root = os.path.dirname(script_dir)
    anim_data_path = os.path.join(project_root, "components", "elements", "animation_data.py")

    spec = spec_from_file_location("animation_data", anim_data_path)
    ad_mod = module_from_spec(spec)
    spec.loader.exec_module(ad_mod)
    shape_data = ad_mod.SHAPE_DATA

    bpy.ops.wm.read_factory_settings(use_empty=True)
    scene = bpy.context.scene

    # Organize collections for Blender file inspection
    col_shapes = bpy.data.collections.new("Solid Shapes")
    col_normals = bpy.data.collections.new("Face Normals")
    col_wireframes = bpy.data.collections.new("Wireframes")
    col_labels = bpy.data.collections.new("Labels")

    scene.collection.children.link(col_shapes)
    scene.collection.children.link(col_normals)
    scene.collection.children.link(col_wireframes)
    scene.collection.children.link(col_labels)

    col_normals.hide_viewport = False
    col_wireframes.hide_viewport = True

    # World & Ambient Lighting
    if not scene.world:
        scene.world = bpy.data.worlds.new("World")
    scene.world.use_nodes = True
    bg_node = scene.world.node_tree.nodes.get("Background")
    if bg_node:
        bg_node.inputs["Color"].default_value = (0.28, 0.30, 0.36, 1.0)
        bg_node.inputs["Strength"].default_value = 1.0

    # Key directional light
    light_key = bpy.data.lights.new(name="KeyLight", type="SUN")
    light_key.energy = 2.2
    obj_key = bpy.data.objects.new("KeyLight", light_key)
    obj_key.rotation_euler = (math.radians(45), math.radians(-35), math.radians(135))
    scene.collection.objects.link(obj_key)

    # Fill directional light
    light_fill = bpy.data.lights.new(name="FillLight", type="SUN")
    light_fill.energy = 0.9
    obj_fill = bpy.data.objects.new("FillLight", light_fill)
    obj_fill.rotation_euler = (math.radians(45), math.radians(35), math.radians(-45))
    scene.collection.objects.link(obj_fill)

    # Materials
    mat_shape = bpy.data.materials.new("Mat_SolidShape")
    mat_shape.use_nodes = True
    bsdf_shape = mat_shape.node_tree.nodes.get("Principled BSDF")
    if bsdf_shape:
        bsdf_shape.inputs["Base Color"].default_value = (0.22, 0.48, 0.82, 1.0)
        bsdf_shape.inputs["Roughness"].default_value = 0.35

    mat_normal = bpy.data.materials.new("Mat_NormalVector")
    mat_normal.use_nodes = True
    bsdf_n = mat_normal.node_tree.nodes.get("Principled BSDF")
    if bsdf_n:
        bsdf_n.inputs["Base Color"].default_value = (1.0, 0.65, 0.05, 1.0)
        if "Emission Color" in bsdf_n.inputs:
            bsdf_n.inputs["Emission Color"].default_value = (1.0, 0.65, 0.05, 1.0)
            bsdf_n.inputs["Emission Strength"].default_value = 1.0

    mat_wire = bpy.data.materials.new("Mat_Wireframe")
    mat_wire.use_nodes = True
    bsdf_w = mat_wire.node_tree.nodes.get("Principled BSDF")
    if bsdf_w:
        bsdf_w.inputs["Base Color"].default_value = (0.9, 0.2, 0.2, 1.0)

    # Text material: unshaded deep charcoal slate (#111827) for high-contrast legibility
    mat_text = bpy.data.materials.new("Mat_Text")
    mat_text.use_nodes = True
    mat_text.node_tree.nodes.clear()
    node_out = mat_text.node_tree.nodes.new("ShaderNodeOutputMaterial")
    node_emit = mat_text.node_tree.nodes.new("ShaderNodeEmission")
    node_emit.inputs["Color"].default_value = (0.05, 0.07, 0.10, 1.0)
    node_emit.inputs["Strength"].default_value = 1.0
    mat_text.node_tree.links.new(node_emit.outputs["Emission"], node_out.inputs["Surface"])

    def create_text(name, text, size, pos_x, pos_y, align_y="TOP", is_title=False):
        c = bpy.data.curves.new(name=f"{name}_curve", type="FONT")
        c.body = text
        c.size = size
        c.align_x = "CENTER"
        c.align_y = align_y
        c.space_character = 1.05
        c.extrude = 0.005
        c.bevel_depth = 0.008 if is_title else 0.005
        c.bevel_resolution = 2
        o = bpy.data.objects.new(name, c)
        o.location = (pos_x, pos_y, 0.01)
        o.data.materials.append(mat_text)
        col_labels.objects.link(o)

    # Grid layout parameters
    num_cols = 7
    spacing_x = 3.6
    spacing_y = 3.4
    grid_center_x = (num_cols - 1) * spacing_x / 2.0
    rot_3d = (math.radians(20), math.radians(-18), math.radians(12))

    curr_y = 0.0

    for group_title, shape_names in SHAPE_GROUPS:
        # Group Header Title
        create_text(
            f"{group_title}_header",
            group_title.upper(),
            0.48,
            grid_center_x,
            curr_y,
            align_y="CENTER",
            is_title=True,
        )
        curr_y -= 1.8

        # Chunk shapes into rows of up to num_cols, centered per row
        chunks = [shape_names[i:i + num_cols] for i in range(0, len(shape_names), num_cols)]
        for chunk in chunks:
            row_count = len(chunk)
            offset_x = (num_cols - row_count) * spacing_x / 2.0

            for col_idx, name in enumerate(chunk):
                pos_x = offset_x + col_idx * spacing_x
                pos_y = curr_y

                data = shape_data[name]
                verts = data["verts"]
                edges = data.get("edges", [])
                faces = data.get("faces", [])
                normals = data.get("normals", [])

                # 1. Solid mesh with custom normals
                mesh = bpy.data.meshes.new(f"{name}_mesh")
                mesh.from_pydata(verts, edges, faces)
                mesh.validate(verbose=False)
                mesh.update()

                if normals and len(normals) == len(mesh.polygons):
                    loop_normals = []
                    for f_idx, poly in enumerate(mesh.polygons):
                        n = normals[f_idx]
                        for _ in range(poly.loop_total):
                            loop_normals.append(n)
                    mesh.normals_split_custom_set(loop_normals)

                mesh.materials.append(mat_shape)
                obj_shape = bpy.data.objects.new(name, mesh)
                obj_shape.location = (pos_x, pos_y, 0.0)
                obj_shape.rotation_euler = rot_3d
                col_shapes.objects.link(obj_shape)

                # 2. Face Normals tick vectors
                if normals and len(normals) == len(faces):
                    norm_verts = []
                    norm_edges = []
                    for f_idx, face in enumerate(faces):
                        v0 = verts[face[0]]
                        v1 = verts[face[1]]
                        v2 = verts[face[2]]
                        cx = (v0[0] + v1[0] + v2[0]) / 3.0
                        cy = (v0[1] + v1[1] + v2[1]) / 3.0
                        cz = (v0[2] + v1[2] + v2[2]) / 3.0
                        nx, ny, nz = normals[f_idx]
                        norm_verts.append([cx, cy, cz])
                        norm_verts.append([cx + nx * 0.25, cy + ny * 0.25, cz + nz * 0.25])
                        start_idx = len(norm_verts) - 2
                        norm_edges.append([start_idx, start_idx + 1])

                    norm_mesh = bpy.data.meshes.new(f"{name}_normals_mesh")
                    norm_mesh.from_pydata(norm_verts, norm_edges, [])
                    norm_mesh.validate(verbose=False)
                    norm_mesh.update()
                    norm_mesh.materials.append(mat_normal)
                    obj_normals = bpy.data.objects.new(f"{name}_normals", norm_mesh)
                    obj_normals.location = (pos_x, pos_y, 0.0)
                    obj_normals.rotation_euler = rot_3d
                    col_normals.objects.link(obj_normals)

                # 3. Pure Wireframe lines
                wire_mesh = bpy.data.meshes.new(f"{name}_wire_mesh")
                wire_mesh.from_pydata(verts, edges, [])
                wire_mesh.validate(verbose=False)
                wire_mesh.update()
                wire_mesh.materials.append(mat_wire)
                obj_wire = bpy.data.objects.new(f"{name}_wireframe", wire_mesh)
                obj_wire.location = (pos_x, pos_y, 0.0)
                obj_wire.rotation_euler = rot_3d
                col_wireframes.objects.link(obj_wire)

                # 4. Shape Label
                create_text(
                    f"{name}_lbl",
                    name.replace("_", " "),
                    0.22,
                    pos_x,
                    pos_y - 1.30,
                    align_y="TOP",
                )

            curr_y -= spacing_y
        curr_y -= 0.8

    # Transparent background film & RGBA
    scene.render.film_transparent = True
    scene.render.image_settings.color_mode = "RGBA"
    scene.render.image_settings.file_format = "PNG"
    scene.render.image_settings.compression = 90

    # Tight Bounding Box framing with balanced padding
    pad_x = 2.4
    pad_y_top = 1.0
    pad_y_bot = 0.8

    min_x = -pad_x
    max_x = (num_cols - 1) * spacing_x + pad_x
    max_y = pad_y_top
    min_y = curr_y + 0.8 - 1.30 - 0.25 - pad_y_bot

    width_span = max_x - min_x
    height_span = max_y - min_y
    center_x = (max_x + min_x) / 2.0
    center_y = (max_y + min_y) / 2.0

    aspect = width_span / height_span
    res_h = 2400
    res_w = int(round(res_h * aspect))
    scene.render.resolution_x = res_w
    scene.render.resolution_y = res_h

    # Orthographic camera for distortion-free grid rendering
    cam_data = bpy.data.cameras.new("OverviewCamera")
    cam_data.type = "ORTHO"
    cam_data.sensor_fit = "VERTICAL"
    cam_data.ortho_scale = height_span
    obj_cam = bpy.data.objects.new("OverviewCamera", cam_data)
    obj_cam.location = (center_x, center_y, 25.0)
    obj_cam.rotation_euler = (0.0, 0.0, 0.0)
    scene.collection.objects.link(obj_cam)
    scene.camera = obj_cam

    # Save Blender file
    build_dir = os.path.join(project_root, "build")
    os.makedirs(build_dir, exist_ok=True)
    blend_path = os.path.join(build_dir, "shapes.blend")
    bpy.ops.wm.save_as_mainfile(filepath=blend_path)
    print(f"Exported {len(shape_data)} polyhedra to {blend_path}")

    # Render PNG overview
    build_png = os.path.join(build_dir, "shapes_overview.png")
    scene.render.filepath = build_png
    bpy.ops.render.render(write_still=True)

    # Copy to assets/shapes.png for README documentation
    assets_dir = os.path.join(project_root, "assets")
    os.makedirs(assets_dir, exist_ok=True)
    assets_png = os.path.join(assets_dir, "shapes.png")
    shutil.copyfile(build_png, assets_png)
    print(f"Rendered transparent overview to {assets_png}")


if __name__ == "__main__":
    main()
