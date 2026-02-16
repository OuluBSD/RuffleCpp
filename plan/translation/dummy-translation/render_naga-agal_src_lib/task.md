# Task: Translate render/naga-agal/src/lib.rs

## Description
Translate the Rust file `render/naga-agal/src/lib.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/naga-agal/src/lib.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved

## Translation Details
- Translated AGAL to Naga shader compiler interface
- Added VertexAttributeFormat enum (Float1-4, Bytes4)
- Added ShaderType enum (Vertex, Fragment)
- Added Error class with various error kinds
- Added SamplerConfig, Filter, Mipmap, Wrapping types
- Implemented agal_to_naga and extract_sampler_configs function declarations
- Defined MAX_VERTEX_ATTRIBUTES (8) and MAX_TEXTURES (8) constants
