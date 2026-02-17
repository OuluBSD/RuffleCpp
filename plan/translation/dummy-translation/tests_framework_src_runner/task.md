# Task: Translate tests/framework/src/runner.rs

## Description
Translate the Rust file `tests/framework/src/runner.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/tests/framework/src/runner.rs
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
- Translated TestRunner class for executing SWF test files
- Implemented TestStatus enum (Continue, Sleep, Finished)
- Implemented create() static factory method for creating test runners
- Implemented tick() method for advancing test execution
- Implemented do_tick() for internal tick logic
- Implemented test() for running tests after each tick
- Implemented last_test() for final test verification
- Implemented take_image_comparison_by_trigger() for image comparison handling
- Added getters: get_player(), get_options(), next_tick_may_be_last(), is_preloaded()
- Used std::shared_ptr and std::mutex for thread-safe player access
- Used std::optional for optional return types
- Added proper forward declarations for all dependencies
