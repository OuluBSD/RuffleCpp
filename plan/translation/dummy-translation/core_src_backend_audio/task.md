# Task: Translate core/src/backend/audio.rs

## Description
Translate the Rust file `core/src/backend/audio.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/backend/audio.rs
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
- Translated AudioBackend trait as abstract base class
- Implemented SoundHandle and SoundInstanceHandle type aliases
- Translated SoundStreamWrapping enum (Swf, Unwrapped)
- Translated SoundStreamInfo struct for stream configuration
- Translated RegisterError and DecodeError enums
- Implemented all AudioBackend virtual methods:
  - play(), pause() for playback control
  - register_sound(), register_mp3() for sound registration
  - start_sound(), start_stream(), start_substream() for playback
  - stop_sound(), stop_all_sounds() for stopping
  - get_sound_position(), get_sound_duration(), get_sound_size() for info
  - get_sound_format() for format info
  - set_sound_transform(), get_sound_peak() for transforms
  - tick(), set_frame_rate(), position_resolution() for timing
  - volume(), set_volume() for volume control
  - get_sample_history() for sample access
  - is_sound_playing() for status check
- Translated NullAudioBackend class as null object pattern
- Translated SoundInstance template struct for instance tracking
- Translated AudioManager template class for sound management
- Implemented MAX_SOUNDS, DEFAULT_STREAM_BUFFER_TIME constants
- Implemented STREAM_RESTART_THRESHOLD, STREAM_DEFAULT_SYNC_THRESHOLD
- Implemented audio_skew_time() for sync calculation
- Used std::optional for optional values
- Used std::array for stereo samples
- Added proper forward declarations for all dependencies
