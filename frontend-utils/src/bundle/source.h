// C++ translation of frontend-utils/src/bundle/source.rs
//! Bundle source module

#ifndef RUFFLE_FRONTEND_UTILS_BUNDLE_SOURCE_H
#define RUFFLE_FRONTEND_UTILS_BUNDLE_SOURCE_H

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <filesystem>
#include <system_error>

// Forward declarations
namespace ruffle {
namespace frontend {
namespace utils {
namespace bundle {

namespace source {

// Forward declare submodules
namespace directory {
    class DirectorySource;
}

namespace zip {
    class ZipSource;
}

} // namespace source

// Bundle information filename constant
constexpr const char* BUNDLE_INFORMATION_FILENAME = "ruffle-bundle.toml";

} // namespace bundle

namespace io_error = std::io;

namespace bundle {
namespace source {

/// Trait for bundle source data (Read + Seek)
class BundleSourceData {
public:
    virtual ~BundleSourceData() = default;
    virtual std::streamsize read(char* s, std::streamsize n) = 0;
    virtual std::streampos seekg(std::streampos off, std::ios_base::seekdir way) = 0;
    virtual std::streampos tellg() = 0;
};

/// Implementation of BundleSourceData for ifstream
class FileBundleSourceData : public BundleSourceData {
public:
    explicit FileBundleSourceData(const std::filesystem::path& path)
        : file_(path, std::ios::binary | std::ios::in) {}

    bool is_open() const { return file_.is_open(); }

    std::streamsize read(char* s, std::streamsize n) override {
        file_.read(s, n);
        return file_.gcount();
    }

    std::streampos seekg(std::streampos off, std::ios_base::seekdir way) override {
        file_.seekg(off, way);
        return file_.tellg();
    }

    std::streampos tellg() override {
        return file_.tellg();
    }

private:
    std::ifstream file_;
};

/// Bundle source implementation interface
class BundleSourceImpl {
public:
    virtual ~BundleSourceImpl() = default;

    /// Reads any file from the bundle.
    virtual std::vector<uint8_t> read_file(const std::string& path) = 0;

    /// Reads a file specifically from the content directory of the bundle.
    virtual std::vector<uint8_t> read_content(const std::string& path) = 0;
};

/// Error types for bundle source operations
enum class BundleSourceErrorType {
    UnknownSource,
    InvalidArchive,
    IoError
};

class BundleSourceError : public std::runtime_error {
public:
    BundleSourceError(BundleSourceErrorType type, const std::string& message)
        : std::runtime_error(message), type_(type) {}

    BundleSourceErrorType type() const { return type_; }

private:
    BundleSourceErrorType type_;
};

/// Bundle source - can be either a directory or a zip file
class BundleSource {
public:
    enum class Type {
        Directory,
        ZipFile
    };

    /// Create from a path (directory or .ruf file)
    static std::unique_ptr<BundleSource> from_path(const std::filesystem::path& path) {
        // Check if it's a directory with bundle info file
        if (std::filesystem::is_directory(path)) {
            auto bundle_info = path / BUNDLE_INFORMATION_FILENAME;
            if (std::filesystem::exists(bundle_info)) {
                return std::make_unique<BundleSource>(path, Type::Directory);
            }
        }

        // Check if it's a file
        if (std::filesystem::is_regular_file(path)) {
            // Check if it's the bundle info file itself
            if (path.filename() == BUNDLE_INFORMATION_FILENAME) {
                auto parent = path.parent_path();
                if (!parent.empty()) {
                    return std::make_unique<BundleSource>(parent, Type::Directory);
                }
            }

            // Check if it's a .ruf file (zip bundle)
            if (path.extension() == ".ruf") {
                return from_reader(std::make_unique<FileBundleSourceData>(path));
            }
        }

        throw BundleSourceError(
            BundleSourceErrorType::UnknownSource,
            "Unknown bundle source: " + path.string()
        );
    }

    /// Create from a reader
    static std::unique_ptr<BundleSource> from_reader(std::unique_ptr<BundleSourceData> reader) {
        // Would create ZipSource in real implementation
        throw BundleSourceError(
            BundleSourceErrorType::InvalidArchive,
            "Zip source not implemented"
        );
    }

    /// Reads any file from the bundle.
    std::vector<uint8_t> read_file(const std::string& path) {
        if (type_ == Type::Directory) {
            return read_file_from_directory(path);
        } else {
            // Would delegate to zip source
            throw BundleSourceError(
                BundleSourceErrorType::InvalidArchive,
                "Zip source not implemented"
            );
        }
    }

    /// Reads a file specifically from the content directory of the bundle.
    std::vector<uint8_t> read_content(const std::string& path) {
        if (type_ == Type::Directory) {
            return read_content_from_directory(path);
        } else {
            // Would delegate to zip source
            throw BundleSourceError(
                BundleSourceErrorType::InvalidArchive,
                "Zip source not implemented"
            );
        }
    }

    Type type() const { return type_; }
    const std::filesystem::path& path() const { return path_; }

private:
    BundleSource(const std::filesystem::path& p, Type t) : path_(p), type_(t) {}

    std::vector<uint8_t> read_file_from_directory(const std::string& path) {
        auto full_path = path_ / path;
        std::ifstream file(full_path, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + full_path.string());
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<uint8_t> buffer(size);
        if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
            throw std::runtime_error("Failed to read file: " + full_path.string());
        }

        return buffer;
    }

    std::vector<uint8_t> read_content_from_directory(const std::string& path) {
        return read_file_from_directory("content/" + path);
    }

    std::filesystem::path path_;
    Type type_;
};

} // namespace source
} // namespace bundle
} // namespace utils
} // namespace frontend
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_BUNDLE_SOURCE_H
