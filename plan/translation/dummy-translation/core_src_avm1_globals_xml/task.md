# Task: Translate core/src/avm1/globals/xml.rs

## Description
Translate the Rust file `core/src/avm1/globals/xml.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/xml.rs
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
- Translated XML class for AVM1 with all prototype methods
- Implemented XmlStatus enum for parse error codes (NoError, DeclNotTerminated, ElementMalformed, OutOfMemory, AttributeNotTerminated, MismatchedEnd)
- Implemented PROTO_DECLS for methods: createElement, createTextNode, parseXML, load, sendAndLoad, onData, getBytesLoaded, getBytesTotal
- Implemented property getters: contentType, docTypeDecl, ignoreWhite, status, xmlDecl, idMap
- Translated constructor for creating XML documents from strings
- Implemented spawn_xml_fetch helper for async XML loading
- Used template pattern for GC types
- Added proper forward declarations for all AVM1 types
