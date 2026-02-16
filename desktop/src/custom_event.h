// C++ translation of desktop/src/custom_event.rs
// Custom event type for desktop ruffle

#ifndef RUFFLE_DESKTOP_CUSTOM_EVENT_H
#define RUFFLE_DESKTOP_CUSTOM_EVENT_H

#include <memory>
#include <variant>

// Forward declarations
namespace ruffle {
namespace core {
namespace events {
    class PlayerNotification;
}
namespace swf {
    struct HeaderExt;
}
}
}

namespace ruffle {
namespace frontend_utils {
namespace content {
    class ContentDescriptor;
}
}
}

namespace ruffle {
namespace desktop {

// Forward declarations from other modules
namespace gui {
    class DialogDescriptor;
}
namespace player {
    class LaunchOptions;
    class PlayerRunnable;
}

/// Type of content to open (file or directory)
enum class OpenType : uint8_t {
    File,
    Directory,
};

/// User-defined events for the desktop Ruffle application
/// These events are used to communicate between the UI and the player
class RuffleEvent {
public:
    /// Variants of user-defined events
    struct TaskPoll {
        player::PlayerRunnable runnable;
    };

    struct OnMetadata {
        ruffle::core::swf::HeaderExt header;
    };

    struct BrowseAndOpen {
        std::unique_ptr<player::LaunchOptions> options;
        OpenType open_type;
    };

    struct Open {
        ruffle::frontend_utils::content::ContentDescriptor descriptor;
        std::unique_ptr<player::LaunchOptions> options;
    };

    struct CloseFile { };

    struct EnterFullScreen { };

    struct ExitFullScreen { };

    struct ExitRequested { };

    struct ContextMenuItemClicked {
        size_t index;
    };

    struct OpenDialog {
        gui::DialogDescriptor descriptor;
    };

    struct PlayerNotification {
        ruffle::core::events::PlayerNotification notification;
    };

    struct ExportBundle { };

    /// Event data stored as variant
    using Data = std::variant<
        TaskPoll,
        OnMetadata,
        BrowseAndOpen,
        Open,
        CloseFile,
        EnterFullScreen,
        ExitFullScreen,
        ExitRequested,
        ContextMenuItemClicked,
        OpenDialog,
        PlayerNotification,
        ExportBundle
    >;

private:
    Data data_;

public:
    // Constructors for each variant
    explicit RuffleEvent(TaskPoll event) : data_(std::move(event)) {}
    explicit RuffleEvent(OnMetadata event) : data_(std::move(event)) {}
    explicit RuffleEvent(BrowseAndOpen event) : data_(std::move(event)) {}
    explicit RuffleEvent(Open event) : data_(std::move(event)) {}
    explicit RuffleEvent(CloseFile) : data_(CloseFile{}) {}
    explicit RuffleEvent(EnterFullScreen) : data_(EnterFullScreen{}) {}
    explicit RuffleEvent(ExitFullScreen) : data_(ExitFullScreen{}) {}
    explicit RuffleEvent(ExitRequested) : data_(ExitRequested{}) {}
    explicit RuffleEvent(ContextMenuItemClicked event) : data_(std::move(event)) {}
    explicit RuffleEvent(OpenDialog event) : data_(std::move(event)) {}
    explicit RuffleEvent(PlayerNotification event) : data_(std::move(event)) {}
    explicit RuffleEvent(ExportBundle) : data_(ExportBundle{}) {}

    // Default constructor
    RuffleEvent() = default;

    // Accessors
    const Data& data() const { return data_; }
    Data& data() { return data_; }
};

} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_CUSTOM_EVENT_H
