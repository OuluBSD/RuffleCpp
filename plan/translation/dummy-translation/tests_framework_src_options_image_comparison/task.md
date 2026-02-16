# Task: Translate tests/framework/src/options/image_comparison.rs

## Description
Translate the Rust file `tests/framework/src/options/image_comparison.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/tests/framework/src/options/image_comparison.rs
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
- Translated ImageComparisonCheck struct with tolerance, max_outliers, and filter fields
- Translated ImageComparison class with optional simple checks and advanced checks vector
- Implemented checks() method returning std::expected with validation logic
- Added validation to prevent mixing simple and advanced checks
- Included ImageTrigger and known_failure fields
- Used std::optional for optional fields
- Added getter and setter methods for all fields
