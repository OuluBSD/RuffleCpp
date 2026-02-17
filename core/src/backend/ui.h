// C++ translation of core/src/backend/ui.rs
// UI backend interface for Ruffle

#ifndef RUFFLE_CORE_BACKEND_UI_H
#define RUFFLE_CORE_BACKEND_UI_H

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <functional>
#include <cstdint>
#include <any>

// Forward declarations
namespace ruffle {
namespace core {

namespace font {
    struct FontFileData;
    struct FontQuery;
    class FontRenderer;
}

namespace loader {
    class Error;
}

} // namespace core
} // namespace ruffle

namespace chrono {
    template<typename T>
    class time_point;
    struct utc_clock;
}

namespace url {
    class Url;
}

namespace fluent_templates {
    class LanguageIdentifier;
}

namespace swf {
    template<typename T>
    struct Font;
    enum class Encoding;
}

namespace ruffle {
namespace core {
namespace backend {
namespace ui {

using DialogLoaderError = ruffle::core::loader::Error;
using DialogResultFuture = std::any;  // Placeholder for OwnedFuture

/// US English language identifier
extern const fluent_templates::LanguageIdentifier US_ENGLISH;

/// A filter specifying a category that can be selected from a file chooser dialog
struct FileFilter {
    /// The description of the category
    std::string description;
    
    /// A semicolon ';' delimited list of acceptable windows file extensions
    /// with a */wildcard before each extension
    std::string extensions;
    
    /// A semicolon ';' delimited list of acceptable MacOS file extensions
    /// Note that a list of file filters will either all have Some(_) mac_type 
    /// or all will have None
    std::optional<std::string> mac_type;

    /// Returns extensions suitable for file dialogs
    /// @param is_mac True when the user uses a Mac
    std::vector<std::string> extensions_for_dialog(bool is_mac) const {
        if (is_mac && mac_type.has_value()) {
            // Split mac_type by semicolon
            std::vector<std::string> result;
            size_t start = 0;
            size_t end = mac_type->find(';');
            while (end != std::string::npos) {
                result.push_back(mac_type->substr(start, end - start));
                start = end + 1;
                end = mac_type->find(';', start);
            }
            result.push_back(mac_type->substr(start));
            return result;
        }

        // Split extensions by semicolon and strip wildcards
        std::vector<std::string> result;
        size_t start = 0;
        size_t end = extensions.find(';');
        while (end != std::string::npos) {
            std::string ext = extensions.substr(start, end - start);
            // Strip leading "*."
            if (ext.size() > 2 && ext[0] == '*' && ext[1] == '.') {
                ext = ext.substr(2);
            }
            result.push_back(ext);
            start = end + 1;
            end = extensions.find(';', start);
        }
        std::string ext = extensions.substr(start);
        if (ext.size() > 2 && ext[0] == '*' && ext[1] == '.') {
            ext = ext.substr(2);
        }
        result.push_back(ext);
        return result;
    }
};

/// A result of a file selection
class FileDialogResult {
public:
    virtual ~FileDialogResult() = default;

    /// Was the file selection canceled by the user
    virtual bool is_cancelled() const = 0;

    virtual std::optional<chrono::time_point<chrono::utc_clock>> creation_time() const {
        return std::nullopt;
    }

    virtual std::optional<chrono::time_point<chrono::utc_clock>> modification_time() const {
        return std::nullopt;
    }

    virtual std::optional<std::string> file_name() const {
        return std::nullopt;
    }

    virtual std::optional<uint64_t> size() const {
        return std::nullopt;
    }

    virtual std::optional<std::string> file_type() const {
        return std::nullopt;
    }

    virtual std::optional<std::string> creator() const {
        return std::nullopt;
    }

    /// Get the file contents
    virtual const std::vector<uint8_t>& contents() const = 0;

    /// Write the given data to the chosen file and refresh any internal metadata
    /// @param data The data to write
    virtual void write_and_refresh(const std::vector<uint8_t>& data) = 0;
};

/// A mouse cursor icon displayed by the Flash Player
enum class MouseCursor {
    /// The default arrow icon (AS3 MouseCursor.ARROW)
    Arrow,

    /// The hand icon indicating a button or link (AS3 MouseCursor.BUTTON)
    Hand,

    /// The text I-beam (AS3 MouseCursor.IBEAM)
    IBeam,

    /// The grabby-dragging hand icon (AS3 MouseCursor.HAND)
    Grab
};

/// Font definition types
enum class FontDefinitionType {
    /// A singular DefineFont tag extracted from a swf
    SwfTag,

    /// A font contained in an external file, such as a ttf
    FontFile,

    /// Font rendered externally
    ExternalRenderer
};

/// A filter specifying a font definition
struct FontDefinition {
    FontDefinitionType type;

    // For SwfTag
    std::shared_ptr<swf::Font<void>> swf_font;
    const swf::Encoding* encoding;

    // For FontFile
    std::string font_name;
    bool is_bold;
    bool is_italic;
    font::FontFileData font_data;
    uint32_t index;

