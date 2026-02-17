# Task: Translate core/src/avm1/globals/text_format.rs

## Description
Translate the Rust file `core/src/avm1/globals/text_format.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/text_format.rs
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
- Translated TextFormat class for AVM1 text formatting
- Implemented constructor with 13 optional parameters (font, size, color, bold, italic, underline, url, target, align, margins, indent, leading)
- Implemented all property getters/setters: font, size, color, url, target, bold, italic, underline, align, leftMargin, rightMargin, indent, leading, blockIndent, tabStops, bullet, display, kerning, letterSpacing
- Implemented getTextExtent() method for measuring text dimensions
- Used round_to_even() for SWF version 8+ floating point handling
- Used std::clamp for value clamping
- Used std::optional for optional values
- Template pattern for GC types
- Added proper forward declarations for all AVM1, html, display_object, and SWF types
