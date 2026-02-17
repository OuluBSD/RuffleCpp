# Task: Translate core/src/avm2/globals/reg_exp.rs

## Description
Translate the Rust file `core/src/avm2/globals/reg_exp.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/reg_exp.rs
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
- Translated RegExp builtin functions (init, call_handler)
- Translated RegExp property getters (dotall, extended, global, ignoreCase, multiline, source, lastIndex)
- Translated RegExp property setter (lastIndex)
- Translated RegExp methods (exec, test)
- Used template functions for GC types following existing patterns
