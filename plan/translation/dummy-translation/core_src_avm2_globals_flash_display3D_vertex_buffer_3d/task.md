# Task: Translate core/src/avm2/globals/flash/display3D/vertex_buffer_3d.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/display3D/vertex_buffer_3d.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/display3D/vertex_buffer_3d.rs
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
- Translated flash.display3D.VertexBuffer3D native methods: uploadFromByteArray(), uploadFromVector()
- uploadFromByteArray() reads vertex data from ByteArray and uploads to GPU
- uploadFromVector() reads vertex data from Vector.<Number> and uploads to GPU
- Uses context3d().upload_vertex_buffer_data() for actual upload
- Uses std::vector for args parameter
- Template pattern for GC types
