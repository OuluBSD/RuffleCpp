# Task: Translate render/src/pixel_bender_support.rs

## Description
Translate the Rust file `render/src/pixel_bender_support.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/src/pixel_bender_support.rs
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
- Translated Pixel Bender shader support types
- Implemented FloatPixelData enum for channel configurations (R, Rg, Rgb, Rgba)
- Implemented ImageInputTexture class with three variants:
  - BitmapVariant: owns a BitmapHandle
  - TextureRefVariant: borrows a RawTexture pointer
  - FloatsVariant: raw float pixel data with width, height, and channel info
- Implemented PixelBenderShaderArgument class with ImageInput and ValueInput variants
- Added helper functions for variant type checking and access
- Added get_channel_count() utility function
- Used std::variant for type-safe union behavior
- Added proper forward declarations for render types
