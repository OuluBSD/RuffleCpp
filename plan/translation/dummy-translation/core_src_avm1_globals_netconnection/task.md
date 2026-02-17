# Task: Translate core/src/avm1/globals/netconnection.rs

## Description
Translate the Rust file `core/src/avm1/globals/netconnection.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/netconnection.rs
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
- Translated NetConnectionData struct with optional handle field
- Translated NetConnection class for AVM1 NetConnection objects
- Implemented cast() for type-safe Value to NetConnection conversion
- Implemented handle() getter and set_handle() setter
- Implemented on_status_event() for status event dispatching
- Implemented on_empty_status_event() for empty status events
- Implemented send_callback() for AMF callback responses
- Translated constructor, create_class() functions
- Translated is_connected, protocol, uri property getters
- Translated add_header, call, close, connect methods
- Used std::optional for optional handle storage
- Added proper forward declarations for all dependencies
