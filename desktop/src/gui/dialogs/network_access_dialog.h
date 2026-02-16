// C++ translation of desktop/src/gui/dialogs/network_access_dialog.rs
// Dialog for requesting network access permission

#ifndef RUFFLE_DESKTOP_GUI_DIALOGS_NETWORK_ACCESS_DIALOG_H
#define RUFFLE_DESKTOP_GUI_DIALOGS_NETWORK_ACCESS_DIALOG_H

#include <string>
#include <optional>
#include <memory>

// Forward declarations
namespace unic_langid {
    class LanguageIdentifier;
}

// egui forward declarations
namespace egui {
    class Context;
    class Ui;
}

// tokio forward declarations
namespace tokio {
namespace sync {
namespace oneshot {
    template<typename T>
    class Sender;
}
}
}

namespace ruffle {
namespace desktop {
namespace gui {
namespace dialogs {

/// Result of the network access dialog
enum class NetworkAccessDialogResult {
    Allow,
    Cancel,
};

/// Configuration for the network access dialog
class NetworkAccessDialogConfiguration {
private:
    std::optional<tokio::sync::oneshot::Sender<NetworkAccessDialogResult>> notifier_;
    std::string host_;
    uint16_t port_;

public:
    /// Create a new network access dialog configuration
    /// @param notifier The sender to notify of the result
    /// @param host The host requesting network access
    /// @param port The port requesting network access
    NetworkAccessDialogConfiguration(
        tokio::sync::oneshot::Sender<NetworkAccessDialogResult> notifier,
        std::string host,
        uint16_t port
    )
        : notifier_(std::move(notifier))
        , host_(std::move(host))
        , port_(port) {}

    // Getters
    const std::optional<tokio::sync::oneshot::Sender<NetworkAccessDialogResult>>& notifier() const { return notifier_; }
    const std::string& host() const { return host_; }
    uint16_t port() const { return port_; }

    // Setters for mutable access
    std::optional<tokio::sync::oneshot::Sender<NetworkAccessDialogResult>>& notifier() { return notifier_; }
};

/// Dialog for requesting network access permission
/// Shows the host and port requesting access and asks the user to allow or cancel
class NetworkAccessDialog {
private:
    NetworkAccessDialogConfiguration config_;

    /// Send the result to the notifier
    void respond(NetworkAccessDialogResult result);

public:
    /// Create a new network access dialog
    /// @param config The dialog configuration
    explicit NetworkAccessDialog(NetworkAccessDialogConfiguration config)
        : config_(std::move(config)) {}

    /// Destructor - sends Cancel result if not already responded
    ~NetworkAccessDialog();

    /// Show the dialog
    /// @param locale The language identifier for localization
    /// @param egui_ctx The egui context
    /// @return true if dialog should remain open, false if closed
    bool show(const unic_langid::LanguageIdentifier& locale, egui::Context* egui_ctx);

    /// Render the window contents
    /// @param locale The language identifier for localization
    /// @param ui The UI to render to
    /// @return true if dialog should close
    bool render_window_contents(const unic_langid::LanguageIdentifier& locale, egui::Ui* ui);

    // Getters
    const NetworkAccessDialogConfiguration& config() const { return config_; }
};

} // namespace dialogs
} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_DIALOGS_NETWORK_ACCESS_DIALOG_H
