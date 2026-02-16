// C++ translation of render/wgpu/src/surface/target.rs
// Command target for WebGPU surface rendering

#ifndef RUFFLE_RENDER_WGPU_SURFACE_TARGET_H
#define RUFFLE_RENDER_WGPU_SURFACE_TARGET_H

// Forward declarations
namespace ruffle {
namespace render_wgpu {
struct Descriptors;
struct Transforms;
class TexturePool;
class Globals;
namespace buffer_pool {
    template<typename T, typename C>
    class PoolEntry;
    struct AlwaysCompatible;
}
namespace backend {
    enum class RenderTargetMode;
}
}
}

// External dependencies
#include <wgpu/wgpu.h>
#include <cell/once_cell.h>
#include <sync/arc.h>
#include <variant>

namespace ruffle {
namespace render_wgpu {
namespace surface {
namespace target {

/// Holds either a PoolEntry texture, or an Arc-wrapped texture.
/// This is used to select between using a texture pool for our framebuffer/resolve-buffer
/// (when rendering to the main screen), or rendering to a non-pooled Texture
/// (when doing an offscreen render to a BitmapData texture)
class PoolOrArcTexture {
public:
    /// Pool-based texture
    using Pool = buffer_pool::PoolEntry<std::pair<wgpu::Texture, wgpu::TextureView>, buffer_pool::AlwaysCompatible>;
    
    /// Manual Arc-based texture
    using Manual = std::pair<wgpu::Texture, wgpu::TextureView>;
    
    /// The variant holding either texture type
    std::variant<Pool, Manual> data;

    /// Get the underlying texture
    const wgpu::Texture* texture() const;
    
    /// Get the texture view
    const wgpu::TextureView* view() const;
};

/// Resolve buffer for MSAA resolve operations
class ResolveBuffer {
public:
    /// Create a new resolve buffer from the texture pool
    ResolveBuffer(
        const Descriptors* descriptors,
        wgpu::Extent3D size,
        wgpu::TextureFormat format,
        wgpu::TextureUsage usage,
        TexturePool* pool);

    /// Create a resolve buffer from a manual texture
    explicit ResolveBuffer(wgpu::Texture texture);

    /// Get the texture view
    const wgpu::TextureView* view() const;

    /// Get the texture
    const wgpu::Texture* texture() const;

    /// Take ownership of the underlying texture
    PoolOrArcTexture take_texture();

private:
    PoolOrArcTexture texture_;
};

/// Frame buffer for rendering
class FrameBuffer {
public:
    /// Create a new frame buffer from the texture pool
    FrameBuffer(
        const Descriptors* descriptors,
        uint32_t sample_count,
        wgpu::Extent3D size,
        wgpu::TextureFormat format,
        wgpu::TextureUsage usage,
        TexturePool* pool);

    /// Create a frame buffer from a manual texture
    FrameBuffer(wgpu::Texture texture, wgpu::Extent3D size);

    /// Get the texture view
    const wgpu::TextureView* view() const;

    /// Get the texture
    const wgpu::Texture* texture() const;

    /// Take ownership of the underlying texture
    PoolOrArcTexture take_texture();

    /// Get the size
    wgpu::Extent3D size() const;

private:
    PoolOrArcTexture texture_;
    wgpu::Extent3D size_;
};

/// Blend buffer for blending operations
class BlendBuffer {
public:
    /// Create a new blend buffer
    BlendBuffer(
        const Descriptors* descriptors,
        wgpu::Extent3D size,
        wgpu::TextureFormat format,
        wgpu::TextureUsage usage,
        TexturePool* pool);

    /// Get the texture view
    const wgpu::TextureView* view() const;

    /// Get the texture
    const wgpu::Texture* texture() const;

private:
    buffer_pool::PoolEntry<std::pair<wgpu::Texture, wgpu::TextureView>, buffer_pool::AlwaysCompatible> texture_;
};

/// Stencil buffer for depth/stencil operations
class StencilBuffer {
public:
    /// Create a new stencil buffer
    StencilBuffer(
        const Descriptors* descriptors,
        uint32_t msaa_sample_count,
        wgpu::Extent3D size,
        TexturePool* pool);

    /// Get the texture view
    const wgpu::TextureView* view() const;

private:
    buffer_pool::PoolEntry<std::pair<wgpu::Texture, wgpu::TextureView>, buffer_pool::AlwaysCompatible> texture_;
};

/// Command target for rendering commands
class CommandTarget {
public:
    /// Create a new command target
    /// @param descriptors WebGPU descriptors
    /// @param pool Texture pool
    /// @param size Render target size
    /// @param format Texture format
    /// @param sample_count MSAA sample count
    /// @param render_target_mode Render target mode
    /// @param encoder Command encoder
    CommandTarget(
        const Descriptors* descriptors,
        TexturePool* pool,
        wgpu::Extent3D size,
        wgpu::TextureFormat format,
        uint32_t sample_count,
        backend::RenderTargetMode render_target_mode,
        wgpu::CommandEncoder* encoder);

    /// Get the width
    uint32_t width() const;

    /// Get the height
    uint32_t height() const;

    /// Ensure the target is cleared
    void ensure_cleared(wgpu::CommandEncoder* encoder);

    /// Take the color texture
    PoolOrArcTexture take_color_texture();

    /// Get the globals
    const Globals* globals() const;

    /// Get the whole-frame bind group
    const wgpu::BindGroup* whole_frame_bind_group(const Descriptors* descriptors) const;

    /// Get color attachments for render pass
    std::optional<wgpu::RenderPassColorAttachment> color_attachments() const;

    /// Get the sample count
    uint32_t sample_count() const;

    /// Get stencil attachment
    std::optional<wgpu::RenderPassDepthStencilAttachment> stencil_attachment(
        const Descriptors* descriptors,
        TexturePool* pool);

    /// Update the blend buffer
    const BlendBuffer* update_blend_buffer(
        const Descriptors* descriptors,
        TexturePool* pool,
        wgpu::CommandEncoder* encoder);

    /// Get the color view
    const wgpu::TextureView* color_view() const;

    /// Get the color texture
    const wgpu::Texture* color_texture() const;

private:
    FrameBuffer frame_buffer_;
    OnceCell<BlendBuffer> blend_buffer_;
    std::optional<ResolveBuffer> resolve_buffer_;
    OnceCell<StencilBuffer> depth_;
    Arc<Globals> globals_;
    wgpu::Extent3D size_;
    wgpu::TextureFormat format_;
    uint32_t sample_count_;
    OnceCell<std::pair<wgpu::Buffer, wgpu::BindGroup>> whole_frame_bind_group_;
    OnceCell<bool> color_needs_clear_;
    backend::RenderTargetMode render_target_mode_;
};

/// Get or create the whole-frame bind group
const wgpu::BindGroup* get_whole_frame_bind_group(
    const OnceCell<std::pair<wgpu::Buffer, wgpu::BindGroup>>* once_cell,
    const Descriptors* descriptors,
    wgpu::Extent3D size);

} // namespace target
} // namespace surface
} // namespace render_wgpu
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_SURFACE_TARGET_H
