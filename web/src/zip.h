// C++ translation of web/src/zip.rs
//! ZIP file writer for web exports

#ifndef RUFFLE_WEB_ZIP_H
#define RUFFLE_WEB_ZIP_H

#include <vector>
#include <string>
#include <unordered_map>
#include <optional>

// Forward declarations
namespace wasm_bindgen {
    class JsValue;
}

namespace ruffle {
namespace web {

/// ZipWriter - Creates ZIP archives for web export
/// 
/// This class collects files in memory and generates a ZIP archive
/// when save() is called. Used for exporting bundles in the web version.
/// 
/// Exposed to JavaScript via wasm_bindgen.
class ZipWriter {
private:
    std::unordered_map<std::string, std::vector<uint8_t>> files_;

public:
    /// Default constructor
    ZipWriter() = default;

    /// Add a file to the ZIP archive
    /// 
    /// \param name The file path within the ZIP
    /// \param bytes The file contents
    void add_file(const std::string& name, const std::vector<uint8_t>& bytes);

    /// Save the ZIP archive
    /// 
    /// \return The ZIP file contents as bytes, or an error message
    std::optional<std::pair<std::vector<uint8_t>, std::string>> save() const;
};

} // namespace web
} // namespace ruffle

#endif // RUFFLE_WEB_ZIP_H
