# Task: Translate core/src/avm2/globals/flash/text/engine/text_line.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/text/engine/text_line.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/text/engine/text_line.rs
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
- Translated get_text_width and get_text_height functions
- Measures text dimensions via edit_text.measure_text()
- Returns pixel values converted from Twips
- Uses std::vector for args parameter
- Template pattern for GC types
