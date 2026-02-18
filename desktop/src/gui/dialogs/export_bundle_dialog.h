// C++ translation of desktop/src/gui/dialogs/export_bundle_dialog.rs
// Export bundle dialog for desktop GUI

#ifndef RUFFLE_DESKTOP_GUI_DIALOGS_EXPORT_BUNDLE_DIALOG_H
#define RUFFLE_DESKTOP_GUI_DIALOGS_EXPORT_BUNDLE_DIALOG_H

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>
#include <filesystem>
#include <optional>

// Forward declarations
namespace unic_langid {
    class LanguageIdentifier;
}

namespace url {
    class Url;
}

namespace ruffle {
namespace core {
    // Forward declare core types
}

namespace frontend_utils {
namespace bundle {
namespace info {
    class BundleInformation;
}
}
namespace content {
    class ContentDescriptor;
}
namespace player_options {
    class PlayerOptions;
}
}

namespace desktop {
namespace gui {
    class FilePicker;
}
}
}

namespace rfd {
    class FileHandle;
}

namespace ruffle {
namespace desktop {
namespace gui {
namespace dialogs {

/// Configuration for the export bundle dialog
struct ExportBundleDialogConfiguration {
    frontend_utils::content::ContentDescriptor content_descriptor;
    frontend_utils::player_options::PlayerOptions player_options;

    ExportBundleDialogConfiguration(
        frontend_utils::content::ContentDescriptor desc,
        frontend_utils::player_options::PlayerOptions opts
    )
        : content_descriptor(std::move(desc))
        , player_options(std::move(opts))
    {}
};

/// Represents a local file to be exported
struct LocalFileToExport {
    bool export_flag;
    std::filesystem::path path;
    std::string displayed_path;

    LocalFileToExport() : export_flag(true), path(), displayed_path() {}

    /// Create a new LocalFileToExport
    /// @param file_path The full path to the file
    /// @param base Optional base path for relative display
    static LocalFileToExport create(
        const std::filesystem::path& file_path,
        const std::optional<std::filesystem::path>& base = std::nullopt
    );

    /// Convert a path to a displayed string
    /// @param file_path The path to convert
    /// @param base Optional base path for relative display
    /// @return The displayed path string
    static std::string to_displayed_path(
        const std::filesystem::path& file_path,
        const std::optional<std::filesystem::path>& base = std::nullopt
    );
};

/// Export status enumeration
enum class ExportStatus {
    Idle,
    Exporting,
    Success,
    FailedUserCanceled,
    FailedToShowPicker,
    FailedIoError,
    FailedOtherError
};

/// Thread-safe export status wrapper
class AtomicExportStatus {
private:
    std::shared_ptr<std::mutex> mutex_;
    ExportStatus status_;

public:
    explicit AtomicExportStatus(ExportStatus initial_status = ExportStatus::Idle);

    ExportStatus get() const;
    void set(ExportStatus status);

    /// Check if UI should be enabled for this status
    bool ui_enabled() const;
};

/// Export bundle dialog for creating Ruffle bundles
class ExportBundleDialog {
private:
    ExportBundleDialogConfiguration config_;
    FilePicker picker_;
    std::string bundle_name_;
    bool bundle_local_files_;
    std::vector<LocalFileToExport> local_files_;
    std::shared_ptr<AtomicExportStatus> export_status_;

public:
    /// Create a new export bundle dialog
    /// @param config The dialog configuration
    /// @param picker The file picker to use
    ExportBundleDialog(
        ExportBundleDialogConfiguration config,
        FilePicker picker
    );

    /// Get the suggested bundle name from a URL
    /// @param url The source URL
    /// @return The suggested bundle name
    static std::string suggested_name(const url::Url& url);

    /// Show the dialog
    /// @param locale The locale for localization
    /// @param egui_ctx The egui context
    /// @return true if the dialog should remain open
    bool show(
        const unic_langid::LanguageIdentifier& locale,
        void* egui_ctx  // egui::Context - forward declared
    );

private:
    /// Render the window contents
    /// @param locale The locale for localization
    /// @param ui The UI to render to
    /// @return true if the dialog should close
    bool render_window_contents(
        const unic_langid::LanguageIdentifier& locale,
        void* ui  // egui::Ui - forward declared
    );

    /// Render the info section
    void render_info(
        const unic_langid::LanguageIdentifier& locale,
        void* ui
    );

    /// Render the status section
    void render_status(
        ExportStatus status,
        const unic_langid::LanguageIdentifier& locale,
        void* ui
    );

    /// Render the local files section
    void render_local_files(
        const unic_langid::LanguageIdentifier& locale,
        void* ui
    );

    /// Trigger the export process
    /// @return The initial export status
    ExportStatus trigger_export();

    /// Perform the actual export (async)
    /// @param selected_file The selected output file
    /// @param bundle_name The bundle name
    /// @param player_options The player options
    /// @param movie_url The movie URL
    /// @param exported_files The files to export
    /// @return The final export status
    static ExportStatus perform_export(
        std::optional<rfd::FileHandle> selected_file,
        const std::string& bundle_name,
        const frontend_utils::player_options::PlayerOptions& player_options,
        const url::Url& movie_url,
        const std::vector<std::filesystem::path>& exported_files
    );
};

} // namespace dialogs
} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_DIALOGS_EXPORT_BUNDLE_DIALOG_H
