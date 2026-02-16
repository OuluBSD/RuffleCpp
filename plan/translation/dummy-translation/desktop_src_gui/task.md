# Task: Translate desktop/src/gui.rs

## Description
Translate the Rust file `desktop/src/gui.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/gui.rs
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
- Translated RuffleGui class as main GUI controller
- Added MENU_HEIGHT constant (24 pixels) for menu bar offset
- Implemented create() static factory method with proper initialization
- Added update() method for rendering GUI with egui context
- Implemented show_context_menu(), close_context_menu(), is_context_menu_visible() methods
- Added on_player_destroyed() and on_player_created() lifecycle methods
- Re-exported LocalizableText, text(), optional_text(), text_with_args(), available_languages()
- Re-exported ThemePreference enum from theme module
- Added forward declarations for MovieView and FilePicker
- Used std::optional and std::unique_ptr for optional/boxed types
