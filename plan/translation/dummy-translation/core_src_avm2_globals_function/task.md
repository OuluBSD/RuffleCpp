# Task: Translate core/src/avm2/globals/function.rs

## Description
Translate the Rust file `core/src/avm2/globals/function.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/function.rs
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
- Translated Function builtin and prototype implementation
- Implemented create_dummy_function() helper using Function.createDummyFunction
- Implemented function_constructor() that creates dummy function (errors if args provided)
- Implemented _init_function_class() to register Function in SystemClasses
- Implemented call() for Function.prototype.call - calls function with this and args
- Implemented apply() for Function.prototype.apply - calls function with this and arg array
  - Handles null/undefined as no arguments
  - Validates array argument and resolves array holes
  - Throws error 1116 for invalid array argument
- Implemented get_length() getter for function parameter count
- Implemented get_prototype() and set_prototype() for function prototype access
- Used template pattern for GC types
- Added proper forward declarations for AVM2 types
