# OpenGL Renderer

## Overview

This project is a simple renderer built using OpenGL. It demonstrates the basics of 3D rendering, including loading models, applying transformations, and rendering them with custom shaders.

## Features

- Model loading from OBJ files
- Basic camera controls for viewing models from different angles
- Simple lighting and shading through vertex and fragment shaders
- Outline rendering for models
- Cross-platform compatibility with Windows, macOS, and Linux

## Getting Started

### Prerequisites

- C++ compiler (GCC, Clang, MSVC)
- [CMake](https://cmake.org/download/) for building the project
- OpenGL 3.3 or higher
- [GLEW](http://glew.sourceforge.net/) for managing OpenGL extensions
- [GLFW](https://www.glfw.org/) for windowing and input
- [GLM](https://glm.g-truc.net/) for mathematics

### Building the Project

1. Clone the repository:

```sh
    git clone git@github.com:cjand225/opengl-renderer.git
```

2. Navigate to the project directory & create a build folder:

```sh
    cd opengl-renderer
    mkdir build && cd build
```

5. Run CMake to configure the project:

```sh
   cmake .. 
```

Note: If you're using a package manager, remember to specifcy the toolchain with cmake.

`-DCMAKE_TOOLCHAIN_FILE=/path/to/dotcmake/file`

5. Build the project with CMake:

```sh
    cmake --build . --config [Debug|Release]
```

Note: Remember to specify the build config type of either Debug or Release.