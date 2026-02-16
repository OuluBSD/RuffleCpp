// C++ translation of video/src/lib.rs
// Video module root

#ifndef RUFFLE_VIDEO_LIB_H
#define RUFFLE_VIDEO_LIB_H

#include <cstdint>

// Forward declarations for video module
namespace ruffle {
namespace video {

// Video stream handle - opaque identifier type
// Equivalent to slotmap::new_key_type! in Rust
struct VideoStreamHandle {
    uint64_t id;

    VideoStreamHandle() : id(0) {}
    explicit VideoStreamHandle(uint64_t value) : id(value) {}

    bool operator==(const VideoStreamHandle& other) const {
        return id == other.id;
    }

    bool operator!=(const VideoStreamHandle& other) const {
        return id != other.id;
    }

    bool operator<(const VideoStreamHandle& other) const {
        return id < other.id;
    }
};

} // namespace video
} // namespace ruffle

// Video submodules
namespace ruffle {
namespace video {
    // Defined in backend.h
    namespace backend { }

    // Defined in error.h (already exists)
    namespace error { }

    // Defined in frame.h (already exists)
    namespace frame { }

    // Defined in null.h (to be created)
    namespace null { }
}
}

#endif // RUFFLE_VIDEO_LIB_H
