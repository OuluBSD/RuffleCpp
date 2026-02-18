# Task: Translate core/src/backend/audio/mixer.rs

## Description
Translate the Rust file `core/src/backend/audio/mixer.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/backend/audio/mixer.rs
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
- Translated AudioMixer class for Flash movie audio mixing
- Translated AudioMixerProxy class for thread-safe audio mixing proxy
- Translated CircBuf class for circular audio sample buffer
- Translated Sound struct for registered sound data
- Translated SoundInstance struct for actively playing sounds
- Implemented SoundHandle and SoundInstanceHandle type aliases
- Implemented mix() template method for audio mixing
- Implemented register_sound() and register_mp3() for sound registration
- Implemented start_sound(), start_stream(), start_substream() for playback
- Implemented stop_sound() and stop_all_sounds() for stopping sounds
- Implemented get_sound_position(), get_sound_duration(), get_sound_size() for info
- Implemented get_sound_format() for format info
- Implemented set_sound_transform() for sound transforms
- Implemented volume() and set_volume() for volume control
- Implemented get_sample_history() for sample access
- Implemented get_sound_peak() for peak amplitude
- Used std::mutex and std::shared_mutex for thread safety
- Used std::optional for optional values
- Used std::array for stereo samples
- Added proper forward declarations for all dependencies
