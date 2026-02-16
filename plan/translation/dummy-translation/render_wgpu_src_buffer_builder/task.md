# Task: Translate render/wgpu/src/buffer_builder.rs

## Description
Translate the Rust file `render/wgpu/src/buffer_builder.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/wgpu/src/buffer_builder.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated BufferBuilder class for WebGPU buffer construction
- Implemented BufferFull error struct for limit exceeded errors
- Added new_for_vertices() and new_for_uniform() static factory methods
- Implemented set_buffer_limit() for configuring maximum buffer size
- Implemented add<T>() template method for adding aligned data to buffer
- Implemented finish() method for creating final wgpu::Buffer
- Implemented copy_to() method for copying data via staging belt
- Used std::vector<uint8_t> for internal buffer storage
- Used std::optional for result type (matching Rust Result)
