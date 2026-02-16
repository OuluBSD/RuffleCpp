# Task: Translate web/src/zip.rs

## Description
Translate the Rust file `web/src/zip.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/web/src/zip.rs
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
- Translated ZipWriter class for creating ZIP archives in web builds
- Implemented add_file() to add files to the ZIP archive
- Implemented save() to generate the final ZIP file as bytes
- Used std::unordered_map for in-memory file storage
- Used std::optional for Result type representation
- Added forward declaration for wasm_bindgen::JsValue
