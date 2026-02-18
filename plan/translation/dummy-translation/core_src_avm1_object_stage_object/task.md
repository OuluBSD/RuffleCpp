# Task: Translate core/src/avm1/object/stage_object.rs

## Description
Translate the Rust file `core/src/avm1/object/stage_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/object/stage_object.rs
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
- Translated get_property() for DisplayObject property lookup
- Translated notify_property_change() for text field bindings
- Translated has_display_object_property() for property existence check
- Translated enumerate_keys() for key enumeration
- Translated DisplayProperty class for property getter/setter pairs
- Translated DisplayPropertyMap class for property map
- Implemented all display property getters/setters (_x, _y, _xscale, _yscale, etc.)
- Implemented resolve_path_property() for path resolution (_root, _parent, _levelN)
- Implemented property_coerce_to_number() helper
- Implemented action_property_coerce() for SetProperty coercion
