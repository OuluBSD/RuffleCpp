# Task: Translate core/src/avm2/globals.rs

## Description
Translate the Rust file `core/src/avm2/globals.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals.rs
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
- Translated SystemClasses struct for all system builtin classes (100+ class objects)
- Translated SystemClassDefs struct for system builtin class definitions
- Translated init_early_classes() for Object, Class, void initialization
- Implemented early class bootstrap sequence (i_class, c_class linking)
- Translated init_builtin_system_classes() for playerglobal class initialization
- Translated init_builtin_system_class_defs() for marking builtin types
- Translated init_native_system_classes() stub for native classes
- Re-exported toplevel functions (decode_uri, parse_int, trace, etc.)
- Defined PLAYERGLOBAL constant for playerglobal.swf data
- Template pattern for GC types throughout
