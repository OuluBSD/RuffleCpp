// C++ translation of desktop/src/backends/ui.rs
//! Desktop UI backend implementation

#ifndef RUFFLE_DESKTOP_BACKENDS_UI_H
#define RUFFLE_DESKTOP_BACKENDS_UI_H

#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <functional>
#include <future>
#include <chrono>
#include <filesystem>

// Forward declarations
namespace ruffle {
namespace core {
    namespace backend {
        namespace ui {
            template<typename T>
            class UiBackend;
            class DialogLoaderError;
            class FileDialogResult;
            class FileFilter;
            class FontDefinition;
            class FullscreenError;
            class LanguageIdentifier;
            class MouseCursor;
        }
    }
    namespace font {
        class FontQuery;
    }
}
namespace desktop {
    namespace cli {
        enum class OpenUrlMode;
    }
    namespace custom_event {
        enum class RuffleEvent;
    }
    namespace gui {
        namespace dialogs {
            namespace message_dialog {
                class MessageDialogConfiguration;
            }
        }
        class DialogDescriptor;
        class FilePicker;
        class LocalizableText;
    }
    namespace preferences {
        class GlobalPreferences;
    }
}
}

// External dependencies (forward declared)
namespace winit {
    namespace window {
        class Window;
    }
    namespace event_loop {
        template<typename T>
        class EventLoopProxy;
    }
}

namespace fontdb {
    class Database;
}

namespace ruffle {
namespace desktop {
namespace backends {
namespace ui {

/// Result from a desktop file dialog
class DesktopFileDialogResult : public core::backend::ui::FileDialogResult {
public:
    /// Create a new DesktopFileDialogResult from a file handle
    static std::future<DesktopFileDialogResult> create(std::optional<FileHandle> handle);

    /// Check if the dialog was cancelled
    bool is_cancelled() const override;

    /// Get the file creation time
    std::optional<std::chrono::system_clock::time_point> creation_time() const override;

    /// Get the file modification time
    std::optional<std::chrono::system_clock::time_point> modification_time() const override;

    /// Get the file name
    std::optional<std::string> file_name() const override;

    /// Get the file size in bytes
    std::optional<uint64_t> size() const override;

    /// Get the file type/extension
    std::optional<std::string> file_type() const override;

    /// Get the file contents
    const std::vector<uint8_t>& contents() const override;

    /// Write new data and refresh the internal state
    void write_and_refresh(const std::vector<uint8_t>& data) override;

private:
    DesktopFileDialogResult(
        std::optional<FileHandle> handle,
        std::optional<std::filesystem::file_time_type> metadata,
        std::vector<uint8_t> contents
    );

    std::optional<FileHandle> handle_;
    std::optional<std::filesystem::file_time_type> metadata_;
    std::vector<uint8_t> contents_;
};

/// Desktop UI backend implementing UiBackend interface
class DesktopUiBackend : public core::backend::ui::UiBackend {
public:
    /// Create a new DesktopUiBackend
    ///
    /// \param window The winit window
    /// \param event_loop Event loop proxy for sending events
    /// \param font_database Font database for font lookups
    /// \param preferences Global preferences
    /// \param file_picker File picker for dialogs
    static std::unique_ptr<DesktopUiBackend> create(
        std::shared_ptr<winit::window::Window> window,
        std::shared_ptr<winit::event_loop::EventLoopProxy<custom_event::RuffleEvent>> event_loop,
        std::shared_ptr<fontdb::Database> font_database,
        std::shared_ptr<preferences::GlobalPreferences> preferences,
        gui::FilePicker file_picker
    );

    /// Check if mouse is visible
    bool mouse_visible() const override;

    /// Set mouse visibility
    void set_mouse_visible(bool visible) override;

    /// Set mouse cursor type
    void set_mouse_cursor(core::backend::ui::MouseCursor cursor) override;

    /// Get clipboard content
    std::string clipboard_content() override;

