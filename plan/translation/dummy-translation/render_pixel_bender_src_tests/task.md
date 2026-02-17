# Task: Translate render/pixel_bender/src/tests.rs

## Description
Translate the Rust file `render/pixel_bender/src/tests.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/pixel_bender/src/tests.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated PixelBenderShader struct for shader representation
- Implemented PixelBenderReg struct for register definitions
- Implemented PixelBenderParamNormal and PixelBenderParamTexture structs
- Implemented Operation variants (Normal, SampleNearest, LoadFloat)
- Implemented Opcode enum for shader operations
- Implemented PixelBenderRegChannel enum with RGBA helper
- Implemented PixelBenderMetadata struct for metadata entries
- Implemented parse_shader() function declaration
- Implemented test_simple_shader() test function declaration
- Used std::variant for union types
- Used std::vector for dynamic arrays
- Added proper forward declarations for parser types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
