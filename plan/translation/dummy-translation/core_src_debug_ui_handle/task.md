# Task: Translate core/src/debug_ui/handle.rs

## Description
Translate the Rust file `core/src/debug_ui/handle.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/debug_ui/handle.rs
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
- Translated DisplayObjectHandle class with DynamicRoot and raw pointer storage
- Implemented AVM1ObjectHandle, AVM2ObjectHandle, and DomainHandle classes
- Added template create() and fetch() methods for GC-aware object access
- Implemented operator==, operator!= for pointer-based equality comparison
- Added hash() method and std::hash specializations for unordered_map/set usage
- Used gc_arena forward declarations for DynamicRoot, DynamicRootSet, Gc, Rootable
- Maintained weak reference pattern to avoid preventing garbage collection
