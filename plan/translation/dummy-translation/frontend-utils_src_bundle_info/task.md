# Task: Translate frontend-utils/src/bundle/info.rs

## Description
Translate the Rust file `frontend-utils/src/bundle/info.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/frontend-utils/src/bundle/info.rs
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
- Translated BundleInformationParseError class with Kind enum for error categorization
- Implemented error variants: InvalidToml, InvalidBundleSection, InvalidName, InvalidUrl
- Added static factory methods for each error type
- Translated BundleInformation struct with name, url, and player fields
- Implemented parse() static method returning variant of ParseDetails or error
- Implemented serialize() method for writing bundle info to output stream
- Added BUNDLE_INFORMATION_FILENAME constant ("ruffle-bundle.toml")
- Used std::variant for result type representation
- Added forward declarations for PlayerOptions, DocumentHolder, ParseDetails
- Maintained TOML structure with [bundle] and [player] sections
