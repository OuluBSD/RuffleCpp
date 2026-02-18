// C++ translation of web/src/ui.rs
// Web UI Backend

#ifndef RUFFLE_WEB_UI_H
#define RUFFLE_WEB_UI_H

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <functional>
#include <chrono>

// Forward declarations
namespace ruffle {
namespace core {
namespace backend {
namespace ui {
    class UiBackend;
    class FileDialogResult;
    class DialogResultFuture;
    enum class MouseCursor;
    enum class FullscreenError;
    struct FontDefinition;
    struct FontQuery;
    struct FileFilter;
    class DialogLoaderError;
}
}
}
}

namespace url {
    class Url;
}

namespace chrono {
    template<typename T> class time_point;
    class utc_clock;
}

namespace ruffle {
namespace web {

// Forward declaration
class JavascriptPlayer;

/// Web file dialog result implementation
class WebFileDialogResult {
private:
    bool canceled;
    std::optional<std::string> file_name;
    std::optional<std::chrono::time_point<chrono::utc_clock>> modification_time;
    std::vector<uint8_t> contents;

public:
    WebFileDialogResult(bool canceled = true,
                       const std::optional<std::string>& file_name = std::nullopt,
                       const std::optional<std::chrono::time_point<chrono::utc_clock>>& mod_time = std::nullopt,
                       const std::vector<uint8_t>& contents = {});

    /// Create from async file dialog handle
    static WebFileDialogResult from_pick(const void* handle);
    
    /// Create for download
    static WebFileDialogResult for_download(const std::string& file_name);

    bool is_cancelled() const { return canceled; }
    std::optional<std::chrono::time_point<chrono::utc_clock>> creation_time() const { return std::nullopt; }
    std::optional<std::chrono::time_point<chrono::utc_clock>> modification_time() const { return modification_time; }
    std::optional<std::string> file_name() const { return file_name; }
    std::optional<uint64_t> size() const { return contents.size(); }
    std::optional<std::string> file_type() const;
    const std::vector<uint8_t>& contents() const { return contents; }
    
    /// Write data and refresh
    void write_and_refresh(const std::vector<uint8_t>& data);
};

/// Get extension from filename
std::optional<std::string> get_extension_from_filename(const std::string& filename);

/// Download data as file
void download_as_file(const std::optional<std::string>& filename, const std::vector<uint8_t>& data);

/// An implementation of UiBackend utilizing web bindings to input APIs.
class WebUiBackend {
private:
    JavascriptPlayer* js_player;
    void* canvas;  // HtmlCanvasElement equivalent
    bool cursor_visible;
    core::backend::ui::MouseCursor cursor;
    std::string language;
    std::string clipboard_content;
    bool dialog_open;
    bool use_canvas_font_renderer;

public:
    /// Create a new web UI backend
    /// @param js_player Reference to JavaScript player
    /// @param canvas Canvas element
    /// @param use_canvas_font_renderer Whether to use canvas font renderer
    WebUiBackend(JavascriptPlayer* js_player, void* canvas, bool use_canvas_font_renderer);

    /// Update mouse cursor appearance
    void update_mouse_cursor();

    /// Set clipboard content buffer
    void set_clipboard_content_buffer(const std::string& content);
};

// UiBackend implementation
class WebUiBackendImpl : public core::backend::ui::UiBackend {
private:
    WebUiBackend backend;

public:
    WebUiBackendImpl(JavascriptPlayer* js_player, void* canvas, bool use_canvas_font_renderer)
        : backend(js_player, canvas, use_canvas_font_renderer) {}

    bool mouse_visible() const override;
    void set_mouse_visible(bool visible) override;
    void set_mouse_cursor(core::backend::ui::MouseCursor cursor) override;
    std::string clipboard_content() override;
    bool clipboard_available() override;
    void set_clipboard_content(const std::string& content) override;
    
    void set_fullscreen(bool is_full) override;
    void display_root_movie_download_failed_message(bool invalid_swf, const std::string& fetch_error) override;
    void message(const std::string& message) override;
    void open_virtual_keyboard() override;
    void close_virtual_keyboard() override;
    std::string language() const override;
    void display_unsupported_video(const url::Url& url) override;
    
    void load_device_font(const core::backend::ui::FontQuery& query,
                         std::function<void(const core::backend::ui::FontDefinition&)> register_fn) override;
    
    std::vector<core::backend::ui::FontQuery> sort_device_fonts(
        const core::backend::ui::FontQuery& query,
        std::function<void(const core::backend::ui::FontDefinition&)> register_fn) override;
    
    std::optional<core::backend::ui::DialogResultFuture> display_file_open_dialog(
        const std::vector<core::backend::ui::FileFilter>& filters) override;
    
    void close_file_dialog() override;
    
    std::optional<core::backend::ui::DialogResultFuture> display_file_save_dialog(
        const std::string& file_name, const std::string& title) override;
};

} // namespace web
} // namespace ruffle

#endif // RUFFLE_WEB_UI_H
