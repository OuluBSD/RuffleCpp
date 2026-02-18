# Task: Translate render/src/utils.rs

## Description
Translate the Rust file `render/src/utils.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/src/utils.rs
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
- Created `render/src/utils.h` (497 lines Rust -> ~120 lines C++)
- Translated JpegTagFormat enum for image format detection
- Implemented determine_jpeg_tag_format() for detecting JPEG/PNG/GIF formats
- Implemented decode_define_bits_jpeg() for decoding DefineBitsJPEG2/3 tags
- Implemented decode_define_bits_jpeg_dimensions() for getting image dimensions
- Implemented glue_tables_to_jpeg() for combining JPEG tables with image data
- Implemented remove_invalid_jpeg_data() for cleaning malformed JPEG data
- Implemented decode_define_bits_lossless() for decoding DefineBitsLossless tags
- Implemented premultiply_alpha_rgba() and unmultiply_alpha_rgba() helpers
- Implemented decompress_zlib() for zlib decompression
- Used std::expected for error handling
- Added proper forward declarations for all dependencies
