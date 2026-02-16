# Task: Translate core/src/avm2/object/socket_object.rs

## Description
Translate the Rust file `core/src/avm2/object/socket_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/socket_object.rs
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
- Translated SocketObject template class for AVM2 socket operations
- Implemented SocketObjectData with base, handle, endian, object_encoding, timeout, read_buffer, write_buffer
- Added SocketObjectWeak for weak references
- Implemented endian/object_encoding/timeout accessors and mutators
- Implemented read_bytes/write_bytes for buffer operations
- Implemented read_boolean/write_boolean
- Implemented read_utf_bytes/read_utf/write_utf for string handling
- Implemented read/write methods for all numeric types (float, double, int, uint, short, ushort, byte, ubyte)
- Added helper functions to_bytes/from_bytes for endian conversion
- Added DebugFormatter specialization
