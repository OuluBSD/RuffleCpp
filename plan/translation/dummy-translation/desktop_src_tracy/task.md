# Task: Translate desktop/src/tracy.rs

## Description
Translate the Rust file `desktop/src/tracy.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/tracy.rs
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
- Translated RuffleTracyConfig class for Tracy profiler configuration
- Implemented formatter() accessor for tracing formatter
- Implemented stack_depth() static method returning 0 (no stack traces to reduce overhead)
- Used forward declarations for tracing::Metadata and tracing_subscriber types
- Added proper namespace structure under ruffle::desktop::tracy
