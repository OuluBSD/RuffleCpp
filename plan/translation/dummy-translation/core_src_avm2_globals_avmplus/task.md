# Task: Translate core/src/avm2/globals/avmplus.rs

## Description
Translate the Rust file `core/src/avm2/globals/avmplus.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/avmplus.rs
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
- Translated avmplus.describeTypeJSON implementation
- Implemented DescribeTypeFlags bitflags class
- Implemented describe_type_json() main function
- Implemented instance_class_describe_type() for type resolution
- Implemented describe_internal_body() for trait description
- Implemented write_params() for method parameter output
- Implemented write_metadata() for metadata output
- Implemented display_name() helper for name formatting
- Handles all trait types: slots, methods, virtual properties
- Supports all flags: INCLUDE_BASES, INCLUDE_INTERFACES, INCLUDE_METHODS, etc.
- Used std::optional for optional values
- Added proper forward declarations for all AVM2 types
