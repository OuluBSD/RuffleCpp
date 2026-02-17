# Task: Translate frontend-utils/src/bundle/source/zip.rs

## Description
Translate the Rust file `frontend-utils/src/bundle/source/zip.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/frontend-utils/src/bundle/source/zip.rs
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
- Translated ZipSource template class for zip-based bundle reading
- Implemented open() static factory method
- Implemented read_file() with path normalization (strips leading slash)
- Implemented read_content() for content directory access
- Implemented file_names() for archive enumeration
- Added mutex for thread-safe access
- Added ZipBundleSource type alias for common usage
