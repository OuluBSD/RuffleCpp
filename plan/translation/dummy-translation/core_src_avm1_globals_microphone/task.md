# Task: Translate core/src/avm1/globals/microphone.rs

## Description
Translate the Rust file `core/src/avm1/globals/microphone.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/microphone.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated Microphone class with PROTO_DECLS and OBJECT_DECLS
- Implemented create_class function using empty_class pattern
- Translated static get method returning Value::Null (no microphone available)
- Added get_names property getter returning empty array
- Translated all stub methods: setSilenceLevel, setRate, setGain, setUseEchoSuppression, setCodec, setFramesPerPacket, setEncodeQuality

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
