# Task: Translate core/src/avm2/vtable.rs

## Description
Translate the Rust file `core/src/avm2/vtable.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/vtable.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated ClassBoundMethod class with super_class_obj, scope, and method fields
- Translated VTableData class with resolved_traits, method_table, slot_classes, default_slots
- Translated VTable class for virtual method table management
- Implemented init_vtable() for building vtable from class traits
- Implemented copy_interface_properties() for interface trait inheritance
- Added make_bound_method() and bind_method() for method binding
- Used std::shared_ptr for GC-managed types
- Used std::unordered_map for metadata tables

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
