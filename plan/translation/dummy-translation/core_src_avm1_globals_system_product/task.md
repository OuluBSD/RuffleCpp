# Task: Translate core/src/avm1/globals/system_product.rs

## Description
Translate the Rust file `core/src/avm1/globals/system_product.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/system_product.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated System.Product class with PROTO_DECLS
- Implemented create_class function returning SystemClass
- Added constructor that sets 'name' property on instance
- Translated all stub methods: is_running, is_installed, launch, download, validate, installed_version
- All methods return appropriate stub values (false for boolean, Undefined for void)

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
