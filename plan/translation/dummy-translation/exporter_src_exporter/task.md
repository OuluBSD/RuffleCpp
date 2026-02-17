# Task: Translate exporter/src/exporter.rs

## Description
Translate the Rust file `exporter/src/exporter.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/exporter/src/exporter.rs
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
- Translated Exporter class for SWF frame export
- Implemented new_exporter() static factory method
- Implemented start_exporting_movie() for beginning movie export
- Translated MovieExport class for per-movie export state
- Added total_frames() method for frame count calculation
- Added run_frame() method for advancing player
- Added capture_frame() method for capturing frame images
- Used std::shared_ptr for Arc<> equivalent
- Used std::mutex for Mutex<> equivalent
- Forward declared WebGPU and core types
