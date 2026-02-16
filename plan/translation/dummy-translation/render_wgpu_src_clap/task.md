# Task: Translate render/wgpu/src/clap.rs

## Description
Translate the Rust file `render/wgpu/src/clap.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/wgpu/src/clap.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated GraphicsBackend enum with Default, Vulkan, Metal, Dx12, Gl variants
- Translated PowerPreference enum with Low and High variants
- Implemented as_str() helper functions for both enums
- Implemented from_str() parsing functions returning std::optional
- Used uint8_t enum class for type safety

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
