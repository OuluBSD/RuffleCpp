// C++ translation of desktop/src/util.rs
// Utility functions for Ruffle desktop

#ifndef RUFFLE_DESKTOP_UTIL_H
#define RUFFLE_DESKTOP_UTIL_H

#include <cstdint>
#include <string>
#include <string_view>
#include <optional>
#include <expected>
#include <filesystem>
#include <utility>

// Forward declarations
namespace winit {
    namespace window {
        class Window;
    }
    namespace dpi {
        struct PhysicalSize;
    }
    namespace event {
        struct KeyEvent;
        struct Modifiers;
    }
    namespace keyboard {
        enum class KeyCode;
        enum class KeyLocation;
        enum class NamedKey;
        enum class PhysicalKey;
    }
}

namespace gilrs {
    enum class Button;
}

namespace url {
    class Url;
}

namespace anyhow {
    class Error;
}

namespace ruffle {
namespace core {
namespace events {
    enum class GamepadButton;
    enum class KeyDescriptor;
    enum class KeyLocation;
    enum class LogicalKey;
    enum class NamedKey;
    enum class PhysicalKey;
    enum class TextControlCode;
}
}
}

namespace ruffle {
namespace desktop {
namespace util {

/// Converts a winit event to a Ruffle `TextControlCode`.
/// Returns `std::nullopt` if there is no match.
///
/// @param event The key event to convert
/// @param modifiers The modifier keys state
/// @return The text control code, or std::nullopt if no match
std::optional<core::events::TextControlCode> winit_to_ruffle_text_control(
    const winit::event::KeyEvent& event,
    const winit::event::Modifiers& modifiers
);

/// Converts a winit key event to a Ruffle key descriptor.
///
/// @param event The key event to convert
/// @return The key descriptor
core::events::KeyDescriptor winit_input_to_ruffle_key_descriptor(
    const winit::event::KeyEvent& event
);

/// Converts a gilrs button to a Ruffle gamepad button.
///
/// @param button The gilrs button to convert
/// @return The gamepad button, or std::nullopt if not supported
std::optional<core::events::GamepadButton> gilrs_button_to_gamepad_button(gilrs::Button button);

/// Get the total screen size across all monitors.
///
/// @param window The window to get monitor information from
/// @return The total screen size
winit::dpi::PhysicalSize<uint32_t> get_screen_size(const winit::window::Window& window);

/// Parse a path as a URL.
///
/// If the path exists as a file, it will be converted to a file:// URL.
/// Otherwise, it will be parsed as a regular URL.
///
/// @param path The path to parse
/// @return The parsed URL, or an error
std::expected<url::Url, anyhow::Error> parse_url(const std::filesystem::path& path);

/// Open a URL in the default browser.
///
/// @param url The URL to open
void open_url(const url::Url& url);

} // namespace util
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_UTIL_H
