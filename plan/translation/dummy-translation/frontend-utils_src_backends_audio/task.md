# Task: Translate frontend-utils/src/backends/audio.rs

## Description
Translate the Rust file `frontend-utils/src/backends/audio.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/frontend-utils/src/backends/audio.rs
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
- Translated CpalAudioBackend class for CPAL-based audio output
- Implemented CpalError enum class with all error variants
- Added play() and pause() methods for stream control
- Implemented mixer() accessor for audio mixer access
- Added new_with_device() and create() static factory methods
- Declared get_suitable_output_device() helper function
- Used forward declarations for cpal and ruffle_core types
- Maintained compatibility with multiple sample formats (F32, I16, U16)
