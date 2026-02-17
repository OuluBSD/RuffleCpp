# Task: Translate core/src/avm2/object/bitmapdata_object.rs

## Description
Translate the Rust file `core/src/avm2/object/bitmapdata_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/bitmapdata_object.rs
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
- Translated BitmapDataObject class with BitmapDataObjectData struct
- Implemented from_bitmap_data_and_class() static constructor
- Implemented from_bitmap_data() static constructor with UpdateContext
- Implemented get_bitmap_data() getter for bitmap data access
- Implemented init_bitmap_data() for initializing bitmap data association
- Implemented gc_base() for TObject trait using HasPrefixField pattern
- Used HasPrefixField pattern for base class
- Template pattern for GC types
- Added proper forward declarations for AVM2 types
