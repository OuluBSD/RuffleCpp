// C++ translation of frontend-utils/src/recents.rs
// Recent entries functionality

#ifndef RUFFLE_FRONTEND_UTILS_RECENTS_H
#define RUFFLE_FRONTEND_UTILS_RECENTS_H

#include <vector>
#include <string>

// Forward declarations
namespace ruffle {
namespace frontend {
namespace utils {
namespace content {
    class ContentDescriptor;
}
}
}
}

namespace ruffle {
namespace frontend {
namespace utils {
namespace recents {

using ContentDescriptor = content::ContentDescriptor;

/// Recent entry structure
/// Represents a single recent file entry
struct Recent {
    ContentDescriptor content_descriptor;
    std::string name;

    Recent() = default;
    Recent(ContentDescriptor desc, std::string n)
        : content_descriptor(std::move(desc))
        , name(std::move(n))
    {}

    /// Check if a recent entry is invalid
    /// @return true if the URL is the invalid URL placeholder
    bool is_invalid() const {
        return content_descriptor.url == INVALID_URL;
    }

    /// Checks if a recent entry is available.
    /// If the URL is a local file, it will be checked if it exists, otherwise returns true.
    /// @return true if the content is available
#ifdef RUFFLE_FEATURE_FS
    bool is_available() const {
        if (content_descriptor.url.scheme() == "file") {
            // Would need to convert URL to file path and check existence
            // This is a placeholder - actual implementation would use std::filesystem
            return false; // TODO: Implement file existence check
        }
        return true;
    }
#endif
};

/// Recent entries, stored from oldest to newest.
using Recents = std::vector<Recent>;

} // namespace recents
} // namespace utils
} // namespace frontend
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_RECENTS_H
