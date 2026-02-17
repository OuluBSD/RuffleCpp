# Task: Translate core/src/system_properties.rs

## Description
Translate the Rust file `core/src/system_properties.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/system_properties.rs
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
- Translated CpuArchitecture enum with PowerPc, X86, Sparc, Arm variants
- Translated OperatingSystem enum with all Windows variants, Linux, MacOS
- Translated Manufacturer enum with Windows, Macintosh, Linux, Other variants
- Implemented ManufacturerExt class with get_manufacturer_string() and get_platform_name()
- Translated Language enum with 21 language variants
- Implemented LanguageExt class with get_language_code() and from_language_identifier()
- Translated ScreenColor enum with Color, Gray, BlackWhite variants
- Translated PlayerType enum with StandAlone, External, PlugIn, ActiveX variants
- Translated SystemCapabilities class with 20 capability flags
- Translated SystemProperties class with all system properties
- Implemented new() static constructor with language identifier parsing
- Implemented get_version_string() for player version formatting
- Implemented has_capability(), encode_capability(), encode_not_capability()
- Implemented encode_string() for URL percent encoding
- Implemented get_server_string() for analytics server string generation
- Added all getters and setters for SystemProperties fields
- Used proper default values matching Flash Player behavior
