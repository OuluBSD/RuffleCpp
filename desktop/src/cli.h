// C++ translation of desktop/src/cli.rs
// Command-line interface for Ruffle desktop

#ifndef RUFFLE_DESKTOP_CLI_H
#define RUFFLE_DESKTOP_CLI_H

#include <string>
#include <vector>
#include <optional>
#include <filesystem>
#include <chrono>
#include <map>

// Forward declarations
namespace url {
    class Url;
}

namespace anyhow {
    class Error;
    template<typename T>
    using Result = std::expected<T, Error>;
}

namespace ruffle {
namespace core {
namespace backend {
namespace navigator {
    enum class SocketMode;
}
}
namespace config {
    enum class Letterbox;
}
namespace events {
    enum class GamepadButton;
    enum class KeyCode;
}
    enum class LoadBehavior;
    enum class PlayerRuntime;
    enum class StageAlign;
    enum class StageScaleMode;
}

namespace render {
namespace quality {
    enum class StageQuality;
}
}

namespace render_wgpu {
namespace clap {
    enum class GraphicsBackend;
    enum class PowerPreference;
}
}

namespace desktop {
    extern const char* RUFFLE_VERSION;
}
}

namespace ruffle {
namespace desktop {
namespace cli {

/// Game mode preference for Linux
enum class GameModePreference {
    Default,
    On,
    Off
};

/// Open URL handling mode
enum class OpenUrlMode {
    Confirm,
    Allow,
    Deny
};

/// Filesystem access mode
enum class FilesystemAccessMode {
    Allow,
    Deny,
    Ask
};

/// Named key code for gamepad button mapping
enum class NamedKeyCode : uint32_t {
    Unknown = 0,
    MouseLeft = 1,
    MouseRight = 2,
    MouseMiddle = 4,
    Backspace = 8,
    Tab = 9,
    Return = 13,
    Command = 15,
    Shift = 16,
    Control = 17,
    Alt = 18,
    Pause = 19,
    CapsLock = 20,
    Numpad = 21,
    Escape = 27,
    Space = 32,
    PgUp = 33,
    PgDown = 34,
    End = 35,
    Home = 36,
    Left = 37,
    Up = 38,
    Right = 39,
    Down = 40,
    Insert = 45,
    Delete = 46,
    Key0 = 48,
    Key1 = 49,
    Key2 = 50,
    Key3 = 51,
    Key4 = 52,
    Key5 = 53,
    Key6 = 54,
    Key7 = 55,
    Key8 = 56,
    Key9 = 57,
    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,
    Numpad0 = 96,
    Numpad1 = 97,
    Numpad2 = 98,
    Numpad3 = 99,
    Numpad4 = 100,
    Numpad5 = 101,
    Numpad6 = 102,
    Numpad7 = 103,
    Numpad8 = 104,
    Numpad9 = 105,
    Multiply = 106,
    Plus = 107,
    NumpadEnter = 108,
    NumpadMinus = 109,
    NumpadPeriod = 110,
    NumpadSlash = 111,
    F1 = 112,
    F2 = 113,
    F3 = 114,
    F4 = 115,
    F5 = 116,
    F6 = 117,
    F7 = 118,
    F8 = 119,
    F9 = 120,
    F10 = 121,
    F11 = 122,
    F12 = 123,
    F13 = 124,
    F14 = 125,
    F15 = 126,
    F16 = 127,
    F17 = 128,
    F18 = 129,
    F19 = 130,
    F20 = 131,
    F21 = 132,
    F22 = 133,
    F23 = 134,
    F24 = 135,
    NumLock = 144,
    ScrollLock = 145,
    Semicolon = 186,
    Equals = 187,
    Comma = 188,
    Minus = 189,
    Period = 190,
    Slash = 191,
    Grave = 192,
    LBracket = 219,
    Backslash = 220,
    RBracket = 221,
    Apostrophe = 222
};

/// Command-line options for Ruffle desktop
struct Opt {
    /// Path or URL of a Flash movie (SWF) to play
    std::optional<url::Url> movie_url;

    /// Flashvars parameters (key=value pairs)
    std::vector<std::string> parameters;

    /// Graphics backend preference
    std::optional<render_wgpu::clap::GraphicsBackend> graphics;

