# Task: Translate frontend-utils/src/bundle/source.rs

## Description
Translate the Rust file `frontend-utils/src/bundle/source.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/frontend-utils/src/bundle/source.rs
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
- Translated BundleSourceData trait as abstract base class with read/seek operations
- Implemented FileBundleSourceData for std::ifstream
- Translated BundleSourceImpl interface for bundle reading
- Translated BundleSource enum as class with Type (Directory, ZipFile)
- Implemented BundleSourceError enum and exception class
- Implemented from_path() and from_reader() factory methods
- Implemented read_file() and read_content() methods
- Added BUNDLE_INFORMATION_FILENAME constant
- Forward declared directory and zip submodules
