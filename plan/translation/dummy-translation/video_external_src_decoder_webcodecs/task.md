# Task: Translate video/external/src/decoder/webcodecs.rs

## Description
Translate the Rust file `video/external/src/decoder/webcodecs.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/video/external/src/decoder/webcodecs.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated H264Decoder class using WebCodecs API
- Added VideoDecoder interface base class
- Added NaluIterator for NAL unit parsing
- Included NALU type constants (NALU_TYPE_IDR)
- Added pixel format and encoded video chunk type enums
- Translated configure_decoder, preload_frame, and decode_frame methods

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
