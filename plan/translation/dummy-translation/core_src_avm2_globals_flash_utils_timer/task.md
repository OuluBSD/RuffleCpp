# Task: Translate core/src/avm2/globals/flash/utils/timer.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/utils/timer.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/utils/timer.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated flash.utils.Timer native methods: stop(), start(), update_delay()
- Implemented stop() to remove timer from timer queue using TimerCallback
- Implemented start() to add/restart timer with Avm2Callback closure
- Implemented update_delay() to modify delay of running timer
- Used slot constants from flash_utils_timer namespace (_TIMER_ID, _DELAY, _ON_UPDATE_CLOSURE)
- Note: start() does not check currentCount vs repeatCount (matches Flash behavior)
- Used template pattern for GC types
- Added proper forward declarations for AVM2 types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
