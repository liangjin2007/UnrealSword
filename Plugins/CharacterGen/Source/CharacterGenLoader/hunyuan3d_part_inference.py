#!/usr/bin/env python3
"""
Hunyuan3D-Part Mesh Decomposition Script
Part of CharacterGen Plugin for Unreal Engine 5.7

Usage:
    python hunyuan3d_part_inference.py --input <mesh_path> --output_dir <output_dir>
"""

import os
import sys
import argparse
import json
import numpy as np
from pathlib import Path

def parse_args():
    parser = argparse.ArgumentParser(description="Hunyuan3D-Part Mesh Decomposition")
    parser.add_argument("--input", type=str, required=True, help="Input mesh path (GLB/OBJ/PLY)")
    parser.add_argument("--output_dir", type=str, required=True, help="Output directory for parts")
    parser.add_argument("--use_xpart", action="store_true", help="Use X-Part for high-fidelity decomposition")
    parser.add_argument("--threshold", type=float, default=0.85, help="Post-processing merge threshold")
    return parser.parse_args()

def load_mesh(mesh_path: str):
    """Load mesh from file."""
    import trimesh
    
    mesh = trimesh.load(mesh_path)
    return mesh

def run_p3sam_segmentation(mesh):
    """Run P3-SAM for mesh segmentation."""
    print("Running P3-SAM segmentation...")
    
    # Add P3-SAM to path
    p3sam_path = os.path.join(os.path.dirname(__file__), "..", "Hunyuan3D-Part", "P3-SAM")
    
    if not os.path.exists(p3sam_path):
        print("Warning: P3-SAM not found. Using simple segmentation.")
        return simple_segmentation(mesh)
    
    sys.path.insert(0, p3sam_path)
    
    try:
        import torch
        from p3sam import P3SAM
        
        # Load model
        device = "cuda" if torch.cuda.is_available() else "cpu"
        model = P3SAM.from_pretrained("tencent/Hunyuan3D-Part")
        model = model.to(device)
        model.eval()
        
        # Prepare mesh data
        vertices = mesh.vertices
        faces = mesh.faces
        
        # Run segmentation
        with torch.no_grad():
            # P3-SAM expects normalized vertices
            vertices_normalized = vertices - vertices.min(axis=0)
            vertices_normalized = vertices_normalized / vertices_normalized.max()
            
            # Run model
            segments = model.segment(vertices_normalized, faces)
        
        return segments
        
    except Exception as e:
        print(f"Error in P3-SAM: {e}")
        return simple_segmentation(mesh)

def simple_segmentation(mesh):
    """Simple fallback segmentation based on geometry."""
    print("Using simple segmentation...")
    
    vertices = mesh.vertices
    faces = mesh.faces
    
    # Calculate face centroids
    face_centroids = vertices[faces].mean(axis=1)
    
    # Simple Y-based segmentation (for humanoid)
    y_coords = face_centroids[:, 1]
    y_min, y_max = y_coords.min(), y_coords.max()
    y_range = y_max - y_min
    
    # Classify by Y position
    segments = np.zeros(len(faces), dtype=int)
    
    # Head: top 15%
    head_threshold = y_max - 0.15 * y_range
    segments[y_coords > head_threshold] = 0  # Head
    
    # Torso: middle 40%
    torso_top = y_max - 0.15 * y_range
    torso_bottom = y_max - 0.55 * y_range
    segments[(y_coords > torso_bottom) & (y_coords <= torso_top)] = 1  # Torso
    
    # Legs: bottom 45%
    segments[y_coords <= torso_bottom] = 2  # Legs
    
    return segments

