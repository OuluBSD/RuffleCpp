# Task: Translate render/src/bitmap.rs

## Description
Translate the Rust file `render/src/bitmap.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/src/bitmap.rs
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
- Translated BitmapHandle struct for bitmap references with shared_ptr implementation
- Implemented BitmapHandleImpl interface for bitmap handle implementations
- Implemented BitmapInfo struct for bitmap registration info
- Implemented BitmapSize struct for bitmap dimensions
- Implemented BitmapSource interface for bitmap lookup by ID
- Implemented SyncHandle interface for async operation synchronization
- Implemented PixelSnapping enum for bitmap pixel grid snapping
- Implemented BitmapFormat enum with Rgb, Rgba, Yuv420p, Yuva420p variants
- Implemented Bitmap class for decoded bitmap data from SWF tags
- Implemented to_rgb() and to_rgba() conversion methods
- Implemented width(), height(), format(), data() accessors
- Implemented chroma_width() and chroma_height() for YUV formats
- Implemented ColorIterator for iterating over bitmap colors
- Implemented PixelRegion struct for bitmap operation regions
- Implemented region creation helpers: for_region, for_region_i32, for_whole_size, for_pixel
- Implemented encompassing_twips(), encompassing_pixels(), encompassing_pixels_i32() helpers
- Implemented clamp(), union_with(), encompass(), intersects() region operations
- Implemented clamp_with_intersection() for overlapping region calculations
- Added pixel_region_from_rectangle() and apply_pixel_snapping() helpers
