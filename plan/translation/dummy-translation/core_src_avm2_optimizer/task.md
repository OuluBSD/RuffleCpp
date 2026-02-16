# Task: Translate core/src/avm2/optimizer.rs

## Description
Translate the Rust file `core/src/avm2/optimizer.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/optimizer.rs
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
- Translated optimizer module with submodule declarations (blocks, dce, nop_remover, peephole, type_aware)
- Implemented optimize() template function that runs all optimizer passes
- Added forward declarations for Activation, Method, Op, Error, Exception, and ResolvedParamConfig
- Used std::vector for code operations and std::unordered_set for jump targets
- Maintained original function signature with GC template parameter
