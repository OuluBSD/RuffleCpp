// C++ translation of core/common/src/sandbox.rs
// Security Sandbox implementation

#ifndef RUFFLE_CORE_COMMON_SANDBOX_H
#define RUFFLE_CORE_COMMON_SANDBOX_H

#include <cstdint>
#include <string>
#include <functional>

// Forward declarations
namespace swf {
    struct HeaderExt;
}

namespace ruffle {
namespace common {
namespace sandbox {

/// Type of sandbox that defines what a movie can access
/// and how movies interact with each other.
///
/// Note: sandbox type is defined *per SWF*.
///
/// Security Sandbox implementation, see
/// https://help.adobe.com/en_US/as3/dev/WS5b3ccc516d4fbf351e63e3d118a9b90204-7e3f.html
enum class SandboxType : uint8_t {
    /// The movie originates from a remote URL.
    ///
    /// In this case domain-based sandbox rules are used,
    /// no filesystem access.
    Remote,

    /// The movie is a local movie with filesystem access.
    ///
    /// This implies no network access.
    LocalWithFile,

    /// The movie is a local movie with network access.
    ///
    /// This implies no filesystem access.
    LocalWithNetwork,

    /// The movie is a trusted local movie with access to both filesystem and network.
    LocalTrusted,

    /// The movie is an AIR application with access to both filesystem and network.
    Application,
};

/// Sandbox type utilities
class SandboxTypeUtils {
public:
    /// Infer sandbox type based on SWF URL and its header.
    ///
    /// When the URL is remote, SandboxType::Remote is used.
    /// When the URL is local, SandboxType::LocalWithFile or
    /// SandboxType::LocalWithNetwork is used depending on
    /// the preference from the header.
    ///
    /// @param url The URL of the SWF file
    /// @param header The SWF header extension
    /// @return The inferred sandbox type
    static SandboxType infer(const std::string& url, const swf::HeaderExt& header);
};

} // namespace sandbox
} // namespace common
} // namespace ruffle

#endif // RUFFLE_CORE_COMMON_SANDBOX_H
