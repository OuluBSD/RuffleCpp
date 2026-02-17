# Task: Translate core/src/avm1/globals/sound.rs

## Description
Translate the Rust file `core/src/avm1/globals/sound.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/sound.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated Sound template class for AVM1 Sound object tied to AudioBackend sound
- Translated QueuedPlay struct for queued play requests with sound_object, start_offset, loops fields
- Translated SoundState enum with Empty, Loading, Loaded variants
- Translated SoundData internal struct with state, sound_instance, owner, position, duration, is_streaming fields
- Implemented Sound::empty() static constructor for creating empty sounds
- Implemented duration()/set_duration() accessors
- Implemented sound()/sound_instance() getters for sound handles
- Implemented set_sound_instance() setter
- Implemented owner(), position()/set_position() accessors
- Implemented is_streaming()/set_is_streaming() accessors
- Implemented play() method for playing queued sounds
- Implemented set_is_loading() for setting loading state
- Implemented load_sound() for loading sound from handle
- Implemented load_id3() for loading ID3 tags from bytes
- Declared all native method functions: constructor, attach_sound, duration, set_duration, get_bytes_loaded, get_bytes_total, get_pan, get_transform, get_volume, load_sound, position, set_pan, set_transform, set_volume, start, stop, set_position, get_position
- Used std::optional for optional values
- Used std::vector for ID3 bytes
- Template pattern for GC types
- Added proper forward declarations for all AVM1, display_object, and backend types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
