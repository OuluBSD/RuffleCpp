# Task: Translate core/src/bitmap/bitmap_data.rs

## Description
Translate the Rust file `core/src/bitmap/bitmap_data.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/bitmap/bitmap_data.rs
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
- Translated BitmapData template class for bitmap manipulation
- Translated BitmapRawData struct for raw pixel data storage
- Translated Color struct for ARGB color values with premultiplied alpha support
- Implemented LehmerRng class for Park-Miller random number generation
- Translated ChannelOptions bitflags for channel selection
- Translated DirtyState enum for CPU/GPU synchronization tracking
- Implemented BitmapRawDataWrapper for thread-safe bitmap access
- Translated IBitmapDrawable union type for bitmap rendering
- Translated ThresholdOperation enum for threshold comparisons
- Used std::vector for pixel storage
- Used std::optional for optional values
- Added proper forward declarations for all AVM2 and render types
