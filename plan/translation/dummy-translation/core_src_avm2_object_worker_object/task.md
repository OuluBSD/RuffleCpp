# Task: Translate core/src/avm2/object/worker_object.rs

## Description
Translate the Rust file `core/src/avm2/object/worker_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/worker_object.rs
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
- Translated WorkerObjectData template class with base ScriptObjectData field
- Translated WorkerObjectWeak template class for weak references using GcWeak
- Translated WorkerObject template class inheriting from TObject
- Implemented gc_base() override returning base script object
- Implemented new_object() static factory method
- Used template pattern for GC types
- Added proper forward declarations for AVM2 types
