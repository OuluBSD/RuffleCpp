# Task: Translate tests/tests/shared_object/mod.rs

## Description
Translate the Rust file `tests/tests/shared_object/mod.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/tests/tests/shared_object/mod.rs
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
- Translated shared_object_avm1() test function for AVM1 SharedObject persistence
- Translated shared_object_self_ref_avm1() test function for AVM1 SharedObject self-reference
- Translated shared_object_avm2() test function for AVM2 SharedObject persistence
- Added forward declarations for ruffle core, test_framework, and libtest_mimic types
- Maintains test structure for verifying SharedObject data persistence across runs
- Uses MemoryStorageBackend for testing storage backend swapping
