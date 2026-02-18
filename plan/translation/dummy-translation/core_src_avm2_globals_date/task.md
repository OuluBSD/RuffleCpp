# Task: Translate core/src/avm2/globals/date.rs

## Description
Translate the Rust file `core/src/avm2/globals/date.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/date.rs
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
- Translated all Date prototype methods for AVM2
- Implemented DateAdjustment template class for date field adjustments
- Implemented init_custom_prototype() for Date prototype initialization
- Implemented init() for Date instance constructor
- Implemented call_handler() for Date function call
- Implemented get_time() and set_time() for timestamp access
- Implemented get_milliseconds() and set_milliseconds() for milliseconds
- Implemented get_seconds() and set_seconds() for seconds
- Implemented get_minutes() and set_minutes() for minutes
- Implemented get_hours() and set_hours() for hours
- Implemented get_date() and set_date() for day of month
- Implemented get_month() and set_month() for month
- Implemented get_full_year() and set_full_year() for year
- Implemented get_day() for day of week
- Implemented UTC variants: get_utc_*() and set_utc_*() for all fields
- Implemented get_timezone_offset() for timezone offset
- Implemented utc() class method for UTC timestamp creation
- Implemented to_string(), to_utc_string(), to_locale_string() for formatting
- Implemented to_time_string(), to_locale_time_string() for time formatting
- Implemented to_date_string() for date formatting
- Implemented parse() class method for date string parsing
- Implemented parse_date(), parse_mon(), parse_hms() helper functions
- Implemented parse_full_date() for full date parsing
- Implemented get_arguments_array() helper for argument handling
- Used std::chrono for date/time handling
- Template pattern for GC types
- Added proper forward declarations for all AVM2 types
