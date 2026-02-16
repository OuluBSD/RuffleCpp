# Task: Translate core/src/avm2/globals/flash/net/shared_object.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/net/shared_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/net/shared_object.rs
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
- Translated flash.net.SharedObject builtin/prototype functions
- Implemented new_lso() helper for AMF3 serialization of shared object data
- Implemented get_local() for retrieving/creating local shared objects
- Validates name characters against INVALID_CHARS set
- Handles secure flag requiring HTTPS connections
- Sandboxes shared objects per-domain based on SWF URL
- Supports localPath parameter for path restriction
- Prefixes names containing '/' with '#' to mimic Flash behavior
- Rejects paths with '..' segments for security
- Implemented get_data() accessor for shared object data
- Implemented flush() for persisting data to disk
- Returns "flushed" string or throws Error #2130 on failure
- Empty LSOs are not written to disk (Flash behavior)
- Implemented get_size() returning byte size of serialized data
- Implemented close() stub method
- Implemented clear() to reset data and remove from storage
- Implemented get_object_encoding(), set_object_encoding() stubs
- Used flash_lso library for LSO parsing and writing
