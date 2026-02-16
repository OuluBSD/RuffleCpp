# Task: Translate swf/src/avm1/types.rs

## Description
Translate the Rust file `swf/src/avm1/types.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/swf/src/avm1/types.rs
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
- Translated Action enum with 96 AVM1 action variants
- Implemented ConstantPool, DefineFunction, DefineFunction2 structs
- Implemented FunctionParam, FunctionFlags for function definitions
- Translated GetUrl, GetUrl2, GetUrl2Flags for URL operations
- Implemented SendVarsMethod enum (None, Get, Post)
- Translated GotoFrame, GotoFrame2, GotoLabel, If, Jump structs
- Implemented Value class with 9 value types (Undefined, Null, Bool, Int, Float, Double, Str, Register, ConstantPool)
- Translated Push, SetTarget, StoreRegister, Try, CatchVar structs
- Implemented TryFlags constants
- Translated WaitForFrame, WaitForFrame2, With, Unknown structs
- Used std::optional for optional data storage
- Added static factory methods for each Action variant
