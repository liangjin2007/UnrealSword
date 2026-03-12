#!/usr/bin/env python3
"""
CharacterGen Python Launcher
Launches AI inference scripts from Unreal Engine

This script is called from C++ via subprocess to run:
1. TRELLIS2 for image-to-3D
2. Hunyuan3D-Part for mesh decomposition
"""

import os
import sys
import subprocess
import argparse
import json
from pathlib import Path

SCRIPT_DIR = Path(__file__).parent

def run_trellis2(input_image: str, output_path: str, model: str = "microsoft/TRELLIS.2-4B"):
    """Run TRELLIS2 inference."""
    script = SCRIPT_DIR / "trellis2_inference.py"
    
    cmd = [
        sys.executable,
        str(script),
        "--input", input_image,
        "--output", output_path,
        "--model", model,
    ]
    
    result = subprocess.run(cmd, capture_output=True, text=True)
    print(result.stdout)
    if result.stderr:
        print(result.stderr, file=sys.stderr)
    
    return result.returncode == 0

def run_hunyuan3d_part(input_mesh: str, output_dir: str, use_xpart: bool = False):
    """Run Hunyuan3D-Part decomposition."""
    script = SCRIPT_DIR / "hunyuan3d_part_inference.py"
    
    cmd = [
        sys.executable,
        str(script),
        "--input", input_mesh,
        "--output_dir", output_dir,
    ]
    
    if use_xpart:
        cmd.append("--use_xpart")
    
    result = subprocess.run(cmd, capture_output=True, text=True)
    print(result.stdout)
    if result.stderr:
        print(result.stderr, file=sys.stderr)
    
    return result.returncode == 0

def check_dependencies():
    """Check if required Python packages are installed."""
    required = ["torch", "numpy", "trimesh", "PIL"]
    missing = []
    
    for package in required:
        try:
            __import__(package)
        except ImportError:
            missing.append(package)
    
    if missing:
        print(f"Warning: Missing packages: {', '.join(missing)}")
        print("Install with: pip install " + " ".join(missing))
        return False
    
    return True

def main():
    parser = argparse.ArgumentParser(description="CharacterGen Python Launcher")
    parser.add_argument("--task", type=str, required=True, 
                        choices=["trellis2", "hunyuan3d_part", "check_deps"],
                        help="Task to run")
    parser.add_argument("--input", type=str, help="Input path")
    parser.add_argument("--output", type=str, help="Output path")
    parser.add_argument("--model", type=str, default="microsoft/TRELLIS.2-4B", help="Model name")
    parser.add_argument("--use_xpart", action="store_true", help="Use X-Part")
    
    args = parser.parse_args()
    
    if args.task == "check_deps":
        success = check_dependencies()
        sys.exit(0 if success else 1)
    
    if args.task == "trellis2":
        if not args.input or not args.output:
            print("Error: --input and --output required for trellis2")
            sys.exit(1)
        success = run_trellis2(args.input, args.output, args.model)
    
    elif args.task == "hunyuan3d_part":
        if not args.input or not args.output:
            print("Error: --input and --output_dir required for hunyuan3d_part")
            sys.exit(1)
        success = run_hunyuan3d_part(args.input, args.output, args.use_xpart)
    
    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main()
