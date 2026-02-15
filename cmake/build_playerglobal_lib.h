/*
 * C++ header for build_playerglobal functionality
 * This replaces the functionality of core/build_playerglobal/src/lib.rs
 */

#ifndef BUILD_PLAYERGLOBAL_LIB_H
#define BUILD_PLAYERGLOBAL_LIB_H

#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <stdexcept>
#include <map>

namespace fs = std::filesystem;

// Forward declarations
struct AbcFile;
struct Multiname;
struct Trait;
struct Namespace;

// Exception class for build errors
class BuildPlayerGlobalException : public std::runtime_error {
public:
    explicit BuildPlayerGlobalException(const std::string& msg) : std::runtime_error(msg) {}
};

// Main function to build playerglobal
bool build_playerglobal(const fs::path& repo_root, const fs::path& out_dir, bool with_stubs = false);

// Helper functions that mirror the Rust functionality
std::string resolve_multiname_name(const AbcFile& abc, const Multiname& multiname);
std::string resolve_multiname_ns(const AbcFile& abc, const Multiname& multiname);
std::string strip_version_mark(const std::string& val);
std::string flash_to_rust_string(const std::string& path, bool uppercase, const std::string& separator);
std::vector<uint8_t> write_native_table(const std::vector<uint8_t>& data, const fs::path& out_dir);
void strip_metadata(AbcFile& abc);

// Stub collection functionality
bool collect_stubs(const fs::path& root, const fs::path& out_dir);

#endif // BUILD_PLAYERGLOBAL_LIB_H