    // For ExternalRenderer
    std::string external_name;
    std::unique_ptr<font::FontRenderer> font_renderer;
};

/// Fullscreen error type
using FullscreenError = std::string;

/// UI backend trait interface
class UiBackend {
public:
    virtual ~UiBackend() = default;

    /// Check if mouse is visible
    virtual bool mouse_visible() const = 0;

    /// Set mouse visibility
    /// @param visible Whether the mouse should be visible
    virtual void set_mouse_visible(bool visible) = 0;

    /// Changes the mouse cursor image
    /// @param cursor The cursor to display
    virtual void set_mouse_cursor(MouseCursor cursor) = 0;

    /// Get the clipboard content
    virtual std::string clipboard_content() = 0;

    /// Check if the clipboard is available and not empty
    virtual bool clipboard_available() {
        return !clipboard_content().empty();
    }

    /// Sets the clipboard to the given content
    /// @param content The content to set
    virtual void set_clipboard_content(const std::string& content) = 0;

    /// Set fullscreen mode
    /// @param is_full Whether to enter fullscreen mode
    /// @return Result or error
    virtual std::optional<FullscreenError> set_fullscreen(bool is_full) = 0;

    /// Displays a message about an error during root movie download
    /// @param invalid_swf Whether the SWF is invalid
    /// @param fetched_error The fetch error message
    virtual void display_root_movie_download_failed_message(bool invalid_swf, 
                                                           const std::string& fetched_error) = 0;

    /// Display a message (unused but kept for future use)
    /// @param message The message to display
    virtual void message(const std::string& message) = 0;

    /// Open the virtual keyboard
    virtual void open_virtual_keyboard() = 0;

    /// Close the virtual keyboard
    virtual void close_virtual_keyboard() = 0;

    /// Get the current language
    virtual fluent_templates::LanguageIdentifier language() const = 0;

    /// Display unsupported video message
    /// @param url The URL of the unsupported video
    virtual void display_unsupported_video(const url::Url& url) = 0;

    /// Called when a previously unknown device font is requested by a movie
    /// @param query The font query
    /// @param register_callback Callback to register found fonts
    virtual void load_device_font(
        const font::FontQuery& query,
        std::function<void(const FontDefinition&)> register_callback
    ) = 0;

    /// Sort device fonts
    /// @param query The font query
    /// @param register_callback Callback to register found fonts
    /// @return Vector of font queries
    virtual std::vector<font::FontQuery> sort_device_fonts(
        const font::FontQuery& query,
        std::function<void(const FontDefinition&)> register_callback
    ) = 0;

    /// Displays a file selection dialog
    /// @param filters List of filters for possible file types
    /// @return Optional future for file selection, None if dialog cannot be displayed
    virtual std::optional<DialogResultFuture> display_file_open_dialog(
        const std::vector<FileFilter>& filters
    ) = 0;

    /// Display a dialog allowing a user to select a destination to save a file
    /// @param file_name Suggested file name
    /// @param title Title to display in the dialog
    /// @return Optional future for file selection
    virtual std::optional<DialogResultFuture> display_file_save_dialog(
        const std::string& file_name,
        const std::string& title
    ) = 0;

    /// Mark that any previously open dialog has been closed
    virtual void close_file_dialog() = 0;
};

/// UiBackend that does nothing (null object pattern)
class NullUiBackend : public UiBackend {
public:
    NullUiBackend() = default;

    bool mouse_visible() const override { return true; }
    void set_mouse_visible(bool) override {}
    void set_mouse_cursor(MouseCursor) override {}
    std::string clipboard_content() override { return ""; }
    void set_clipboard_content(const std::string&) override {}
    
    std::optional<FullscreenError> set_fullscreen(bool) override {
        return std::nullopt;
    }

    void display_root_movie_download_failed_message(bool, const std::string&) override {}
    void message(const std::string&) override {}
    void display_unsupported_video(const url::Url&) override {}
    
    void load_device_font(
        const font::FontQuery&,
        std::function<void(const FontDefinition&)>
    ) override {}

    std::vector<font::FontQuery> sort_device_fonts(
        const font::FontQuery&,
        std::function<void(const FontDefinition&)>
    ) override {
        return {};
    }

    void open_virtual_keyboard() override {}
    void close_virtual_keyboard() override {}

    fluent_templates::LanguageIdentifier language() const override {
        return US_ENGLISH;
    }

    std::optional<DialogResultFuture> display_file_open_dialog(
        const std::vector<FileFilter>&
    ) override {
        // Return a completed future with NullFileDialogResult
        return std::any();  // Placeholder
    }

    void close_file_dialog() override {}

    std::optional<DialogResultFuture> display_file_save_dialog(
        const std::string&,
        const std::string&
    ) override {
        return std::nullopt;
    }
};

/// Null file dialog result (always cancelled)
class NullFileDialogResult : public FileDialogResult {
public:
    NullFileDialogResult() = default;

    bool is_cancelled() const override { return true; }
    const std::vector<uint8_t>& contents() const override {
        static const std::vector<uint8_t> empty;
        return empty;
    }
    void write_and_refresh(const std::vector<uint8_t>&) override {}
};

} // namespace ui
} // namespace backend
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_BACKEND_UI_H
