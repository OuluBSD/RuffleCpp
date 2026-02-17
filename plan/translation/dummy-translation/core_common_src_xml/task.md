# Task: Translate core/common/src/xml.rs

## Description
Translate the Rust file `core/common/src/xml.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/common/src/xml.rs
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
- Translated avm1_unescape() function for AVM1 XML entity decoding
- Translated avm2_unescape() function for AVM2 E4X XML entity decoding
- Translated custom_unescape() helper function for shared unescaping logic
- Preserved AVM1 behavior (decodes entities even when preceded by bare '&')
- Preserved AVM2 behavior (preserves entities preceded by bare '&')
- Used std::string for return type and uint8_t* for input byte arrays
