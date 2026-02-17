// C++ translation of render/src/backend.rs
// Render backend interface

#ifndef RUFFLE_RENDER_BACKEND_H
#define RUFFLE_RENDER_BACKEND_H

#include <cstdint>
#include <memory>
#include <vector>
#include <optional>
#include <string>
#include <functional>
#include <array>

// Forward declarations
namespace swf {
    struct Color;
    struct Rectangle;
    class Twips;
}

namespace ruffle {
namespace core {
    class WStr;
}
}

namespace ruffle {
namespace render {

// Forward declarations
class Bitmap;
struct BitmapHandle;
class BitmapSource;
struct PixelRegion;
class SyncHandle;
struct Filter;
class PixelBenderShader;
struct PixelBenderShaderHandle;
enum class StageQuality;
struct DistilledShape;
struct ShapeHandle;
struct CommandList;
class Error;

using RgbaBufRead = std::function<void(const uint8_t*, uint32_t)>;

/// Bitmap cache entry for rendering
struct BitmapCacheEntry {
    BitmapHandle handle;
    CommandList commands;
    swf::Color clear;
    std::vector<Filter> filters;
};

/// Viewport dimensions
struct ViewportDimensions {
    /// The dimensions of the stage's containing viewport.
    uint32_t width;
    uint32_t height;

    /// The scale factor of the containing viewport from standard-size pixels
    /// to device-scale pixels.
    double scale_factor;
};

/// Pixel snapping mode for bitmap rendering
enum class PixelSnapping {
    /// Always snap the bitmap to the pixel grid
    Always,

    /// Only snap if no rotation/skew and ~100% scale
    Auto,

    /// Never snap the bitmap to the pixel grid
    Never
};

/// Context3D texture format
enum class Context3DTextureFormat {
    Bgra,
    BgraPacked,
    BgrPacked,
    Compressed,
    CompressedAlpha,
    RgbaHalfFloat
};

/// Context3D blend factor
enum class Context3DBlendFactor {
    DestinationAlpha,
    DestinationColor,
    One,
    OneMinusDestinationAlpha,
    OneMinusDestinationColor,
    OneMinusSourceAlpha,
    OneMinusSourceColor,
    SourceAlpha,
    SourceColor,
    Zero
};

/// Buffer usage mode
enum class BufferUsage {
    DynamicDraw,
    StaticDraw
};

/// Shader program type
enum class ProgramType {
    Vertex,
    Fragment
};

/// Context3D vertex buffer format
enum class Context3DVertexBufferFormat {
    Float1,
    Float2,
    Float3,
    Float4,
    Bytes4
};

/// Context3D triangle face culling
enum class Context3DTriangleFace {
    None,
    Back,
    Front,
    FrontAndBack
};

/// Context3D profile level
enum class Context3DProfile {
    Baseline,
    BaselineConstrained,
    BaselineExtended,
    Standard,
    StandardConstrained,
    StandardExtended
};

/// Context3D depth/stencil compare mode
enum class Context3DCompareMode {
    Never,
    Less,
    Equal,
    LessEqual,
    Greater,
    NotEqual,
    GreaterEqual,
    Always
};

/// Context3D texture wrap mode
enum class Context3DWrapMode {
    Clamp,
    ClampURepeatV,
    Repeat,
    RepeatUClampV
};

/// Context3D texture filter mode
enum class Context3DTextureFilter {
    Anisotropic16X,
    Anisotropic2X,
    Anisotropic4X,
    Anisotropic8X,
    Linear,
    Nearest
};

/// PixelBender target for shader output
enum class PixelBenderTarget {
    /// Write to bitmap texture
    Bitmap,
    /// Write to temporary texture, read back as bytes
    Bytes
};

/// PixelBender shader output
enum class PixelBenderOutput {
    Bitmap,
    Bytes
};

/// Index buffer interface
class IndexBuffer {
public:
    virtual ~IndexBuffer() = default;
};

/// Vertex buffer interface
class VertexBuffer {
public:
    virtual ~VertexBuffer() = default;
};

/// Shader module interface
class ShaderModule {
public:
    virtual ~ShaderModule() = default;
};

/// Texture interface
class Texture {
public:
    virtual ~Texture() = default;
    virtual uint32_t width() const = 0;
    virtual uint32_t height() const = 0;
};

/// Raw texture interface
class RawTexture {
public:
    virtual ~RawTexture() = default;
    virtual bool equals(const RawTexture* other) const = 0;
};

/// Context3D command for rendering
struct Context3DCommand {
    enum class Type {
        Clear,
        ConfigureBackBuffer,
        SetRenderToTexture,
        SetRenderToBackBuffer,
        UploadToIndexBuffer,
        UploadToVertexBuffer,
        DrawTriangles,
        SetVertexBufferAt,
        UploadShaders,
        SetShaders,
        SetProgramConstantsFromVector,
        SetCulling,
        CopyBitmapToTexture,
        SetTextureAt,
        SetColorMask,
        SetDepthTest,
        SetBlendFactors,
        SetSamplerStateAt,
        SetScissorRectangle
    };

