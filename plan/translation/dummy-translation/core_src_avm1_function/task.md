# Task: Translate core/src/avm1/function.rs

## Description
Translate the Rust file `core/src/avm1/function.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/function.rs
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
- Translated Avm1Function template class for AVM1 bytecode functions
- Translated FunctionObject template class for callable function objects
- Translated Param struct for function parameters
- Translated ExecutionReason enum for call type identification
- Implemented from_swf_function() for creating functions from SWF data
- Implemented exec() for executing function bytecode
- Implemented load_this(), load_arguments(), load_super() for register preloading
- Implemented load_root(), load_parent(), load_global() for scope preloading
- Implemented FunctionObject::build() for creating function objects
- Implemented FunctionObject::empty(), bytecode(), native(), table_native() constructors
- Implemented FunctionObject::constructor() for native constructors
- Implemented call(), construct(), construct_on_existing() for function invocation
- Implemented exec_constructor() for constructor execution
- Used std::function for NativeFunction and TableNativeFunction types
- Used std::optional for optional values
- Template pattern for GC types
- Added proper forward declarations for all AVM1 types
