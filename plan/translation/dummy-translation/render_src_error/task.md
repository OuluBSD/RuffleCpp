# Task: Translate render/src/error.rs

## Description
Translate the Rust file `render/src/error.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/src/error.rs
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
- Translated Error class with Kind enum for error categorization
- Implemented all error variants: TooLarge, UnknownType, InvalidZlibCompression, InvalidJpeg, InvalidPng, InvalidGif, EmptyGif, UnsupportedLosslessFormat, UnknownHandle, Unimplemented
- Added JavascriptError variant for web/Emscripten builds (conditionally compiled)
- Used std::variant for error data storage (type-safe union)
- Implemented static factory methods for each error type
- Added to_string() method for error message formatting
- Implemented stream output operator for easy logging
- Used forward declarations for BitmapHandle and BitmapFormat
