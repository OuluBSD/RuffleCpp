# Task: Translate core/src/avm2/object/texture_object.rs

## Description
Translate the Rust file `core/src/avm2/object/texture_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/texture_object.rs
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
- Created `core/src/avm2/object/texture_object.h` (80 lines Rust -> ~180 lines C++)
- Translated TextureObject template class for Texture3D object representation
- Translated TextureObjectWeak for weak references with upgrade() method
- Translated TextureObjectData internal struct with base, context3d, format, handle fields
- Implemented from_handle() static constructor for creating TextureObjects
- Implemented original_format(), handle(), context3d() accessors
- Implemented gc_base() using HasPrefixField pattern for TObject trait
- Used std::shared_ptr for Texture handle (non-GC collected)
- Added proper forward declarations for all AVM2 and render types
- Added std::formatter for debug output
