// C++ translation of flv/src/script.rs
//! FLV Script Data Structures

#ifndef RUFFLE_FLV_SCRIPT_H
#define RUFFLE_FLV_SCRIPT_H

#include <cstdint>
#include <vector>
#include <variant>
#include <optional>

namespace ruffle {
namespace flv {

// Forward declarations
class FlvReader;
class Error;

/// Parse a string from the reader.
///
/// \param reader The FLV reader
/// \param is_long_string Whether this is a long string (u32 length) or regular string (u16 length)
/// \return The parsed string bytes or an error
std::optional<std::vector<uint8_t>> parse_string(FlvReader* reader, bool is_long_string);

/// Script value types
///
/// Strings are yielded as byte arrays, as there is no guidance in the FLV
/// specification as to how they are to be decoded.
struct Value {
    /// Number value (f64)
    struct Number {
        double value;
    };

    /// Boolean value
    struct Boolean {
        bool value;
    };

    /// String value (byte array)
    struct String {
        std::vector<uint8_t> bytes;
    };

    /// Object (list of variables)
    struct Object {
        std::vector<struct Variable> variables;
    };

    /// MovieClip (string that defines a MovieClip path)
    struct MovieClip {
        std::vector<uint8_t> path;
    };

    /// Null value
    struct Null {};

    /// Undefined value
    struct Undefined {};

    /// Reference (u16)
    struct Reference {
        uint16_t id;
    };

    /// ECMA Array (associative array)
    struct EcmaArray {
        std::vector<struct Variable> variables;
    };

    /// Strict Array (indexed array)
    struct StrictArray {
        std::vector<Value> values;
    };

    /// Date value
    struct Date {
        /// The number of milliseconds since January 1st, 1970.
        double unix_time;

        /// Local time offset in minutes from UTC.
        /// Time zones west of Greenwich, UK are negative.
        int16_t local_offset;
    };

    /// Long string value (byte array with u32 length)
    struct LongString {
        std::vector<uint8_t> bytes;
    };

    std::variant<
        Number,
        Boolean,
        String,
        Object,
        MovieClip,
        Null,
        Undefined,
        Reference,
        EcmaArray,
        StrictArray,
        Date,
        LongString
    > data;

    /// Parse a script value.
    ///
    /// Strings are yielded as byte arrays, as there is no guidance in the FLV
    /// specification as to how they are to be decoded.
    ///
    /// \param reader The FLV reader
    /// \return Parsed value or an error
    static std::optional<Value> parse(FlvReader* reader);
};

/// An individual object in a ScriptData tag.
///
/// This corresponds to both the `SCRIPTDATAOBJECT` and `SCRIPTDATAVARIABLE`
/// structures as defined in the FLV specification. These structures are
/// otherwise identical.
struct Variable {
    std::vector<uint8_t> name;
    Value data;

    /// Parse a variable from the reader.
    ///
    /// \param reader The FLV reader
    /// \return Parsed variable or an error
    static std::optional<Variable> parse(FlvReader* reader);
};

/// Script data structure
///
/// Contains a list of variables parsed from the script data tag.
struct ScriptData {
    std::vector<Variable> variables;

    /// Parse a script data structure.
    ///
    /// No data size parameter is accepted; we parse until we reach an object
    /// terminator, reach invalid data, or we run out of bytes in the reader.
    ///
    /// \param reader The FLV reader
    /// \param data_size The size of the script data structure
    /// \return Parsed script data or an error
    static std::optional<ScriptData> parse(FlvReader* reader, uint32_t data_size);
};

} // namespace flv
} // namespace ruffle

#endif // RUFFLE_FLV_SCRIPT_H
