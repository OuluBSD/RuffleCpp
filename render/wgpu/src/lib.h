// C++ translation of render/wgpu/src/lib.rs
// WebGPU render backend main module

#ifndef RUFFLE_RENDER_WGPU_LIB_H
#define RUFFLE_RENDER_WGPU_LIB_H

#include <cstdint>
#include <memory>
#include <array>
#include <optional>
#include <variant>
#include <functional>
#include <atomic>
#include <any>

// Forward declarations
namespace ruffle {
namespace render {
namespace backend {
    class RawTexture;
}
namespace bitmap {
    class BitmapHandle;
    class BitmapHandleImpl;
    struct PixelRegion;
    class SyncHandle;
}
namespace shape_utils {
    enum class GradientType;
}
namespace tessellator {
    struct Vertex;
    struct Gradient;
}
}
namespace swf {
    enum class GradientSpread;
    enum class GradientInterpolation;
}
}

namespace wgpu {
    // Forward declarations for wgpu types
    class Device;
    class Texture;
    class Buffer;
    class BindGroupLayout;
    class BindGroup;
    class CommandEncoder;
    class SubmissionIndex;
}

namespace ruffle {
namespace render {
namespace wgpu_backend {

// Module forward declarations
namespace backend {
    struct ActiveFrame;
}
namespace bitmaps {
    class BitmapSamplers;
}
namespace buffer_pool {
    template<typename T, typename D>
    class BufferPool;
    template<typename T, typename D>
    struct PoolEntry;
}
namespace descriptors {
    struct Descriptors;
    struct Quad;
}
namespace mesh {
    struct BitmapBinds;
}
namespace pipelines {
    class Pipelines;
}
namespace target {
    class RenderTarget;
    class SwapChainTarget;
}
namespace utils {
    struct BufferDimensions;
}

/// Error type for WebGPU backend
using Error = std::shared_ptr<std::exception>;

/// Mask rendering state
enum class MaskState {
    NoMask,
    DrawMaskStencil,
    DrawMaskedContent,
    ClearMaskStencil
};

/// Transform uniforms for shader
struct Transforms {
    std::array<std::array<float, 4>, 4> world_matrix;
    std::array<float, 4> mult_color;
    std::array<float, 4> add_color;
};

/// Texture transform uniforms
struct TextureTransforms {
    std::array<std::array<float, 4>, 4> u_matrix;
};

/// Position vertex for basic geometry
struct PosVertex {
    std::array<float, 2> position;
    
    static PosVertex from_tess_vertex(const tessellator::Vertex& vertex);
};

/// Position and color vertex
struct PosColorVertex {
    std::array<float, 2> position;
    std::array<float, 4> color;
    
    static PosColorVertex from_tess_vertex(const tessellator::Vertex& vertex);
};

/// Gradient uniforms for shader
struct GradientUniforms {
    float focal_point;
    int32_t interpolation;
    int32_t shape;
    int32_t repeat;
    
    static GradientUniforms from_tess_gradient(const tessellator::Gradient& gradient);
};

/// Texture wrapper for WebGPU
///
/// Implements BitmapHandleImpl for WebGPU textures.
class Texture : public bitmap::BitmapHandleImpl {
public:
    wgpu::Texture texture;
    std::optional<BitmapBinds> bind_linear;
    std::optional<BitmapBinds> bind_nearest;
    std::atomic<uint8_t> copy_count;
    
    /// Get or create bind group for this texture
    ///
    /// \param smoothed Whether to use linear or nearest sampling
    /// \param device WebGPU device
    /// \param layout Bind group layout
    /// \param quad Quad descriptor for texture transforms
    /// \param handle Bitmap handle for debugging
    /// \param samplers Bitmap samplers
    /// \return Bind group for this texture
    const BitmapBinds& bind_group(
        bool smoothed,
        const wgpu::Device& device,
        const wgpu::BindGroupLayout& layout,
        const Quad& quad,
        const bitmap::BitmapHandle& handle,
        const BitmapSamplers& samplers);
};

/// Queue synchronization handle for bitmap capture
///
/// Represents either an already-copied buffer or a pending copy operation.
class QueueSyncHandle : public bitmap::SyncHandle {
public:
    /// Already copied state
    struct AlreadyCopied {
        std::optional<wgpu::SubmissionIndex> index;
        PoolEntry<wgpu::Buffer, BufferDimensions> buffer;
        BufferDimensions copy_dimensions;
        std::shared_ptr<Descriptors> descriptors;
    };
    
    /// Not yet copied state
    struct NotCopied {
        bitmap::BitmapHandle handle;
        PixelRegion copy_area;
        std::shared_ptr<Descriptors> descriptors;
        std::shared_ptr<BufferPool<wgpu::Buffer, BufferDimensions>> pool;
    };
    
private:
    std::variant<AlreadyCopied, NotCopied> state_;
    
public:
    /// Create an already-copied handle
    static QueueSyncHandle already_copied(
        std::optional<wgpu::SubmissionIndex> index,
        PoolEntry<wgpu::Buffer, BufferDimensions> buffer,
        BufferDimensions copy_dimensions,
        std::shared_ptr<Descriptors> descriptors);
    
    /// Create a not-yet-copied handle
    static QueueSyncHandle not_copied(
        bitmap::BitmapHandle handle,
        PixelRegion copy_area,
        std::shared_ptr<Descriptors> descriptors,
        std::shared_ptr<BufferPool<wgpu::Buffer, BufferDimensions>> pool);
    
    /// Capture texture data and process with callback
    ///
    /// \param with_rgba Callback function receiving RGBA data and width
    /// \param frame Active frame for command submission
    /// \return Result from callback
    template<typename R, typename F>
    R capture(F with_rgba, ActiveFrame& frame) {
        // Implementation would handle both states
        // AlreadyCopied: use existing buffer
        // NotCopied: perform copy operation then capture
        return R{};
    }
};

/// Convert BitmapHandle to Texture reference
///
/// \param handle Bitmap handle to convert
/// \return Reference to underlying Texture
Texture& as_texture(const bitmap::BitmapHandle& handle);

/// Convert RawTexture to wgpu::Texture reference
///
/// \param handle Raw texture to convert
/// \return Reference to underlying wgpu::Texture
const wgpu::Texture& raw_texture_as_texture(const backend::RawTexture& handle);

} // namespace wgpu_backend
} // namespace render
} // namespace ruffle

// Module declarations (submodules are in separate headers)
namespace ruffle {
namespace render {
namespace wgpu_backend {

// Submodule headers would be included here:
// #include "backend.h"
// #include "bitmaps.h"
// #include "context3d.h"
// #include "globals.h"
// #include "pipelines.h"
// #include "pixel_bender.h"
// #include "target.h"
// #include "blend.h"
// #include "buffer_builder.h"
// #include "buffer_pool.h"
// #include "descriptors.h"
// #include "dynamic_transforms.h"
// #include "filters.h"
// #include "layouts.h"
// #include "mesh.h"
// #include "shaders.h"
// #include "surface.h"

} // namespace wgpu_backend
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_LIB_H
