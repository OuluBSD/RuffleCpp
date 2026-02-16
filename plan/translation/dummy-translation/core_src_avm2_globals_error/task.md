# Task: Translate core/src/avm2/globals/error.rs

## Description
Translate the Rust file `core/src/avm2/globals/error.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/error.rs
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
- Translated Error class functions: get_error_message and get_stack_trace
- Implemented get_error_message() returning formatted "Error #<id>" string
- Implemented get_stack_trace() with version-dependent behavior:
  - Flash Player 11.5+ (player_version >= 18) and SWF >= 18: stack traces always enabled
  - Flash Player 11.4 and earlier, or SWF < 18: enabled only in Debug mode
- Returns null when stack traces are disabled or not available
- Used template pattern for GC types
- Added proper forward declarations for AVM2 types
