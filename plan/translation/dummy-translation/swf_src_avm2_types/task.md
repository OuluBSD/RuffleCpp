# Task: Translate swf/src/avm2/types.rs

## Description
Translate the Rust file `swf/src/avm2/types.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/swf/src/avm2/types.rs
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
- Implemented Index<T> template class for type-safe constant pool indexing
- Translated Namespace class with 7 namespace kinds
- Implemented NamespaceSet type alias
- Translated Multiname class with 11 multiname kinds (QName, QNameA, RTQName, RTQNameA, RTQNameL, RTQNameLA, Multiname, MultinameA, MultinameL, MultinameLA, TypeName)
- Implemented MethodFlags constants
- Translated Method, MethodParam structs
- Implemented DefaultValue class with 15 default value kinds
- Translated ConstantPool, Metadata, MetadataItem structs
- Implemented Exception struct for exception handlers
- Translated TraitKind class with 7 trait kinds (Slot, Method, Getter, Setter, Class, Function, Const)
- Translated Trait, Instance, Class, Script structs
- Implemented MethodBody struct with code and exceptions
- Translated LookupSwitch struct
- Implemented Op class with 169 AVM2 opcode variants
- Translated AbcFile struct for complete ABC file representation
- Used std::optional for optional data storage
- Added static factory methods for complex types
