// C++ translation of frontend-utils/src/bookmarks/write.rs
// Bookmarks writer for TOML serialization

#ifndef RUFFLE_FRONTEND_UTILS_BOOKMARKS_WRITE_H
#define RUFFLE_FRONTEND_UTILS_BOOKMARKS_WRITE_H

#include <string>
#include <vector>
#include <cstddef>

// Forward declarations
namespace ruffle {
namespace frontend {
namespace utils {

class Bookmark;
class Bookmarks;
class ContentDescriptor;

namespace parse {
    class DocumentHolder;
}

} // namespace utils
} // namespace frontend
} // namespace ruffle

namespace ruffle {
namespace frontend {
namespace utils {

/// Bookmarks writer for serializing bookmarks to TOML format.
///
/// This class provides methods to add, modify, and remove bookmarks
/// from a TOML document.
class BookmarksWriter {
private:
    parse::DocumentHolder<Bookmarks>* bookmarks_;

    /// Helper to operate on the underlying TOML array of tables.
    ///
    /// @param fun Function to call with the bookmarks and array of tables
    void with_underlying_table(
        const std::function<void(Bookmarks&, void*)>& fun
    );

    /// Helper to operate on a specific bookmark table.
    ///
    /// @param index The index of the bookmark to modify
    /// @param fun Function to call with the bookmarks and table
    void with_bookmark_table(
        size_t index,
        const std::function<void(Bookmarks&, void*)>& fun
    );

public:
    /// Create a new BookmarksWriter.
    ///
    /// @param bookmarks The DocumentHolder containing bookmarks to write
    explicit BookmarksWriter(parse::DocumentHolder<Bookmarks>* bookmarks)
        : bookmarks_(bookmarks) {}

    /// Add a new bookmark to the list.
    ///
    /// @param bookmark The bookmark to add
    void add(const Bookmark& bookmark);

    /// Set the content descriptor for an existing bookmark.
    ///
    /// @param index The index of the bookmark to modify
    /// @param content_descriptor The new content descriptor
    void set_content_descriptor(size_t index, const ContentDescriptor& content_descriptor);

    /// Set the name for an existing bookmark.
    ///
    /// @param index The index of the bookmark to modify
    /// @param name The new name
    void set_name(size_t index, const std::string& name);

    /// Remove a bookmark from the list.
    ///
    /// @param index The index of the bookmark to remove
    void remove(size_t index);
};

} // namespace utils
} // namespace frontend
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_BOOKMARKS_WRITE_H
