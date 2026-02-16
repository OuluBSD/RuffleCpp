# Task: Translate core/src/avm2/object/vertex_buffer_3d_object.rs

## Description
Translate the Rust file `core/src/avm2/object/vertex_buffer_3d_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/vertex_buffer_3d_object.rs
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
- Translated VertexBuffer3DObject template class for AVM2
- Implemented VertexBuffer3DObjectData struct with base, context3d, handle, and data32_per_vertex fields
- Added VertexBuffer3DObjectWeak for weak references
- Implemented from_handle() static factory method
- Added accessor methods: handle(), context3d(), data32_per_vertex()
- Used std::shared_ptr for VertexBuffer handle
- Added DebugFormatter specialization
