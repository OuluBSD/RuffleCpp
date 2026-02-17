# Task: Translate video/software/src/decoder.rs

## Description
Translate the Rust file `video/software/src/decoder.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/video/software/src/decoder.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Created `video/software/src/decoder.h` (46 lines Rust -> ~80 lines C++)
- Translated `VideoDecoder` trait to abstract base class
- Preserved conditional module declarations for h263, vp6, and screen decoders

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
