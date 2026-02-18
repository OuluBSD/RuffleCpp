# Task: Translate core/build_playerglobal/src/lib.rs

## Description
Translate the Rust file `core/build_playerglobal/src/lib.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/build_playerglobal/src/lib.rs
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
- Translated build_playerglobal() main function for building playerglobal.swf
- Translated all metadata constants (RUFFLE_METADATA_NAME, METADATA_INSTANCE_ALLOCATOR, etc.)
- Translated flash_to_rust_string() for path conversion
- Translated check_weird_namespace_lookup() for ABC validation
- Translated collect_stubs() for stub collection
- Translated write_native_table() for native method table generation
- Translated strip_metadata() for metadata removal
- Translated trait_has_metadata() for metadata checking
- Translated resolve_multiname_name() and resolve_multiname_ns() for multiname resolution
- Translated strip_version_mark() for version mark removal
- Translated rust_method_path() and rust_path_and_trait_name() for path generation
- Translated NativeAccessibleSlot and NativeCallableMethod structs
- Translated NativeTableResult struct for native table processing results
- Used std::filesystem::path for file paths
- Used std::optional for error handling
- Added proper forward declarations for all dependencies
