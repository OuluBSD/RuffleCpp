# Task: Translate wstr/src/buf.rs

## Description
Translate the Rust file `wstr/src/buf.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/wstr/src/buf.rs
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
- Translated WString class for owned, extensible UCS2 string management
- Implemented WSTR_MAX_LEN constant (0x7FFFFFFF)
- Added factory methods: from_buf(), from_wstr(), from_utf8(), from_utf8_owned(), from_utf8_bytes(), from_unit(), from_char()
- Implemented as_wstr(), as_wstr_mut() accessors
- Implemented into_buf(), into_raw_parts(), from_raw_parts() for buffer management
- Implemented clear(), push(), push_byte(), push_char() for modification
- Implemented push_utf8(), push_utf8_bytes(), push_str() for string appending
- Added len(), is_empty(), capacity(), reserve(), shrink_to_fit() utility methods
- Implemented is_wide(), units(), units_mut() for buffer access
- Added copy/move constructors and assignment operators
- Used void* for type-erased data pointer (matching Rust NonNull)
- Maintained WStrMetadata for length and wideness tracking
