# Task: Translate desktop/src/backends/external_interface.rs

## Description
Translate the Rust file `desktop/src/backends/external_interface.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/backends/external_interface.rs
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
- Translated DesktopExternalInterfaceProvider class implementing ExternalInterfaceProvider trait
- Implemented is_location_href() helper function for detecting location.href references
- Added spoof_url_ field for URL spoofing support
- Implemented call_method() override for handling ExternalInterface calls
- Implemented on_callback_available() and get_id() stub methods
- Used std::optional for optional spoof URL
- Added proper forward declarations for core types
