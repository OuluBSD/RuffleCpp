# Task: Translate core/src/avm1/runtime.rs

## Description
Translate the Rust file `core/src/avm1/runtime.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/runtime.rs
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
- Translated GlobalEnv class for AVM1 global environment (case-sensitive and case-insensitive)
- Implemented global_scope_, prototypes_, broadcaster_functions_, constructor_registry_ fields
- Translated Avm1 class for AVM1 runtime state
- Implemented player_version_, constant_pool_, env_case_sensitive_, env_case_insensitive_ fields
- Implemented display_properties_, stack_, registers_, halted_, max_recursion_depth_ fields
- Implemented has_mouse_listener_, clip_exec_list_, use_new_invalid_bounds_value_ fields
- Implemented constructor Avm1(StringContext*, player_version)
- Implemented run_stack_frame_for_action() for timeline scope execution
- Implemented run_with_stack_frame_for_display_object() template for initializer scope
- Implemented run_stack_frame_for_init_action() for init action execution
- Implemented run_stack_frame_for_method() for method execution
- Implemented notify_system_listeners() for system event notification
- Implemented has_mouse_listener(), halt(), stack_len(), clear(), push(), pop() methods
- Implemented is_case_sensitive() static constexpr method
- Implemented global_scope(), prototypes(), constant_pool(), set_constant_pool() accessors
- Implemented display_properties(), max_recursion_depth(), set_max_recursion_depth() methods
- Implemented broadcaster_functions(), player_version(), get_register(), get_register_mut() accessors
- Implemented run_frame() static method for frame execution
- Implemented add_to_exec_list() for movie clip execution list management
- Implemented get_registered_constructor(), register_constructor() for constructor registry
- Implemented get_use_new_invalid_bounds_value(), activate_use_new_invalid_bounds_value() methods
- Implemented debug output methods with AVM_DEBUG feature gate
- Implemented find_display_objects_pending_removal() and remove_pending() helper methods
- Implemented root_error_handler() for error handling
- Used template pattern for GC types
- Added proper forward declarations for all AVM1 types
