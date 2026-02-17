# Task: Translate core/src/backend/audio/decoders/mp3.rs

## Description
Translate the Rust file `core/src/backend/audio/decoders/mp3.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/backend/audio/decoders/mp3.rs
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
- Translated MP3 decoder using Symphonia library
- Implemented Mp3Decoder class with Decoder and SeekableDecoder traits
- Implemented new_from_reader() and new_seekable() constructors
- Implemented next_frame() for decoding MP3 frames
- Implemented next() for sample frame iteration
- Implemented seek_to_sample_frame() for accurate seeking
- Implemented mp3_metadata() for getting MP3 metadata
- Used std::optional for optional return values
- Used std::array for stereo sample pairs
- Added proper forward declarations for all dependencies
