# Task: Translate core/src/html/line_wrapping.rs

## Description
Translate the Rust file `core/src/html/line_wrapping.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/html/line_wrapping.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated wrap_line function for HTML text line wrapping
- Added find_allowed_breaks helper function
- Included SWF version-specific behavior (SWF >= 8 vs SWF <= 7)
- Added CJK character handling helpers (is_cjk_like, is_opening, is_closing)

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
