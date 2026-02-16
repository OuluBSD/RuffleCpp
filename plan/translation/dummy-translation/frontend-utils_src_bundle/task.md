# Task: Translate frontend-utils/src/bundle.rs

## Description
Translate the Rust file `frontend-utils/src/bundle.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/frontend-utils/src/bundle.rs
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
- Translated BundleError class with Kind enum for error categorization
- Implemented Bundle class for representing loaded game bundles
- Added from_path() static method for loading bundles from filesystem
- Implemented from_source() static method for loading from BundleSource
- Added source(), warnings(), and information() accessors
- Used std::variant for Result type representation
- Used std::shared_ptr for BundleSource and BundleInformation
- Maintained bundle metadata and warnings tracking
