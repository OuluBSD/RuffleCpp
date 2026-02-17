# Task: Translate desktop/src/gui/locale.rs

## Description
Translate the Rust file `desktop/src/gui/locale.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/gui/locale.rs
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
- Translated localization functions: text(), optional_text(), available_languages(), text_with_args()
- Translated reorder_bidi() and mirror_char() helper functions for RTL text support
- Translated LocalizableText class for text that may need localization
- Used forward declarations for unic_langid and fluent_bundle types
- Maintained original function signatures and documentation
