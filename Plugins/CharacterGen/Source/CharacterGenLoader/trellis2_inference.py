#!/usr/bin/env python3
"""
TRELLIS2 Image to 3D Inference Script
Part of CharacterGen Plugin for Unreal Engine 5.7

Usage:
    python trellis2_inference.py --input <image_path> --output <output_path> [--model microsoft/TRELLIS.2-4B]
"""

import os
import sys
import argparse
import json
import base64
import numpy as np
from pathlib import Path

# Add TRELLIS2 to path
TRELLIS2_PATH = os.path.join(os.path.dirname(__file__), "..", "trellis2")
if os.path.exists(TRELLIS2_PATH):
    sys.path.insert(0, TRELLIS2_PATH)

def parse_args():
    parser = argparse.ArgumentParser(description="TRELLIS2 Image to 3D Generation")
    parser.add_argument("--input", type=str, required=True, help="Input image path")
    parser.add_argument("--output", type=str, required=True, help="Output GLB path")
    parser.add_argument("--model", type=str, default="microsoft/TRELLIS.2-4B", help="Model name or path")
    parser.add_argument("--seed", type=int, default=42, help="Random seed")
    parser.add_argument("--resolution", type=int, default=512, help="Output resolution")
    parser.add_argument("--simplify", type=float, default=0.95, help="Mesh simplification ratio")
    return parser.parse_args()

def preprocess_image(image_path: str, target_size: int = 512) -> np.ndarray:
    """Preprocess image for TRELLIS2 input."""
    try:
        from PIL import Image
        import cv2
        
        img = Image.open(image_path).convert("RGB")
        
        # Resize to target size while maintaining aspect ratio
        w, h = img.size
        if w > h:
            new_w = target_size
            new_h = int(h * target_size / w)
        else:
            new_h = target_size
            new_w = int(w * target_size / h)
        
        img = img.resize((new_w, new_h), Image.LANCZOS)
        
        # Convert to numpy array
        img_array = np.array(img)
        
        # Pad to square
        if new_w != new_h:
            pad_size = max(new_w, new_h)
            padded = np.zeros((pad_size, pad_size, 3), dtype=np.uint8)
            y_offset = (pad_size - new_h) // 2
            x_offset = (pad_size - new_w) // 2
            padded[y_offset:y_offset+new_h, x_offset:x_offset+new_w] = img_array
            img_array = padded
        
        return img_array
        
    except ImportError:
        print("Error: PIL not installed. Run: pip install pillow")
        sys.exit(1)

def run_trellis2_inference(image_path: str, output_path: str, model_name: str = "microsoft/TRELLIS.2-4B", seed: int = 42):
    """Run TRELLIS2 image to 3D generation."""
    
    print(f"Loading TRELLIS2 model: {model_name}")
    print(f"Input image: {image_path}")
    print(f"Output path: {output_path}")
    
    # Set environment variables for GPU memory optimization
    os.environ["PYTORCH_CUDA_ALLOC_CONF"] = "expandable_segments:True"
    os.environ["OPENCV_IO_ENABLE_OPENEXR"] = "1"
    
    try:
        # Import TRELLIS2 components
        import torch
        from trellis2.pipelines import Trellis2ImageTo3DPipeline
        from trellis2.utils import render_utils
        from trellis2.renderers import EnvMap
        import o_voxel
        import cv2
        import imageio
        
        # Check CUDA availability
        if not torch.cuda.is_available():
            print("Warning: CUDA not available. This will be very slow on CPU.")
        
        # Load pipeline
        print("Loading model...")
        pipeline = Trellis2ImageTo3DPipeline.from_pretrained(model_name)
        if torch.cuda.is_available():
            pipeline.cuda()
        
        # Preprocess image
        print("Preprocessing image...")
        image = preprocess_image(image_path)
        
        # Run inference
        print("Running inference (this may take several minutes)...")
        outputs = pipeline.run(
            image,
            seed=seed,
            # Optional: control quality/speed tradeoff
            # num_inference_steps=25,
            # guidance_scale=7.5,
        )
        
        # Post-process and export
        print("Exporting to GLB...")
        
        # Get mesh data
        mesh = outputs["mesh"][0]
        
        # Convert to GLB
        glb = o_voxel.postprocess.to_glb(
            vertices=mesh.vertices,
            faces=mesh.faces,
            attr_volume=mesh.attrs,
            coords=mesh.coords,
            attr_layout=mesh.layout,
            voxel_size=mesh.voxel_size,
            aabb=[[-0.5, -0.5, -0.5], [0.5, 0.5, 0.5]],
            simplify=0.95,
        )
        
        # Save GLB
        output_dir = os.path.dirname(output_path)
        os.makedirs(output_dir, exist_ok=True)
        glb.export(output_path)
        
        print(f"Success! Output saved to: {output_path}")
        
        # Return metadata
        result = {
            "success": True,
            "output_path": output_path,
            "vertex_count": len(mesh.vertices),
            "face_count": len(mesh.faces),
        }
        return result
        
    except ImportError as e:
        print(f"Error: TRELLIS2 not installed: {e}")
        print("Please install TRELLIS2: git clone --recurse-submodules https://github.com/microsoft/TRELLIS.2.git")
        return {"success": False, "error": str(e)}
        
    except Exception as e:
        print(f"Error during inference: {e}")
        import traceback
        traceback.print_exc()
        return {"success": False, "error": str(e)}

def main():
    args = parse_args()
    
    result = run_trellis2_inference(
        image_path=args.input,
        output_path=args.output,
        model_name=args.model,
        seed=args.seed,
    )
    
    # Write result to JSON for UE to parse
    result_path = args.output.replace(".glb", "_result.json")
    with open(result_path, "w") as f:
        json.dump(result, f, indent=2)
    
    if not result.get("success", False):
        sys.exit(1)

if __name__ == "__main__":
    main()
