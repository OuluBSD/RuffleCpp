// C++ translation of core/build_playerglobal/src/main.rs
// Main entry point for build_playerglobal

#ifndef RUFFLE_CORE_BUILD_PLAYERGLOBAL_MAIN_H
#define RUFFLE_CORE_BUILD_PLAYERGLOBAL_MAIN_H

#include <string>
#include <filesystem>

namespace ruffle {
namespace core {
namespace build_playerglobal {

/// Manually builds `playerglobal.swf` without building the `core` crate.
/// This function is invoked with the repo root and output directory.
/// 
/// \param repo_root The location of the Ruffle repository
/// \param out_dir The directory where `playerglobal.swf` should be written
/// \param verbose Enable verbose output
void build_playerglobal_main(
    const std::filesystem::path& repo_root,
    const std::filesystem::path& out_dir,
    bool verbose = false
);

} // namespace build_playerglobal
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_BUILD_PLAYERGLOBAL_MAIN_H
