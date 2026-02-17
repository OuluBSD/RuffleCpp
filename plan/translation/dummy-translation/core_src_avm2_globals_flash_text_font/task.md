# Task: Translate core/src/avm2/globals/flash/text/font.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/text/font.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/text/font.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated flash.text.Font builtin/prototype functions
- Implemented get_font_name() returning font descriptor name
- Implemented get_font_style() returning style string (regular, italic, bold, boldItalic)
- Implemented get_font_type() returning type string (embedded, embeddedCFF, device)
- Implemented has_glyphs() for checking glyph availability
- Implemented enumerate_fonts() returning array of available fonts with sorting
- Implemented register_font() for registering font classes globally
- Re-exported font_allocator from object module
- Template pattern for GC types
- Added proper forward declarations for AVM2 types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
