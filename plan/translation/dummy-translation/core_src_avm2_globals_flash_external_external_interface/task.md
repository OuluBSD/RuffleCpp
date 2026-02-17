# Task: Translate core/src/avm2/globals/flash/external/external_interface.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/external/external_interface.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/external/external_interface.rs
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
- Translated flash.external.ExternalInterface native methods: call(), available getter, addCallback(), objectID getter
- call() invokes ExternalInterface::call_method() with converted arguments
- addCallback() registers AVM2 callbacks for JavaScript to call
- get_object_id() returns SWF's DOM object ID or null
- check_available() helper throws error 2067 if ExternalInterface unavailable
- Uses std::vector for args parameter
- Template pattern for GC types
