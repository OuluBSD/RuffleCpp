// C++ translation of desktop/src/tracy.rs
//! Tracy profiler configuration for desktop Ruffle

#ifndef RUFFLE_DESKTOP_TRACY_H
#define RUFFLE_DESKTOP_TRACY_H

#include <cstdint>

// Forward declarations
namespace tracing {
    struct Metadata;
}

namespace tracing_subscriber {
namespace fmt {
namespace format {
    struct DefaultFields;
}
}
}

namespace ruffle {
namespace desktop {
namespace tracy {

/// Configuration for Tracy profiler integration
/// 
/// This struct implements the tracing_tracy::Config trait
/// to configure how Tracy profiling events are captured.
class RuffleTracyConfig {
private:
    tracing_subscriber::fmt::format::DefaultFields formatter_;

public:
    /// Default constructor
    RuffleTracyConfig() = default;

    /// Get the formatter for Tracy events
    const tracing_subscriber::fmt::format::DefaultFields& formatter() const {
        return formatter_;
    }

    /// Get the stack depth for Tracy events
    /// 
    /// \param metadata The metadata of the event being traced
    /// \return Stack depth (0 = no stack trace to reduce overhead)
    static uint16_t stack_depth(const tracing::Metadata& /*metadata*/) {
        // How much, if any, of the stack trace to capture for each event
        // Obviously, this adds overhead
        return 0;
    }
};

} // namespace tracy
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_TRACY_H
