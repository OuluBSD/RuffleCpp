# Task: Translate desktop/src/gui/dialogs/bookmarks_dialog.rs

## Description
Translate the Rust file `desktop/src/gui/dialogs/bookmarks_dialog.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/gui/dialogs/bookmarks_dialog.rs
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
- Translated BookmarkAddDialog class for adding new bookmarks
- Translated SelectedBookmark struct for holding selected bookmark data during editing
- Translated BookmarksDialog class for managing bookmarks list
- Implemented show_bookmark_table() private method for displaying bookmark table
- Implemented show_bookmark_panel() private method for editing selected bookmark
- Uses egui and egui_extras for GUI rendering with localization support
- Added proper forward declarations for egui, unic_langid, and ruffle types
- Maintains integration with GlobalPreferences for bookmark persistence
