// C++ translation of render/wgpu/src/surface.rs
// WGPU surface handling for rendering

#ifndef RUFFLE_RENDER_WGPU_SURFACE_H
#define RUFFLE_RENDER_WGPU_SURFACE_H

#include <memory>
#include <vector>
#include <cstdint>

// Forward declarations for wgpu types
namespace wgpu {
    struct Extent3d {
        uint32_t width;
        uint32_t height;
        uint32_t depth_or_array_layers;
    };

    enum class TextureFormat;
    class TextureView;
    class CommandEncoder;
    class Device;
    class BindGroup;
    class Sampler;

    namespace util {
        class StagingBelt;
    }

    struct RenderPassDescriptor;
    struct BindGroupDescriptor;
    struct BindGroupEntry;
    enum class BindingResource;
    enum class IndexFormat;
}

namespace ruffle {
namespace render {
    namespace quality {
        enum class StageQuality;
    }
    namespace commands {
        class CommandList;
    }
    namespace pixel_bender_support {
        struct ImageInputTexture;
        struct PixelBenderShaderArgument;
    }
}
namespace render_wgpu {
    namespace blend {
        class ComplexBlend;
    }
    namespace buffer_pool {
        class TexturePool;
    }
    namespace dynamic_transforms {
        class DynamicTransforms;
    }
    namespace mesh {
        class Mesh;
    }
    namespace pixel_bender {
        enum class ShaderMode;
    }
    namespace filters {
        struct FilterSource;
    }

    enum class RenderTargetMode;

    struct MaskState {
        enum Value {
            NoMask,
            DrawMaskStencil,
            DrawMaskedContent,
            ClearMaskStencil
        };
    };

    struct Descriptors;
    struct Pipelines;

    namespace surface {
        namespace commands {
            class Chunk;
            enum class ChunkBlendMode;
            class CommandRenderer;
            class LayerRef;
            class CommandTarget;
        }
    }
}
}

namespace ruffle {
namespace render {
namespace wgpu {

using namespace ruffle::render_wgpu;

/// Surface representation for WGPU rendering
class Surface {
private:
    wgpu::Extent3d size;
    ruffle::render::quality::StageQuality quality;
    uint32_t sample_count;
    std::shared_ptr<Pipelines> pipelines;
    wgpu::TextureFormat format;
    wgpu::TextureFormat actual_surface_format;

public:
    /// Create a new surface
    /// @param descriptors WGPU descriptors
    /// @param quality Stage quality setting
    /// @param width Surface width in pixels
    /// @param height Surface height in pixels
    /// @param surface_format The surface texture format
    Surface(
        const Descriptors* descriptors,
        ruffle::render::quality::StageQuality quality,
        uint32_t width,
        uint32_t height,
        wgpu::TextureFormat surface_format);

    /// Draw commands and copy to target texture
    /// @param frame_view The target texture view
    /// @param render_target_mode Render target mode
    /// @param descriptors WGPU descriptors
    /// @param staging_belt Staging belt for buffer uploads
    /// @param dynamic_transforms Dynamic transform buffers
    /// @param draw_encoder Command encoder for recording commands
    /// @param meshes Vector of meshes to draw
    /// @param commands Command list to execute
    /// @param layer Layer reference for blending
    /// @param texture_pool Texture pool for temporary textures
    void draw_commands_and_copy_to(
        wgpu::TextureView* frame_view,
        RenderTargetMode render_target_mode,
        const Descriptors* descriptors,
        wgpu::util::StagingBelt* staging_belt,
        const DynamicTransforms* dynamic_transforms,
        wgpu::CommandEncoder* draw_encoder,
        const std::vector<Mesh>* meshes,
        const ruffle::render::commands::CommandList& commands,
        const surface::commands::LayerRef& layer,
        buffer_pool::TexturePool* texture_pool);

    /// Draw commands to a command target
    /// @param render_target_mode Render target mode
    /// @param descriptors WGPU descriptors
    /// @param meshes Vector of meshes to draw
    /// @param commands Command list to execute
    /// @param staging_belt Staging belt for buffer uploads
    /// @param dynamic_transforms Dynamic transform buffers
    /// @param draw_encoder Command encoder for recording commands
    /// @param nearest_layer Nearest layer reference
    /// @param texture_pool Texture pool for temporary textures
    /// @return The command target
    surface::commands::CommandTarget draw_commands(
        RenderTargetMode render_target_mode,
        const Descriptors* descriptors,
        const std::vector<Mesh>* meshes,
        const ruffle::render::commands::CommandList& commands,
        wgpu::util::StagingBelt* staging_belt,
        const DynamicTransforms* dynamic_transforms,
        wgpu::CommandEncoder* draw_encoder,
        surface::commands::LayerRef nearest_layer,
        buffer_pool::TexturePool* texture_pool);

    /// Get the stage quality
    ruffle::render::quality::StageQuality quality() const {
        return quality;
    }

    /// Get the sample count
    uint32_t sample_count() const {
        return sample_count;
    }

    /// Get the surface size
    wgpu::Extent3d size() const {
        return size;
    }

    /// Get the surface width
    uint32_t width() const {
        return size.width;
    }

    /// Get the surface height
    uint32_t height() const {
        return size.height;
    }
};

} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_SURFACE_H
