# Task: Translate core/src/string.rs

## Description
Translate the Rust file `core/src/string.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/string.rs
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
- Translated SwfStrExt trait as a class with static methods
- Implemented decode() and decode_owned() methods for SWF string conversion
- Added forward declarations for WStr, WString, and swf::Encoding types
- Re-exported wstr types for convenience
