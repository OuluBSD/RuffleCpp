# Task: Translate video/software/src/decoder/h263.rs

## Description
Translate the Rust file `video/software/src/decoder/h263.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/video/software/src/decoder/h263.rs
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
- Created `video/software/src/decoder/h263.h` (123 lines Rust -> ~100 lines C++)
- Translated H263Decoder class for H.263/Sorenson Spark video decoding
- Translated H263Error enum for decoder error handling
- Implemented preload_frame() for determining frame dependencies
- Implemented decode_frame() for decoding video frames
- Supports deblocking filter with configurable strength
- Uses h263_rs library for H.263 decoding
- Added proper forward declarations for all dependencies
