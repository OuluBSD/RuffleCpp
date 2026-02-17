# Task: Translate core/src/avm2/globals/json.rs

## Description
Translate the Rust file `core/src/avm2/globals/json.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/json.rs
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
- Translated JSON.parse function for parsing JSON strings to AVM2 values
- Translated JSON.stringify function for serializing AVM2 values to JSON strings
- Implemented Replacer struct for handling function and property list replacers
- Implemented AvmSerializer class for recursive value serialization
- Implemented circular reference detection using object stack
- Implemented toJSON method handling for custom serialization
- Used std::vector for argument lists
- Template pattern for GC types
- Added proper forward declarations for all AVM2 types
