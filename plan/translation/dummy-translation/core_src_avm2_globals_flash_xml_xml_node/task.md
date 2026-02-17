# Task: Translate core/src/avm2/globals/flash/xml/xml_node.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/xml/xml_node.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/xml/xml_node.rs
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
- Translated flash.xml.XMLNode._escape_xml function
- Escapes special XML characters: &, <, >, ", '
- Returns escaped string if special characters found, otherwise original string
- Uses std::vector for args parameter
- Template pattern for GC types
