# Task: Translate tests/framework/src/image_trigger.rs

## Description
Translate the Rust file `tests/framework/src/image_trigger.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/tests/framework/src/image_trigger.rs
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
- Translated ImageTrigger class with Type enum (LastFrame, SpecificIteration, FsCommand)
- Implemented constructors for each trigger type
- Implemented type(), iteration(), is_specific_iteration(), is_last_frame(), is_fs_command() accessors
- Implemented equality operators (==, !=)
- Added parse_image_trigger() functions for string and integer parsing
- Added proper error handling with std::invalid_argument and std::out_of_range exceptions
