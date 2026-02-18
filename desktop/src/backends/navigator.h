// C++ translation of desktop/src/backends/navigator.rs
// Desktop navigator backend

#ifndef RUFFLE_DESKTOP_BACKENDS_NAVIGATOR_H
#define RUFFLE_DESKTOP_BACKENDS_NAVIGATOR_H

#include <memory>
#include <vector>
#include <filesystem>
#include <mutex>
#include <atomic>

// Forward declarations
namespace url {
    class Url;
}

namespace winit {
namespace event_loop {
    template<typename T>
    class EventLoopProxy;
}
}

namespace ruffle {
namespace frontend {
namespace utils {
namespace content {
    class ContentDescriptor;
}
namespace backends {
namespace navigator {
    class NavigatorInterface;
}
}
}
}
}

namespace ruffle {
namespace desktop {

// CLI types
namespace cli {
    enum class FilesystemAccessMode;
    enum class OpenUrlMode;
}

// Custom event
namespace custom_event {
    enum class RuffleEvent;
}

// Preferences
namespace preferences {
    class GlobalPreferences;
}

// GUI dialogs
namespace gui {
namespace dialogs {
    class FilesystemAccessDialogConfiguration;
    class NetworkAccessDialogConfiguration;
    enum class DialogDescriptor;
}
}

} // namespace desktop
} // namespace ruffle

namespace ruffle {
namespace desktop {
namespace backends {
namespace navigator {

/// Allow list for file paths
class PathAllowList {
private:
    std::shared_ptr<std::mutex> mutex_;
    std::vector<std::filesystem::path> allowed_path_prefixes_;

public:
    explicit PathAllowList(const ruffle::frontend::utils::content::ContentDescriptor* content_descriptor);

    /// Check if path is allowed
    bool is_path_allowed(const std::filesystem::path& path) const;

    /// Add allowed path prefix
    void add_allowed_path_prefix(const std::filesystem::path& path_prefix);
};

/// Desktop navigator interface implementation
class DesktopNavigatorInterface {
private:
    ruffle::desktop::preferences::GlobalPreferences preferences_;
    std::shared_ptr<std::mutex> event_loop_mutex_;
    std::shared_ptr<winit::event_loop::EventLoopProxy<ruffle::desktop::custom_event::RuffleEvent>> event_loop_;
    PathAllowList allow_list_;
    ruffle::desktop::cli::FilesystemAccessMode filesystem_access_mode_;

    /// Ask for filesystem access asynchronously
    std::atomic<bool> ask_for_filesystem_access(const std::filesystem::path& path);

public:
    DesktopNavigatorInterface(
        const ruffle::desktop::preferences::GlobalPreferences& preferences,
        std::shared_ptr<winit::event_loop::EventLoopProxy<ruffle::desktop::custom_event::RuffleEvent>> event_loop,
        const PathAllowList& initial_allow_list,
        ruffle::desktop::cli::FilesystemAccessMode filesystem_access_mode
    );
};

} // namespace navigator
} // namespace backends
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_BACKENDS_NAVIGATOR_H
