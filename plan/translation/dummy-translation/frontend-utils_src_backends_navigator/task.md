# Task: Translate frontend-utils/src/backends/navigator.rs

## Description
Translate the Rust file `frontend-utils/src/backends/navigator.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/frontend-utils/src/backends/navigator.rs
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
- Translated ExternalNavigatorBackend class for non-web navigator operations
- Implemented NavigatorInterface for platform-specific navigation operations
- Implemented FutureSpawner template interface for async task spawning
- Implemented SocketMode enum with Allow, Deny, Ask variants
- Implemented navigate_to_website() for opening URLs in browser
- Implemented open_file() for filesystem file access
- Implemented confirm_socket() for sandbox socket permission prompts
- Implemented NavigatorBackend trait methods: navigate_to_url, fetch, resolve_url
- Implemented spawn_future() for async task execution
- Implemented pre_process_url() for URL preprocessing (https upgrade)
- Implemented connect_socket() for TCP socket connections with timeout handling
- Added spawn_tokio() helper for spawning futures in tokio runtime
- Template pattern for FutureSpawner and NavigatorInterface types
- Added proper forward declarations for all dependencies
