// C++ translation of desktop/src/backends/external_interface.rs
//! Desktop external interface provider implementation

#ifndef RUFFLE_DESKTOP_BACKENDS_EXTERNAL_INTERFACE_H
#define RUFFLE_DESKTOP_BACKENDS_EXTERNAL_INTERFACE_H

#include <string>
#include <optional>
#include <vector>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace context {
    template<typename GC>
    class UpdateContext;
}
namespace external {
    class Value;
    class ExternalInterfaceProvider;
}
}
}

namespace ruffle {
namespace desktop {
namespace backends {

/// Check if the given code is a location.href reference
/// 
/// Matches: "document.location.href", "window.location.href", "top.location.href"
/// 
/// @param code The code string to check
/// @return true if it's a location.href reference
inline bool is_location_href(const std::string& code) {
    return code == "document.location.href" || 
           code == "window.location.href" || 
           code == "top.location.href";
}

/// Desktop external interface provider
/// 
/// Provides spoofing for location.href access and handles ExternalInterface calls
class DesktopExternalInterfaceProvider : public core::external::ExternalInterfaceProvider {
public:
    /// Create a new desktop external interface provider
    /// @param spoof_url Optional URL to spoof for location.href access
    explicit DesktopExternalInterfaceProvider(std::optional<std::string> spoof_url = std::nullopt)
        : spoof_url_(spoof_url) {}

    /// Call a method through the external interface
    /// 
    /// Handles special cases for location.href spoofing and eval calls
    /// 
    /// @param context The update context
    /// @param name The method name to call
    /// @param args The arguments to pass
    /// @return The result value
    core::external::Value call_method(
        core::context::UpdateContext<void>* context,
        const std::string& name,
        const std::vector<core::external::Value>& args) override;

    /// Called when a callback becomes available
    /// 
    /// @param name The callback name
    void on_callback_available(const std::string& name) override;

    /// Get the ID of this external interface provider
    /// 
    /// @return Optional ID string (always None for desktop)
    std::optional<std::string> get_id() const override;

private:
    /// URL to spoof for location.href access
    std::optional<std::string> spoof_url_;
};

} // namespace backends
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_BACKENDS_EXTERNAL_INTERFACE_H
