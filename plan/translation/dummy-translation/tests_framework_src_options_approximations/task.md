# Task: Translate tests/framework/src/options/approximations.rs

## Description
Translate the Rust file `tests/framework/src/options/approximations.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/tests/framework/src/options/approximations.rs
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
- Created `tests/framework/src/options/approximations.h` (68 lines Rust -> ~150 lines C++)
- Translated Approximations class for floating point comparison configuration
- Implemented compare() method with epsilon and max_relative support
- Implemented validate() method for configuration validation
- Implemented compiled_number_patterns() for regex pattern compilation
- Used std::optional for optional epsilon and max_relative values
- Used std::vector for number pattern storage
- Used std::regex for compiled regex patterns
- Implemented relative_eq() helper function for floating point comparison
- Added serde-like default function for bare_numbers field
