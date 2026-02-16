// C++ translation of frontend-utils/src/lib.rs
// Frontend utilities library

#ifndef RUFFLE_FRONTEND_UTILS_LIB_H
#define RUFFLE_FRONTEND_UTILS_LIB_H

#include <string_view>
#include <string>

// Forward declarations
namespace ruffle {
namespace frontend {
namespace utils {

// Submodules
namespace bookmarks {
    // Bookmarks functionality
}

namespace bundle {
    // Bundle functionality
}

namespace parse {
    // Parse functionality
}

namespace recents {
    // Recents functionality
}

namespace write {
    // Write functionality
}

namespace backends {
    // Backends functionality
}

namespace content {
    // Content functionality
}

namespace player_options {
    // Player options functionality
}

/// Invalid URL constant
/// Used as a placeholder for invalid URLs
inline constexpr const char* INVALID_URL = "invalid:///";

/// Convert a URL to a human-readable name
/// Extracts the last path segment and URL-decodes it
/// @param url The URL to convert
/// @return A human-readable name for the URL
std::string url_to_readable_name(std::string_view url);

} // namespace utils
} // namespace frontend
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_LIB_H
