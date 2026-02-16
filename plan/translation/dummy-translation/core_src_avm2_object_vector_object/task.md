# Task: Translate core/src/avm2/object/vector_object.rs

## Description
Translate the Rust file `core/src/avm2/object/vector_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/vector_object.rs
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
- Translated VectorObject template class for AVM2 typed vector storage
- Implemented VectorObjectData with base ScriptObjectData and VectorStorage
- Added VectorObjectWeak for weak references
- Implemented from_vector() static factory method for wrapping existing storage
- Added as_vector_index() helper for parsing string indices
- Implemented fail_read_error() and fail_write_error() for error generation
- Implemented set_element() for type-coerced element setting
- Added storage() and storage_mut() accessors
- Used std::mutex for thread-safe vector access
- Added DebugFormatter specialization
- Preserved SWF version-dependent error behavior (v10 vs v11+)
