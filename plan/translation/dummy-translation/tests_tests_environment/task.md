# Task: Translate tests/tests/environment.rs

## Description
Translate the Rust file `tests/tests/environment.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/tests/tests/environment.rs
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
- Translated NativeEnvironment class for test environment
- Implemented is_render_supported() method (conditionally compiled with RUFFLE_IMGTESTS)
- Implemented create_renderer() method for creating render interface/backend pairs
- Translated renderer::NativeRenderInterface class implementing RenderInterface trait
- Implemented create_pair() static factory method for WGPU renderer creation
- Implemented name() method returning OS and backend info
- Implemented capture() method for frame capture
- Implemented is_supported() function for checking WGPU availability
- Implemented get_descriptors() singleton for caching WGPU descriptors
- Implemented create_wgpu_device() for WGPU device creation
- Implemented build_wgpu_descriptors() for descriptor construction
- Used std::optional for optional return types
- Used std::unique_ptr for ownership management
- Added proper forward declarations for all dependencies
