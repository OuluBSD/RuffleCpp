# Task: Translate render/wgpu/src/context3d/shader_pair.rs

## Description
Translate the Rust file `render/wgpu/src/context3d/shader_pair.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/wgpu/src/context3d/shader_pair.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated ShaderPairAgal class for AGAL to Naga shader compilation
- Implemented CompiledShaderProgram struct with vertex/fragment modules and bind group layout
- Implemented ShaderTextureInfo enum (D2, Cube variants)
- Implemented ShaderCompileData struct for shader compilation cache key
- Added fragment_sampler_configs() accessor method
- Implemented compile() method returning RefMut<CompiledShaderProgram>
- Used lru::LruCache for compiled shader program caching
- Inherited from ruffle::render::ShaderModule base class
- Added MAX_VERTEX_ATTRIBUTES constant (16)
- Used std::array and std::optional for fixed-size optional collections
