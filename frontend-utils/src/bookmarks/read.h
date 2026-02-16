// C++ translation of frontend-utils/src/bookmarks/read.rs
// Bookmarks reader for TOML parsing

#ifndef RUFFLE_FRONTEND_UTILS_BOOKMARKS_READ_H
#define RUFFLE_FRONTEND_UTILS_BOOKMARKS_READ_H

#include <string>
#include <vector>

// Forward declarations
namespace ruffle {
namespace frontend {
namespace utils {

class Bookmark;
class Bookmarks;
class ContentDescriptor;

namespace parse {
    class DocumentHolder;
    struct ParseDetails;
    struct ParseWarning;
}

} // namespace utils
} // namespace frontend
} // namespace ruffle

namespace ruffle {
namespace frontend {
namespace utils {

/// Invalid URL constant used as fallback for invalid URLs.
inline constexpr const char* INVALID_URL = "about:invalid";

/// Parse bookmarks from a TOML string.
///
/// @param input The TOML string to parse
/// @return ParseDetails containing the parsed bookmarks and any warnings
parse::ParseDetails<Bookmarks> read_bookmarks(const std::string& input);

} // namespace utils
} // namespace frontend
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_BOOKMARKS_READ_H
