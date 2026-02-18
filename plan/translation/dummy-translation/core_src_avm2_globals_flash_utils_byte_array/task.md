# Task: Translate core/src/avm2/globals/flash/utils/byte_array.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/utils/byte_array.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/utils/byte_array.rs
```

## Translation Details
- Translated ByteArray functions for AVM2 flash.utils.ByteArray
- Translated get_default_object_encoding() and set_default_object_encoding() for encoding configuration
- Translated write_byte() for single byte writing
- Translated write_bytes() and read_bytes() for bytearray-to-bytearray operations
- Translated write_utf() and read_utf() for UTF-8 string operations
- Translated strip_bom() for BOM stripping from UTF-8/UTF-16 data
- Translated to_string() for bytearray to string conversion
- Translated clear() for clearing bytearray contents
- Translated get_position() and set_position() for position management
- Translated get_bytes_available() and get_length() for size queries
- Translated set_length() for resizing bytearray
- Translated get_endian() and set_endian() for endianness control
- Translated read methods: read_short(), read_unsigned_short(), read_double(), read_float()
- Translated read methods: read_int(), read_unsigned_int(), read_boolean(), read_byte()
- Translated read methods: read_utf_bytes(), read_unsigned_byte()
- Translated write methods: write_float(), write_double(), write_boolean()
- Translated write methods: write_int(), write_unsigned_int(), write_short()
- Translated write_multi_byte() and read_multi_byte() for charset-encoded strings
- Translated write_utf_bytes() for raw UTF-8 byte writing
- Translated compress() and uncompress() for compression operations
- Translated read_object() and write_object() for AMF serialization
- Translated get_object_encoding() and set_object_encoding() for object encoding
- Used template pattern for GC types
- Added proper forward declarations for all AVM2 and encoding types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
