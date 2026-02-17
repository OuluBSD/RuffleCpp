# Task: Translate core/src/avm2/globals/flash/text/text_format.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/text/text_format.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/text/text_format.rs
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
- Translated all TextFormat property getter/setter functions for AVM2
- Implemented get_align()/set_align() for text alignment property
- Implemented get_block_indent()/set_block_indent() for block indentation
- Implemented get_bold()/set_bold() for bold text property
- Implemented get_bullet()/set_bullet() for bullet property
- Implemented get_color()/set_color() for text color (RGBA conversion)
- Implemented get_display()/set_display() for text display mode (block/inline/none)
- Implemented get_font()/set_font() for font name property
- Implemented get_indent()/set_indent() for text indentation
- Implemented get_italic()/set_italic() for italic text property
- Implemented get_kerning()/set_kerning() for kerning property
- Implemented get_leading()/set_leading() for line leading
- Implemented get_left_margin()/set_left_margin() for left margin
- Implemented get_letter_spacing()/set_letter_spacing() for letter spacing
- Implemented get_right_margin()/set_right_margin() for right margin
- Implemented get_size()/set_size() for font size
- Implemented get_tab_stops()/set_tab_stops() for tab stops (ArrayObject conversion)
- Implemented get_target()/set_target() for text target
- Implemented get_underline()/set_underline() for underline property
- Implemented get_url()/set_url() for URL property
- All functions use template pattern for GC types
- Used Value<GC>, Activation<GC>, Error<GC>, Object<GC> types
- Used swf::TextAlign and html::TextDisplay enums
- Added proper forward declarations for all AVM2 types
- Re-exported text_format_allocator from allocator module
