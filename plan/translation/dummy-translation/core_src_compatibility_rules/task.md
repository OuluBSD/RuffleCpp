# Task: Translate core/src/compatibility_rules.rs

## Description
Translate the Rust file `core/src/compatibility_rules.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/compatibility_rules.rs
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
- Translated UrlRewriteStage enum for rewrite timing
- Translated UrlRewriteRule struct for URL rewriting rules
- Translated UrlBlockRule struct for URL blocking rules
- Translated RuleSet struct for grouping rules
- Translated CompatibilityRules class for managing rule sets
- Implemented empty() and builtin_rules() constructors
- Implemented block_or_rewrite_swf_url() for URL processing
- Implemented domain_matches() helper function for domain pattern matching
- Added built-in rules for kongregate, fpdownload, and mochiads
- Template pattern for GC types
