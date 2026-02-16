# Task: Translate flv/src/script.rs

## Description
Translate the Rust file `flv/src/script.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/flv/src/script.rs
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
- Translated parse_string() helper function for reading u16/u32 length-prefixed strings
- Implemented Value struct with Number, Boolean, String, Object, MovieClip, Null, Undefined, Reference, EcmaArray, StrictArray, Date, LongString variants using std::variant
- Added parse() static method for Value to read script values from FlvReader
- Implemented Variable struct with name (byte array) and data (Value) fields
- Implemented ScriptData struct containing vector of Variable objects
- Added parse() static method for ScriptData with terminator handling
- Used std::vector<uint8_t> for byte arrays and string data
