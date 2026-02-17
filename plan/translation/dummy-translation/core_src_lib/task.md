# Task: Translate core/src/lib.rs

## Description
Translate the Rust file `core/src/lib.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/lib.rs
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
- Translated core/src/lib.rs module declarations to C++ header
- Created forward declarations for all core modules: avm1, avm2, bitmap, character, context, etc.
- Re-exported StageAlign, StageDisplayState, StageScaleMode from display_object module
- Declared all major classes: AvmRng, BinaryData, Character, UpdateContext, StringContext, etc.
- Declared enum classes: PlayerMode, LoadBehavior, Message
- Template declarations for GC-managed types: Library, Loader, NetConnection, Socket, Timer, etc.
- Re-exported types from other modules: ContextMenuItem, PlayerEvent, LoadBehavior, Player, etc.
- Re-exported ViewportDimensions from render module
- Re-exported Color from swf module
- Defined constants: NEWEST_PLAYER_VERSION (51), DEFAULT_PLAYER_VERSION (32)
- Added feature gate for debug_ui (FEATURE_EGUI)
- Used proper include guards and forward declarations
