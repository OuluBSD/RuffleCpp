# Task: Translate video/external/src/decoder.rs

## Description
Translate the Rust file `video/external/src/decoder.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/video/external/src/decoder.rs
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
- Created `video/external/src/decoder.h` (17 lines Rust -> ~50 lines C++)
- Translated module file with conditional compilation for OpenH264 and WebCodecs
- Re-exported VideoDecoder from ruffle_video_software::decoder
- Added namespace structure for openh264 and webcodecs submodules
- Used preprocessor directives for feature-gated compilation
