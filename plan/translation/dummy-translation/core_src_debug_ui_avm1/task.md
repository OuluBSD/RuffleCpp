# Task: Translate core/src/debug_ui/avm1.rs

## Description
Translate the Rust file `core/src/debug_ui/avm1.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/debug_ui/avm1.rs
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
- Translated Avm1ObjectWindow class for AVM1 object inspection
- Implemented Panel enum for Object and NativeObject panels
- Translated show_object_panel() for displaying object properties
- Translated show_avm1_value() for inspecting and editing AVM1 values
- Translated num_edit_ui() and string_edit_ui() for value editing
- Translated show_style_sheet_panel() for style sheet inspection
- Translated show_value_type_combo_box() for type selection
- Translated value_label() for type name display
- Translated UiExt trait extension for edit/set/cancel buttons
- Translated object_name() helper function
- Added icon constants for UI buttons
- Used std::optional for optional values
- Added proper forward declarations for all AVM1 types
