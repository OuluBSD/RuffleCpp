// C++ translation of frontend-utils/src/backends.rs
// Frontend utilities backends module

#ifndef RUFFLE_FRONTEND_UTILS_BACKENDS_H
#define RUFFLE_FRONTEND_UTILS_BACKENDS_H

// Frontend-utils backends submodules
// These are conditionally compiled based on features

namespace ruffle {
namespace frontend {
namespace utils {
namespace backends {

// Audio backend (conditional on cpal feature)
namespace audio {
    // Audio functionality defined in audio.h
}

// Navigator backend (conditional on navigator feature)
namespace navigator {
    // Navigator functionality defined in navigator.h
}

// Storage backend (conditional on fs feature)
namespace storage {
    // Storage functionality defined in storage.h
}

} // namespace backends
} // namespace utils
} // namespace frontend
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_BACKENDS_H
