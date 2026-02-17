# Task: Translate video/software/src/decoder/screen.rs

## Description
Translate the Rust file `video/software/src/decoder/screen.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/video/software/src/decoder/screen.rs
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
- Translated ScreenError enum with 5 error variants
- Translated ByteReader class for bitstream parsing
- Implemented read_byte(), read_u16be(), read_buf_ref() methods
- Translated ScreenVideoDecoder class implementing VideoDecoder interface
- Implemented frame dimension fields (w_, h_, block_w_, block_h_)
- Implemented tile_ scratch buffer and last_frame_ reference storage
- Implemented decode_v1() for Screen Video V1 decompression using flate2
- Implemented flush() for clearing reference frame
- Implemented preload_frame() for frame dependency detection
- Implemented decode_frame() for full frame decoding with BGR to RGB conversion
- Used std::optional for reference frame storage
- Added forward declarations for video and render types
