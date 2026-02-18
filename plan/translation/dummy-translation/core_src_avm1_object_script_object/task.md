# Task: Translate core/src/avm1/object/script_object.rs

## Description
Translate the Rust file `core/src/avm1/object/script_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/object/script_object.rs
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
- Translated Object template class for AVM1 script objects
- Translated ObjectWeak template class for weak references
- Translated ObjectHandle class for stashing across GC mutations
- Translated ObjectData internal struct with RefLock for interior mutability
- Implemented new_object(), new_with_native(), new_without_proto() constructors
- Implemented get_data(), set_data() for data property access
- Implemented own_properties() for property enumeration
- Implemented get_local_stored(), set_local() for local property access
- Implemented call(), construct(), construct_on_existing() for invocation
- Implemented getter(), setter() for virtual property access
- Implemented delete_property() for property deletion
- Implemented add_property(), add_property_with_case() for virtual properties
- Implemented call_watcher(), watch(), unwatch() for property watchers
- Implemented define_value() for value definition
- Implemented set_attributes() for attribute management
- Implemented proto(), prototype() for prototype access
- Implemented has_property(), has_own_property(), has_own_virtual() for checks
- Implemented get_stored(), set_stored() for stored property access
- Implemented native(), native_no_super(), as_super_object() accessors
- Implemented interfaces(), set_interfaces() for interface handling
- Implemented length(), set_length(), delete_element() for array operations
- Implemented is_instance_of() for instanceof checks
- Translated Watcher template class for property watchers
- Used RefLock for interior mutability
- Template pattern for GC types throughout
- Added proper forward declarations for all AVM1 types
