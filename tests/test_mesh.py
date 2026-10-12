#!/usr/bin/env python3
"""Comprehensive test suite for 3D polyhedral mesh data in components/elements/animation_data.py.

Validates:
1. Schema & shape registry integrity (all 28 shapes defined with verts, edges, faces, normals).
2. Circumradius bounding (all vertices normalized to |v| <= 1.0 + eps).
3. Face validity (in-bounds indices, no degenerate or zero-area triangles).
4. 2-Manifold topology (watertight closed surfaces, consistent half-edge pairing, Euler characteristic chi = 2).
5. Normal vectors (unit length |n| = 1.0, CCW winding alignment dot >= 0.99).
6. Analytical accuracy for Kepler-Poinsot star polyhedra.
7. Sphere radial outward orientation (dot > 0.95).
"""

import importlib.util
import math
import os
import unittest
from collections import defaultdict

def load_shape_data():
    project_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    ad_path = os.path.join(project_root, "components", "elements", "animation_data.py")
    spec = importlib.util.spec_from_file_location("animation_data", ad_path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod.SHAPE_DATA

def vec_len(v):
    return math.sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2])

def vec_sub(a, b):
    return [a[0] - b[0], a[1] - b[1], a[2] - b[2]]

def vec_add(a, b):
    return [a[0] + b[0], a[1] + b[1], a[2] + b[2]]

def vec_scale(a, s):
    return [a[0] * s, a[1] * s, a[2] * s]

def vec_dot(a, b):
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]

def vec_cross(a, b):
    return [
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0],
    ]

def vec_norm(v):
    l = vec_len(v)
    return [v[0] / l, v[1] / l, v[2] / l] if l > 1e-12 else [0.0, 0.0, 0.0]


class MeshDataTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.shape_data = load_shape_data()

    def test_shape_registry(self):
        """Verify all 37 expected polyhedra and primitives exist in SHAPE_DATA."""
        expected = {
            "tetrahedron", "cube", "octahedron", "icosahedron", "dodecahedron",
            "small_stellated_dodecahedron", "great_stellated_dodecahedron",
            "truncated_tetrahedron", "cuboctahedron", "truncated_cube", "truncated_octahedron",
            "rhombicuboctahedron", "truncated_cuboctahedron", "snub_cube", "icosidodecahedron",
            "truncated_dodecahedron", "truncated_icosahedron", "rhombicosidodecahedron",
            "truncated_icosidodecahedron", "snub_dodecahedron", "triangular_prism",
            "pentagonal_prism", "square_antiprism", "pentagonal_antiprism",
            "triangular_bipyramid", "square_bipyramid", "pentagonal_bipyramid", "sphere",
            "stella_octangula", "star", "triakis_tetrahedron", "pyramid",
            "cylinder", "cone", "torus", "diamond", "rhombic_dodecahedron"
        }
        self.assertEqual(set(self.shape_data.keys()), expected)
        for name, data in self.shape_data.items():
            self.assertIn("verts", data, f"{name} missing verts")
            self.assertIn("edges", data, f"{name} missing edges")
            self.assertIn("faces", data, f"{name} missing faces")
            self.assertIn("normals", data, f"{name} missing normals")
            self.assertGreater(len(data["verts"]), 0, f"{name} has empty verts")
            self.assertGreater(len(data["edges"]), 0, f"{name} has empty edges")
            self.assertGreater(len(data["faces"]), 0, f"{name} has empty faces")
            self.assertEqual(len(data["faces"]), len(data["normals"]),
                             f"{name} face and normal counts do not match")

    def test_circumradius_bound(self):
        """All vertices must be pre-normalized to circumradius R <= 1.0001."""
        for name, data in self.shape_data.items():
            for i, v in enumerate(data["verts"]):
                r = vec_len(v)
                self.assertLessEqual(r, 1.0001,
                                     f"{name}: vertex {i} has radius {r:.6f} > 1.0001")

    def test_face_indices_and_degeneracy(self):
        """Check vertex index ranges and ensure no degenerate zero-area faces."""
        for name, data in self.shape_data.items():
            num_verts = len(data["verts"])
            verts = data["verts"]
            for fi, f in enumerate(data["faces"]):
                self.assertEqual(len(f), 3, f"{name}: face {fi} is not a triangle")
                for vi in f:
                    self.assertTrue(0 <= vi < num_verts,
                                    f"{name}: face {fi} index {vi} out of bounds [0, {num_verts})")
                    self.assertLess(vi, 256, f"{name}: index {vi} exceeds uint8_t capacity")
                # No duplicate indices in a single triangle
                self.assertEqual(len(set(f)), 3, f"{name}: face {fi} has duplicate vertex indices {f}")
                # Area check
                p0, p1, p2 = verts[f[0]], verts[f[1]], verts[f[2]]
                cross = vec_cross(vec_sub(p1, p0), vec_sub(p2, p0))
                area = 0.5 * vec_len(cross)
                self.assertGreater(area, 1e-7, f"{name}: face {fi} has degenerate area {area:.2e}")

    def test_manifold_topology_and_euler(self):
        """Every mesh must be a closed, watertight 2-manifold with Euler characteristic chi = 2."""
        for name, data in self.shape_data.items():
            verts = data["verts"]
            faces = data["faces"]

            half_edges = defaultdict(int)
            undirected_edges = defaultdict(int)

            for f in faces:
                for a, b in [(f[0], f[1]), (f[1], f[2]), (f[2], f[0])]:
                    half_edges[(a, b)] += 1
                    pair = (min(a, b), max(a, b))
                    undirected_edges[pair] += 1

            # 1. Consistent half-edge orientation: every directed half-edge appears exactly once
            for edge, count in half_edges.items():
                self.assertEqual(count, 1, f"{name}: duplicate directed half-edge {edge}")
                opp = (edge[1], edge[0])
                self.assertEqual(half_edges[opp], 1,
                                 f"{name}: half-edge {edge} lacks paired opposite {opp}")

            # 2. 2-Manifold: every edge shared by exactly 2 faces
            for edge, count in undirected_edges.items():
                self.assertEqual(count, 2, f"{name}: edge {edge} shared by {count} faces (expected 2)")

            # 3. Euler characteristic V - E + F == 2 (0 for genus-1 torus)
            v = len(verts)
            e = len(undirected_edges)
            f = len(faces)
            chi = v - e + f
            expected_chi = 0 if name == "torus" else 2
            self.assertEqual(chi, expected_chi, f"{name}: Euler characteristic chi={chi} != {expected_chi} (V={v}, E={e}, F={f})")

    def test_normals_unit_length_and_alignment(self):
        """Unit normals must have |n| == 1.0 and align with CCW face winding cross product."""
        for name, data in self.shape_data.items():
            verts = data["verts"]
            faces = data["faces"]
            normals = data["normals"]

            for fi, (f, n) in enumerate(zip(faces, normals)):
                length = vec_len(n)
                self.assertAlmostEqual(length, 1.0, places=4,
                                       msg=f"{name}: normal {fi} is not unit length ({length:.6f})")

                p0, p1, p2 = verts[f[0]], verts[f[1]], verts[f[2]]
                cross = vec_cross(vec_sub(p1, p0), vec_sub(p2, p0))
                geom_n = vec_norm(cross)
                dot = vec_dot(n, geom_n)
                self.assertGreaterEqual(dot, 0.99,
                                        f"{name}: normal {fi} misaligned with CCW face winding (dot={dot:.4f})")

    def test_star_polyhedra_analytical_accuracy(self):
        """Verify analytical vertex radii for small and great stellated dodecahedra."""
        phi = (1.0 + math.sqrt(5.0)) / 2.0
        norm_ico = math.sqrt(1.0 + phi * phi)

        # 1. small_stellated_dodecahedron (12 apexes at R=1.0, 20 base vertices)
        ssd = self.shape_data["small_stellated_dodecahedron"]
        apex_r = [vec_len(v) for v in ssd["verts"][:12]]
        base_r = [vec_len(v) for v in ssd["verts"][12:32]]
        expected_base_r = math.sqrt(3.0) / (phi * norm_ico)
        for r in apex_r:
            self.assertAlmostEqual(r, 1.0, places=5)
        for r in base_r:
            self.assertAlmostEqual(r, expected_base_r, places=5)

        # 2. great_stellated_dodecahedron (20 apexes at R=1.0, 12 base vertices)
        gsd = self.shape_data["great_stellated_dodecahedron"]
        gsd_apex_r = [vec_len(v) for v in gsd["verts"][:20]]
        gsd_base_r = [vec_len(v) for v in gsd["verts"][20:32]]
        for r in gsd_apex_r:
            self.assertAlmostEqual(r, 1.0, places=5)
        base_spread = max(gsd_base_r) - min(gsd_base_r)
        self.assertLess(base_spread, 1e-5)

    def test_sphere_globe_radial_alignment(self):
        """Verify globe sphere face normals point outward from origin."""
        sphere = self.shape_data["sphere"]
        verts = sphere["verts"]
        for fi, (f, n) in enumerate(zip(sphere["faces"], sphere["normals"])):
            p0, p1, p2 = verts[f[0]], verts[f[1]], verts[f[2]]
            centroid = vec_scale(vec_add(vec_add(p0, p1), p2), 1.0 / 3.0)
            c_hat = vec_norm(centroid)
            dot = vec_dot(n, c_hat)
            self.assertGreater(dot, 0.95, f"Sphere face {fi} radial dot={dot:.4f} <= 0.95")

    def test_new_shapes_counts(self):
        """Verify vertex, edge, and face counts for the 9 new shapes."""
        expected_counts = {
            "stella_octangula": (14, 36, 24),
            "star": (12, 30, 20),
            "triakis_tetrahedron": (8, 18, 12),
            "pyramid": (5, 8, 6),
            "cylinder": (18, 24, 32),
            "cone": (10, 16, 16),
            "torus": (32, 64, 64),
            "diamond": (18, 32, 32),
            "rhombic_dodecahedron": (14, 24, 24),
        }
        for name, (nv, ne, nf) in expected_counts.items():
            shape = self.shape_data[name]
            self.assertEqual(len(shape["verts"]), nv, f"{name}: vert count mismatch")
            self.assertEqual(len(shape["edges"]), ne, f"{name}: edge count mismatch")
            self.assertEqual(len(shape["faces"]), nf, f"{name}: face count mismatch")


if __name__ == "__main__":
    unittest.main()

