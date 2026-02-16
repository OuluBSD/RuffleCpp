# Task: Translate core/src/avm2/namespace.rs

## Description
Translate the Rust file `core/src/avm2/namespace.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/namespace.rs
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
- Translated Namespace template class for AVM2 namespace representation
- Implemented NamespaceData template struct with variant for different namespace types
- Namespace variants: Namespace, PackageInternal, Protected, Explicit, StaticProtected, Private
- Added Namespace::any() static method for wildcard namespace
- Implemented package() and internal() factory methods
- Added predicate methods: is_public(), is_any(), is_private(), is_namespace()
- Implemented as_uri_opt() and as_uri() for namespace URI access
- Added exact_version_match() and matches_ns() for namespace comparison
- Translated CommonNamespaces template class for common AVM namespace management
- Used std::shared_ptr for GC-managed data simulation
