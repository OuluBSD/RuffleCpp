# Task: Translate flv/src/video.rs

## Description
Translate the Rust file `flv/src/video.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/flv/src/video.rs
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
- Translated FrameType enum with Keyframe, Interframe, InterframeDisposable, Generated, CommandFrame variants
- Translated CodecId enum with Jpeg, SorensonH263, ScreenVideo, On2Vp6, On2Vp6Alpha, ScreenVideo2, Avc variants
- Translated CommandFrame enum with StartOfClientSideSeek and EndOfClientSideSeek
- Implemented VideoPacket struct with Data, Vp6Data, AvcSequenceHeader, AvcNalu, AvcEndOfSequence, CommandFrame variants using std::variant
- Implemented VideoData struct with frame_type, codec_id, and data fields
- Added parse() static method declaration for parsing video data from FlvReader
- Used std::vector<uint8_t> for byte arrays
