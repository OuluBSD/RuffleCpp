# Task: Translate core/src/backend/audio/decoders/adpcm.rs

## Description
Translate the Rust file `core/src/backend/audio/decoders/adpcm.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/backend/audio/decoders/adpcm.rs
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
- Translated ADPCM audio decoder
- Implemented AdpcmDecoder class with Decoder trait
- Implemented INDEX_TABLE and STEP_TABLE constants
- Implemented SAMPLE_DELTA_CALCULATOR for 2-5 bit samples
- Implemented Channel struct for per-channel state
- Implemented next() for sample frame iteration
- Implemented SeekableAdpcmDecoder for cursor-based readers
- Implemented AdpcmSubstreamDecoder for sound stream blocks
- Used std::optional for optional return values
- Used std::array for stereo sample pairs
- Added proper forward declarations for all dependencies
