# Task: Translate core/src/avm2/object/soundtransform_object.rs

## Description
Translate the Rust file `core/src/avm2/object/soundtransform_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/soundtransform_object.rs
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
- Translated SoundTransformObject template class for AVM2 sound transformation
- Implemented SoundTransformObjectData struct with base and audio mixing fields
- Added SoundTransformObjectWeak for weak references using GcWeak
- Implemented left_to_left, left_to_right, right_to_left, right_to_right accessors/mutators
- Implemented volume accessor/mutator
- Implemented sound_transform_allocator() factory function
- Used HasPrefixField pattern for base class access
- Template pattern for GC types
- Added DebugFormatter specialization with formatted output
