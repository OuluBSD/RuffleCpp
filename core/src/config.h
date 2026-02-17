// C++ translation of core/src/config.rs
// Configuration options for Ruffle

#ifndef RUFFLE_CORE_CONFIG_H
#define RUFFLE_CORE_CONFIG_H

#include <string>
#include <optional>
#include <stdexcept>

namespace ruffle {
namespace core {
namespace config {

/// Controls whether the content is letterboxed or pillarboxed when the
/// player's aspect ratio does not match the movie's aspect ratio.
///
/// When letterboxed, black bars will be rendered around the exterior
/// margins of the content.
enum class Letterbox {
    /// The content will never be letterboxed.
    Off,

    /// The content will only be letterboxed if the content is running fullscreen.
    Fullscreen,

    /// The content will always be letterboxed.
    On
};

/// Convert Letterbox to string
inline std::string letterbox_to_string(Letterbox letterbox) {
    switch (letterbox) {
        case Letterbox::Off: return "off";
        case Letterbox::Fullscreen: return "fullscreen";
        case Letterbox::On: return "on";
    }
    return "off"; // Default
}

/// Parse Letterbox from string
inline Letterbox letterbox_from_string(const std::string& s) {
    if (s == "off") return Letterbox::Off;
    if (s == "fullscreen") return Letterbox::Fullscreen;
    if (s == "on") return Letterbox::On;
    throw std::runtime_error("Invalid Letterbox value");
}

/// The networking API access mode of the Ruffle player.
/// This setting is only used on web.
enum class NetworkingAccessMode {
    /// All networking APIs are permitted in the SWF file.
    All,

    /// The SWF file may not call browser navigation or browser interaction APIs.
    ///
    /// The APIs getURL(), navigateToURL(), fscommand() and ExternalInterface.call()
    /// are prevented in this mode.
    Internal,

    /// The SWF file may not call browser navigation or browser interaction APIs
    /// and it cannot use any SWF-to-SWF communication APIs.
    ///
    /// Additionally to the ones in internal mode, the APIs sendToURL(),
    /// FileReference.download(), FileReference.upload(), Loader.load(),
    /// LocalConnection.connect(), LocalConnection.send(), NetConnection.connect(),
    /// NetStream.play(), Security.loadPolicyFile(), SharedObject.getLocal(),
    /// SharedObject.getRemote(), Socket.connect(), Sound.load(), URLLoader.load(),
    /// URLStream.load() and XMLSocket.connect() are prevented in this mode.
    ///
    /// This mode is not implemented yet.
    None
};

} // namespace config
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_CONFIG_H
