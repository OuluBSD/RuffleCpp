# Task: Translate core/src/avm2/specification.rs

## Description
Translate the Rust file `core/src/avm2/specification.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/specification.rs
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
- Translated ParamInfo, ClassInfo, VariableInfo, FunctionInfo structs for JSON serialization
- Translated TraitList and Definition structs for class definition capture
- Translated ClassStubs class for tracking stubbed methods
- Implemented capture_specification() function for JSON export
- Implemented helper functions: escape_string(), format_value(), format_signature()
- Added serialization skip helpers for optional fields
- Template pattern for GC types
- Added proper forward declarations for all dependencies
