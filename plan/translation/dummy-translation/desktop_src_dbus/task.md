# Task: Translate desktop/src/dbus.rs

## Description
Translate the Rust file `desktop/src/dbus.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/dbus.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated FreedesktopSettings class for freedesktop.org settings access
- Translated ColorScheme enum with NoPreference, Light, and Dark variants
- Translated GameModeGuard RAII guard for GameMode session management
- Translated GameModeSession class with shared ownership pattern
- Used std::optional for Result type representation
- Added Linux-specific #ifdef guard

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
