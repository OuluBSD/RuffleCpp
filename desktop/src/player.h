// C++ translation of desktop/src/player.rs
//! Desktop player initialization and management

#ifndef RUFFLE_DESKTOP_PLAYER_H
#define RUFFLE_DESKTOP_PLAYER_H

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <memory>
#include <mutex>
#include <atomic>

// Forward declarations
namespace ruffle {
namespace core {
    class Player;
    class PlayerBuilder;
    enum class StageScaleMode;
    enum class StageAlign;
    enum class Letterbox;
    enum class LoadBehavior;
    enum class PlayerRuntime;
    struct HeaderExt;
    enum class DefaultFont;

    namespace backend {
    namespace navigator {
        enum class SocketMode;
    }
    }

    namespace events {
        enum class GamepadButton;
        enum class KeyCode;
    }
}

namespace render {
namespace quality {
    enum class StageQuality;
}
}

namespace desktop {

enum class FilesystemAccessMode;
enum class GameModePreference;

struct PlayerOptions;
struct GlobalPreferences;
struct ContentDescriptor;

namespace backends {
    struct PathAllowList;
}

} // namespace desktop
} // namespace ruffle

namespace winit {
namespace event_loop {
    template<typename T> class EventLoopProxy;
}
namespace window {
    class Window;
}
} // namespace winit

namespace url {
    class Url;
}

namespace ruffle {
namespace desktop {

/// Options used when creating a Player (passed through to a PlayerBuilder)
struct LaunchOptions {
    PlayerOptions player;
    std::optional<url::Url> proxy;
    std::unordered_set<std::string> socket_allowed;
    std::optional<core::backend::navigator::SocketMode> tcp_connections;
    bool fullscreen = false;
    std::string save_directory;
    std::string cache_directory;
    FilesystemAccessMode filesystem_access_mode;
    std::unordered_map<core::events::GamepadButton, core::events::KeyCode> gamepad_button_mapping;
    bool avm2_optimizer_enabled = true;
};

/// Unique identifier for a given Player instance
class PlayerId {
public:
    PlayerId() : id_(NEXT.fetch_add(1, std::memory_order_relaxed)) {}
    
    int64_t value() const { return id_; }
    
    bool operator==(const PlayerId& other) const { return id_ == other.id_; }
    bool operator!=(const PlayerId& other) const { return id_ != other.id_; }

private:
    int64_t id_;
    static std::atomic<int64_t> NEXT;
};

/// Represents a current Player and any associated state with that player
class ActivePlayer {
public:
    /// Create a new active player
    ///
    /// \param opt Launch options
    /// \param event_loop Event loop proxy
    /// \param content_descriptor Content descriptor
    /// \param window Window reference
    /// \param descriptors GPU descriptors
    /// \param movie_view Movie view
    /// \param font_database Font database
    /// \param preferences Global preferences
    /// \param file_picker File picker
    ActivePlayer(
        const LaunchOptions& opt,
        winit::event_loop::EventLoopProxy<RuffleEvent>* event_loop,
        const ContentDescriptor& content_descriptor,
        std::shared_ptr<winit::window::Window> window,
        std::shared_ptr<Descriptors> descriptors,
        MovieView movie_view,
        std::shared_ptr<fontdb::Database> font_database,
        GlobalPreferences preferences,
        FilePicker file_picker
    );

    /// Get player ID
    PlayerId id() const { return id_; }

    /// Get player instance (thread-safe)
    std::unique_lock<std::mutex> lock_player();

private:
    PlayerId id_;
    std::shared_ptr<std::mutex> player_mutex_;
    std::shared_ptr<core::Player> player_;

#ifdef __linux__
    std::optional<GameModeSession> gamemode_session_;
#endif
};

/// A Player-bound future that is currently running
class PlayerRunnable {
public:
    explicit PlayerRunnable(async_task::Runnable<PlayerId> runnable)
        : runnable_(std::move(runnable)) {}

    void run() { runnable_.run(); }

    const PlayerId& metadata() const { return runnable_.metadata(); }

private:
    async_task::Runnable<PlayerId> runnable_;
};

/// Owner of a Ruffle Player (via ActivePlayer)
class PlayerController {
public:
    /// Create a new player controller
    ///
    /// \param event_loop Event loop proxy
    /// \param window Window reference
    /// \param descriptors GPU descriptors
    /// \param font_database Font database
    /// \param preferences Global preferences
    /// \param file_picker File picker
    PlayerController(
        winit::event_loop::EventLoopProxy<RuffleEvent>* event_loop,
        std::shared_ptr<winit::window::Window> window,
        std::shared_ptr<Descriptors> descriptors,
        std::shared_ptr<fontdb::Database> font_database,
        GlobalPreferences preferences,
        FilePicker file_picker
    );

    /// Create a new player
    ///
    /// \param opt Launch options
    /// \param content_descriptor Content descriptor
    /// \param movie_view Movie view
    void create(
        const LaunchOptions& opt,
        const ContentDescriptor& content_descriptor,
        MovieView movie_view
    );

    /// Destroy the current player
    void destroy();

    /// Get player instance (thread-safe)
    std::optional<std::unique_lock<std::mutex>> get();

    /// Handle player event
    ///
    /// \param event Player event
    /// \return True if event was handled
    bool handle_event(const core::PlayerEvent& event);

    /// Poll a player-bound future
    ///
    /// \param task Task to poll
    void poll(PlayerRunnable task);

private:
    std::optional<ActivePlayer> player_;
    winit::event_loop::EventLoopProxy<RuffleEvent>* event_loop_;
    std::shared_ptr<winit::window::Window> window_;
    std::shared_ptr<Descriptors> descriptors_;
    std::shared_ptr<fontdb::Database> font_database_;
    GlobalPreferences preferences_;
    FilePicker file_picker_;
};

/// Bare-bones executor that schedules tasks on the winit event loop
class WinitExecutor {
public:
    WinitExecutor(
        winit::event_loop::EventLoopProxy<RuffleEvent>* event_loop,
        PlayerId player_id
    ) : event_loop_(event_loop), player_id_(player_id) {}

    /// Spawn a future
    ///
    /// \param future Future to spawn
    template<typename E>
    void spawn(async_task::OwnedFuture<void, E> future);

private:
    winit::event_loop::EventLoopProxy<RuffleEvent>* event_loop_;
    PlayerId player_id_;
};

} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_PLAYER_H
