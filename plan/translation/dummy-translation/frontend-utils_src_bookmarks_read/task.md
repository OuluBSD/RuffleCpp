# Task: Translate frontend-utils/src/bookmarks/read.rs

## Description
Translate the Rust file `frontend-utils/src/bookmarks/read.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/frontend-utils/src/bookmarks/read.rs
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
- Translated read_bookmarks() function for parsing bookmarks from TOML
- Implemented INVALID_URL constant for fallback invalid URLs
- Added parse::ParseDetails return type for parsed results with warnings
- Used DocumentMut for TOML document parsing
- Implemented bookmark array parsing with error recovery
- Added URL parsing with fallback to INVALID_URL on failure
- Added name fallback using url_to_readable_name() for missing names
- Included conditional compilation for fs feature (root_content_path)
- Added forward declarations for Bookmark, Bookmarks, ContentDescriptor, ParseDetails
