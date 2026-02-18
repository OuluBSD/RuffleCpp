# Task: Translate desktop/src/gui/theme.rs

## Description
Translate the Rust file `desktop/src/gui/theme.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/gui/theme.rs
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
- Created `desktop/src/gui/theme.h` (204 lines Rust -> ~180 lines C++)
- Translated ThemePreference enum for theme selection (System, Light, Dark)
- Translated ThemeControllerData class for internal theme state
- Translated ThemeController class for managing window and egui themes
- Implemented create() static factory method
- Implemented set_theme() for setting window theme
- Implemented get_system_theme() for reading system theme
- Implemented set_theme_internal() for internal theme application
- Added Linux-specific FreedesktopSettings support with conditional compilation
- Added theme_preference_as_str() and theme_preference_from_str() helper functions
- Used std::weak_ptr for window reference
- Used std::optional for optional values
- Added proper forward declarations for all dependencies
