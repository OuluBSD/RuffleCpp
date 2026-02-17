# Task: Translate core/src/avm2/globals/flash/display3D/textures/cube_texture.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/display3D/textures/cube_texture.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/display3D/textures/cube_texture.rs
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
- Translated flash.display3D.textures.CubeTexture native methods
- Implemented upload_from_byte_array() for uploading texture data from ByteArray to cube map sides
- Implemented upload_compressed_texture_from_byte_array() for compressed texture upload with async support
- Implemented upload_from_bitmap_data() for BitmapData texture upload
- Uses do_copy() helper from texture module for actual data upload
- Template pattern for GC types
- Added proper forward declarations for AVM2 types
