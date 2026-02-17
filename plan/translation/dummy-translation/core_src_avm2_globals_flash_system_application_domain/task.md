# Task: Translate core/src/avm2/globals/flash/system/application_domain.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/system/application_domain.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/system/application_domain.rs
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
- Translated flash.system.ApplicationDomain class functions
- Implemented init() for ApplicationDomain constructor initialization
- Implemented get_current_domain() static property getter
- Implemented get_parent_domain() property getter (returns null for playerglobals domain)
- Implemented get_definition() method for retrieving definitions by name
- Implemented has_definition() method for checking definition existence
- Implemented get_qualified_definition_names() method (Flash Player 11.3+)
  - Filters out private namespace definitions
  - Returns Vector of qualified names
- Implemented set_domain_memory() property setter
- Implemented get_domain_memory() property getter (returns null for default memory)
- Used ParametersExt for argument handling
- Used VectorStorage for array return values
- Template pattern for GC types
- Added proper forward declarations for all dependencies
