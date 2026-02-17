# Task: Translate core/src/avm2/object/xml_object.rs

## Description
Translate the Rust file `core/src/avm2/object/xml_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/xml_object.rs
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
- Translated XmlObject template class for XML object representation
- Translated XmlObjectWeak for weak references with upgrade() method
- Translated XmlObjectData internal struct with base and node fields
- Implemented new_xml() static constructor for creating XML objects
- Implemented child() for XML.prototype.child(propertyName)
- Implemented elements() for getting elements by name
- Implemented get_child_list() helper for child retrieval
- Implemented length(), local_name(), namespace_object() accessors
- Implemented matches_name() for name matching
- Implemented deep_copy() for XML deep cloning
- Implemented as_xml_string() for XML serialization
- Implemented equals() and abstract_eq() for equality comparison
- Implemented TObject trait methods: get_property_local, set_property_local, delete_property_local
- Implemented xml_descendants() for descendant retrieval
- Implemented get_next_enumerant(), get_enumerant_name(), get_enumerant_value()
- Used gc_arena::lock::Lock for interior mutability of E4XNode
- Template pattern for GC types
- Added proper forward declarations for all AVM2 and e4x types
