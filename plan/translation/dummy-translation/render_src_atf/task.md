# Task: Translate render/src/atf.rs

## Description
Translate the Rust file `render/src/atf.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/src/atf.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated ATFFormat enum with all format variants (RGB888, RGBA8888, Compressed, etc.)
- Created ATFTextureData variant types using std::variant for different data formats
- Translated ATFTexture class with width, height, cubemap, format, and mip data fields
- Added from_bytes static method declaration for parsing ATF files
- Added ATFError exception class for parse errors

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
