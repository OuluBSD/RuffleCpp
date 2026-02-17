# Task: Translate core/src/avm1/globals/shared_object.rs

## Description
Translate the Rust file `core/src/avm1/globals/shared_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/shared_object.rs
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
- Translated SharedObject class for AVM1 local storage
- Implemented PROTO_DECLS for instance methods: connect, send, flush, close, getSize, setFps, clear, onStatus, onSync
- Implemented OBJECT_DECLS for static methods: deleteAll, getDiskUsage, getLocal, getRemote
- Implemented get_local with full path validation and security checks
- Implemented flush and get_size with AMF serialization
- Implemented clear for removing all data from shared object
- Translated constructor for creating SharedObject instances
- Implemented serialize helper for AMF value serialization
- Used template pattern for GC types
- Added proper forward declarations for all AVM1 types
