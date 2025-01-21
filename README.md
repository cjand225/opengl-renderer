# OpenGL Renderer

## Overview

This project is a 3D renderer built using OpenGL with an Entity Component System (ECS) architecture. It demonstrates modern rendering techniques and game engine design patterns, including model loading, transformations, and custom shaders.

## Features

### Rendering
- Model loading from OBJ files with material support
- Texture loading from DDS files
- Basic camera controls with mouse and keyboard input
- Modern OpenGL practices using VAOs and VBOs

### Entity Component System
- Flexible component-based architecture
- Entity management with dynamic component attachment/detachment
- System-based logic processing
- Extensible design for adding new components and systems

## Prerequisites

- C++17 compatible compiler
- [CMake](https://cmake.org/download/) 3.20 or higher
- [vcpkg](https://github.com/Microsoft/vcpkg) accessible from command line/terminal
- OpenGL 3.3 or higher capable GPU

The following dependencies are automatically handled by vcpkg:
- GLFW3 for windowing and input
- GLEW for OpenGL extension loading (non-macOS platforms only)
- GLM for mathematics
- Dear ImGui for UI
- GTest for unit testing

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

3. Run tests (Debug/RelWithDebInfo builds only):

```sh
cd build
ctest --output-on-failure
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

- Left Mouse Button: Orbit camera around target
  - Drag horizontally to rotate around the target (theta)
  - Drag vertically to change elevation angle (phi)
- Mouse Scroll: Zoom in/out
  - Scroll up to zoom in
  - Scroll down to zoom out
- ESC: Exit

The camera uses an orbital system where:
- The camera orbits around a target point
- Zoom is controlled by adjusting the orbital radius
- Movement is constrained to maintain a stable viewing angle

## Project Structure

```
.
├── assets/             # Shader files, models, and textures
├── include/            # Header files
│   └── ECS/           # Entity Component System headers
│       ├── Components/  # Component declarations
│       ├── Systems/    # System declarations
│       ├── Component.h # Base component class
│       ├── Entity.h    # Entity class
│       └── System.h    # Base system class
├── src/               # Source files
│   └── ECS/          # Entity Component System implementations
│       ├── Components/ # Component implementations
│       ├── Systems/   # System implementations
│       ├── Component.cpp
│       ├── Entity.cpp
│       └── System.cpp
├── tests/             # Test files
│   └── src/
│       └── ecs/      # ECS tests
│           ├── Components/  # Component tests
│           ├── Systems/    # System tests
│           └── mocks/      # Test mocks
├── CMakeLists.txt     # CMake build configuration
├── vcpkg.json         # vcpkg dependencies
└── README.md          # This file
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.
