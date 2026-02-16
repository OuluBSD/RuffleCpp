# Task: Translate core/src/avm1/globals/load_vars.rs

## Description
Translate the Rust file `core/src/avm1/globals/load_vars.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/load_vars.rs
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
- Translated LoadVars class with full PROTO_DECLS for all methods
- Implemented create_class function using empty_class pattern
- Translated all LoadVars methods: load, send, sendAndLoad, decode
- Added getBytesLoaded, getBytesTotal, toString methods
- Implemented onLoad, onData default handlers
- Added addRequestHeader stub method
- Implemented spawn_load_var_fetch helper for async loading
- Used indexmap::IndexMap equivalent for form value storage
- Maintained Flash LoadVars behavior for HTTP form submissions
