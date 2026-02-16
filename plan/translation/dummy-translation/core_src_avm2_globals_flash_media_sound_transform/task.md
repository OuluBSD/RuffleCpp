# Task: Translate core/src/avm2/globals/flash/media/sound_transform.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/media/sound_transform.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/media/sound_transform.rs
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
- Translated flash.media.SoundTransform builtin/prototype functions
- Implemented get_left_to_left(), set_left_to_left() for left channel mixing
- Implemented get_left_to_right(), set_left_to_right() for cross-channel mixing
- Implemented get_right_to_left(), set_right_to_left() for cross-channel mixing
- Implemented get_right_to_right(), set_right_to_right() for right channel mixing
- Implemented get_volume(), set_volume() for volume control
- Implemented get_pan(), set_pan() for stereo panning
- pan getter calculates from mixing coefficients: sqrt(1 - left_to_left^2)
- pan setter adjusts left_to_left and right_to_right using sqrt formula
- Re-exported sound_transform_allocator from object module
- Used template pattern for GC types
- Added proper forward declarations for AVM2 types
