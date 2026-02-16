# Task: Translate scanner/src/file_results.rs

## Description
Translate the Rust file `scanner/src/file_results.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/scanner/src/file_results.rs
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
- Translated AvmType enum class (Avm1, Avm2)
- Implemented Compression enum class (None, Zlib, Lzma)
- Added from_swf_compression() helper function declaration
- Translated Step enum class for scanner process stages (Start, Read, Decompress, Parse, Execute, Complete)
- Implemented FileResults struct with all scan result fields
- Added constructors: default and explicit with filename parameter
- Declared hash_to_hex() method for hex formatting
- Declared static hex_to_hash() method for hex parsing
- Used std::optional for all optional fields
- Used uint128_t for testing_time field
- Maintained JSON serialization compatibility with field naming
