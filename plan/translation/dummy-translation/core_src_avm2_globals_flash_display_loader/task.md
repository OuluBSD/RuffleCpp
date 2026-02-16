# Task: Translate core/src/avm2/globals/flash/display/loader.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/display/loader.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/display/loader.rs
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
- Translated flash.display.Loader builtin/prototype functions
- Implemented loader_allocator() for creating Loader display objects
- Allocator initializes contentLoaderInfo with empty SwfMovie
- contentLoaderInfo set before super() for subclass observation
- Implemented load() for loading content from URLRequest
- Unloads existing content before loading new content
- Uses LoadManager for async movie loading
- Implemented request_from_url_request() helper for URLRequest conversion
- Extracts URL, method, headers, and data from URLRequest
- Handles GET method with query string appending
- Handles POST method with payload and content type
- Supports ByteArray and String data payloads
- Empty payload forces GET method (Flash behavior)
- Implemented load_bytes() for loading from byte array
- Uses LoadManager::load_movie_into_clip_bytes() for synchronous loading
- Implemented unload() stub method
- Uses slot constants from flash_display_loader, flash_net_url_request namespaces
- Uses indexmap::IndexMap for header storage
