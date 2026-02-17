// C++ translation of desktop/src/gui/widgets/path_or_url_field.rs
// Path or URL field widget for Ruffle GUI

#ifndef RUFFLE_DESKTOP_GUI_WIDGETS_PATH_OR_URL_FIELD_H
#define RUFFLE_DESKTOP_GUI_WIDGETS_PATH_OR_URL_FIELD_H

#include <string>
#include <memory>
#include <mutex>
#include <optional>
#include <filesystem>

// Forward declarations
namespace unic_langid {
    class LanguageIdentifier;
}

namespace egui {
    class Ui;
    class Layout;
    class Align;
    class TextEdit;
}

namespace ruffle {
namespace frontend_utils {
namespace content {
    class ContentDescriptor;
}
}
namespace desktop {
namespace gui {
    class FilePicker;
    class LocalizableText;
}
}
}

namespace ruffle {
namespace desktop {
namespace gui {
namespace widgets {

/// Internal value storage for PathOrUrlField
class PathOrUrlFieldValue {
private:
    std::optional<ruffle::frontend_utils::content::ContentDescriptor> content_descriptor_;
    std::string representation_;

public:
    PathOrUrlFieldValue() = default;

    /// Create from a ContentDescriptor
    ///
    /// @param value The content descriptor to initialize from
    explicit PathOrUrlFieldValue(const ruffle::frontend_utils::content::ContentDescriptor& value);

    /// Create from a file path
    ///
    /// @param path The file path
    static PathOrUrlFieldValue from_path(const std::filesystem::path& path);

    /// Create from a string (path or URL)
    ///
    /// @param string The string representation
    static PathOrUrlFieldValue from_string(const std::string& string);

    /// Create from a picked directory and content path
    ///
    /// @param directory The directory to display
    /// @param content The actual content path
    static PathOrUrlFieldValue from_picked_directory(
        const std::filesystem::path& directory,
        const std::filesystem::path& content
    );

    /// Convert a path to its display representation
    ///
    /// @param path The path to convert
    /// @return String representation (filename only if available)
    static std::string path_to_representation(const std::filesystem::path& path);

    /// Get the content descriptor
    const std::optional<ruffle::frontend_utils::content::ContentDescriptor>& content_descriptor() const {
        return content_descriptor_;
    }

    /// Get the string representation
    const std::string& representation() const {
        return representation_;
    }

    /// Set the string representation
    void set_representation(const std::string& representation) {
        representation_ = representation;
    }

    /// Set the content descriptor
    void set_content_descriptor(
        const std::optional<ruffle::frontend_utils::content::ContentDescriptor>& descriptor
    ) {
        content_descriptor_ = descriptor;
    }
};

/// A field widget for entering a file path or URL
class PathOrUrlField {
private:
    FilePicker picker_;
    std::shared_ptr<std::mutex> value_mutex_;
    std::shared_ptr<PathOrUrlFieldValue> value_;
    LocalizableText hint_;

    /// Lock the value mutex and return a guard
    ///
    /// @param value The value to lock
    /// @return Locked reference to the value
    static PathOrUrlFieldValue& lock_value(const std::shared_ptr<PathOrUrlFieldValue>& value);

public:
    /// Create a new PathOrUrlField
    ///
    /// @param default_content Optional default content descriptor
    /// @param hint The hint text to display
    /// @param picker The file picker to use
    PathOrUrlField(
        std::optional<ruffle::frontend_utils::content::ContentDescriptor> default_content,
        const LocalizableText& hint,
        const FilePicker& picker
    );

    /// Render the UI for this field
    ///
    /// @param locale The current locale for localization
    /// @param ui The egui UI context
    /// @return Reference to self for chaining
    PathOrUrlField& ui(const unic_langid::LanguageIdentifier& locale, egui::Ui* ui);

    /// Get the current result (content descriptor)
    ///
    /// @return Optional content descriptor, or nullopt if invalid
    std::optional<ruffle::frontend_utils::content::ContentDescriptor> result() const;
};

} // namespace widgets
} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_WIDGETS_PATH_OR_URL_FIELD_H
