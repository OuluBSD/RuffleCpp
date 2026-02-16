# Task: Translate render/wgpu/src/target.rs

## Description
Translate the Rust file `render/wgpu/src/target.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/wgpu/src/target.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated RenderTargetFrame abstract interface for frame handling
- Created RenderTarget template interface with resize, format, width, height methods
- Translated SwapChainTargetFrame and SwapChainTarget for window rendering
- Translated TextureTarget for off-screen texture rendering
- Added TextureBufferInfo and MaybeOwnedBufferType for buffer management
- Implemented proper C++ class hierarchy with virtual methods

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
