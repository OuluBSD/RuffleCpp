# Task: Translate tests/socket-format/src/lib.rs

## Description
Translate the Rust file `tests/socket-format/src/lib.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/tests/socket-format/src/lib.rs
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
- Translated SocketEvent class with Receive, Send, WaitForDisconnect, and Disconnect variants
- Implemented factory methods: receive(), send(), wait_for_disconnect(), disconnect()
- Implemented type checking methods: is_receive(), is_send(), is_wait_for_disconnect(), is_disconnect()
- Implemented accessor methods: get_expected(), get_payload()
- Implemented from_file() and from_stream() static methods for JSON loading
- Used std::variant for union type storage
- Used std::optional for optional return types
- Added proper forward declarations for all types
