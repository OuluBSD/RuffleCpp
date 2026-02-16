# Task: Translate wstr/src/ops.rs

## Description
Translate the Rust file `wstr/src/ops.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/wstr/src/ops.rs
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
- Translated Iter class for WStr code unit iteration
- Implemented Chars class for UTF-16 decoded character iteration
- Implemented CharIndices class for character iteration with indices
- Implemented str_iter(), str_char_indices() factory functions
- Implemented str_fmt(), str_debug_fmt() formatting functions
- Implemented str_eq(), str_eq_ignore_case() equality comparison
- Implemented str_cmp(), str_cmp_ignore_case() lexicographic comparison
- Implemented str_hash() for hashing WStr
- Implemented str_offset_in() for offset calculation
- Implemented str_to_ascii_lowercase(), str_make_ascii_lowercase() case conversion
- Implemented str_to_ascii_uppercase(), str_make_ascii_uppercase() case conversion
- Implemented str_is_latin1() for character set checking
- Implemented str_join() for joining strings with separator
- Implemented str_repeat() for string repetition
- Implemented str_replace(), str_find(), str_rfind() for pattern operations
- Implemented Split class and str_split() for string splitting
- Implemented str_split_once(), str_rsplit_once() for single split operations
- Implemented starts_with(), ends_with() for prefix/suffix checking
- Implemented strip_prefix(), strip_suffix() for prefix/suffix removal
- Implemented str_trim_matches(), str_trim_start_matches(), str_trim_end_matches() for trimming
- Used std::strong_ordering for comparison results
