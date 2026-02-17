// C++ translation of frontend-utils/src/bookmarks.rs
// Bookmarks module for content descriptors

#ifndef RUFFLE_FRONTEND_UTILS_BOOKMARKS_H
#define RUFFLE_FRONTEND_UTILS_BOOKMARKS_H

#include <string>
#include <vector>

// Forward declarations
namespace ruffle {
namespace frontend {
namespace utils {
namespace content {
    struct ContentDescriptor;
}
}
}
}

namespace ruffle {
namespace frontend {
namespace utils {
namespace bookmarks {

/// Represents a bookmark with a content descriptor and name
struct Bookmark {
    /// The content descriptor for the bookmark
    ruffle::frontend::utils::content::ContentDescriptor content_descriptor;
    
    /// The name of the bookmark
    std::string name;
    
    /// Check if the bookmark is invalid
    /// Returns true if the URL matches the invalid URL constant
    bool is_invalid() const;
};

/// Type alias for a collection of bookmarks
using Bookmarks = std::vector<Bookmark>;

/// Read bookmarks from storage
/// Forward declared from read submodule
Bookmarks read_bookmarks();

/// Writer for bookmarks
/// Forward declared from write submodule
class BookmarksWriter;

} // namespace bookmarks
} // namespace utils
} // namespace frontend
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_BOOKMARKS_H
