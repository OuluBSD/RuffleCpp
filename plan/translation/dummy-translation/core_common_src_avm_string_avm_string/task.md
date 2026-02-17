# Task: Translate core/common/src/avm_string/avm_string.rs

## Description
Translate the Rust file `core/common/src/avm_string/avm_string.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/common/src/avm_string/avm_string.rs
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
- Translated AvmString class template for AVM string representation
- Implemented factory methods: new_ascii_static, new_utf8, new_utf8_bytes, new
- Added substring and concat static methods
- Implemented is_dependent, as_wstr, as_interned accessors
- Added ptr_eq for pointer comparison
- Template-based design for GC compatibility
- Used std::shared_ptr for garbage collected representation
