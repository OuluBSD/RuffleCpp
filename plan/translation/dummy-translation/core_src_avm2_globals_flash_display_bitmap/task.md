# Task: Translate core/src/avm2/globals/flash/display/bitmap.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/display/bitmap.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/display/bitmap.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated flash.display.Bitmap builtin/prototype functions
- Implemented bitmap_allocator() for creating Bitmap display objects
- Allocator handles both direct Bitmap and subclass instantiation
- Subclasses with registered symbols create bitmap from character data
- Implemented init() method called from constructor
- init() accepts bitmapData, pixelSnapping, and smoothing parameters
- pixelSnapping validates "always", "auto", "never" values
- Implemented get_bitmap_data(), set_bitmap_data() accessors
- set_bitmap_data creates dummy BitmapData when null is passed
- Implemented get_pixel_snapping(), set_pixel_snapping() accessors
- Implemented get_smoothing(), set_smoothing() accessors
- Used PixelSnapping enum for pixel snapping modes
- Used fill_bitmap_data_from_symbol() from bitmap_data module
- Used initialize_for_allocator() for display object setup
- Throws Error #2008 for invalid pixelSnapping values
