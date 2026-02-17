# Task: Translate exporter/src/player_ext.rs

## Description
Translate the Rust file `exporter/src/player_ext.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/exporter/src/player_ext.rs
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
- Created `exporter/src/player_ext.h` (50 lines Rust -> ~110 lines C++)
- Translated PlayerExporterExt trait as abstract base class
- Implemented capture_frame() for capturing frames as RGBA images
- Implemented header_frames() for getting SWF header frame count
- Implemented force_root_clip_play() for resuming playback
- Translated PlayerExporterImpl for Arc<Mutex<Player>> pattern
- Used std::shared_ptr and std::mutex for thread-safe player access
- Used std::unique_lock for RAII-based mutex locking
- Added forward declarations for WgpuRenderBackend and TextureTarget
