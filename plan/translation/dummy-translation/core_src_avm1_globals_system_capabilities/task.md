# Task: Translate core/src/avm1/globals/system_capabilities.rs

## Description
Translate the Rust file `core/src/avm1/globals/system_capabilities.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/system_capabilities.rs
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
- Translated System.capabilities object with all 36 property getters
- Implemented create() function for creating the capabilities object
- Translated all boolean capability getters (hasAccessibility, hasTLS, hasAudio, etc.)
- Translated string capability getters (playerType, screenColor, language, manufacturer, os, cpuArchitecture, version, serverString, maxLevelIDC)
- Translated numeric capability getters (pixelAspectRatio, screenDPI, screenResolutionX, screenResolutionY)
- Translated inverse capability getters (avHardwareDisable, localFileReadDisable, windowlessDisable)
- Used template pattern for GC types
- Added proper forward declarations for Activation, Error, Object, Value, AvmString, SystemCapabilities
