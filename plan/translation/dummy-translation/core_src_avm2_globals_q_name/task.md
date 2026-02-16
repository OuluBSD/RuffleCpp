# Task: Translate core/src/avm2/globals/q_name.rs

## Description
Translate the Rust file `core/src/avm2/globals/q_name.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/q_name.rs
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
- Translated QName call_handler for constructor logic
- Translated q_name_constructor with argument handling:
  - No arguments: creates QName with empty local name
  - One argument: copies QName or uses as local name with public namespace
  - Two arguments: first is namespace, second is local name
- Implemented get_local_name() getter for QName.localName
- Implemented get_uri() getter for QName.uri
- Implemented to_string() for QName.AS3::toString and prototype.toString
- Used template pattern for GC types
- Added proper forward declarations for AVM2 types
