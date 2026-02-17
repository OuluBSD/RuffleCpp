# Task: Translate core/common/src/avm_string/common.rs

## Description
Translate the Rust file `core/common/src/avm_string/common.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/common/src/avm_string/common.rs
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
- Translated CommonStrings class template for common string interning
- Defined all common string fields (ascii_chars array and 200+ string constants)
- Maintained alphabetical ordering of string constants
- Template-based design for GC compatibility
- Used std::array for ASCII character storage
- Added create static method template for initialization
