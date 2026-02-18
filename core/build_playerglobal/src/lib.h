// C++ translation of core/build_playerglobal/src/lib.rs
//! An internal Ruffle utility to build our playerglobal library.swf

#ifndef RUFFLE_CORE_BUILD_PLAYERGLOBAL_LIB_H
#define RUFFLE_CORE_BUILD_PLAYERGLOBAL_LIB_H

#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <cstdint>
#include <optional>
#include <memory>

namespace ruffle {
namespace core {
namespace build_playerglobal {

/// Metadata name - all metadata in our .as files should be of the form
/// [Ruffle(key1 = value1, key2 = value2)]
constexpr const char* RUFFLE_METADATA_NAME = "Ruffle";

/// Indicates that we should generate a reference to an instance allocator method
constexpr const char* METADATA_INSTANCE_ALLOCATOR = "InstanceAllocator";

/// Indicates that we should generate a reference to a class call handler method
constexpr const char* METADATA_CALL_HANDLER = "CallHandler";

/// Indicates that we should generate a class call handler that constructs the class being called
constexpr const char* METADATA_CONSTRUCT_ON_CALL = "ConstructOnCall";

/// Indicates that we should generate a reference to a custom constructor method
constexpr const char* METADATA_CUSTOM_CONSTRUCTOR = "CustomConstructor";

/// Indicates that the class can't be directly instantiated (but its child classes might be)
constexpr const char* METADATA_ABSTRACT = "Abstract";

/// A slot defined by an AS3 const or var but that should have its slot ID
/// recorded so that it can be directly accessed by native code
constexpr const char* METADATA_NATIVE_ACCESSIBLE = "NativeAccessible";

/// Like METADATA_NATIVE_ACCESSIBLE, but for methods instead of slots
constexpr const char* METADATA_NATIVE_CALLABLE = "NativeCallable";

/// Indicates that this method does not read any properties of the Activation
/// passed to it except UpdateContext fields. This is used as an optimization
constexpr const char* METADATA_FAST_CALL = "FastCall";

/// The name for metadata for namespace versioning
constexpr const char* API_METADATA_NAME = "API";

/// Build playerglobal.swf from ActionScript sources
///
/// If successful, returns a list of paths that were used. If this is run
/// from a build script, these paths should be printed with cargo:rerun-if-changed
///
/// @param repo_root Path to the repository root
/// @param out_dir Output directory for generated files
/// @param with_stubs Whether to collect stubs
/// @return Result indicating success or error message
std::optional<std::string> build_playerglobal(
    const std::filesystem::path& repo_root,
    const std::filesystem::path& out_dir,
    bool with_stubs
);

/// Convert a Flash-style path to a Rust-style path
///
/// @param path The Flash-style path (e.g., "flash.display")
/// @param uppercase Whether to use uppercase (snake_case vs SNAKE_CASE)
/// @param separator The separator to use between components
/// @return The converted path
std::string flash_to_rust_string(
    const std::string& path,
    bool uppercase,
    const std::string& separator
);

/// Check for weird namespace lookup in ABC file
///
/// This detects cases where "getlex AS3" appears in method bodies,
/// which indicates missing namespace declarations.
///
/// @param abc_data The ABC file data
/// @return Error message if weird namespace found, nullopt otherwise
std::optional<std::string> check_weird_namespace_lookup(const std::vector<uint8_t>& abc_data);

/// Collect stubs from ActionScript source files
///
/// @param classes_dir Directory containing ActionScript class files
/// @param out_dir Output directory for generated stub files
/// @return Result indicating success or error message
std::optional<std::string> collect_stubs(
    const std::filesystem::path& classes_dir,
    const std::filesystem::path& out_dir
);

/// Write native method table to ABC file
///
/// The high-level idea is to generate code which builds a table -
/// mapping from the method ids of native functions to Rust function
/// pointers which implement them.
///
/// @param data The ABC file data
/// @param out_dir Output directory for generated files
/// @return Modified ABC data or error message
std::pair<std::optional<std::vector<uint8_t>>, std::optional<std::string>> write_native_table(
    const std::vector<uint8_t>& data,
    const std::filesystem::path& out_dir
);

/// Strip metadata from ABC file
///
/// @param abc_data The ABC file data (modified in place)
void strip_metadata(std::vector<uint8_t>& abc_data);

/// Check if a trait has a certain metadata on it
///
/// @param abc_data The ABC file data
/// @param trait_index Index of the trait to check
/// @param metadata_value The metadata value to look for
/// @return True if the trait has the metadata
bool trait_has_metadata(
    const std::vector<uint8_t>& abc_data,
    size_t trait_index,
    const std::string& metadata_value
);

/// Resolve the name field of a Multiname
///
/// This only handles the cases that we need for our custom playerglobal.swf
///
/// @param abc_data The ABC file data
/// @param multiname_index Index of the multiname to resolve
/// @return The resolved name
std::string resolve_multiname_name(const std::vector<uint8_t>& abc_data, size_t multiname_index);

/// Resolve the namespace of a Multiname
///
/// @param abc_data The ABC file data
/// @param multiname_index Index of the multiname to resolve
/// @return The resolved namespace
std::string resolve_multiname_ns(const std::vector<uint8_t>& abc_data, size_t multiname_index);

/// Strip off the version mark inserted by asc.jar
///
/// This gives us a valid Rust module name. The actual versioning logic
/// is handled in Ruffle when we load playerglobals.
///
/// @param val The string potentially containing a version mark
/// @return The string with version mark removed
std::string strip_version_mark(const std::string& val);

/// Generate Rust method path from ABC trait
///
/// @param abc_data The ABC file data
/// @param trait_index Index of the trait
/// @param parent_index Optional parent multiname index
/// @param prefix Prefix to add to the method name
/// @param suffix Suffix to add to the method name
/// @return The generated Rust path as a string
std::string rust_method_path(
    const std::vector<uint8_t>& abc_data,
    size_t trait_index,
    std::optional<size_t> parent_index,
    const std::string& prefix,
    const std::string& suffix
);

/// Get Rust path and trait name from ABC trait
///
/// @param abc_data The ABC file data
/// @param trait_index Index of the trait
/// @param parent_index Optional parent multiname index
/// @return Pair of (path, trait_name)
std::pair<std::string, std::string> rust_path_and_trait_name(
    const std::vector<uint8_t>& abc_data,
    size_t trait_index,
    std::optional<size_t> parent_index
);

/// Information about a native accessible slot
struct NativeAccessibleSlot {
    std::string trait_name;
    std::string const_name;
    uint32_t slot_id;
};

/// Information about a native callable method
struct NativeCallableMethod {
    std::string trait_name;
    std::string const_name;
    uint32_t disp_id;
};

/// Result of processing native methods
struct NativeTableResult {
    /// Map from method index to Rust path
    std::unordered_map<size_t, std::string> rust_paths;

    /// Map from class index to instance allocator path
    std::unordered_map<size_t, std::string> rust_instance_allocators;

    /// Map from class index to call handler path
    std::unordered_map<size_t, std::string> rust_call_handlers;

    /// Map from class index to custom constructor path
    std::unordered_map<size_t, std::string> rust_custom_constructors;

    /// List of fast call method indices
    std::vector<size_t> rust_fast_calls;

    /// Map from trait name to accessible slots
    std::unordered_map<std::string, std::vector<NativeAccessibleSlot>> rust_accessible_slots;

    /// Map from trait name to accessible methods
    std::unordered_map<std::string, std::vector<NativeCallableMethod>> rust_accessible_methods;
};

} // namespace build_playerglobal
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_BUILD_PLAYERGLOBAL_LIB_H
