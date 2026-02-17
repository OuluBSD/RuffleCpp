# Task: Translate render/pixel_bender/src/parser.rs

## Description
Translate the Rust file `render/pixel_bender/src/parser.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/pixel_bender/src/parser.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated PixelBender bytecode parser structures
- Created PixelBenderType variant for type values
- Created PixelBenderReg for register representation
- Created Operation struct for shader operations
- Created PixelBenderParam for shader parameters
- Created PixelBenderMetadata for metadata entries
- Created PixelBenderShader for complete shader representation
- Added utility functions for type/opcode/channel string conversion
- Added PixelBenderShaderHandle for shared pointer wrapper
- Added PixelBenderShaderImpl trait interface
- Added PixelBenderParseException for error handling

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
