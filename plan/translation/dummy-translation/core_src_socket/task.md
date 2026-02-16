# Task: Translate core/src/socket.rs

## Description
Translate the Rust file `core/src/socket.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/socket.rs
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
- Translated SocketHandle type alias for socket identification
- Implemented ConnectionState enum (Connected, Failed, TimedOut)
- Implemented SocketAction enum for socket events
- Implemented SocketTarget template class for AVM1/AVM2 object storage
- Implemented Socket template class for individual socket state
- Implemented Sockets template class for socket collection management
- Added connect_avm1() and connect_avm2() methods for socket connections
- Added send(), close(), close_all() methods for socket operations
- Added is_connected() method for connection state checking
- Implemented update_sockets() static method for event dispatching
- Implemented sanitize_host() helper for null-terminated host strings
