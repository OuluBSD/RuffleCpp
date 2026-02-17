# Task: Translate core/src/avm2/object/qname_object.rs

## Description
Translate the Rust file `core/src/avm2/object/qname_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/qname_object.rs
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
- Translated QNameObject struct representing a boxed QName
- Translated QNameObjectWeak weak reference type
- Translated QNameObjectData internal struct with base and name fields
- Implemented new_empty() constructor for empty QName
- Implemented from_name() for boxing Multiname into QNameObject
- Implemented name(), set_namespace(), set_local_name() accessors
- Implemented local_name(), uri(), is_any_namespace() query methods
- Implemented set_is_qname() and init_name() mutators
- Implemented TObject trait methods: gc_base(), get_next_enumerant(), get_enumerant_value(), get_enumerant_name()
- Used RefLock for interior mutability of Multiname field
- Template pattern for GC types
- Added proper forward declarations for all dependencies
