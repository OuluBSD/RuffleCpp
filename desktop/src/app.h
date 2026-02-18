// C++ translation of desktop/src/app.rs
// Desktop application main entry point

#ifndef RUFFLE_DESKTOP_APP_H
#define RUFFLE_DESKTOP_APP_H

#include <cstdint>
#include <string>
#include <memory>
#include <optional>
#include <chrono>
#include <functional>

// Forward declarations
namespace ruffle {
namespace core {
    class Player;
    namespace swf {
        struct HeaderExt;
    }
    namespace events {
        struct PlayerEvent;
        enum class MouseButton;
        struct MouseWheelDelta;
        enum class PlayerNotification;
        struct ImeEvent;
    }
}
namespace render {
namespace backend {
    struct ViewportDimensions;
}
}
namespace desktop {
namespace gui {
    class GuiController;
    constexpr int MENU_HEIGHT = 30; // Example value
}
namespace preferences {
    class GlobalPreferences;
}
}
}

// Winit types (windowing library)
namespace winit {
namespace dpi {
    struct LogicalSize;
    struct PhysicalPosition;
    struct PhysicalSize;
    struct Size;
}
namespace event {
    struct WindowEvent;
    struct KeyEvent;
    struct Modifiers;
    enum class ElementState;
    enum class MouseButton;
    enum class MouseScrollDelta;
    struct Ime;
}
namespace event_loop {
    class ActiveEventLoop;
    enum class ControlFlow;
    enum class StartCause;
    template<typename T>
    class EventLoopProxy;
}
namespace keyboard {
    enum class Key;
    enum class NamedKey;
}
namespace window {
    enum class Fullscreen;
    struct WindowAttributes;
    struct WindowId;
    class Window;
}
}

// Gilrs types (gamepad library)
namespace gilrs {
    struct Event;
    enum class EventType;
    class Gilrs;
}

// Custom event types
namespace ruffle {
namespace desktop {
namespace custom_event {
    enum class OpenType;
    enum class RuffleEvent;
}
}
}

// Other dependencies
namespace fontdb {
    class Database;
}

namespace tokio {
namespace runtime {
    class Runtime;
}
}

namespace ruffle {
namespace desktop {

/// Loading state for the main window
enum class LoadingState {
    Loading,
    WaitingForResize,
    Loaded
};

/// Main window structure
struct MainWindow {
    std::shared_ptr<preferences::GlobalPreferences> preferences;
    std::unique_ptr<gui::GuiController> gui;
    std::shared_ptr<ruffle::core::Player> player;
    bool minimized;
    winit::dpi::PhysicalPosition<double> mouse_pos;
    winit::event::Modifiers modifiers;
    winit::dpi::LogicalSize<uint32_t> min_window_size;
    winit::dpi::PhysicalSize<uint32_t> max_window_size;
    bool no_gui;
    std::optional<double> preferred_width;
    std::optional<double> preferred_height;
    bool start_fullscreen;
    LoadingState loaded;
    std::chrono::steady_clock::time_point time;
    std::optional<std::chrono::steady_clock::time_point> next_frame_time;
    winit::event_loop::EventLoopProxy<custom_event::RuffleEvent> event_loop_proxy;

    /// Handle window events
    void window_event(
        const winit::event_loop::ActiveEventLoop* event_loop,
        const winit::event::WindowEvent& event
    );

    /// Handle metadata from SWF header
    void on_metadata(const ruffle::core::swf::HeaderExt& swf_header);

    /// Handle about-to-wait event (core loop)
    void about_to_wait(gilrs::Gilrs* gilrs);

    /// Check if redraw is needed
    void check_redraw() const;
};

/// Main application class
/// Handles the desktop application lifecycle
class App {
private:
    std::optional<MainWindow> main_window;
    std::unique_ptr<tokio::runtime::Runtime> runtime;
    std::optional<gilrs::Gilrs> gilrs;
    winit::event_loop::EventLoopProxy<custom_event::RuffleEvent> event_loop_proxy;
    std::shared_ptr<preferences::GlobalPreferences> preferences;
    fontdb::Database font_database;

public:
    /// Create a new application
    /// 
    /// @param preferences Global preferences
    /// @return Pair of App instance and event loop
    static std::pair<std::unique_ptr<App>, std::unique_ptr<winit::event_loop::EventLoop<custom_event::RuffleEvent>>>
    create(std::shared_ptr<preferences::GlobalPreferences> preferences);

    /// Get the main window
    MainWindow* get_main_window() {
        return main_window.has_value() ? &main_window.value() : nullptr;
    }

    /// Get the event loop proxy
    const winit::event_loop::EventLoopProxy<custom_event::RuffleEvent>& get_event_loop_proxy() const {
        return event_loop_proxy;
    }

    /// Get preferences
    const std::shared_ptr<preferences::GlobalPreferences>& get_preferences() const {
        return preferences;
    }

    /// Get font database
    const fontdb::Database& get_font_database() const {
        return font_database;
    }

    /// Check if gamepad support is available
    bool has_gamepad_support() const {
        return gilrs.has_value();
    }

    /// Get gamepad instance
    gilrs::Gilrs* get_gilrs() {
        return gilrs.has_value() ? &gilrs.value() : nullptr;
    }
};

// Helper functions

/// Convert gilrs button to gamepad button
std::optional<ruffle::core::events::PlayerEvent> gilrs_button_to_gamepad_button(int button);

/// Convert winit input to ruffle key descriptor
std::string winit_input_to_ruffle_key_descriptor(const winit::event::KeyEvent& event);

/// Convert winit event to ruffle text control
std::optional<char> winit_to_ruffle_text_control(
    const winit::event::KeyEvent& event,
    const winit::event::Modifiers& modifiers
);

/// Get screen size for a window
winit::dpi::PhysicalSize<uint32_t> get_screen_size(const winit::window::Window* window);

/// Plot stats in tracy (profiling)
void plot_stats_in_tracy(void* wgpu_instance);

} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_APP_H
