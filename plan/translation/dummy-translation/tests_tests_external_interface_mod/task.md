# Task: Translate tests/tests/external_interface/mod.rs

## Description
Translate the Rust file `tests/tests/external_interface/mod.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/tests/tests/external_interface/mod.rs
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
- Translated ExternalInterfaceTestProvider class template for testing ExternalInterface functionality
- Implemented do_trace(), do_ping(), do_reentry() handler functions
- Implemented ExternalInterfaceTestProviderImpl class implementing ExternalInterfaceProvider interface
- call_method() dispatches to appropriate handler based on method name
- on_callback_available() is a no-op for test provider
- get_id() returns std::nullopt
- Template pattern for GC types
- Added proper forward declarations for all dependencies
