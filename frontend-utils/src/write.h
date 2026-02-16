// C++ translation of frontend-utils/src/write.rs
// Frontend utilities write helpers

#ifndef RUFFLE_FRONTEND_UTILS_WRITE_H
#define RUFFLE_FRONTEND_UTILS_WRITE_H

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace ruffle {
namespace frontend {
namespace utils {

// Forward declarations for TOML editing
// In a real implementation, these would interface with a TOML library
namespace toml_edit {
    class ArrayOfTables;
    class Table;
    class Array;
}

/// Extension trait for TOML Table to help with array of tables
class TableExt {
public:
    /// Gets an existing array of tables, or creates a new one if it does not exist or type is different.
    virtual toml_edit::ArrayOfTables* get_or_create_array_of_tables(const std::string& key) = 0;

    virtual ~TableExt() = default;
};

/// Helper macros for serialization tests
/// Note: C++ macros are different from Rust macros, so this is a conceptual translation
/// In practice, these would be implemented as template functions or test fixtures

// define_serialization_test_helpers equivalent
// This would be used in test code to define roundtrip check helpers
// Usage pattern (conceptual):
// DEFINE_SERIALIZATION_TEST_HELPERS(read_method, DocStruct, Writer)
//
// Which would expand to create:
// - check_roundtrip(preferences) - verifies serialization roundtrip
// - test(original, fun, expected) - tests transformation with expected output

// For actual implementation, test helpers would be defined per-document type
// using Google Test or similar framework

} // namespace utils
} // namespace frontend
} // namespace ruffle

// Note: The Rust macro define_serialization_test_helpers! is translated
// conceptually. In C++, such functionality would typically be implemented
// using templates, test fixtures, or a testing framework's macros.

#endif // RUFFLE_FRONTEND_UTILS_WRITE_H
