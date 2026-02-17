# Task: Translate core/src/avm2/globals/toplevel.rs

## Description
Translate the Rust file `core/src/avm2/globals/toplevel.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/toplevel.rs
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
- Translated global scope built-in functions for AVM2
- Implemented trace() for debug output
- Implemented is_finite() for finite number checking
- Implemented is_na_n() for NaN checking
- Implemented parse_int() for integer parsing
- Implemented parse_float() for floating-point parsing
- Implemented is_xml_name() for XML name validation
- Implemented escape() and unescape() for string encoding
- Implemented encode_uri() and encode_uri_component() for URI encoding
- Implemented decode_uri() and decode_uri_component() for URI decoding
- Added proper forward declarations for AVM2 types
