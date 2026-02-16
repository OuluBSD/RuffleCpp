// C++ translation of core/src/avm2/e4x/is_xml_name.rs
// XML name validation for E4X

#ifndef RUFFLE_CORE_AVM2_E4X_IS_XML_NAME_H
#define RUFFLE_CORE_AVM2_E4X_IS_XML_NAME_H

#include <cstdint>
#include <array>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename T> class AvmString;
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace e4x {
namespace is_xml_name {

// Unicode character range tables for XML name validation
// Based on https://www.w3.org/TR/2004/REC-xml-20040204/

/// Table of valid letter characters for XML names
/// Each entry is a (start, end) range of Unicode code points
extern const std::array<std::pair<uint32_t, uint32_t>, 213> LETTER_TABLE;

/// Table of valid digit characters for XML names
/// Each entry is a (start, end) range of Unicode code points
extern const std::array<std::pair<uint32_t, uint32_t>, 15> DIGIT_TABLE;

/// Table of valid combining characters for XML names
/// Each entry is a (start, end) range of Unicode code points
extern const std::array<std::pair<uint32_t, uint32_t>, 93> COMBINING_TABLE;

/// Table of valid extender characters for XML names
/// Each entry is a (start, end) range of Unicode code points
extern const std::array<std::pair<uint32_t, uint32_t>, 11> EXTENDER_TABLE;

/// Check if a character is a valid XML letter
///
/// \param c The Unicode code point to check
/// \return true if the character is a valid XML letter
bool is_letter(uint32_t c);

/// Check if a character is a valid XML digit
///
/// \param c The Unicode code point to check
/// \return true if the character is a valid XML digit
bool is_digit(uint32_t c);

/// Check if a character is a valid XML combining character
///
/// \param c The Unicode code point to check
/// \return true if the character is a valid combining character
bool is_combining(uint32_t c);

/// Check if a character is a valid XML extender
///
/// \param c The Unicode code point to check
/// \return true if the character is a valid extender
bool is_extender(uint32_t c);

/// Validate if a string is a valid XML name according to E4X specification
///
/// Based on https://github.com/adobe/avmplus/blob/858d034a3bd3a54d9b70909386435cf4aec81d21/core/AvmCore.cpp#L3478
///
/// A valid XML name must:
/// - Not be empty
/// - Start with a letter or underscore
/// - Subsequent characters can be letters, digits, '.', '-', '_',
///   combining characters, or extenders
///
/// \param name The string to validate
/// \return true if the string is a valid XML name
bool is_xml_name(AvmString<GC> name);

/// Verify that the character tables are sorted
///
/// Used for testing to ensure the binary search tables are valid.
///
/// \return true if all tables are properly sorted
bool verify_tables_sorted();

} // namespace is_xml_name
} // namespace e4x
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_E4X_IS_XML_NAME_H
