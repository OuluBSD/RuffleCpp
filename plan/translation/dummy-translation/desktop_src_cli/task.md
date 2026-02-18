# Task: Translate desktop/src/cli.rs

## Description
Translate the Rust file `desktop/src/cli.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/cli.rs
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
- Created `desktop/src/cli.h` (503 lines Rust -> ~280 lines C++)
- Translated Opt struct with all command-line options
- Translated GameModePreference enum for Linux GameMode support
- Translated OpenUrlMode enum for URL handling configuration
- Translated FilesystemAccessMode enum for filesystem access control
- Translated NamedKeyCode enum for gamepad button mapping
- Implemented helper functions: parse_movie_file_or_url(), parse_duration_seconds(), parse_align(), parse_gamepad_button()
- Implemented default directory functions for save/config/cache
- Added proper forward declarations for all dependencies
