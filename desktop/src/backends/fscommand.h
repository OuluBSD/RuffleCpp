// C++ translation of desktop/src/backends/fscommand.rs
//! Desktop FS command provider implementation

#ifndef RUFFLE_DESKTOP_BACKENDS_FSCOMMAND_H
#define RUFFLE_DESKTOP_BACKENDS_FSCOMMAND_H

#include <string>

// Forward declarations
namespace ruffle {
namespace core {
namespace external {
    class FsCommandProvider;
}
}
}

namespace winit {
namespace event_loop {
    template<typename T>
    class EventLoopProxy;
}
}

namespace ruffle {
namespace desktop {

/// Ruffle event types
enum class RuffleEvent {
    ExitRequested,
    EnterFullScreen,
    ExitFullScreen
    // Other event types would be defined here
};

namespace backends {

/// Desktop FS command provider
///
/// Handles FS commands for the desktop application, including:
/// - quit: Request application exit
/// - fullscreen: Toggle fullscreen mode
class DesktopFSCommandProvider : public core::external::FsCommandProvider {
private:
    winit::event_loop::EventLoopProxy<RuffleEvent>* event_loop_;

public:
    /// Create a new desktop FS command provider
    /// @param event_loop The event loop proxy for sending events
    explicit DesktopFSCommandProvider(winit::event_loop::EventLoopProxy<RuffleEvent>* event_loop)
        : event_loop_(event_loop) {}

    /// Handle an FSCommand
    ///
    /// Handles the following commands:
    /// - "quit": Sends ExitRequested event
    /// - "fullscreen": Sends EnterFullScreen or ExitFullScreen based on args
    ///
    /// @param command The command name
    /// @param args The command arguments
    /// @return true if the command was handled, false otherwise
    bool on_fs_command(const std::string& command, const std::string& args) override {
        if (command == "quit") {
            if (event_loop_) {
                event_loop_->send_event(RuffleEvent::ExitRequested);
            }
            return true;
        } else if (command == "fullscreen") {
            if (args == "true") {
                if (event_loop_) {
                    event_loop_->send_event(RuffleEvent::EnterFullScreen);
                }
            } else if (args == "false") {
                if (event_loop_) {
                    event_loop_->send_event(RuffleEvent::ExitFullScreen);
                }
            }
            return true;
        }
        return false;
    }
};

} // namespace backends
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_BACKENDS_FSCOMMAND_H
