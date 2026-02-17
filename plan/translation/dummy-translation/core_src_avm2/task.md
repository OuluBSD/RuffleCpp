# Task: Translate core/src/avm2.rs

## Description
Translate the Rust file `core/src/avm2.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2.rs
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
- Translated Avm2 struct as the main AVM2 interpreter state
- Translated all fields: player_version, stack, scope_stack, call_stack, domains
- Translated system_classes and system_class_defs optionals
- Translated broadcast_list for broadcast event handling
- Translated alias_to_class_map and class_to_alias_map
- Implemented constructor with all initialization
- Implemented load_player_globals() static method
- Implemented dispatch_event() and broadcast_event() methods
- Implemented register_broadcast_listener() for broadcast registration
- Implemented do_abc() for loading ABC files
- Implemented load_builtin_abc() for playerglobal loading
- Implemented class alias registration methods
- Added avm_debug! macro for debug output
- Template pattern for GC types
- Added proper forward declarations for all dependencies
