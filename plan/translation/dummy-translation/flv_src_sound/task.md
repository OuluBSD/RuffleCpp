# Task: Translate flv/src/sound.rs

## Description
Translate the Rust file `flv/src/sound.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/flv/src/sound.rs
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
- Translated SoundFormat, SoundRate, SoundSize, SoundType enums
- Implemented conversion functions from uint8_t for each enum type
- Translated AudioDataType struct with variant for Raw, AacSequenceHeader, AacRaw
- Translated AudioData struct with parse() static method
- Used std::variant for type-safe union representation
- Maintained original enum values and error handling
