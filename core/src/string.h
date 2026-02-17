// C++ translation of core/src/string.rs
// SWF string handling extensions

#ifndef RUFFLE_CORE_STRING_H
#define RUFFLE_CORE_STRING_H

#include <string>
#include <string_view>

// Forward declarations
namespace ruffle {
namespace common {
namespace wstr {
    class WStr;
    class WString;
}
}
}

// Re-export wstr types
namespace ruffle {
namespace core {
namespace string {

using WStr = ruffle::common::wstr::WStr;
using WString = ruffle::common::wstr::WString;

} // namespace string
} // namespace core
} // namespace ruffle

// SWF namespace for encoding
namespace swf {
    struct Encoding;
}

namespace ruffle {
namespace core {
namespace string {

/// Extension trait for SWF string decoding
class SwfStrExt {
public:
    /// Converts a SWF-encoded string into a WStr.
    /// @param swf_str The SWF-encoded string
    /// @param encoding The encoding to use for decoding
    /// @return A borrowed or owned WStr depending on conversion result
    static const WStr* decode(const char* swf_str, const swf::Encoding* encoding);
    
    /// Converts a SWF-encoded string into an owned WString.
    /// @param swf_str The SWF-encoded string
    /// @param encoding The encoding to use for decoding
    /// @return An owned WString
    static WString decode_owned(const char* swf_str, const swf::Encoding* encoding);
};

} // namespace string
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_STRING_H
