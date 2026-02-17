# Task: Translate core/src/avm1/globals/xml_node.rs

## Description
Translate the Rust file `core/src/avm1/globals/xml_node.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/xml_node.rs
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
- Translated XMLNode class with all prototype methods
- Implemented PROTO_DECLS for instance methods: cloneNode, removeNode, insertBefore, appendChild, hasChildNodes, toString, getNamespaceForPrefix, getPrefixForNamespace
- Implemented property getters/setters: attributes, childNodes, firstChild, lastChild, nodeName, nodeType, nodeValue, parentNode, previousSibling, nextSibling, prefix, localName, namespaceURI
- Translated constructor for creating XMLNode objects
- Used template pattern for GC types
- Added proper forward declarations for all AVM1 types
