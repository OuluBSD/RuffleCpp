# Task: Translate core/src/avm2/api_version.rs

## Description
Translate the Rust file `core/src/avm2/api_version.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/api_version.rs
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
- Translated ApiVersion enum class with 53 API version variants (AllVersions through VM_INTERNAL)
- Translated PlayerRuntime enum class with AIR and FlashPlayer variants
- Implemented get_transfer_table() inline function returning static array of version mappings
- Implemented to_valid_playerglobals_version() function for runtime-based version conversion
- Implemented from_swf_version() function mapping SWF versions (9-31+) to API versions
- Used std::array<std::pair<ApiVersion, ApiVersion>, 53> for transfer table storage
- Added comprehensive comments referencing Adobe AVMPlus source and Ruffle wiki
- Maintained original version mapping logic for AIR vs FlashPlayer runtimes
