# Task: Translate wstr/src/utils.rs

## Description
Translate the Rust file `wstr/src/utils.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/wstr/src/utils.rs
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
- Translated UTF-16 string utility functions for SWF string handling
- Implemented is_surrogate_pair_at() for surrogate pair detection
- Added prev_char_boundary() and next_char_boundary() for character navigation
- Implemented swf_is_whitespace() and swf_is_newline() for character classification
- Added split_ascii_prefix_bytes() and split_ascii_prefix() for ASCII prefix extraction
- Implemented utf16_code_unit_to_char() for UTF-16 to char conversion
- Added swf_to_lowercase() and swf_to_uppercase() for Flash-specific case conversion
- Implemented swf_is_ascii_hexdigit(), swf_is_cjk_like(), swf_is_opening(), swf_is_closing()
- Created DecodeAvmUtf8 iterator class for Flash's non-standard UTF-8 decoding
- Added encode_raw_utf16() for raw UTF-16 encoding
- Used binary search for case conversion table lookups
