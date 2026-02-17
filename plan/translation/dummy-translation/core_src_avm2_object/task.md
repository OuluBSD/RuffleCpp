# Task: Translate core/src/avm2/object.rs

## Description
Translate the Rust file `core/src/avm2/object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated TObject trait for AVM2 object interface
- Created Object enum for all AVM2 object types
- Created WeakObject for weak references
- Added forward declarations for all object types (ScriptObject, ClassObject, FunctionObject, etc.)
- Added downcast methods for type-specific access
- Added storage access methods (array, bytearray, vector)
- Added helper methods for DisplayObject, Event, Font, RegExp, BitmapData, etc.
- Added abstract_class_allocator and construct_call_handler functions

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
