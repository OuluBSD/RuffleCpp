# Task: Translate core/src/avm2/method.rs

## Description
Translate the Rust file `core/src/avm2/method.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/method.rs
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
- Translated NativeMethodImpl function pointer type for native methods
- Translated ResolvedParamConfig and ParamConfig structs for method parameters
- Translated MethodKind enum with Bytecode and Native variants
- Translated MethodAssociation struct for method binding
- Translated MethodData internal struct for method data
- Translated Method class with all accessors and methods
- Translated ResolvedMethodInfo for resolved signature info
- Implemented from_method_index() constructor
- Implemented verify(), resolve_info(), associate() methods
- Template pattern for GC types
- Added proper forward declarations for all dependencies
