# Task: Translate core/src/avm1/xml/tree.rs

## Description
Translate the Rust file `core/src/avm1/xml/tree.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/xml/tree.rs
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
- Translated XmlNode class representing XML tree nodes
- Implemented ELEMENT_NODE (1) and TEXT_NODE (3) constants
- Implemented new_node() and from_start_event() constructors
- Implemented parent/sibling navigation methods
- Implemented child management: insert_child, append_child, remove_node
- Implemented node properties: node_type, node_name, node_value
- Implemented script_object management for AVM1 integration
- Implemented duplicate() for deep/shallow cloning
- Implemented lookup_namespace_uri() for namespace resolution
- Implemented into_string() for XML serialization
- Used std::shared_ptr for GC-managed data in C++
