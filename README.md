# VKM - OpenGL-based 3D Rendering Engine

VKM is an open-source 3D rendering engine designed for efficient graphics rendering using the OpenGL API. It supports advanced rendering techniques such as ADS (Ambient, Diffuse, Specular) shading, PBR (Physically Based Rendering), and upcoming support for Nvidia RTX for ray tracing.

## Features

- **OpenGL Integration:** Utilizes the OpenGL graphics API for rendering.
- **Modern C++ Codebase:** Developed with modern C++ standards and practices.
- **Cross-Platform:** Designed to work seamlessly across various platforms, including Windows, Linux.
- **ADS Shading:** Supports Ambient, Diffuse, and Specular shading techniques.
- **PBR Rendering:** Implements Physically Based Rendering for realistic material rendering.
- **Nvidia RTX Support:** Upcoming support for Nvidia RTX technology and ray tracing.
- **Resource Management:** Efficient handling of textures, models, and other graphical resources.

## Getting Started

### Prerequisites

- C++17 compiler
- OpenGL SDK
- CMake (3.21.0)
- GLFW
- GLM
- ImGui
- STBimage
- TinyObjLoader
- Nlohmann


### Installing
```
git clone https://github.com/V-KMilev/Ray-tracer.git
git submodule update --init --recursive
```

### Building
```Bash
mkdir build
cd build
cmake ..
make
```

### Usage
For detailed usage instructions and code examples, refer to the documentation provided.

### Documentation
Comprehensive documentation is available in the docs directory. It covers installation instructions, API reference, and usage guides.

### Contributing
Contributions are welcome! If you find a bug, have a feature request, or would like to contribute code, please follow the contributing guidelines.

### License
This project is licensed under the MIT License.

### Acknowledgments
- OpenGL API
- Nvidia RTX Technology