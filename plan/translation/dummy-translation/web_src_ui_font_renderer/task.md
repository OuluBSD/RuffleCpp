# Task: Translate web/src/ui/font_renderer.rs

## Description
Translate the Rust file `web/src/ui/font_renderer.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/web/src/ui/font_renderer.rs
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
- Translated CanvasFontRenderer class implementing FontRenderer interface
- Implemented OffscreenCanvas-based glyph rendering for web platform
- Added SIZE_PX (64.0) and SCALE (20.0) constants for font rendering
- Implemented new_renderer() static factory method with OffscreenCanvas support check
- Implemented private helper methods: is_offscreen_canvas_supported(), to_font_str(), apply_style()
- Implemented calculate_width(), ensure_canvas_large_enough() for text measurement
- Implemented render_glyph_internal() for glyph bitmap generation
- Implemented calculate_kerning_internal() for kerning calculation
- Implemented FontRenderer interface: get_font_metrics(), has_kerning_info(), render_glyph(), calculate_kerning()
- Used std::shared_ptr for canvas and context ownership
- Added forward declarations for web_sys and js_sys types
