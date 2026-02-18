# Task: Translate core/src/avm2/object/class_object.rs

## Description
Translate the Rust file `core/src/avm2/object/class_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/class_object.rs
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
- Translated ClassObject template class for AVM2 class objects
- Translated ClassObjectData internal struct with base, class, prototype, scopes fields
- Translated ClassObjectWeak weak reference type with upgrade() method
- Implemented from_class() for full class construction
- Implemented from_class_partial() for partial class allocation
- Implemented from_class_minimal() for minimal class allocation
- Implemented allocate_prototype() for prototype allocation
- Implemented bind_methods() for method binding
- Implemented validate_class() for class validation
- Implemented run_class_initializer() for running class initializers
- Implemented into_finished_class() for finishing class initialization
- Implemented link_prototype() and link_type() for prototype/type linking
- Implemented call_init() for instance initialization
- Implemented call_super(), get_super(), set_super() for supercalls
- Implemented add_application() and parametrize() for generic classes
- Implemented accessors: inner_class_definition(), prototype(), superclass_object()
- Implemented scope and vtable accessors
- Used HasPrefixField pattern for base class access
- Template pattern for GC types throughout
- Added proper forward declarations for all AVM2 types
