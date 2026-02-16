# Task: Translate video/external/src/backend.rs

## Description
Translate the Rust file `video/external/src/backend.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/video/external/src/backend.rs
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
- Translated ExternalVideoBackend class for H.264 hardware decoding
- Implemented VideoStream class for individual video stream state
- Created ProxyOrStream variant class for proxied vs owned streams
- Added conditional compilation for OpenH264 and WebCodecs features
- Implemented register_video_stream() for stream registration
- Added configure_video_stream_decoder() for decoder configuration
- Implemented preload_video_stream_frame() for frame preloading
- Added decode_video_stream_frame() for frame decoding with renderer
- Used std::variant for Result type representation
- Maintained fallback to SoftwareVideoBackend for non-H.264 codecs
