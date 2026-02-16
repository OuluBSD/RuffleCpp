# Task: Translate core/src/avm2/object/function_object.rs

## Description
Translate the Rust file `core/src/avm2/object/function_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/function_object.rs
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
- Translated FunctionObject class with FunctionObjectData struct
- Implemented from_method() static constructor for creating functions from ABC methods
- Implemented call() method for function invocation
- Implemented construct() method for constructor calls
- Implemented prototype() getter and set_prototype() setter
- Implemented executable() getter for bound method access
- Implemented to_string() for string representation
- Implemented gc_base() for TObject trait using HasPrefixField pattern
- Used HasPrefixField pattern for base class
- Template pattern for GC types
- Added proper forward declarations for AVM2 types
