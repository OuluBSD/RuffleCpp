# Task: Translate core/src/avm2/qname.rs

## Description
Translate the Rust file `core/src/avm2/qname.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/qname.rs
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
- Translated QName struct for qualified names (namespace + local name pair)
- Implemented new() constructor
- Implemented from_abc_multiname() for pulling QName from multiname pool
- Implemented from_qualified_name() for constructing from qualified name strings
- Implemented to_qualified_name(), to_qualified_name_no_mc() for conversion
- Implemented to_qualified_name_err_message() for error message formatting
- Implemented as_uri() for getting full URI string
- Implemented local_name() and namespace() accessors
- Implemented PartialEq and Debug traits
- Template pattern for GC types
- Added Either type for returning different types without allocation
