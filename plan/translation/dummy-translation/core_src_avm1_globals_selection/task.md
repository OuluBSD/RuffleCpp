# Task: Translate core/src/avm1/globals/selection.rs

## Description
Translate the Rust file `core/src/avm1/globals/selection.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/selection.rs
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
- Translated Selection object with OBJECT_DECLS for all methods
- Implemented create function for Selection singleton with broadcaster initialization
- Translated get_begin_index, get_end_index, get_caret_index methods for text selection
- Implemented get_focus method to return focused display object path
- Implemented set_focus method to set focus to display object
- Translated set_selection method to set selection range in text field
- Used forward declarations for EditText, InteractiveObject, TextSelection types
- Maintained Flash Selection object behavior for text field selection management
