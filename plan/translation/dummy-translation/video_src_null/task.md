# Task: Translate video/src/null.rs

## Description
Translate the Rust file `video/src/null.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/video/src/null.rs
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
- Translated NullVideoBackend class implementing VideoBackend interface
- Implemented register_video_stream() that succeeds but does nothing
- Implemented configure_video_stream_decoder() that always succeeds
- Implemented preload_video_stream_frame() that returns FrameDependency::None
- Implemented decode_video_stream_frame() that returns Error::DecodingNotSupported
- Used SlotMap for stream storage (forward declared)
- Added proper forward declarations for video and render types
