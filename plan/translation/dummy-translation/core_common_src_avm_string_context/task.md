# Task: Translate core/common/src/avm_string/context.rs

## Description
Translate the Rust file `core/common/src/avm_string/context.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/common/src/avm_string/context.rs
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
- Translated StringContext class template for managing AvmStrings
- Implemented interning methods: intern_wstr, intern_static, intern, get_interned
- Added helper methods: empty, make_char, ascii_char, substring
- Translated HasStringContext trait as abstract base class
- Used std::optional for optional return types
- Template-based design for GC compatibility
