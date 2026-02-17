// C++ translation of desktop/src/gui/picker.rs
// File picker for Ruffle desktop GUI

#ifndef RUFFLE_DESKTOP_GUI_PICKER_H
#define RUFFLE_DESKTOP_GUI_PICKER_H

#include <string>
#include <memory>
#include <optional>
#include <filesystem>
#include <atomic>
#include <vector>

// Forward declarations
namespace winit {
    namespace event_loop {
        template<typename T>
        class EventLoopProxy;
    }
    namespace window {
        class Window;
    }
}

namespace rfd {
    class AsyncFileDialog;
}

namespace unic_langid {
    class LanguageIdentifier;
}

namespace ruffle {
namespace desktop {
    struct RuffleEvent;
    class DialogDescriptor;
    namespace preferences {
        class GlobalPreferences;
    }
}
namespace frontend_utils {
namespace bundle {
    class Bundle;
}
}
}

namespace ruffle {
namespace desktop {
namespace gui {

/// File picker data (internal structure)
class FilePickerData {
private:
    winit::event_loop::EventLoopProxy<RuffleEvent>* event_loop_;
    std::weak_ptr<winit::window::Window> parent_;
    std::atomic<bool> picking_;
    GlobalPreferences preferences_;

public:
    FilePickerData(
        winit::event_loop::EventLoopProxy<RuffleEvent>* event_loop,
        std::weak_ptr<winit::window::Window> parent,
        const GlobalPreferences& preferences
    );

    winit::event_loop::EventLoopProxy<RuffleEvent>* event_loop() const { return event_loop_; }
    std::weak_ptr<winit::window::Window> parent() const { return parent_; }
    std::atomic<bool>& picking() { return picking_; }
    const GlobalPreferences& preferences() const { return preferences_; }
};

/// File picker for selecting files and directories
class FilePicker {
private:
    std::shared_ptr<FilePickerData> data_;

public:
    /// Create a new file picker
    ///
    /// @param parent Weak reference to the parent window
    /// @param preferences Global preferences for localization
    /// @param event_loop Event loop proxy for sending events
    FilePicker(
        std::weak_ptr<winit::window::Window> parent,
        const GlobalPreferences& preferences,
        winit::event_loop::EventLoopProxy<RuffleEvent>* event_loop
    );

    /// Show a dialog with a custom operation
    ///
    /// @param dialog The async file dialog to show
    /// @param f The operation to perform on the dialog
    /// @return Optional result from the operation
    template<typename F, typename O>
    std::optional<O> show_dialog(rfd::AsyncFileDialog dialog, F&& f);

    /// Pick a Ruffle-compatible file
    ///
    /// @param dir Optional starting directory
    /// @return Selected file path, or nullopt if cancelled
    std::optional<std::filesystem::path> pick_ruffle_file(
        std::optional<std::filesystem::path> dir = std::nullopt
    );

    /// Pick a Ruffle-compatible directory
    ///
    /// @param dir Optional starting directory
    /// @return Selected directory path, or nullopt if cancelled
    std::optional<std::filesystem::path> pick_ruffle_directory(
        std::optional<std::filesystem::path> dir = std::nullopt
    );

    /// Pick a directory and content to play
    ///
    /// This can return:
    ///  - A Ruffle bundle as a directory (directory, directory)
    ///  - A multi-asset SWF movie (directory, root_movie.swf)
    ///
    /// @param dir Optional starting directory
    /// @return Pair of (directory, content path), or nullopt if cancelled
    std::optional<std::pair<std::filesystem::path, std::filesystem::path>>
    pick_ruffle_directory_and_content(
        std::optional<std::filesystem::path> dir = std::nullopt
    );
};

} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_PICKER_H
