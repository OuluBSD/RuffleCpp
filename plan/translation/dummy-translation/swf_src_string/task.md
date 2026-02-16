# Task: Translate swf/src/string.rs

## Description
Translate the Rust file `swf/src/string.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/swf/src/string.rs
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
- Translated SwfStr class for SWF string handling
- Implemented SwfEncoding struct with UTF_8, WINDOWS_1252, SHIFT_JIS constants
- Added factory methods: from_bytes(), from_bytes_null_terminated(), from_utf8_str(), from_utf8_str_view()
- Implemented encoding_for_version() static method (UTF-8 for SWF6+, WINDOWS-1252 for older)
- Added accessor methods: as_bytes(), is_empty(), len()
- Implemented to_str_lossy() and to_string_lossy() for encoding conversion
- Used std::string_view for efficient string reference handling
