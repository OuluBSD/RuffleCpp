# Task: Translate wstr/src/pattern.rs

## Description
Translate the Rust file `wstr/src/pattern.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/wstr/src/pattern.rs
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
- Translated SearchStep enum class with Match, Reject, and Done variants
- Implemented Searcher template class as abstract base for pattern searching
- Added next(), next_back(), next_match(), next_match_back(), next_reject(), next_reject_back() methods
- Implemented Predicate template class for pattern matching
- Implemented ValuePredicate, AnyOfPredicate, and FnPredicate concrete classes
- Implemented PredSearcher template class for predicate-based searching
- Implemented SliceSearcher template class for slice pattern matching
- Implemented EmptySearcher class for empty pattern matching
- Implemented Either template class for holding one of two searcher types
- Used std::variant for type-safe union of searcher types
