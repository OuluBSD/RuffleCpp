# Task: Translate core/src/avm2/regexp.rs

## Description
Translate the Rust file `core/src/avm2/regexp.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/regexp.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated RegExpFlags bitfield enum with Global, IgnoreCase, Multiline, DotAll, Extended flags
- Translated CachedText class for UTF-8/UTF-16 index mapping
- Translated RegExp class with source, flags, last_index properties
- Implemented test(), exec(), split() methods
- Implemented replace_fn() and replace_string() static methods
- Added find_utf16_match() and find_utf8_match_at() helper methods
- Used regress::Regex placeholder for regex engine

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