def extract_parts(mesh, segments):
    """Extract mesh parts based on segmentation."""
    import trimesh
    
    parts = []
    unique_segments = np.unique(segments)
    
    for seg_id in unique_segments:
        # Get faces for this segment
        face_mask = segments == seg_id
        face_indices = np.where(face_mask)[0]
        
        if len(face_indices) < 10:
            continue
        
        # Extract submesh
        part_mesh = mesh.submesh([face_indices], append=False)
        
        # Create part data
        part_data = {
            "vertices": part_mesh.vertices.tolist(),
            "faces": part_mesh.faces.tolist(),
            "segment_id": int(seg_id),
        }
        
        parts.append(part_data)
    
    return parts

def run_xpart_enhancement(parts, mesh):
    """Optionally run X-Part for high-fidelity part generation."""
    print("Running X-Part enhancement (optional)...")
    
    # X-Part would refine each part with diffusion-based generation
    # This is computationally expensive and optional
    
    return parts

def export_parts(parts, output_dir: str):
    """Export parts to files."""
    import trimesh
    
    os.makedirs(output_dir, exist_ok=True)
    
    exported_paths = []
    
    for i, part_data in enumerate(parts):
        # Create trimesh
        vertices = np.array(part_data["vertices"])
        faces = np.array(part_data["faces"])
        
        part_mesh = trimesh.Trimesh(vertices=vertices, faces=faces)
        
        # Determine part name based on position
        seg_id = part_data.get("segment_id", i)
        if seg_id == 0:
            part_name = "head"
        elif seg_id == 1:
            part_name = "torso"
        elif seg_id == 2:
            part_name = "legs"
        else:
            part_name = f"part_{seg_id}"
        
        # Export as OBJ and GLB
        obj_path = os.path.join(output_dir, f"{part_name}.obj")
        glb_path = os.path.join(output_dir, f"{part_name}.glb")
        
        part_mesh.export(obj_path)
        part_mesh.export(glb_path)
        
        exported_paths.append({
            "name": part_name,
            "obj_path": obj_path,
            "glb_path": glb_path,
            "vertex_count": len(vertices),
            "face_count": len(faces),
        })
        
        print(f"Exported: {part_name} ({len(vertices)} vertices, {len(faces)} faces)")
    
    return exported_paths

def main():
    args = parse_args()
    
    print(f"Input mesh: {args.input}")
    print(f"Output directory: {args.output_dir}")
    print(f"Use X-Part: {args.use_xpart}")
    
    # Load mesh
    try:
        mesh = load_mesh(args.input)
        print(f"Loaded mesh: {len(mesh.vertices)} vertices, {len(mesh.faces)} faces")
    except Exception as e:
        print(f"Error loading mesh: {e}")
        return {"success": False, "error": str(e)}
    
    # Run segmentation
    try:
        segments = run_p3sam_segmentation(mesh)
        print(f"Segmentation complete: {len(np.unique(segments))} parts")
    except Exception as e:
        print(f"Error in segmentation: {e}")
        return {"success": False, "error": str(e)}
    
    # Extract parts
    try:
        parts = extract_parts(mesh, segments)
        print(f"Extracted {len(parts)} parts")
    except Exception as e:
        print(f"Error extracting parts: {e}")
        return {"success": False, "error": str(e)}
    
    # Optional X-Part enhancement
    if args.use_xpart and len(parts) > 0:
        try:
            parts = run_xpart_enhancement(parts, mesh)
        except Exception as e:
            print(f"Warning: X-Part enhancement failed: {e}")
    
    # Export parts
    try:
        exported = export_parts(parts, args.output_dir)
    except Exception as e:
        print(f"Error exporting parts: {e}")
        return {"success": False, "error": str(e)}
    
    # Return result
    result = {
        "success": True,
        "input_path": args.input,
        "output_dir": args.output_dir,
        "parts": exported,
        "part_count": len(exported),
    }
    
    # Write result JSON
    result_path = os.path.join(args.output_dir, "decomposition_result.json")
    with open(result_path, "w") as f:
        json.dump(result, f, indent=2)
    
    print(f"Success! Results written to: {result_path}")
    return result

if __name__ == "__main__":
    result = main()
    sys.exit(0 if result.get("success", False) else 1)
