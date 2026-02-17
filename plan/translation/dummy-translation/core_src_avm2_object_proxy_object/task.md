# Task: Translate core/src/avm2/object/proxy_object.rs

## Description
Translate the Rust file `core/src/avm2/object/proxy_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/proxy_object.rs
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
- Translated ProxyObject struct representing Proxy instances
- Translated ProxyObjectWeak weak reference type
- Translated ProxyObjectData internal struct with base field
- Implemented proxy_allocator() for allocating Proxy objects
- Implemented TObject trait methods delegating to Proxy traps:
  - get_property_local() -> getProperty trap
  - set_property_local() -> setProperty trap
  - call_property_local() -> callProperty trap
  - delete_property_local() -> deleteProperty trap
  - has_property_via_in() -> hasProperty trap (in operator)
  - has_own_property_string() -> hasProperty trap
  - get_next_enumerant() -> nextNameIndex trap
  - get_enumerant_name() -> nextName trap
  - get_enumerant_value() -> nextValue trap
- Used QNameObject for property name handling
- Delegated to flash_utils_proxy methods for trap calls
- Template pattern for GC types
- Added proper forward declarations for all dependencies
