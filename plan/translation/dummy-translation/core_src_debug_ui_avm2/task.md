# Task: Translate core/src/debug_ui/avm2.rs

## Description
Translate the Rust file `core/src/debug_ui/avm2.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/debug_ui/avm2.rs
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
- Translated Avm2ObjectWindow class for AVM2 object inspection
- Implemented Panel enum for Information, Properties, Elements, Class, StyleSheet panels
- Translated ValueResultWidget for displaying value results
- Implemented show_information() panel with object metadata
- Implemented show_properties() panel with property table
- Implemented show_elements() panel for array inspection
- Implemented show_class() panel for class information
- Implemented show_style_sheet() panel for style sheet inspection
- Implemented show_property() for individual property display
- Translated show_avm2_value() helper function
- Translated object_name() helper function
- Used std::unordered_map for getter value caching
- Used std::optional for optional values
- Added proper forward declarations for all AVM2 types
