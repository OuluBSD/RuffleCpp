// C++ translation of render/wgpu/src/dynamic_transforms.rs
// Dynamic transforms buffer management for WebGPU

#ifndef RUFFLE_RENDER_WGPU_DYNAMIC_TRANSFORMS_H
#define RUFFLE_RENDER_WGPU_DYNAMIC_TRANSFORMS_H

#include <cstdint>
#include <memory>
#include <cstddef>

// Forward declarations
namespace wgpu {
    class Buffer;
    class BindGroup;
}

namespace ruffle {
namespace render {
namespace wgpu_backend {
namespace descriptors {
    struct Descriptors;
}

struct Transforms;

/// Dynamic transforms buffer and bind group
///
/// Manages a uniform buffer for transform data and its associated bind group.
/// The buffer is sized to hold an estimated number of transform objects per chunk.
struct DynamicTransforms {
    /// The uniform buffer for transform data
    wgpu::Buffer buffer;

    /// The bind group for accessing the transform buffer in shaders
    wgpu::BindGroup bind_group;

    /// Estimated number of transform objects per chunk
    static constexpr uint64_t ESTIMATED_OBJECTS_PER_CHUNK = 200;

    /// Create new dynamic transforms buffer and bind group
    ///
    /// \param descriptors WebGPU device and resource descriptors
    /// \return New DynamicTransforms instance
    static DynamicTransforms new_dynamic_transforms(const descriptors::Descriptors& descriptors);
};

} // namespace wgpu_backend
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_DYNAMIC_TRANSFORMS_H
