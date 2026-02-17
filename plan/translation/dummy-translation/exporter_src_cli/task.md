# Task: Translate exporter/src/cli.rs

## Description
Translate the Rust file `exporter/src/cli.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/exporter/src/cli.rs
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
- Translated SizeOpt struct for export size configuration
- Translated FrameSelection enum (All, Count variants)
- Implemented FrameSelectionWrapper class with parsing logic
- Added from_string() static method for CLI argument parsing
- Implemented total_frames() method for calculating frame count
- Translated GraphicsBackend enum for backend selection
- Translated PowerPreference enum for power settings
- Translated Opt struct with all CLI options
- Used std::filesystem::path for file paths
- Used std::optional for optional fields
