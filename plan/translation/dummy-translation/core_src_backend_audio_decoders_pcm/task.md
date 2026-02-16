# Task: Translate core/src/backend/audio/decoders/pcm.rs

## Description
Translate the Rust file `core/src/backend/audio/decoders/pcm.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/backend/audio/decoders/pcm.rs
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
- Translated PcmDecoder template class for PCM audio decoding
- Implemented constructor with is_stereo, sample_rate, is_16_bit parameters
- Added read_sample() helper method for reading individual samples
- Implemented Iterator pattern returning stereo sample pairs [left, right]
- Translated Decoder trait implementation with num_channels() and sample_rate()
- Translated SeekableDecoder trait with reset() and seek_to_sample_frame()
- Used std::optional for sample reading with end-of-stream handling
- Maintained 8-bit unsigned to 16-bit signed conversion logic
