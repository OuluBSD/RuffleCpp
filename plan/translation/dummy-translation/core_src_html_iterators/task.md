# Task: Translate core/src/html/iterators.rs

## Description
Translate the Rust file `core/src/html/iterators.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/html/iterators.rs
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
- Translated TextSpanIter template class for iterating over FormatSpans
- Implemented has_next(), next(), and reset() methods
- Used std::optional for iterator return type
- Added forward declarations for FormatSpans, TextSpan, and WStr types
- Used std::tuple for return value containing (start_pos, end_pos, text_slice, span)
- Implemented proper iterator state management with start_pos_, base_, and index_ fields
