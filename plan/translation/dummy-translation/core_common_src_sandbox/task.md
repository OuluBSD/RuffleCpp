# Task: Translate core/common/src/sandbox.rs

## Description
Translate the Rust file `core/common/src/sandbox.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/common/src/sandbox.rs
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
- Translated SandboxType enum with all 5 variants (Remote, LocalWithFile, LocalWithNetwork, LocalTrusted, Application)
- Implemented SandboxTypeUtils class with infer() static method
- Added URL parsing logic for sandbox type inference based on scheme
- Used swf::HeaderExt for use_network_sandbox() check
- Added forward declaration for swf::HeaderExt
