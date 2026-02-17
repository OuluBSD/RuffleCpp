# Task: Translate core/src/focus_tracker.rs

## Description
Translate the Rust file `core/src/focus_tracker.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/focus_tracker.rs
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
- Translated Highlight enum for focus highlight states
- Translated FocusTrackerData internal struct
- Translated FocusTracker class for managing focus state
- Implemented set(), set_by_mouse(), set_by_key() methods
- Implemented tab_order() and cycle() for keyboard navigation
- Implemented navigate() for directional navigation
- Translated TabOrder class for tab ordering
- Translated TabOrdering trait with CustomTabOrdering and AutomaticTabOrdering
- Translated NavigationDirection and NavigationOrdering for keyboard navigation
- Implemented render_highlight() for drawing focus outline
- Template pattern for GC types
- Added proper forward declarations for all dependencies
