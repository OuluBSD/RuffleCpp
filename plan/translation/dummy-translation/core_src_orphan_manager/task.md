# Task: Translate core/src/orphan_manager.rs

## Description
Translate the Rust file `core/src/orphan_manager.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/orphan_manager.rs
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
- Translated OrphanManager template class for AVM2 orphan object management
- Implemented add_orphan_obj() to add display objects to orphan list
- Added each_orphan_obj() static method for iterating over orphans
- Implemented cleanup_dead_orphans() to remove garbage-collected orphans
- Added valid_orphan() helper function
- Used std::shared_ptr to simulate Rust Rc behavior
- Maintained Flash behavior for orphan movie clips
