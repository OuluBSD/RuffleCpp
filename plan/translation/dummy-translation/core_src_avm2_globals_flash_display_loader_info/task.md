# Task: Translate core/src/avm2/globals/flash/display/loader_info.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/display/loader_info.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/display/loader_info.rs
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
- Translated flash.display.LoaderInfo getter functions
- Implemented get_action_script_version() returning 3 for AVM2, 2 for AVM1
- Implemented get_application_domain() for ApplicationDomain access
- Implemented get_bytes_total() for total byte count
- Implemented get_bytes_loaded() for loaded byte count with bitmap handling
- Implemented get_content() with expose_content() check
- Implemented get_content_type() returning MIME type strings
- Implemented get_frame_rate() for SWF frame rate
- Implemented get_height() and get_width() for dimensions in pixels
- Implemented get_is_url_inaccessible() as stub returning false
- Implemented get_same_domain() as stub returning false
- Implemented get_child_allows_parent() with URL host comparison
- Implemented get_parent_allows_child() with URL host comparison
- Implemented get_swf_version() for SWF version number
- Implemented get_url() with expose_content() check
- Implemented get_bytes() with SWF header writing and image stub
- Implemented get_loader() for Loader access
- Implemented get_loader_url() for loader URL
- Implemented get_parameters() building parameters object
- Implemented get_shared_events() for SharedEventDispatcher
- Implemented get_uncaught_error_events() for UncaughtErrorEvents
- Used std::optional for optional values
- Used std::vector for parameter building
- Added proper forward declarations for all AVM2 and display_object types
