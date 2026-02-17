# Task: Translate core/src/debug_ui/movie.rs

## Description
Translate the Rust file `core/src/debug_ui/movie.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/debug_ui/movie.rs
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
- Translated MovieListWindow class for displaying list of known movies
- Translated MovieWindow class for displaying movie details
- Implemented show() methods for both windows with egui UI
- Implemented show_information() panel with movie metadata display
- Implemented show_characters() panel with character list and search
- Translated movie_name() helper function for display names
- Translated open_movie_button() and open_character_button() helper functions
- Translated save_swf() function for saving SWF files
- Used std::shared_ptr for SwfMovie references
- Added proper forward declarations for all dependencies
