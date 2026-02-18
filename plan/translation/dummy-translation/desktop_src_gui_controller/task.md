# Task: Translate desktop/src/gui/controller.rs

## Description
Translate the Rust file `desktop/src/gui/controller.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/gui/controller.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated GuiController class for wgpu+winit to egui integration
- Implemented constructor with window, event loop, preferences, font database, and initial movie URL
- Implemented set_theme() for theme control
- Implemented descriptors(), file_picker(), window() accessors
- Implemented resize() and reconfigure_surface() for surface management
- Implemented handle_event() for window event processing
- Implemented close_movie() and create_movie() for player lifecycle
- Implemented height_offset(), window_to_movie_position(), movie_to_window_position() for coordinate conversion
- Implemented render() for egui and movie rendering with wgpu
- Implemented show_context_menu(), is_context_menu_visible() for context menu
- Implemented needs_render() for render timing
- Implemented show_open_dialog(), open_dialog() for dialog handling
- Implemented set_ime_allowed(), set_ime_purpose(), set_ime_cursor_area() for IME support
- Implemented export_bundle() for bundle export
- Translated select_wgpu_backend() for backend selection with fallback
- Translated try_wgpu_backend() for backend instance creation
- Translated load_system_fonts() for system font loading with CJK support
- Translated register_family(), register_family_font(), load_system_font() for font registration
- Translated mmap_system_font() for memory-mapped font file loading
- Used std::shared_ptr for shared resources
- Used std::optional for optional values
- Added proper forward declarations for all wgpu, winit, egui, and ruffle types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
