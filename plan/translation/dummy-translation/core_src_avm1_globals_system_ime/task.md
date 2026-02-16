# Task: Translate core/src/avm1/globals/system_ime.rs

## Description
Translate the Rust file `core/src/avm1/globals/system_ime.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/system_ime.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated System.IME object creation with broadcaster support
- Implemented conversion mode constants (UNKNOWN, KOREAN, JAPANESE_*, CHINESE, ALPHANUMERIC_*)
- Translated all IME methods: getEnabled, setEnabled, getConversionMode, setConversionMode
- Added setCompositionString, doConversion, onIMEComposition methods
- Used template pattern for GC types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
