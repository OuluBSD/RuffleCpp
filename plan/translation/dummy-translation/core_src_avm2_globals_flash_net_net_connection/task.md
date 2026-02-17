# Task: Translate core/src/avm2/globals/flash/net/net_connection.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/net/net_connection.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/net/net_connection.rs
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
- Translated flash.net.NetConnection builtin/prototype functions
- Implemented connect() for connecting to HTTP(S) or local connections
- Implemented close() for closing connections
- Implemented get_connected() for checking connection status
- Implemented get_connected_proxy_type() for getting proxy type
- Implemented get_far_id(), get_far_nonce() for remote connection info
- Implemented get_near_id(), get_near_nonce() for local connection info
- Implemented get_protocol() for getting connection protocol
- Implemented get_uri() for getting connection URI
- Implemented get_using_tls() for checking TLS usage
- Implemented call() for invoking remote methods with AMF serialization
- Implemented add_header() for adding Flash Remoting headers
- Used std::vector for argument lists
- Used std::optional for optional values
- Added proper forward declarations for all AVM2 and net_connection types
