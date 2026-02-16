# Task: Translate core/src/avm2/globals/flash/net/xml_socket.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/net/xml_socket.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/net/xml_socket.rs
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
- Translated get_domain helper function for XMLSocket
- Extracts domain from movie URL for connection
- Returns "localhost" for file:// URLs or parse failures
- Uses AvmString for domain string
- Returns Value::Null on URL parse error