    Type type;
    // Command data would be stored in variant or union
    // Simplified for header-only declaration
};

/// Context3D rendering interface
class Context3D {
public:
    virtual ~Context3D() = default;

    virtual Context3DProfile profile() const = 0;
    virtual BitmapHandle bitmap_handle() const = 0;
    virtual bool should_render() const = 0;

    virtual std::shared_ptr<IndexBuffer> disposed_index_buffer_handle() const = 0;
    virtual std::shared_ptr<VertexBuffer> disposed_vertex_buffer_handle() const = 0;

    virtual std::unique_ptr<IndexBuffer> create_index_buffer(
        BufferUsage usage,
        uint32_t num_indices
    ) = 0;

    virtual std::shared_ptr<VertexBuffer> create_vertex_buffer(
        BufferUsage usage,
        uint32_t num_vertices,
        uint8_t data_32_per_vertex
    ) = 0;

    virtual std::shared_ptr<Texture> create_texture(
        uint32_t width,
        uint32_t height,
        Context3DTextureFormat format,
        bool optimize_for_render_to_texture,
        uint32_t streaming_levels
    ) = 0;

    virtual std::shared_ptr<Texture> create_cube_texture(
        uint32_t size,
        Context3DTextureFormat format,
        bool optimize_for_render_to_texture,
        uint32_t streaming_levels
    ) = 0;

    virtual void process_command(const Context3DCommand& command) = 0;
    virtual void present() = 0;
};

/// Render backend trait interface
class RenderBackend {
public:
    virtual ~RenderBackend() = default;

    virtual ViewportDimensions viewport_dimensions() const = 0;
    virtual void set_viewport_dimensions(const ViewportDimensions& dimensions) = 0;

    virtual ShapeHandle register_shape(
        const DistilledShape& shape,
        const BitmapSource* bitmap_source
    ) = 0;

    virtual std::unique_ptr<SyncHandle> render_offscreen(
        BitmapHandle handle,
        const CommandList& commands,
        StageQuality quality,
        const PixelRegion& bounds
    ) = 0;

    /// Apply filter to bitmap
    virtual std::unique_ptr<SyncHandle> apply_filter(
        BitmapHandle source,
        std::pair<uint32_t, uint32_t> source_point,
        std::pair<uint32_t, uint32_t> source_size,
        BitmapHandle destination,
        std::pair<int32_t, int32_t> dest_point,
        const Filter& filter
    ) {
        return nullptr;
    }

    virtual bool is_filter_supported(const Filter& filter) const {
        return false;
    }

    virtual bool is_offscreen_supported() const {
        return false;
    }

    virtual void submit_frame(
        const swf::Color& clear,
        const CommandList& commands,
        std::vector<BitmapCacheEntry> cache_entries
    ) = 0;

    virtual std::unique_ptr<BitmapHandle> create_empty_texture(
        uint32_t width,
        uint32_t height
    ) = 0;

    virtual std::unique_ptr<BitmapHandle> register_bitmap(const Bitmap& bitmap) = 0;

    virtual void update_texture(
        const BitmapHandle& handle,
        const Bitmap& bitmap,
        const PixelRegion& region
    ) = 0;

    virtual std::unique_ptr<Context3D> create_context3d(Context3DProfile profile) = 0;

    virtual std::string debug_info() const = 0;

    /// Internal name identifying the render backend
    virtual const char* name() const = 0;

    virtual void set_quality(StageQuality quality) = 0;

    virtual std::unique_ptr<PixelBenderShaderHandle> compile_pixelbender_shader(
        PixelBenderShader shader
    ) = 0;

    virtual void run_pixelbender_shader(
        const PixelBenderShaderHandle& handle,
        const std::vector<void*>& arguments,
        PixelBenderTarget target
    ) = 0;

    virtual void resolve_sync_handle(
        std::unique_ptr<SyncHandle> handle,
        RgbaBufRead with_rgba
    ) = 0;
};

/// Convert WStr to Context3DTextureFormat
inline std::optional<Context3DTextureFormat> context3d_texture_format_from_wstr(
    const core::WStr* wstr
) {
    // Implementation would compare wstr to known format strings
    return std::nullopt;
}

/// Convert WStr to Context3DBlendFactor
inline std::optional<Context3DBlendFactor> context3d_blend_factor_from_wstr(
    const core::WStr* wstr
) {
    return std::nullopt;
}

/// Convert WStr to Context3DProfile
inline std::optional<Context3DProfile> context3d_profile_from_wstr(
    const core::WStr* wstr
) {
    return std::nullopt;
}

/// Convert WStr to Context3DCompareMode
inline std::optional<Context3DCompareMode> context3d_compare_mode_from_wstr(
    const core::WStr* wstr
) {
    return std::nullopt;
}

/// Convert WStr to Context3DWrapMode
inline std::optional<Context3DWrapMode> context3d_wrap_mode_from_wstr(
    const core::WStr* wstr
) {
    return std::nullopt;
}

/// Convert WStr to Context3DTextureFilter
inline std::optional<Context3DTextureFilter> context3d_texture_filter_from_wstr(
    const core::WStr* wstr
) {
    return std::nullopt;
}

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_BACKEND_H
