# Task: Translate core/common/src/avm_string/repr.rs

## Description
Translate the Rust file `core/common/src/avm_string/repr.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/common/src/avm_string/repr.rs
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
- Translated AvmStringRepr class template for internal string representation
- Implemented WStrMetadata struct for string metadata
- Added factory methods: from_raw, from_raw_static, new_dependent
- Implemented try_append_inline for in-place string concatenation
- Added is_dependent, owner, as_wstr, is_interned, mark_interned methods
- Template-based design for GC compatibility
- Used std::optional for optional owner reference
- Used std::shared_ptr for garbage collected representation
