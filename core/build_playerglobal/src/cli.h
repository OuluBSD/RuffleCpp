// C++ translation of core/build_playerglobal/src/cli.rs
// Command-line interface for build_playerglobal

#ifndef RUFFLE_CORE_BUILD_PLAYERGLOBAL_CLI_H
#define RUFFLE_CORE_BUILD_PLAYERGLOBAL_CLI_H

#include <string>
#include <variant>

namespace ruffle {
namespace core {
namespace build_playerglobal {
namespace cli {

/// Commands for the CLI
struct CompileCommand {
    std::string out_dir;
};

/// CLI command variant
using Commands = std::variant<
    CompileCommand
>;

/// CLI argument parser result
struct Cli {
    Commands command;
};

} // namespace cli
} // namespace build_playerglobal
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_BUILD_PLAYERGLOBAL_CLI_H
