# Task: Translate tests/fs-tests-runner/src/lib.rs

## Description
Translate the Rust file `tests/fs-tests-runner/src/lib.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/tests/fs-tests-runner/src/lib.rs
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
- Translated filter_to_test_name() function to remove kind prefix from test names
- Implemented is_candidate() function for test filtering based on arguments
- Translated TestLoaderParams struct with args, test_dir, test_dir_real, test_name fields
- Added TestLoader function type using std::function
- Implemented FsTestsRunner class with builder pattern methods
- Added with_root_dir(), with_descriptor_name(), with_additional_test() methods
- Implemented with_test_loader(), with_canonicalize_paths() configuration methods
- Added run() method declaration marked as [[noreturn]]
- Included private methods: ensure_root_dir_exists(), look_up_test(), load_test()
- Used std::shared_ptr for VfsPath and std::optional for test_loader_
- Added forward declarations for libtest_mimic and vfs types
- Maintained compatibility with libtest_mimic test framework
