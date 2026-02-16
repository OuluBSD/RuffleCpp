# Task: Translate core/src/avm2/globals/flash/display3D/program_3d.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/display3D/program_3d.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/display3D/program_3d.rs
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
- Translated upload function for flash.display3D.Program3D
- Uploads vertex and fragment AGAL shaders from ByteArray
- Calls context3d().upload_shaders() with extracted shader bytes
- Returns Value::Undefined
- Uses std::vector for args parameter
