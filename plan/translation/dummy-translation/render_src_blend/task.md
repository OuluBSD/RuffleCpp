# Task: Translate render/src/blend.rs

## Description
Translate the Rust file `render/src/blend.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/src/blend.rs
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
- Translated ExtendedBlendMode enum with all 16 blend mode variants
- Implemented extended_blend_mode_from_str() returning std::optional<ExtendedBlendMode>
- Implemented extended_blend_mode_to_str() returning const char*
- ExtendedBlendMode extends swf::BlendMode with additional Shader variant
