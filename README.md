# OpenGL Renderer

## Overview

This project is a simple renderer built using OpenGL. It demonstrates the basics of 3D rendering, including loading models, applying transformations, and rendering them with custom shaders. It's designed to work on modern systems with OpenGL 3.3+ support.

## Features

- Model loading from OBJ files with material support
- Texture loading from DDS files
- Basic camera controls with mouse and keyboard input
- Modern OpenGL practices using VAOs and VBOs
- Cross-platform compatibility with Windows, macOS, and Linux

## Prerequisites

- C++17 compatible compiler
- [CMake](https://cmake.org/download/) 3.20 or higher
- [vcpkg](https://github.com/Microsoft/vcpkg) accessible from command line/terminal
- OpenGL 3.3 or higher capable GPU

The following dependencies are automatically handled by vcpkg:

- GLFW3 for windowing and input
- GLEW for OpenGL extension loading (non-macOS platforms only)
- GLM for mathematics

## Building the Project

1. Clone the repository:

```sh
git clone https://github.com/cjand225/opengl-renderer.git
cd opengl-renderer
```

2. Configure and build:

```sh
# Configure using vcpkg preset
cmake --preset=vcpkg

# Build
cmake --build build
```

## Platform-Specific Notes

### macOS

- OpenGL is deprecated on macOS but still fully functional
- GLEW is not required and will not be installed
- Both Intel and Apple Silicon (M1/M2) Macs are supported

### Windows

- Requires Visual Studio 2019 or higher with C++17 support
- All dependencies are handled through vcpkg

### Linux

- Requires GCC 8+ or Clang 7+
- May need to install OpenGL development packages:
  ```sh
  # Ubuntu/Debian
  sudo apt-get install libgl1-mesa-dev
  ```

## Controls

- WASD/Arrow Keys: Move camera
- Mouse: Look around
- ESC: Exit

## Project Structure

```
.
├── assets/            # Shader files, models, and textures
├── include/           # Header files
├── src/              # Source files
├── CMakeLists.txt    # CMake build configuration
├── vcpkg.json        # vcpkg dependencies
└── README.md         # This file
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.
