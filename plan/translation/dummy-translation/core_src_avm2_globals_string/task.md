# Task: Translate core/src/avm2/globals/string.rs

## Description
Translate the Rust file `core/src/avm2/globals/string.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/string.rs
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
- Translated String builtin/prototype functions for AVM2
- Implemented string_constructor() and call_handler() for String construction
- Implemented get_length() for String.length property
- Implemented char_at(), char_code_at() for character access
- Implemented concat() for string concatenation
- Implemented from_char_code() for creating strings from char codes
- Implemented index_of(), last_index_of() for string searching
- Implemented locale_compare() for locale-aware comparison
- Implemented match_internal() for regex matching
- Implemented replace() for string replacement
- Implemented search() for regex searching
- Implemented slice(), split(), substr(), substring() for string slicing
- Implemented to_lower_case(), to_upper_case() for case conversion
- Implemented string_index() and string_wrapping_index() helper functions
- Used std::vector for argument lists
- Template pattern for GC types
- Added proper forward declarations for all AVM2 types