    /// Set clipboard content
    void set_clipboard_content(const std::string& content) override;

    /// Set fullscreen mode
    std::result::result<void, core::backend::ui::FullscreenError> set_fullscreen(bool is_full) override;

    /// Display error message when root movie download fails
    void display_root_movie_download_failed_message(bool invalid_swf, const std::string& fetch_error) override;

    /// Show a message dialog
    void message(const std::string& message) override;

    /// Display unsupported video error and optionally open URL
    void display_unsupported_video(const std::string& url) override;

    /// Load a device font matching the query
    void load_device_font(
        const core::font::FontQuery& query,
        std::function<void(const core::backend::ui::FontDefinition&)> register_callback
    ) override;

    /// Sort device fonts (fontconfig support)
    std::vector<core::font::FontQuery> sort_device_fonts(
        const core::font::FontQuery& query,
        std::function<void(const core::backend::ui::FontDefinition&)> register_callback
    ) override;

    /// Open virtual keyboard (unused on desktop)
    void open_virtual_keyboard() override;

    /// Close virtual keyboard (unused on desktop)
    void close_virtual_keyboard() override;

    /// Get the current language
    core::backend::ui::LanguageIdentifier language() const override;

    /// Display file open dialog
    std::optional<std::future<std::result::result<std::unique_ptr<core::backend::ui::FileDialogResult>, core::backend::ui::DialogLoaderError>>>
    display_file_open_dialog(std::vector<core::backend::ui::FileFilter> filters) override;

    /// Display file save dialog
    std::optional<std::future<std::result::result<std::unique_ptr<core::backend::ui::FileDialogResult>, core::backend::ui::DialogLoaderError>>>
    display_file_save_dialog(const std::string& file_name, const std::string& title) override;

    /// Close file dialog
    void close_file_dialog() override;

    /// Get the egui cursor icon based on current state
    egui::CursorIcon cursor() const;

private:
    DesktopUiBackend(
        std::shared_ptr<winit::window::Window> window,
        std::shared_ptr<winit::event_loop::EventLoopProxy<custom_event::RuffleEvent>> event_loop,
        std::shared_ptr<fontdb::Database> font_database,
        std::shared_ptr<preferences::GlobalPreferences> preferences,
        gui::FilePicker file_picker
    );

    std::shared_ptr<winit::window::Window> window_;
    std::shared_ptr<winit::event_loop::EventLoopProxy<custom_event::RuffleEvent>> event_loop_;
    bool cursor_visible_;
    core::backend::ui::MouseCursor preferred_cursor_;
    std::shared_ptr<fontdb::Database> font_database_;
    std::shared_ptr<preferences::GlobalPreferences> preferences_;
    gui::FilePicker file_picker_;
    Clipboard clipboard_;
};

/// Load a font from a file
///
/// \param path Path to the font file
/// \param name Font name
/// \param index Font index in collection
/// \param is_bold Whether the font is bold
/// \param is_italic Whether the font is italic
/// \return FontDefinition or error
std::result::result<core::backend::ui::FontDefinition, std::error_code>
load_font_from_file(
    const std::filesystem::path& path,
    const std::string& name,
    uint32_t index,
    bool is_bold,
    bool is_italic
);

/// Load a font from fontdb
///
/// \param name Font name
/// \param face Font face info
/// \return FontDefinition or error
std::result::result<core::backend::ui::FontDefinition, std::error_code>
load_fontdb_font(const std::string& name, const fontdb::FaceInfo& face);

#ifdef FONTCONFIG_ENABLED
/// Sort device fonts using fontconfig
///
/// \param query Font query
/// \param register Register callback for found fonts
/// \return List of font queries for registered fonts
std::vector<core::font::FontQuery> fontconfig_sort_device_fonts(
    const core::font::FontQuery& query,
    std::function<void(const core::backend::ui::FontDefinition&)> register
);
#endif

} // namespace ui
} // namespace backends
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_BACKENDS_UI_H
