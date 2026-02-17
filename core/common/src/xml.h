// C++ translation of core/common/src/xml.rs
// XML unescaping utilities for AVM1 and AVM2

#ifndef RUFFLE_CORE_COMMON_XML_H
#define RUFFLE_CORE_COMMON_XML_H

#include <string>
#include <cstdint>

namespace ruffle {
namespace common {
namespace xml {

/// AVM1 XML unescaping. Decodes entities individually, even when
/// preceded by a bare '&' (e.g. "&&amp;" becomes "&&").
///
/// @param input The XML-encoded byte array
/// @return The unescaped string, or error on UTF-8 conversion failure
std::string avm1_unescape(const uint8_t* input, size_t length);

/// AVM2 E4X XML unescaping. Does not decode entities preceded by a
/// bare '&' (e.g. "&&amp;" is preserved as "&&amp;").
///
/// @param input The XML-encoded byte array
/// @return The unescaped string, or error on UTF-8 conversion failure
std::string avm2_unescape(const uint8_t* input, size_t length);

/// Handles flash-specific XML unescaping behavior.
/// We accept all XML entities, and also accept standalone '&' without
/// a corresponding ';'
///
/// @param input The XML-encoded byte array
/// @param use_avm1_behavior If true, use AVM1 unescaping; otherwise use AVM2
/// @return The unescaped string, or error on UTF-8 conversion failure
std::string custom_unescape(const uint8_t* input, size_t length, bool use_avm1_behavior);

} // namespace xml
} // namespace common
} // namespace ruffle

#endif // RUFFLE_CORE_COMMON_XML_H
