// C++ translation of desktop/src/dbus.rs
//! Types and methods utilized for communicating with D-Bus
//! Linux-specific functionality

#ifndef RUFFLE_DESKTOP_DBUS_H
#define RUFFLE_DESKTOP_DBUS_H

#ifdef __linux__

#include <memory>
#include <mutex>
#include <optional>
#include <functional>
#include <future>

namespace ruffle {
namespace desktop {
namespace dbus {

/// Result type for D-Bus operations
template<typename T>
using Result = std::optional<T>;

/// Forward declarations for async operations
template<typename T>
class Future;

/// Color scheme from freedesktop settings
enum class ColorScheme : uint8_t {
    NoPreference,
    Light,
    Dark,
};

/// FreedesktopSettings - Interface to freedesktop.org settings
class FreedesktopSettings {
public:
    /// Create a new FreedesktopSettings instance
    /// Returns std::nullopt if the settings proxy cannot be created
    static std::optional<FreedesktopSettings> create();

    /// Get the current color scheme
    /// Returns std::nullopt if the operation fails
    Future<ColorScheme> color_scheme() const;

    /// Watch for color scheme changes
    /// Returns a stream of color scheme updates
    Future<std::function<ColorScheme()>> watch_color_scheme() const;

private:
    FreedesktopSettings() = default;
};

/// GameModeGuard - RAII guard for GameMode session
class GameModeGuard {
public:
    /// Create a new GameModeGuard
    /// If enabled is false, the guard will be inactive
    static Future<GameModeGuard> create(bool enabled);

    /// Unregister from GameMode (called automatically on destruction)
    void unregister();

private:
    explicit GameModeGuard(bool enabled);

    bool enabled_;
    bool registered_;
};

/// GameModeSession - Manages a GameMode session
/// Uses shared ownership to ensure the session stays active
class GameModeSession {
public:
    /// Create a new GameModeSession
    ///
    /// \param enabled Whether GameMode should be enabled
    /// \return A new GameModeSession instance
    static GameModeSession create(bool enabled);

private:
    explicit GameModeSession(std::shared_ptr<std::mutex> guard)
        : guard_(guard) {}

    std::shared_ptr<std::mutex> guard_;
};

} // namespace dbus
} // namespace desktop
} // namespace ruffle

#endif // __linux__

#endif // RUFFLE_DESKTOP_DBUS_H
