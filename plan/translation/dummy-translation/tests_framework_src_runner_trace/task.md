# Task: Translate tests/framework/src/runner/trace.rs

## Description
Translate the Rust file `tests/framework/src/runner/trace.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/tests/framework/src/runner/trace.rs
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
- Created `tests/framework/src/runner/trace.h` (219 lines Rust -> ~100 lines C++)
- Translated compare_trace_output() function for trace comparison
- Translated PrettyString helper class for assertion formatting
- Implemented test() function with approximation support
- Implemented assert_text_matches() for text comparison
- Implemented path_with_suffix() for VfsPath manipulation
- Supports floating point approximations for numeric comparisons
- Handles known failure cases with appropriate error messages
- Added proper forward declarations for all dependencies
