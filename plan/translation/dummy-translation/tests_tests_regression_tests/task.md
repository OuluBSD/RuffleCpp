# Task: Translate tests/tests/regression_tests.rs

## Description
Translate the Rust file `tests/tests/regression_tests.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/tests/tests/regression_tests.rs
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
- Translated TEST_TOML_NAME constant
- Translated RuffleTestOpts struct with ignore_known_failures option
- Implemented parse() method for command line argument parsing
- Translated TestStatus enum (Continue, Sleep, Finished)
- Translated TestRunner interface with tick() method
- Implemented load_test_dir() for loading tests from directory
- Translated Trial struct for test description
- Implemented run_test() for executing test runner loop
- Implemented regression_tests_main() entry point
- Added environment variable handling for RUFFLE_TEST_OPTS
