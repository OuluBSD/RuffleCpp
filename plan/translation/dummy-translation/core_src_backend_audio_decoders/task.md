# Task: Translate core/src/backend/audio/decoders.rs

## Description
Translate the Rust file `core/src/backend/audio/decoders.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/backend/audio/decoders.rs
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
- Translated DecoderError enum for decoder error types
- Translated Decoder trait/base class for audio decoding
- Translated StreamDecoder for streaming audio
- Translated StandardStreamDecoder for standard stream decoding
- Translated AdpcmStreamDecoder for ADPCM stream decoding
- Translated SeekableDecoder for seekable audio streams
- Translated StreamTagReader for reading SWF tag audio data
- Translated SubstreamTagReader for reading substream audio data
- Translated Mp3Metadata struct for MP3 metadata
- Translated StandardSubstreamDecoder for substream decoding
- Implemented make_decoder() factory function
- Implemented make_stream_decoder() for stream decoder creation
- Implemented make_substream_decoder() for substream decoder creation
- Template pattern for GC types
