// C++ translation of desktop/src/gui/controller.rs
// Integration layer connecting wgpu+winit to egui

#ifndef RUFFLE_DESKTOP_GUI_CONTROLLER_H
#define RUFFLE_DESKTOP_GUI_CONTROLLER_H

#include <any>
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <optional>
#include <functional>
#include <filesystem>

// Forward declarations
namespace wgpu {
    class Instance;
    class Surface;
    class Adapter;
    class Device;
    class Queue;
    enum class Backends;
    enum class TextureFormat;
    enum class TextureUsages;
    struct SurfaceConfiguration;
    struct CommandEncoderDescriptor;
    struct RenderPassDescriptor;
    struct RenderPassColorAttachment;
    struct Operations;
    struct LoadOp;
    struct StoreOp;
    struct Color;
}

namespace winit {
    namespace dpi {
        struct PhysicalPosition;
        struct PhysicalSize;
    }
    namespace event {
        struct WindowEvent;
        struct KeyEvent;
    }
    namespace keyboard {
        enum class Key;
        enum class NamedKey;
    }
    namespace window {
        enum class Theme;
        enum class ImePurpose;
        class Window;
    }
    namespace event_loop {
        template<typename T>
        class EventLoopProxy;
    }
}

namespace egui {
    class Context;
    struct FontData;
    struct FontDefinitions;
    struct ViewportId;
    enum class FontFamily;
}

namespace egui_wgpu {
    class Renderer;
    struct ScreenDescriptor;
}

namespace egui_winit {
    class State;
}

namespace fontdb {
    class Database;
    enum class Family;
    struct Query;
    enum class Source;
}

namespace unic_langid {
    struct LanguageIdentifier;
}

namespace url {
    class Url;
}

namespace ruffle {
namespace core {
    class Player;
    class ContextMenuItem;
    namespace events {
        struct ImeCursorArea;
        enum class ImePurpose;
    }
}
namespace render {
namespace wgpu {
    namespace backend {
        class WgpuRenderBackend;
    }
    namespace descriptors {
        class Descriptors;
    }
}
}
namespace frontend {
namespace utils {
namespace content {
    struct ContentDescriptor;
}
}
}
namespace desktop {
namespace player {
    struct LaunchOptions;
    class PlayerController;
}
namespace preferences {
    class GlobalPreferences;
}
namespace gui {
namespace theme {
    class ThemeController;
}
namespace movie {
    class MovieView;
    class MovieViewRenderer;
}
namespace dialogs {
    struct ExportBundleDialogConfiguration;
    struct DialogDescriptor;
    class FilePicker;
}
}
}

namespace ruffle {
namespace desktop {
namespace gui {

/// Menu height constant
constexpr int MENU_HEIGHT = 30;

/// Integration layer connecting wgpu+winit to egui.
class GuiController {
private:
    std::shared_ptr<ruffle::render::wgpu::descriptors::Descriptors> descriptors_;
    std::unique_ptr<egui_winit::State> egui_winit_;
    std::unique_ptr<egui_wgpu::Renderer> egui_renderer_;
    std::unique_ptr<RuffleGui> gui_;
    std::shared_ptr<winit::window::Window> window_;
    std::chrono::steady_clock::time_point last_update_;
    std::chrono::duration<double> repaint_after_;
    std::unique_ptr<wgpu::Surface> surface_;
    wgpu::TextureFormat surface_format_;
    std::shared_ptr<ruffle::desktop::gui::movie::MovieViewRenderer> movie_view_renderer_;
    winit::dpi::PhysicalSize<u32> size_;
    bool no_gui_;
    std::unique_ptr<ruffle::desktop::gui::theme::ThemeController> theme_controller_;

public:
    GuiController(
        std::shared_ptr<winit::window::Window> window,
        winit::event_loop::EventLoopProxy<ruffle::desktop::custom_event::RuffleEvent> event_loop,
        const ruffle::desktop::preferences::GlobalPreferences& preferences,
        const fontdb::Database& font_database,
        std::optional<url::Url> initial_movie_url,
        bool no_gui
    );

    void set_theme(winit::window::Theme theme);

    const std::shared_ptr<ruffle::render::wgpu::descriptors::Descriptors>& descriptors() const {
        return descriptors_;
    }

    ruffle::desktop::gui::dialogs::FilePicker file_picker() const;

    const std::shared_ptr<winit::window::Window>& window() const {
        return window_;
    }

    void resize(winit::dpi::PhysicalSize<u32> size);

    void reconfigure_surface();

    bool handle_event(const winit::event::WindowEvent& event);

    void close_movie(ruffle::desktop::player::PlayerController& player);

    void create_movie(
        ruffle::desktop::player::PlayerController& player,
        const ruffle::desktop::player::LaunchOptions& opt,
        const ruffle::frontend::utils::content::ContentDescriptor& content_descriptor
    );

    double height_offset() const;

    std::pair<double, double> window_to_movie_position(const winit::dpi::PhysicalPosition<double>& position) const;

    winit::dpi::PhysicalPosition<double> movie_to_window_position(double x, double y) const;

    void render(std::optional<std::unique_lock<std::mutex>> player_lock);

    void show_context_menu(
        const std::vector<ruffle::core::ContextMenuItem>& menu,
        ruffle::core::events::PlayerEvent close_event
    );

    bool is_context_menu_visible() const;

    bool needs_render() const;

    void show_open_dialog();

    void open_dialog(const ruffle::desktop::gui::dialogs::DialogDescriptor& dialog_event);

    void set_ime_allowed(bool allowed);

    void set_ime_purpose(ruffle::core::events::ImePurpose purpose);

    void set_ime_cursor_area(const ruffle::core::events::ImeCursorArea& cursor_area);

    void export_bundle();
};

// Helper functions

/// Select the appropriate wgpu backend based on preferences
std::pair<std::unique_ptr<wgpu::Instance>, wgpu::Backends>
select_wgpu_backend(wgpu::Backends preferred_backends);

/// Try to create a wgpu instance for a specific backend
std::optional<wgpu::Instance> try_wgpu_backend(wgpu::Backends backend);

/// Load fallback fonts based on locale
egui::FontDefinitions load_system_fonts(
    const fontdb::Database& font_database,
    const unic_langid::LanguageIdentifier& locale
);

} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_CONTROLLER_H
