# Task: Translate desktop/src/gui/dialogs/preferences_dialog.rs

## Description
Translate the Rust file `desktop/src/gui/dialogs/preferences_dialog.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/gui/dialogs/preferences_dialog.rs
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
- Translated PreferencesDialog class for preferences UI
- Implemented show() for displaying the preferences dialog with egui
- Implemented restart_required() for checking if restart is needed
- Implemented save() for persisting preference changes
- Implemented show_graphics_preferences() for graphics backend and power settings
- Implemented show_language_preferences() for language selection
- Implemented show_theme_preferences() for theme selection
- Implemented show_gamemode_preferences() for Linux gamemode setting
- Implemented show_open_url_mode_preferences() for URL opening behavior
- Implemented show_ime_preferences() for IME input method setting
- Implemented show_audio_preferences() for audio output device selection
- Implemented show_video_preferences() for OpenH264 codec settings
- Implemented show_log_preferences() for log filename pattern
- Implemented show_storage_preferences() for storage backend selection
- Implemented show_misc_preferences() for recent files limit
- Added helper functions for displaying preference names
- Used egui for immediate mode GUI
- Used unic_langid for language identification
