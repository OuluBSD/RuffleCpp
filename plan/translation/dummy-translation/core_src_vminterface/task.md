# Task: Translate core/src/vminterface.rs

## Description
Translate the Rust file `core/src/vminterface.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/vminterface.rs
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
- Translated Instantiator enum with Movie, Avm1, Avm2 variants
- Added instantiator_is_avm() helper function
- Translated AvmObject template class using std::variant for AVM1/AVM2 object storage
- Implemented as_avm1_object() and as_avm2_object() methods returning std::optional
- Used forward declarations for AVM1 Object and AVM2 StageObject templates
