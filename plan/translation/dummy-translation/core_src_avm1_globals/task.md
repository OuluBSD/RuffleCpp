# Task: Translate core/src/avm1/globals.rs

## Description
Translate the Rust file `core/src/avm1/globals.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals.rs
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
- Translated SystemPrototypes struct for system builtin classes
- Translated global function implementations: escape, unescape, parseInt, parseFloat, trace
- Translated isFinite, isNaN, getInfinity, getNaN functions
- Translated setInterval, setTimeout, clearInterval, updateAfterEvent timer functions
- Translated getDepth and remove_display_object for display objects
- Defined AVM_DEPTH_BIAS, AVM_MAX_DEPTH, AVM_MAX_REMOVE_DEPTH constants
- Implemented method dispatcher for global functions
- Template pattern for GC types
