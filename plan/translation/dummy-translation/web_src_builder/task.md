# Task: Translate web/src/builder.rs

## Description
Translate the Rust file `web/src/builder.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/web/src/builder.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated RuffleInstanceBuilder class for web platform Ruffle instance creation
- Implemented all configuration fields: allow_script_access, background_color, letterbox, etc.
- Implemented all setter methods: set_allow_script_access(), set_background_color(), set_letterbox(), etc.
- Implemented set_stage_align() for parsing stage alignment flags from string
- Implemented set_quality() for stage quality selection (low, medium, high, best, 8x8 variants)
- Implemented set_scale() for stage scale mode selection
- Implemented set_open_url_mode() for URL mode configuration
- Implemented set_allow_networking() for networking access mode
- Implemented add_socket_proxy() for socket proxy configuration
- Implemented set_player_runtime() for runtime selection (AIR, FlashPlayer)
- Implemented add_font() for custom font registration
- Implemented set_default_font() for default font configuration
- Implemented add_gamepad_button_mapping() for gamepad mapping
- Implemented add_url_rewrite_rule() for URL rewrite rules
- Implemented set_scrolling_behavior() for scrolling behavior
- Implemented set_device_font_renderer() for font renderer selection
- Implemented setup_fonts() for loading custom and default fonts into player
- Implemented create_log_subscriber() for tracing subscriber creation
- Implemented create_renderer() for renderer backend selection with fallback
- Implemented register_ttf_face_by_name() for TTF font registration
- Used std::optional for optional values
- Used std::unordered_map for font and gamepad mappings
- Added proper forward declarations for all ruffle core and web types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
