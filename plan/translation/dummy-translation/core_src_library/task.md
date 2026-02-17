# Task: Translate core/src/library.rs

## Description
Translate the Rust file `core/src/library.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/library.rs
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
- Translated Avm2ClassRegistry template class for AVM2 class-to-symbol mappings
- Implemented class_symbol() and set_class_symbol() methods
- Translated MovieLibrary template class for single SWF symbol library
- Implemented register_character(), register_export(), register_import() methods
- Implemented character lookup: contains_character(), character_by_id(), character_by_export_name()
- Implemented instantiate_by_id() and instantiate_by_export_name() for display object creation
- Implemented get_font(), get_graphic(), get_morph_shape(), get_sound(), get_text() accessors
- Implemented jpeg_tables management with set_jpeg_tables() and jpeg_tables()
- Implemented AVM2 domain management with set_avm2_domain(), avm2_domain(), try_avm2_domain()
- Translated MovieLibrarySource for bitmap source interface
- Translated Library template class for multi-movie symbol management
- Implemented library_for_movie() and library_for_movie_mut() accessors
- Implemented default_font() for built-in font resolution (_sans, _serif, _typewriter)
- Implemented font caching with font_sort_cache_ and default_font_cache_
- Implemented set_default_font() and register_device_font() for font management
- Used std::unordered_map for character and export storage
- Used std::optional for optional values
- Used std::shared_ptr and std::weak_ptr for movie references
- Added proper forward declarations for all AVM, display_object, and backend types
