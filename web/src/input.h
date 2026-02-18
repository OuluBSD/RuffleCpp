// C++ translation of web/src/input.rs
// Web input handling - keyboard event conversion

#ifndef RUFFLE_WEB_INPUT_H
#define RUFFLE_WEB_INPUT_H

#include <string>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
namespace events {
    struct KeyDescriptor;
    enum class PhysicalKey;
    enum class LogicalKey;
    enum class KeyLocation;
    enum class NamedKey;
    enum class TextControlCode;
}
}
}

// Forward declare web_sys types (would be provided by web binding layer)
namespace web_sys {
    class KeyboardEvent;
}

namespace ruffle {
namespace web {
namespace input {

/// Convert a web KeyboardEvent to a Ruffle KeyDescriptor
/// @param event The web keyboard event
/// @return The Ruffle key descriptor
core::events::KeyDescriptor web_input_to_ruffle_key_descriptor(const web_sys::KeyboardEvent& event);

/// Convert a web KeyboardEvent.code value into a Ruffle PhysicalKey
/// @param key_code The code string from the keyboard event
/// @return The corresponding PhysicalKey enum value
core::events::PhysicalKey map_physical_key(const std::string& key_code);

/// Convert a web KeyboardEvent.key value into a Ruffle LogicalKey
/// @param key The key string from the keyboard event
/// @return The corresponding LogicalKey enum value
core::events::LogicalKey map_logical_key(const std::string& key);

/// Convert a web KeyboardEvent.location value into a Ruffle KeyLocation
/// @param location The location value (0-3) from the keyboard event
/// @return The corresponding KeyLocation enum value
core::events::KeyLocation map_key_location(uint32_t location);

/// Convert a web KeyboardEvent.key value to a Ruffle TextControlCode
/// @param key The key string from the keyboard event
/// @param ctrl_key Whether the control key is pressed
/// @param shift_key Whether the shift key is pressed
/// @return Optional TextControlCode if the key combination matches a control action
std::optional<core::events::TextControlCode> web_to_ruffle_text_control(
    const std::string& key,
    bool ctrl_key,
    bool shift_key
);

} // namespace input
} // namespace web
} // namespace ruffle

#endif // RUFFLE_WEB_INPUT_H
