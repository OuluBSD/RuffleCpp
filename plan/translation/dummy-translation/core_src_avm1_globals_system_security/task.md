# Task: Translate core/src/avm1/globals/system_security.rs

## Description
Translate the Rust file `core/src/avm1/globals/system_security.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/system_security.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated System.security object with OBJECT_DECLS
- Implemented create function returning security object
- Translated all methods: allow_domain, allow_insecure_domain, load_policy_file, escape_domain
- Implemented get_sandbox_type with sandbox type string mapping (remote, localWithFile, localWithNetwork, localTrusted)
- Added stub implementations for get_choose_local_swf_path and policy_file_resolver

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
