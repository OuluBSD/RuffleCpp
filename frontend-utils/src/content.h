// C++ translation of frontend-utils/src/content.rs
// Content descriptor and playing content functionality

#ifndef RUFFLE_FRONTEND_UTILS_CONTENT_H
#define RUFFLE_FRONTEND_UTILS_CONTENT_H

#include <string>
#include <optional>
#include <variant>
#include <memory>

// Forward declarations
namespace url {
    class Url;
}

namespace ruffle {
namespace frontend {
namespace utils {

// Forward declarations
namespace bundle {
    class Bundle;
    class BundleInformation;
}

/// Describes the content to load.
/// In case of local content, it contains not only the URL, but also the
/// root content path.
class ContentDescriptor {
public:
    Url url;

#ifdef RUFFLE_FEATURE_FS
    /// Path representing the root of the content, optional even for local
    /// files. If not specified, Ruffle can assume the URL points to
    /// a standalone content that does not require neighboring files.
    std::optional<std::filesystem::path> root_content_path;
#endif

    ContentDescriptor() = default;

    /// Create a remote content descriptor
    /// @param url The URL of the remote content
    static ContentDescriptor new_remote(const Url& url) {
        ContentDescriptor desc;
        desc.url = url;
#ifdef RUFFLE_FEATURE_FS
        desc.root_content_path = std::nullopt;
#endif
        return desc;
    }

#ifdef RUFFLE_FEATURE_FS
    /// Create a local content descriptor
    /// @param file The file path
    /// @param root_content_path Optional root content path
    /// @return Optional content descriptor (nullopt if URL conversion fails)
    static std::optional<ContentDescriptor> new_local(
        const std::filesystem::path& file,
        std::optional<std::filesystem::path> root_content_path = std::nullopt
    ) {
        // Would need to convert file path to URL
        // This is a placeholder - actual implementation would use url::Url::from_file_path
        ContentDescriptor desc;
        // desc.url = Url::from_file_path(file); // Would need actual implementation
        desc.root_content_path = root_content_path;
        return desc;
    }
#endif

    /// Get a human-readable description of the content
    /// @return Description string
    std::string describe() const {
#ifdef RUFFLE_FEATURE_FS
        if (root_content_path.has_value()) {
            return url.str() + " within " + root_content_path->string();
        }
#endif
        return url.str();
    }
};

/// Similar to ContentDescriptor, but represents content that is already
/// opened and playing. Contains additional metadata.
class PlayingContent {
public:
    /// Variant holding either a direct file or a bundle
    std::variant<
        ContentDescriptor,
        std::pair<ContentDescriptor, std::shared_ptr<Bundle>>
    > data_;

    /// Direct file constructor
    /// @param desc The content descriptor
    explicit PlayingContent(ContentDescriptor desc)
        : data_(std::move(desc))
    {}

    /// Bundle constructor
    /// @param desc The content descriptor
    /// @param bundle The bundle
    PlayingContent(ContentDescriptor desc, std::shared_ptr<Bundle> bundle)
        : data_(std::make_pair(std::move(desc), std::move(bundle)))
    {}

    /// Get the initial SWF URL
    /// @return Reference to the URL
    const Url& initial_swf_url() const {
        if (auto* desc = std::get_if<ContentDescriptor>(&data_)) {
            return desc->url;
        } else {
            const auto& [d, b] = std::get<std::pair<ContentDescriptor, std::shared_ptr<Bundle>>>(data_);
            return b->information().url;
        }
    }

    /// Get the content name
    /// @return The name as a string
    std::string name() const {
        if (auto* desc = std::get_if<ContentDescriptor>(&data_)) {
            return url_to_readable_name(desc->url.str());
        } else {
            const auto& [d, b] = std::get<std::pair<ContentDescriptor, std::shared_ptr<Bundle>>>(data_);
            return b->information().name;
        }
    }

#ifdef RUFFLE_FEATURE_NAVIGATOR
    /// Get a local file from the content
    /// @param url The URL to fetch
    /// @param interface The navigator interface
    /// @return The file contents or an error
    std::expected<std::vector<uint8_t>, std::io::Error> get_local_file(
        const Url& url,
        NavigatorInterface interface
    ) const {
        // Implementation would handle both direct file and bundle cases
        // This is a placeholder for the async implementation
        return std::unexpected(std::io::Error::not_found());
    }
#endif
};

} // namespace utils
} // namespace frontend
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_CONTENT_H
