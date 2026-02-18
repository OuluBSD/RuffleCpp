# Task: Translate core/src/avm1/globals/array.rs

## Description
Translate the Rust file `core/src/avm1/globals/array.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/array.rs
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
- Translated SortOptions bitflags class for sort options
- Translated ArrayBuilder template class for constructing arrays
- Translated all Array methods: push, pop, shift, unshift, reverse, join, slice, splice, concat, toString, sort, sortOn
- Implemented qsort() quicksort algorithm mimicking Flash behavior
- Implemented sort_compare() and sort_on_compare() comparison functions
- Implemented sort_internal() common code for sort and sortOn
- Used std::compare_ordering for comparison results
- Template pattern for GC types
