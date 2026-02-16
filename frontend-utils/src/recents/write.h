// C++ translation of frontend-utils/src/recents/write.rs
//! Recents writer - manages writing recent files list to TOML

#ifndef RUFFLE_FRONTEND_UTILS_RECENTS_WRITE_H
#define RUFFLE_FRONTEND_UTILS_RECENTS_WRITE_H

#include <string>
#include <vector>
#include <cstddef>
#include <optional>
#include <filesystem>

// Forward declarations
namespace ruffle {
namespace frontend_utils {

class ContentDescriptor;
struct Recent;
class Recents;

namespace parse {
    template<typename T>
    class DocumentHolder;
}

} // namespace frontend_utils
} // namespace ruffle

namespace ruffle {
namespace frontend_utils {
namespace recents {

/// Writer for managing recent files list
/// 
/// Provides methods to add, remove, and persist recent file entries
/// to a TOML document format.
class RecentsWriter {
public:
    /// Create a new RecentsWriter
    /// @param recents The document holder containing recents data
    explicit RecentsWriter(parse::DocumentHolder<std::vector<Recent>>* recents);

    /// Clear all recent entries
    /// 
    /// Removes all entries from both the internal list and the TOML document
    void clear();

    /// Push a new recent entry to the top of the list
    /// 
    /// If an entry with the same URL already exists, it will be moved to the top.
    /// If the list exceeds the limit after adding, old entries are removed from the bottom.
    /// 
    /// @param recent The recent entry to add
    /// @param limit Maximum number of entries to keep (0 means do nothing)
    void push(const Recent& recent, size_t limit);

private:
    /// Pointer to the document holder containing recents data
    parse::DocumentHolder<std::vector<Recent>>* recents_;

    /// Execute a function with the underlying TOML array and values vector
    /// 
    /// @param fun Function to execute with the arrays
    void with_underlying_table(
        std::function<void(std::vector<Recent>&, void*)> fun);

    /// Create a TOML table from a Recent entry
    /// 
    /// @param recent The recent entry to convert
    /// @return TOML table representation
    void* create_recent_table(const Recent& recent);
};

/// Recent entry structure
/// 
/// Represents a single recently opened file with its descriptor and display name
struct Recent {
    ContentDescriptor content_descriptor;
    std::string name;
};

/// Recents type alias - vector of Recent entries
/// Stored from oldest to newest
using Recents = std::vector<Recent>;

} // namespace recents
} // namespace frontend_utils
} // namespace ruffle

// Inline implementations

namespace ruffle {
namespace frontend_utils {
namespace recents {

inline RecentsWriter::RecentsWriter(parse::DocumentHolder<std::vector<Recent>>* recents)
    : recents_(recents) {}

inline void RecentsWriter::clear() {
    with_underlying_table([](std::vector<Recent>& values, void* array) {
        // Clear the TOML array (would need actual toml_edit binding)
        // array->clear();
        
        // Clear the values vector
        values.clear();
    });
}

inline void RecentsWriter::push(const Recent& recent, size_t limit) {
    if (limit == 0) {
        // Do not even bother
        return;
    }

    with_underlying_table([this, &recent, limit](std::vector<Recent>& values, void* array) {
        // First, check if we already have an existing entry with the same URL
        // and move it to the top
        std::optional<size_t> existing_index;
        for (size_t i = 0; i < values.size(); ++i) {
            if (values[i].content_descriptor.url == recent.content_descriptor.url) {
                existing_index = i;
                break;
            }
        }

        if (existing_index.has_value()) {
            // Existing entry, just move it to the top
            
            // Update TOML first, then internal values
            // array->remove(existing_index.value());
            // array->push(create_recent_table(&recent));
            
            auto it = values.begin() + existing_index.value();
            Recent moved = std::move(*it);
            values.erase(it);
            values.push_back(std::move(moved));
        } else {
            // New entry
            // Evict old entries if we are at or over the limit
            if (values.size() >= limit) {
                // Remove n elements over limit plus 1, since we need to push a new one too
                size_t elements_to_remove = (values.size() - limit) + 1;

                // This is inefficient, but this is not hot code
                // (usually we only need to remove 1 element, unless the limit changed)
                for (size_t i = 0; i < elements_to_remove; ++i) {
                    // array->remove(0);
                    values.erase(values.begin());
                }
            }

            // Create a new table and push it
            // array->push(create_recent_table(&recent));
            values.push_back(recent);
        }
    });
}

} // namespace recents
} // namespace frontend_utils
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_RECENTS_WRITE_H
