# Task: Translate core/src/avm2/globals/vector.rs

## Description
Translate the Rust file `core/src/avm2/globals/vector.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/vector.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated Vector builtin/prototype functions for AVM2 Vector class
- Implemented vector_allocator() for generic Vector allocation (throws error)
- Implemented instance_init() for Vector instance constructor with length and fixed parameters
- Implemented call_handler() for Vector constructor called as function with type coercion
- Implemented get_length()/set_length() for Vector.length property
- Implemented get_fixed()/set_fixed() for Vector.fixed property
- Implemented concat_helper() for Vector.concat implementation
- Implemented join() for Vector.join with separator handling
- Implemented every(), for_each() delegated to array module
- Implemented some() for Vector.some
- Implemented filter() for Vector.filter with callback
- Implemented index_of(), last_index_of() for searching
- Implemented map() for Vector.map with type coercion
- Implemented pop(), push() for stack operations
- Implemented shift(), unshift() for queue operations
- Implemented insert_at(), remove_at() for indexed operations
- Implemented reverse() for reversing vector
- Implemented slice() for extracting sub-vectors
- Implemented sort() with compare function and SortOptions support
- Implemented splice() for inserting/removing elements
- Implemented setup_vector_class() for setting up builtin vector classes
- Implemented init_vector_class_defs() for initializing Vector.<Number>, Vector.<int>, Vector.<uint>, Vector.<*>
- Implemented setup_vector_class_object() and init_vector_class_objects() for ClassObject setup
- Used std::vector for argument lists
- Template pattern for GC types
- Added proper forward declarations for all AVM2 types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
