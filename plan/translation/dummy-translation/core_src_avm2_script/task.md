# Task: Translate core/src/avm2/script.rs

## Description
Translate the Rust file `core/src/avm2/script.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/script.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated TranslationUnitData class storing all ABC items (classes, methods, scripts, strings, namespaces, multinames)
- Translated TranslationUnit class with lazy-loading pattern for ABC items
- Implemented pool_string(), pool_namespace(), pool_multiname_static() methods for constant pool access
- Translated Script class with globals object, domain, init method
- Implemented create_globals_object() for script initialization
- Added from_abc_index() static factory method for Script
- Used std::shared_ptr for GC-managed types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
