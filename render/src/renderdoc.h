// C++ translation of render/src/renderdoc.rs
// RenderDoc integration for graphics debugging

#ifndef RUFFLE_RENDER_RENDERDOC_H
#define RUFFLE_RENDER_RENDERDOC_H

#include <memory>
#include <optional>
#include <mutex>

namespace ruffle {
namespace render {

// Forward declaration - RenderDoc would be defined by renderdoc-cpp bindings
// This is a stub for the RenderDoc API integration
namespace renderdoc_api {
    class RenderDoc;
    struct V141;
    enum class Error;
}

/// Thread-local storage for RenderDoc instance
/// In C++, we use a thread_local variable instead of Rust's thread_local! macro
class RenderDocManager {
public:
    /// Get or initialize the RenderDoc instance for this thread
    static std::optional<std::unique_ptr<renderdoc_api::RenderDoc>>& get_instance();

    /// Start a frame capture
    static void start_frame_capture();

    /// End a frame capture
    static void end_frame_capture();
};

/// Start a RenderDoc frame capture
inline void start_frame_capture() {
    RenderDocManager::start_frame_capture();
}

/// End a RenderDoc frame capture
inline void end_frame_capture() {
    RenderDocManager::end_frame_capture();
}

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_RENDERDOC_H
