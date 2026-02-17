# Task: Translate core/src/avm2/globals/flash/display3D/textures/atf_jpegxr.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/display3D/textures/atf_jpegxr.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/display3D/textures/atf_jpegxr.rs
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
- Translated ATF JPEG-XR compressed texture upload implementation
- Implemented do_compressed_upload() for uploading compressed ATF textures
- Implemented jpegxr_to_raw_pixels() helper for JPEG-XR decoding
- Implemented jpegxr_to_tiff() helper for JPEG-XR to TIFF conversion
- Handles multiple ATF texture formats:
  - JpegXR: Standard JPEG-XR compressed textures
  - CompressedAlpha: DXT1/DXT5 with JPEG-XR encoded block values
    - LZMA decompression for DXT lookup tables
    - JPEG-XR decoding for block values split across image halves
    - Reconstructs DXT data from separate components
  - CompressedRawAlpha: DXT5 fallback with stub warning
  - Unknown: Returns error for unsupported formats
- Validates texture dimensions and cube map flags
- Uses image crate for TIFF decoding to RGBA
- Uses jpegxr crate for JPEG-XR decoding
- Uses lzma_rs for LZMA decompression (optional feature)
- Template pattern for GC types
- Added proper forward declarations for all dependencies
