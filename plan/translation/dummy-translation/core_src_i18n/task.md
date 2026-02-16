# Task: Translate core/src/i18n.rs

## Description
Translate the Rust file `core/src/i18n.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/i18n.rs
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
- Translated LanguageIdentifier struct for locale identification
- Implemented FluentValue type using std::variant for localization arguments
- Created TextArgs type alias for argument maps
- Declared core_text() function for looking up localized strings by ID
- Declared core_text_with_args() function for localized strings with arguments
- Added init_i18n() function for initializing the localization system
- Used std::string_view for efficient string parameter passing
- Added LanguageIdentifierHash for use in unordered_map/set
