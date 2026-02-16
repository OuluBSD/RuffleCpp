# Task: Translate core/src/avm2/optimizer/nop_remover.rs

## Description
Translate the Rust file `core/src/avm2/optimizer/nop_remover.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/optimizer/nop_remover.rs
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
- Translated remove_nops() template function for NOP instruction removal
- Implemented offset mapping vector for tracking position shifts
- Added NOP removal loop with op shifting logic
- Implemented jump offset rewriting for IfTrue, IfFalse, Jump, PopJump operations
- Added LookupSwitch offset rewriting for case and default offsets
- Implemented exception table offset updates (from_offset, to_offset, target_offset)
- Used std::vector for code and exceptions storage
- Template pattern for GC types
