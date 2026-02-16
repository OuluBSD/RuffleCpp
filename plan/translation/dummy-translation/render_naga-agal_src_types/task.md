# Task: Translate render/naga-agal/src/types.rs

## Description
Translate the Rust file `render/naga-agal/src/types.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/naga-agal/src/types.rs
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
- Translated AGAL (Adobe Graphics Assembly Language) types
- Implemented Opcode enum with all 39 opcodes (Mov, Add, Sub, Mul, etc.)
- Added RegisterType, DirectMode, Mask, Filter, Mipmap, Wrapping, Dimension enums
- Implemented DestField, SourceField, SamplerField, Special structs with parse methods
- Added Source2 class for union of SourceField and SamplerField
- Used uint8_t enum classes for type safety
