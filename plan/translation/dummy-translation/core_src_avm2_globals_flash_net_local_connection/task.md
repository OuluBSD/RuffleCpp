# Task: Translate core/src/avm2/globals/flash/net/local_connection.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/net/local_connection.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/net/local_connection.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated flash.net.LocalConnection implementation
- Implemented get_domain() getter returning movie domain for LocalConnection
- Implemented send() method with AMF0 serialization for method calls
- Implemented connect() method with connection name validation
- Implemented close() method to disconnect LocalConnection
- Implemented get_client() and set_client() accessors
- Added error handling for invalid connectionName/methodName (Error #2085)
- Added validation for reserved method names (Error #2004)
- Used local_connection_allocator re-export from object module
- Used template pattern for GC types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