    /// Power preference for graphics device
    std::optional<render_wgpu::clap::PowerPreference> power;

    /// GameMode preference (Linux only)
    std::optional<GameModePreference> gamemode;

    /// Storage backend preference
    std::optional<std::string> storage;

    /// Window width in pixels
    std::optional<double> width;

    /// Window height in pixels
    std::optional<double> height;

    /// Maximum script execution duration
    std::optional<std::chrono::duration<double>> max_execution_duration;

    /// Base URL for resolving relative paths
    std::optional<url::Url> base;

    /// Default movie quality
    std::optional<core::quality::StageQuality> quality;

    /// Stage alignment
    std::optional<core::StageAlign> align;

    /// Force stage alignment (prevent movie changes)
    bool force_align = false;

    /// Stage scale mode
    std::optional<core::StageScaleMode> scale;

    /// Audio volume (0.0 to 1.0)
    std::optional<float> volume;

    /// Force stage scale mode (prevent movie changes)
    bool force_scale = false;

    /// Save data directory
    std::filesystem::path save_directory;

    /// Configuration directory
    std::filesystem::path config_directory;

    /// Cache directory
    std::filesystem::path cache_directory;

    /// Proxy URL for loading movies
    std::optional<url::Url> proxy;

    /// Socket whitelist endpoints
    std::vector<std::string> socket_allow;

    /// TCP socket connection mode
    std::optional<core::backend::navigator::SocketMode> tcp_connections;

    /// Upgrade HTTP URLs to HTTPS
    bool upgrade_to_https = false;

    /// Start in fullscreen mode
    bool fullscreen = false;

    /// Load behavior preference
    std::optional<core::LoadBehavior> load_behavior;

    /// Letterbox handling mode
    std::optional<core::Letterbox> letterbox;

    /// Spoofed SWF URL
    std::optional<url::Url> spoof_url;

    /// Spoofed HTTP referer
    std::optional<url::Url> referer;

    /// Spoofed HTTP cookie
    std::optional<std::string> cookie;

    /// Player version to emulate
    std::optional<uint8_t> player_version;

    /// Player runtime to emulate
    std::optional<core::PlayerRuntime> player_runtime;

    /// Fixed frame rate
    std::optional<double> frame_rate;

    /// Open URL handling mode
    std::optional<OpenUrlMode> open_url_mode;

    /// Filesystem access mode
    FilesystemAccessMode filesystem_access_mode = FilesystemAccessMode::Ask;

    /// Provide dummy External Interface
    bool dummy_external_interface = false;

    /// Hide the menu bar
    bool no_gui = false;

    /// Gamepad button mappings
    std::vector<std::pair<core::events::GamepadButton, core::events::KeyCode>> gamepad_button;

    /// Disable AVM2 optimizer
    bool no_avm2_optimizer = false;

    /// Default constructor with default values
    Opt();

    /// Get parameters as key-value pairs iterator
    auto parameters_map() const -> std::map<std::string, std::string>;
};

/// Parse a movie file path or URL
/// @param path The path string to parse
/// @return Result containing the parsed URL
anyhow::Result<url::Url> parse_movie_file_or_url(const std::string& path);

/// Parse duration from seconds string
/// @param value The string value to parse
/// @return Result containing the duration
anyhow::Result<std::chrono::duration<double>> parse_duration_seconds(const std::string& value);

/// Parse stage alignment from string
/// @param value The string value to parse
/// @return Result containing the StageAlign enum
anyhow::Result<core::StageAlign> parse_align(const std::string& value);

/// Parse gamepad button mapping
/// @param mapping The mapping string (e.g., "d-pad-up=up")
/// @return Result containing the button and key code pair
anyhow::Result<std::pair<core::events::GamepadButton, core::events::KeyCode>>
parse_gamepad_button(const std::string& mapping);

/// Get the default save directory
/// @return The default save directory path
std::filesystem::path get_default_save_directory();

/// Get the default config directory
/// @return The default config directory path
std::filesystem::path get_default_config_directory();

/// Get the default cache directory
/// @return The default cache directory path
std::filesystem::path get_default_cache_directory();

} // namespace cli
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_CLI_H
