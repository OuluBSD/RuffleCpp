# Task: Translate core/src/avm1/globals/button.rs

## Description
Translate the Rust file `core/src/avm1/globals/button.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/button.rs
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
- Translated Button class with PROTO_DECLS for all button properties
- Implemented create_class function using empty_class pattern
- Translated blend_mode getter/setter for blend mode string
- Implemented filters getter/setter for bitmap filter array
- Translated cache_as_bitmap getter/setter for bitmap caching
- Implemented scale_9_grid getter/setter for 9-grid scaling rectangle
- Translated tab_index getter/setter for tab order
- Used Avm1Button template type for button display object
- Added forward declarations for Avm1Button, InteractiveObject, BlendMode
- Maintained Flash Button object behavior for interactive button properties
