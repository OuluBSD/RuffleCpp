# Task: Translate swf/src/avm1/opcode.rs

## Description
Translate the Rust file `swf/src/avm1/opcode.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/swf/src/avm1/opcode.rs
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
- Translated OpCode enum with all 73 AVM1 opcodes
- Used uint8_t enum class for type safety
- Implemented from_u8() function using switch statement for opcode lookup
- Implemented format() function for string representation of opcodes
- Maintained original opcode hex values
- Added op_code_utils namespace for utility functions
