// C++ translation of tests/framework/src/backends/ui.rs
// Test UI backend

#ifndef RUFFLE_TESTS_FRAMEWORK_BACKENDS_UI_H
#define RUFFLE_TESTS_FRAMEWORK_BACKENDS_UI_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>
#include <chrono>
#include <future>

// Forward declarations
namespace url {
    class Url;
}

namespace ruffle {
namespace core {
namespace font {
    class FontQuery;
    struct FontFileData;
}
namespace backend {
namespace ui {
    class UiBackend;
    class FileDialogResult;
    struct FileFilter;
    struct FontDefinition;
    enum class MouseCursor;
    enum class FullscreenError;
    using LanguageIdentifier = std::string;
    using DialogResultFuture = std::future<std::unique_ptr<FileDialogResult>>;
}
}
}
}

namespace ruffle {
namespace tests {
namespace framework {
namespace test {
    class Font;
}
namespace backends {
namespace ui {

/// A simulated file dialog response, for use in tests
///
/// Currently this can only simulate either a user cancellation result,
/// or a successful file selection
class TestFileDialogResult : public core::backend::ui::FileDialogResult {
private:
    bool canceled_;
    std::optional<std::string> file_name_;
    std::vector<uint8_t> contents_;

public:
    TestFileDialogResult()
        : canceled_(false), file_name_(std::nullopt), contents_() {}

    /// Create a canceled result
    static TestFileDialogResult new_canceled() {
        TestFileDialogResult result;
        result.canceled_ = true;
        result.file_name_ = std::nullopt;
        result.contents_.clear();
        return result;
    }

    /// Create a success result with the given file name
    ///
    /// @param file_name The file name to return
    static TestFileDialogResult new_success(const std::string& file_name) {
        TestFileDialogResult result;
        result.canceled_ = false;
        result.file_name_ = file_name;
        result.contents_ = std::vector<uint8_t>(
            reinterpret_cast<const uint8_t*>("Hello, World!"),
            reinterpret_cast<const uint8_t*>("Hello, World!") + 13
        );
        return result;
    }

    bool is_cancelled() const override { return canceled_; }

    std::optional<std::chrono::system_clock::time_point> creation_time() const override {
        return std::nullopt;
    }

    std::optional<std::chrono::system_clock::time_point> modification_time() const override {
        return std::nullopt;
    }

    std::optional<std::string> file_name() const override { return file_name_; }

    std::optional<uint64_t> size() const override {
        return static_cast<uint64_t>(contents_.size());
    }

    std::optional<std::string> file_type() const override {
        return (!canceled_) ? std::optional<std::string>(".txt") : std::nullopt;
    }

    const std::vector<uint8_t>& contents() const override { return contents_; }

    void write_and_refresh(const std::vector<uint8_t>& data) override {
        contents_ = data;
    }
};

/// This is an implementation of UiBackend, designed for use in tests
///
/// Fundamentally, this is mostly the same as NullUiBackend with the following differences:
/// - Attempting to display an open dialog with a filter with description "debug-select-success"
///   will simulate successfully selecting a file, otherwise a user cancellation will be simulated
/// - Attempting to display a file save dialog with a file name hint of "debug-success.txt"
///   will simulate successfully selecting a destination, otherwise a user cancellation will be simulated
/// - Simulated in-memory clipboard
class TestUiBackend : public core::backend::ui::UiBackend {
private:
    std::unordered_map<core::font::FontQuery, test::Font> fonts_;
    std::unordered_map<core::font::FontQuery, std::vector<core::font::FontQuery>> font_sorts_;
    std::string clipboard_;

public:
    /// Create a new test UI backend
    ///
    /// @param fonts Map of font queries to fonts
    /// @param font_sorts Map of font queries to sorted font queries
    TestUiBackend(
        std::unordered_map<core::font::FontQuery, test::Font> fonts,
        std::unordered_map<core::font::FontQuery, std::vector<core::font::FontQuery>> font_sorts
    );

    bool mouse_visible() const override { return true; }

    void set_mouse_visible(bool visible) override {}

    void set_mouse_cursor(core::backend::ui::MouseCursor cursor) override {}

    std::string clipboard_content() override { return clipboard_; }

    void set_clipboard_content(const std::string& content) override {
        clipboard_ = content;
    }

    std::result_t<void, core::backend::ui::FullscreenError> set_fullscreen(bool is_full) override {
        return {};
    }

    void display_root_movie_download_failed_message(bool invalid_swf, const std::string& fetch_error) override {}

    void message(const std::string& message) override {}

    void open_virtual_keyboard() override {}

    void close_virtual_keyboard() override {}

    core::backend::ui::LanguageIdentifier language() const override;

    void display_unsupported_video(const url::Url& url) override {}

    void load_device_font(
        const core::font::FontQuery& query,
        std::function<void(const core::backend::ui::FontDefinition&)> register_fn
    ) override;

    std::vector<core::font::FontQuery> sort_device_fonts(
        const core::font::FontQuery& query,
        std::function<void(const core::backend::ui::FontDefinition&)> register_fn
    ) override;

    std::optional<core::backend::ui::DialogResultFuture> display_file_open_dialog(
        std::vector<core::backend::ui::FileFilter> filters
    ) override;

    std::optional<core::backend::ui::DialogResultFuture> display_file_save_dialog(
        const std::string& file_name,
        const std::string& title
    ) override;

    void close_file_dialog() override {}
};

} // namespace ui
} // namespace backends
} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_BACKENDS_UI_H
