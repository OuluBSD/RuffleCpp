# Task: Translate render/wgpu/src/mesh.rs

## Description
Translate the Rust file `render/wgpu/src/mesh.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/wgpu/src/mesh.rs
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
- Translated Mesh class for shape mesh rendering
- Implemented PendingDraw struct for deferred draw creation
- Translated PendingDrawType enum (Color, Gradient, Bitmap)
- Implemented Draw struct for finalized draws
- Translated DrawType enum for draw classification
- Implemented BitmapBinds struct for bitmap bind groups
- Translated CommonGradient class for gradient texture/uniform data
- Added GRADIENT_SIZE constant (256) for gradient textures
- Implemented srgb_to_linear() and lerp() helper functions
- Implemented create_texture_transforms() for matrix conversion
