# Task: Translate core/src/avm2/object/script_object.rs

## Description
Translate the Rust file `core/src/avm2/object/script_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/script_object.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated ScriptObject template class as default AVM2 Object implementation
- Translated ScriptObjectWeak for weak references
- Translated ScriptObjectHandle for stashing objects across GC mutations with stash() and fetch() methods
- Translated ScriptObjectData base class with values_, slots_, bound_methods_, proto_, instance_class_, vtable_ fields
- Implemented scriptobject_allocator() function for allocating ScriptObjects
- Implemented ScriptObject::new_object() for creating Object class instances
- Implemented ScriptObject::custom_object() for low-level object construction
- Implemented ScriptObjectData::new_instance() and custom_new() constructors
- Translated ScriptObjectWrapper for additional methods on ScriptObjectData
- Implemented get_property_local(), set_property_local(), init_property_local(), delete_property_local()
- Implemented get_slot(), set_slot() for slot access
- Implemented get_bound_method() for method table access
- Implemented has_own_dynamic_property(), has_own_property() for property checking
- Implemented get_next_enumerant(), get_enumerant_name() for enumeration
- Implemented property_is_enumerable(), set_local_property_is_enumerable() for enumerable control
- Implemented install_bound_method() for method installation
- Implemented maybe_int_property() helper for converting property names to DynamicKey
- Implemented get_dynamic_property() general-purpose dynamic property lookup function
- Used std::optional for optional values
- Used std::vector for slots and bound methods
- Template pattern for GC types
- Added proper forward declarations for all AVM2 types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
