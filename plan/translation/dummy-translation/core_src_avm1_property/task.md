# Task: Translate core/src/avm1/property.rs

## Description
Translate the Rust file `core/src/avm1/property.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/property.rs
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
- Translated Property template class for AVM1 user-defined properties
- Implemented Attribute enum with DONT_ENUM, DONT_DELETE, READ_ONLY, and version flags
- Implemented bitwise operators for Attribute enum
- Implemented VERSION_MASKS array for SWF version property visibility
- Implemented new_stored() static method for stored properties
- Implemented new_virtual() static method for virtual properties with getter/setter
- Implemented data(), getter(), setter() accessors
- Implemented set_data() for stored value updates (respects READ_ONLY)
- Implemented set_virtual() for converting to virtual property
- Implemented attributes(), set_attributes() for attribute management
- Implemented is_enumerable(), can_delete(), is_overwritable(), is_virtual() predicates
- Implemented allow_swf_version() for SWF version visibility checking
