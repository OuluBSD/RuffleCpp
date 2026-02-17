# Task: Translate core/src/avm1/globals/date.rs

## Description
Translate the Rust file `core/src/avm1/globals/date.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/date.rs
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
- Translated Date class implementing ECMA-262 Date specification
- Implemented time calculations: year(), month(), date(), hours(), minutes(), seconds(), milliseconds()
- Implemented timezone handling: local(), utc(), timezone_offset(), local_tza()
- Implemented ECMA-262 static methods: make_time(), make_day(), make_date(), day_from_year()
- Implemented date_constructor() for new Date() calls
- Implemented date_function() for Date() without new (returns string)
- Implemented date_utc() for Date.UTC()
- Implemented date_method_handler() for all Date prototype methods
- Added date_to_string() helper for formatting
- Implemented leap year detection and month offset calculations
- Template pattern for GC types
