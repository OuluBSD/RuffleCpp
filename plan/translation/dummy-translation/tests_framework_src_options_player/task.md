# Task: Translate tests/framework/src/options/player.rs

## Description
Translate the Rust file `tests/framework/src/options/player.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/tests/framework/src/options/player.rs
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
- Translated PlayerOptions class for test framework player configuration
- Implemented setter methods for all configuration options
- Added setup() method for configuring PlayerBuilder
- Added can_run() method for checking environment support
- Added viewport_dimensions() method for calculating output size
- Added create_renderer() method for creating render backend
- Used std::optional for optional configuration fields
- Used std::chrono::milliseconds for duration type
- Forward declared core and render types
