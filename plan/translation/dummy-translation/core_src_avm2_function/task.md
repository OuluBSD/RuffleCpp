# Task: Translate core/src/avm2/function.rs

## Description
Translate the Rust file `core/src/avm2/function.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/function.rs
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
- Translated FunctionArgs class for function argument storage
- Translated FunctionArgsIter iterator for argument iteration
- Translated BoundMethod class for bound method representation
- Implemented exec() for executing bound methods
- Implemented debug_full_name() for debugging output
- Implemented display_function() helper for function name formatting
- Template pattern for GC types
- Added proper forward declarations for all AVM2 types
