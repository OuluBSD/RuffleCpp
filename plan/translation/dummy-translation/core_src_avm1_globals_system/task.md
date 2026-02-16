# Task: Translate core/src/avm1/globals/system.rs

## Description
Translate the Rust file `core/src/avm1/globals/system.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/system.rs
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
- Translated System object for AVM1 with all property declarations
- Implemented SettingsPanel enum for showSettings method
- Translated set_clipboard() method for setting system clipboard content
- Translated show_settings() method for showing Flash Player settings dialog
- Translated useCodepage property getter/setter
- Translated exactSettings property getter/setter
- Translated on_status() stub method for status event handling
- Used template pattern for GC types
- Added proper forward declarations for Activation, Error, Object, Value, DeclContext, StaticDeclarations
