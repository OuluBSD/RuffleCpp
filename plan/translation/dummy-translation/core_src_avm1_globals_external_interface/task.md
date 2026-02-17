# Task: Translate core/src/avm1/globals/external_interface.rs

## Description
Translate the Rust file `core/src/avm1/globals/external_interface.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/external_interface.rs
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
- Translated ExternalInterface static object with all methods
- Implemented get_available() for checking ExternalInterface availability
- Implemented add_callback() for registering ActionScript callbacks
- Implemented call() for calling JavaScript functions
- Implemented escape_xml()/unescape_xml() for XML entity handling
- Implemented js_quote_string() for JavaScript string escaping
- Implemented to_xml() and helper functions for XML serialization
- Implemented to_as() for XML to ActionScript conversion
- Implemented array_to_xml(), object_to_xml(), arguments_to_xml() helpers
- Stub implementations for _initJS, _objectID, _addCallback, _evalJS, _callOut
- Stub implementations for _callIn, _useSetReturnValueHack
- Stub implementations for _objectToAS, _arrayToAS, _toAS, _argumentsToAS
- Stub implementations for _arrayToJS, _objectToJS, _toJS
- Used std::vector for argument lists
- Template pattern for GC types
- Added proper forward declarations for all AVM1 and external types
