# Task: Translate core/src/display_object/interactive.rs

## Description
Translate the Rust file `core/src/display_object/interactive.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/display_object/interactive.rs
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
- Translated InteractiveObjectFlags bitflags enum (MouseEnabled, DoubleClickEnabled, HasFocus)
- Translated InteractiveObjectBase template class with base, context_menu, tab_index, flags fields
- Implemented contains_flag() and set_flag() for flag manipulation
- Implemented all property getters/setters: context_menu, tab_index, tab_enabled, focus_rect
- Translated InteractiveObject variant class using std::variant for type union
- Implemented type checking: is_stage(), is_avm1_button(), is_avm2_button(), is_movie_clip(), is_edit_text(), is_loader_display()
- Implemented variant getters: as_stage(), as_avm1_button(), as_avm2_button(), as_movie_clip(), as_edit_text(), as_loader_display()
- Translated TInteractiveObject template trait as abstract base class
- Implemented raw_interactive() and as_displayobject() pure virtual methods
- Implemented mouse_enabled(), double_click_enabled(), has_focus() property accessors
- Implemented focus_rect() for focus highlight control
- Implemented filter_clip_event(), propagate_to_children(), event_dispatch() pure virtual methods
- Implemented event_dispatch_to_avm2() for AVM2 event conversion
- Implemented handle_clip_event() for event propagation chain
- Implemented mouse_pick_avm1() and mouse_pick_avm2() for hit testing
- Implemented mouse_cursor(), is_focusable(), is_focusable_by_mouse() for input handling
- Implemented on_focus_changed(), drop_focus(), call_focus_handler() for focus management
- Implemented is_highlightable(), is_highlight_enabled(), highlight_bounds() for focus rendering
- Implemented is_tabbable(), tab_enabled(), set_tab_enabled_value(), tab_index(), set_tab_index_value() for tab ordering
- Used std::optional for optional values
- Added proper forward declarations for all AVM, display_object, and backend types
