# Task: Translate core/src/avm2/object/sound_object.rs

## Description
Translate the Rust file `core/src/avm2/object/sound_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/sound_object.rs
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
- Translated SoundObject class for AVM2 Sound object representation
- Translated SoundObjectData internal struct with base, loading_state, sound_data, id3 fields
- Translated SoundLoadingState enum with New, Loading, Loaded variants
- Translated SoundData variant type with NotLoaded and Loaded states
- Translated QueuedPlay struct for queued sound play information
- Translated SoundObjectWeak weak reference type
- Translated SoundObjectHandle for stashing across GC mutations
- Implemented sound_allocator() for Sound object allocation
- Implemented sound_handle(), loading_state(), set_loading_state() accessors
- Implemented play() for playing sounds with queuing support
- Implemented set_sound() for setting loaded sound handle
- Implemented id3() and set_id3() for ID3 metadata handling
- Implemented read_and_call_id3_event() for ID3 parsing and event dispatch
- Used HasPrefixField pattern for efficient base class access
- Template pattern for GC types
- Added proper forward declarations for all dependencies
