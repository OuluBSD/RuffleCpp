// C++ translation of tests/framework/src/util.rs
// Utility functions for test framework

#ifndef RUFFLE_TESTS_FRAMEWORK_UTIL_H
#define RUFFLE_TESTS_FRAMEWORK_UTIL_H

#include <vector>
#include <cstdint>
#include <memory>

// Forward declarations
namespace vfs {
    class VfsPath;
    class VfsError;
}

namespace ruffle {
namespace tests {
namespace framework {
namespace util {

/// Read all bytes from a VFS path
///
/// \param path The VFS path to read from
/// \return Vector of bytes read from the file
/// \throws VfsError on read failure
std::vector<uint8_t> read_bytes(const vfs::VfsPath& path);

/// Write bytes to a VFS path
///
/// \param path The VFS path to write to
/// \param data The data to write
/// \throws VfsError on write failure
void write_bytes(const vfs::VfsPath& path, const std::vector<uint8_t>& data);

} // namespace util
} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_UTIL_H
