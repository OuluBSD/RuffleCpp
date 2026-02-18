# Task: Translate core/src/avm2/globals/flash/net/socket.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/net/socket.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/net/socket.rs
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
- Created `core/src/avm2/globals/flash/net/socket.h` (812 lines Rust -> ~350 lines C++)
- Translated all flash.net.Socket native methods as template functions
- Implemented connect() for socket connection
- Implemented get_timeout(), set_timeout() for timeout management
- Implemented close() for socket closure
- Implemented get_bytes_available() for read buffer status
- Implemented get_endian(), set_endian() for endianness control
- Implemented get_connected() for connection status
- Implemented get_object_encoding(), set_object_encoding() for AMF version
- Implemented flush() for sending buffered data
- Implemented read methods: read_boolean(), read_byte(), read_bytes(), read_double(),
  read_float(), read_int(), read_multi_byte(), read_object(), read_short(),
  read_unsigned_byte(), read_unsigned_int(), read_unsigned_short(), read_utf(), read_utf_bytes()
- Implemented write methods: write_boolean(), write_byte(), write_bytes(), write_double(),
  write_float(), write_int(), write_multi_byte(), write_object(), write_short(),
  write_unsigned_int(), write_utf(), write_utf_bytes()
- Template pattern for GC types
- Added proper forward declarations for all AVM2 types
