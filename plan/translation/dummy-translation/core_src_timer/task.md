# Task: Translate core/src/timer.rs

## Description
Translate the Rust file `core/src/timer.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/timer.rs
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
- Translated TimerCallback template struct with Avm1Function, Avm1Method, Avm2Callback variants using std::variant
- Implemented Timer template struct with id, callback, tick_time, interval, and is_timeout fields
- Implemented Timers template class with priority queue storage for min-heap timer ordering
- Added TimerComparator for proper min-heap ordering (earliest tick_time first)
- Implemented add_timer(), remove(), remove_all(), set_delay() methods
- Added update_timers() static method for ticking all timers during frame loop
- Included constants: MIN_INTERVAL (10), MAX_TICKS (10), TIMER_SCALE (1000.0)
- Used std::priority_queue with custom comparator for efficient timer management
- Added forward declarations for AVM1/AVM2 types and DisplayObject
