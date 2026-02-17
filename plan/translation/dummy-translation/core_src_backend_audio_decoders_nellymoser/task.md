# Task: Translate core/src/backend/audio/decoders/nellymoser.rs

## Description
Translate the Rust file `core/src/backend/audio/decoders/nellymoser.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/backend/audio/decoders/nellymoser.rs
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
- Created `core/src/backend/audio/decoders/nellymoser.h` (42 lines Rust -> ~90 lines C++)
- Translated Decoder and SeekableDecoder trait interfaces as template base classes
- Translated NellymoserDecoder class with template parameter for reader type
- Implemented num_channels() returning 1 (mono)
- Implemented sample_rate() returning configured sample rate
- Implemented next() method for getting stereo samples
- Added SeekableNellymoserDecoder for cursor-based readers with reset() method
- Used std::optional for optional sample return values
- Used std::array for stereo sample pairs
