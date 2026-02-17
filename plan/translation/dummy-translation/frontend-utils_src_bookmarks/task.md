# Task: Translate frontend-utils/src/bookmarks.rs

## Description
Translate the Rust file `frontend-utils/src/bookmarks.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/frontend-utils/src/bookmarks.rs
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
- Translated Bookmark struct with content_descriptor and name fields
- Implemented is_invalid() method to check if URL matches invalid URL constant
- Created Bookmarks type alias as std::vector<Bookmark>
- Forward declared read_bookmarks() function from read submodule
- Forward declared BookmarksWriter class from write submodule
- Added proper forward declarations for ContentDescriptor
