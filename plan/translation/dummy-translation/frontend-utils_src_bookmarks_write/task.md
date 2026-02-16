# Task: Translate frontend-utils/src/bookmarks/write.rs

## Description
Translate the Rust file `frontend-utils/src/bookmarks/write.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/frontend-utils/src/bookmarks/write.rs
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
- Translated BookmarksWriter class for TOML bookmark serialization
- Implemented with_underlying_table() helper for TOML array editing
- Implemented with_bookmark_table() helper for individual bookmark editing
- Implemented add() method for adding new bookmarks with duplicate detection
- Implemented set_content_descriptor() method for updating bookmark URLs
- Implemented set_name() method for updating bookmark names
- Implemented remove() method for deleting bookmarks
- Used parse::DocumentHolder<Bookmarks> for document management
- Added forward declarations for Bookmark, Bookmarks, ContentDescriptor types
- Maintained move-to-top behavior for existing entries
