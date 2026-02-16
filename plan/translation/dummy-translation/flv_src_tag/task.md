# Task: Translate flv/src/tag.rs

## Description
Translate the Rust file `flv/src/tag.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/flv/src/tag.rs
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
- Translated TagData struct with AudioTag, VideoTag, ScriptTag, InvalidTag variants using std::variant
- Implemented Tag struct with timestamp (int32_t), stream_id (uint32_t), and data fields
- Added parse() static method for parsing FLV tags with proper error handling
- Added skip_back() static method for reverse seeking in FLV files
- Used forward declarations for FlvReader, Error, AudioData, VideoData, and ScriptData
- Maintained original error handling semantics (EndOfData vs Invalid)
