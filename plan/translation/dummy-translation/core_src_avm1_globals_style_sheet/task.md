# Task: Translate core/src/avm1/globals/style_sheet.rs

## Description
Translate the Rust file `core/src/avm1/globals/style_sheet.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/style_sheet.rs
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
- Translated StyleSheetObject template class for AVM1 style sheets
- Implemented create() static constructor for empty style sheets
- Implemented set_style() and style_sheet() accessors
- Translated PROTO_DECLS for StyleSheet prototype methods
- Translated create_class() for class creation
- Translated all native method functions:
  - constructor() for object construction
  - shallow_copy() for object copying
  - set_style() for setting style on selector
  - get_style() for getting style by selector
  - get_style_names() for listing style names
  - load() for loading CSS from URL
  - transform() for converting style object to TextFormat
  - parse_css() for parsing CSS text
  - clear() for clearing all styles
- Translated helper functions: parse_color(), parse_suffixed_number_i32(), parse_suffixed_number_f64()
- Used std::shared_ptr for StyleSheet reference
- Added proper forward declarations for all AVM1 and HTML types
