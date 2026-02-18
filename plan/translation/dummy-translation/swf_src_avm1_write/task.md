# Task: Translate swf/src/avm1/write.rs

## Description
Translate the Rust file `swf/src/avm1/write.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/swf/src/avm1/write.rs
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
- Translated Writer class template for AVM1 action writing
- Implemented write_action() for writing AVM1 actions to output stream
- Implemented write_action_header() for writing action headers with opcode and length
- Implemented write_opcode_and_length() for writing opcodes with length
- Implemented write_small_action() for actions without payload
- Implemented write_constant_pool() for constant pool actions
- Implemented write_define_function() and write_define_function_2() for function definitions
- Implemented write_get_url() and write_get_url_2() for URL actions
- Implemented write_goto_frame() and write_goto_frame_2() for frame navigation
- Implemented write_goto_label() for label navigation
- Implemented write_if() and write_jump() for control flow
- Implemented write_push() and write_push_value() for stack operations
- Implemented write_set_target() for target setting
- Implemented write_store_register() for register storage
- Implemented write_try() for try-catch blocks
- Implemented write_wait_for_frame() and write_wait_for_frame_2() for frame waiting
- Implemented write_with() for with blocks
- Implemented write_unknown() for unknown actions
- Used C++20 concepts for SwfWriteExt trait
- Used std::rotl and std::bit_cast for f64 special encoding
