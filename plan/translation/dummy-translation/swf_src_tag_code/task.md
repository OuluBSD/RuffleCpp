# Task: Translate swf/src/tag_code.rs

## Description
Translate the Rust file `swf/src/tag_code.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/swf/src/tag_code.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated TagCode enum with all 56 tag codes
- Used uint16_t enum class for type safety
- Implemented from_u16 function using switch statement
- Implemented format function for string representation
- Maintained original tag code numeric values

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
