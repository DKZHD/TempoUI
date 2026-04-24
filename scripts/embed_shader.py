import sys
import os
import re

def main():
    if len(sys.argv) < 3:
        print("Wrong Format! | Expected: python embed_shader.py <input> <output>")

    inputPath = sys.argv[1]
    outputPath = sys.argv[2]

    filename = os.path.basename(inputPath)
    cleanName = re.sub(r'[^a-zA-Z0-9]', '_', filename)

    try:
        with open(inputPath) as f:
            shaderSource = f.read()
        header = f"""#pragma once
namespace Generated{{
    inline constexpr std::string_view {cleanName} = R"(
{shaderSource}
    )";
}}
"""
        outputDir = os.path.dirname(outputPath)
        if outputDir:
            os.makedirs(os.path.dirname(outputPath), exist_ok = True)
        
        with open(outputPath, 'w') as f:
            f.write(header)

    except Exception as e:
        print(f"Couldn't embed shader: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()