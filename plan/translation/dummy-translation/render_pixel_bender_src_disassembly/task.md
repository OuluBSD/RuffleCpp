# Task: Translate render/pixel_bender/src/disassembly.rs

## Description
Translate the Rust file `render/pixel_bender/src/disassembly.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/pixel_bender/src/disassembly.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated PixelBenderShaderDisassembly class for Pixel Bender shader debugging
- Implemented to_string() method for converting shader to human-readable format
- Added private helper methods: fmt_metadata(), fmt_parameters(), fmt_reg(), fmt_type()
- Added fmt_operations() and fmt_operation() for operation formatting
- Added prefix_inc() and prefix_dec() for indentation management
- Added qualifier_to_str(), type_to_str(), channel_to_str(), opcode_to_str() converters
- Used forward declarations for all parser module types
- Maintained original disassembly output format for debugging compatibility
