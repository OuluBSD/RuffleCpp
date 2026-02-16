// C++ translation of wstr/src/lib.rs
// Provides UCS2 string types for usage in AVM1 and AVM2

#ifndef RUFFLE_WSTR_LIB_H
#define RUFFLE_WSTR_LIB_H

#include "common.h"
#include "buf.h"
#include "ops.h"
#include "parse.h"
#include "pattern.h"
#include "ptr.h"
#include "utils.h"

#include <string>
#include <string_view>

namespace ruffle {
namespace wstr {

/// Flattens a slice of strings, placing `sep` as a separator between each.
inline WString join(const std::vector<const WStr*>& elems, const WStr* sep) {
    // TODO: Implement str_join from ops module
    return WString();
}

/// Converts a borrowed UTF-8 string to a `WStr` slice.
inline const WStr* from_utf8(std::string_view s) {
    // Check if string is ASCII-only
    bool is_ascii = true;
    size_t ascii_len = 0;
    for (char c : s) {
        if (static_cast<unsigned char>(c) >= 0x80) {
            is_ascii = false;
            break;
        }
        ascii_len++;
    }
    
    if (is_ascii) {
        // We can directly reinterpret ASCII bytes as LATIN1.
        return WStr::from_units(reinterpret_cast<const uint8_t*>(s.data()), s.size());
    } else {
        // Need to convert non-ASCII UTF-8
        // TODO: Implement WString::from_utf8_inner
        return nullptr;
    }
}

/// Converts a slice of UTF-8 bytes to a `WStr` slice.
///
/// Invalid UTF-8 sequences are treated as described in `utils::DecodeAvmUtf8`.
inline const WStr* from_utf8_bytes(const uint8_t* bytes, size_t len) {
    // Check if bytes are ASCII-only
    bool is_ascii = true;
    size_t ascii_len = 0;
    for (size_t i = 0; i < len; i++) {
        if (bytes[i] >= 0x80) {
            is_ascii = false;
            break;
        }
        ascii_len++;
    }
    
    if (is_ascii) {
        // We can directly reinterpret ASCII bytes as LATIN1.
        return WStr::from_units(bytes, len);
    } else {
        // Need to convert non-ASCII UTF-8 bytes
        // TODO: Implement WString::from_utf8_bytes_inner
        return nullptr;
    }
}

} // namespace wstr
} // namespace ruffle

#endif // RUFFLE_WSTR_LIB_H
