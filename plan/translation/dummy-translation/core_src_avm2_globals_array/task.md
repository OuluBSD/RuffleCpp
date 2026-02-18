# Task: Translate core/src/avm2/globals/array.rs

## Description
Translate the Rust file `core/src/avm2/globals/array.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/array.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated all Array prototype methods for AVM2
- Implemented SortOptions bitflags class for sort options
- Implemented ArrayIter template class for safe array iteration with user code execution
- Implemented array_initializer() for Array construction
- Implemented get_length() and set_length() for length property
- Implemented build_array() helper for array creation
- Implemented concat() for array concatenation
- Implemented resolve_array_hole() for hole resolution
- Implemented join() for array joining with separator
- Implemented for_each(), map(), filter() for iteration methods
- Implemented every(), some() for testing methods
- Implemented index_of(), last_index_of() for search methods
- Implemented pop(), push() for stack operations
- Implemented reverse() for array reversal
- Implemented shift(), unshift() for queue operations
- Implemented resolve_index() for negative index handling
- Implemented slice(), splice() for array slicing
- Implemented sort() with custom comparator and options support
- Implemented sort_on() for field-based sorting
- Implemented remove_at() for element removal
- Implemented compare_string_case_sensitive() and compare_string_case_insensitive()
- Implemented compare_numeric() template for numeric comparison
- Implemented qsort() quicksort algorithm ported from avmplus
- Used std::vector for argument lists
- Template pattern for GC types
- Added proper forward declarations for all AVM2 types
