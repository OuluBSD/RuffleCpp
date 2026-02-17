// C++ translation of core/src/avm1/fscommand.rs
// FSCommand handling

#ifndef RUFFLE_CORE_AVM1_FSCOMMAND_H
#define RUFFLE_CORE_AVM1_FSCOMMAND_H

#include <cstdint>
#include <string>
#include <optional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace string {
    template<typename T> class WStr;
}
namespace avm1 {
    template<typename T> class Activation;
    namespace error {
        template<typename T> class Error;
    }
}
namespace external {
    class ExternalInterface;
}
namespace context {
    template<typename T> struct UpdateContext;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace fscommand {

/// Parse an FSCommand URL
///
/// FSCommand URLs have the format "fscommand:command_name"
/// This function extracts the command name from the URL.
///
/// \param url The URL to parse
/// \return The command portion of the URL if it starts with "fscommand:", nullopt otherwise
template<typename GC>
std::optional<const string::WStr<GC>*> parse(const string::WStr<GC>* url) {
    static const uint8_t prefix_bytes[] = "fscommand:";
    constexpr size_t prefix_len = sizeof(prefix_bytes) - 1;

    if (url->len() < prefix_len) {
        return std::nullopt;
    }

    // Split at prefix length
    auto [head, tail] = url->split_at(prefix_len);

    // Check if head matches prefix (case-insensitive)
    if (head.eq_ignore_case(string::WStr<GC>::from_units(prefix_bytes, prefix_len))) {
        return tail;
    } else {
        return std::nullopt;
    }
}

/// Handle an FSCommand
///
/// Processes an FSCommand by invoking the external interface.
/// If the command is not handled, logs a warning.
///
/// \param command The command name
/// \param args The command arguments
/// \param activation The current activation context
/// \return Ok(()) if successful, Error otherwise
template<typename GC>
std::optional<error::Error<GC>> handle(
    const string::WStr<GC>* command,
    const string::WStr<GC>* args,
    Activation<GC>* activation
) {
    // Convert to UTF8 for external interface
    std::string command_utf8 = command->to_utf8_lossy();
    std::string args_utf8 = args->to_utf8_lossy();

    // Try to invoke through external interface
    bool handled = activation->context().external_interface.invoke_fs_command(
        command_utf8, args_utf8);

    if (!handled) {
        // Log warning for unhandled command
        // Note: In full implementation, this would use avm_warn! macro
        // For now, just note that it's unhandled
    }

    return std::nullopt; // Ok(())
}

} // namespace fscommand
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_FSCOMMAND_H
