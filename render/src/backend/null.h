// C++ translation of render/src/backend/null.rs
//! Null renderer implementation

#ifndef RUFFLE_RENDER_BACKEND_NULL_H
#define RUFFLE_RENDER_BACKEND_NULL_H

#include <memory>
#include <string>
#include <optional>
#include <cstdint>

// Forward declarations
namespace swf {
    struct Color;
}

namespace ruffle {
namespace render {

// Forward declarations from backend.h
struct ViewportDimensions;
class Bitmap;
struct BitmapHandle;
class BitmapSource;
struct PixelRegion;
class SyncHandle;
class PixelBenderShader;
struct PixelBenderShaderHandle;
enum class StageQuality;
struct DistilledShape;
struct ShapeHandle;
class CommandList;
class Error;
class Context3D;
enum class Context3DProfile;
enum class PixelBenderTarget;
enum class PixelBenderOutput;

using RgbaBufRead = std::function<void(const uint8_t*, uint32_t)>;

// Forward declarations from bitmap.h
struct BitmapSize;

namespace backend {

/// Null bitmap source
///
/// A bitmap source that always returns None for bitmap queries.
/// Used for testing or when no bitmap data is available.
class NullBitmapSource : public BitmapSource {
public:
    /// Get the size of a bitmap by ID
    /// @param id The bitmap ID
    /// @return Always None
    std::optional<BitmapSize> bitmap_size(uint16_t id) override {
        return std::nullopt;
    }

    /// Get a bitmap handle by ID
    /// @param id The bitmap ID
    /// @param renderer The render backend
    /// @return Always None
    std::optional<BitmapHandle> bitmap_handle(uint16_t id, RenderBackend* renderer) override {
        return std::nullopt;
    }
};

/// Null bitmap handle implementation
class NullBitmapHandleImpl {
public:
    virtual ~NullBitmapHandleImpl() = default;
};

/// Null shape handle implementation
class NullShapeHandleImpl {
public:
    virtual ~NullShapeHandleImpl() = default;
};

/// Null renderer
///
/// A renderer that does nothing. Used for testing or when
/// no actual rendering is needed.
class NullRenderer : public RenderBackend {
private:
    ViewportDimensions dimensions_;

public:
    /// Create a new null renderer
    /// @param dimensions The viewport dimensions
    explicit NullRenderer(const ViewportDimensions& dimensions)
        : dimensions_(dimensions) {}

    /// Get the current viewport dimensions
    /// @return The viewport dimensions
    ViewportDimensions viewport_dimensions() const override {
        return dimensions_;
    }

    /// Set the viewport dimensions
    /// @param dimensions The new viewport dimensions
    void set_viewport_dimensions(const ViewportDimensions& dimensions) override {
        dimensions_ = dimensions;
    }

    /// Register a shape for rendering
    ///
    /// Creates a null shape handle that does nothing.
    ///
    /// @param shape The shape to register
    /// @param bitmap_source The bitmap source for texture lookups
    /// @return A null shape handle
    ShapeHandle register_shape(
        const DistilledShape& shape,
        const BitmapSource* bitmap_source
    ) override {
        return ShapeHandle(std::make_shared<NullShapeHandleImpl>());
    }

    /// Render offscreen to a bitmap
    ///
    /// Not supported by the null renderer.
    ///
    /// @param handle The bitmap handle
    /// @param commands The rendering commands
    /// @param quality The rendering quality
    /// @param bounds The pixel region bounds
    /// @return Always None
    std::unique_ptr<SyncHandle> render_offscreen(
        BitmapHandle handle,
        const CommandList& commands,
        StageQuality quality,
        const PixelRegion& bounds
    ) override {
        return nullptr;
    }

    /// Submit a frame for rendering
    ///
    /// Does nothing in the null renderer.
    ///
    /// @param clear The clear color
    /// @param commands The rendering commands
    /// @param cache_entries Bitmap cache entries
    void submit_frame(
        const swf::Color& clear,
        const CommandList& commands,
        std::vector<BitmapCacheEntry> cache_entries
    ) override {
        // Does nothing
    }

    /// Register a bitmap
    ///
    /// Creates a null bitmap handle.
    ///
    /// @param bitmap The bitmap to register
    /// @return A null bitmap handle
    std::unique_ptr<BitmapHandle> register_bitmap(const Bitmap& bitmap) override {
        return std::make_unique<BitmapHandle>(std::make_shared<NullBitmapHandleImpl>());
    }

    /// Update a texture with new bitmap data
    ///
    /// Does nothing in the null renderer.
    ///
    /// @param handle The bitmap handle to update
    /// @param bitmap The new bitmap data
    /// @param region The region to update
    /// @return Success
    void update_texture(
        const BitmapHandle& handle,
        const Bitmap& bitmap,
        const PixelRegion& region
    ) override {
        // Does nothing
    }

    /// Create a Context3D instance
    ///
    /// Not supported by the null renderer.
    ///
    /// @param profile The Context3D profile
    /// @return Error indicating unimplemented
    std::unique_ptr<Context3D> create_context3d(Context3DProfile profile) override {
        return nullptr;  // Would return Error in full implementation
    }

    /// Get debug information about the renderer
    /// @return Debug info string
    std::string debug_info() const override {
        return "Renderer: Null";
    }

    /// Get the internal name of the renderer
    /// @return Empty string for null renderer
    const char* name() const override {
        return "";
    }

    /// Set the rendering quality
    ///
    /// Does nothing in the null renderer.
    ///
    /// @param quality The quality level
    void set_quality(StageQuality quality) override {
        // Does nothing
    }

    /// Run a PixelBender shader
    ///
    /// Not supported by the null renderer.
    ///
    /// @param shader The shader handle
    /// @param arguments The shader arguments
    /// @param target The output target
    /// @return Error indicating unimplemented
    void run_pixelbender_shader(
        const PixelBenderShaderHandle& shader,
        const std::vector<void*>& arguments,
        PixelBenderTarget target
    ) override {
        // Would return Error in full implementation
    }

    /// Resolve a sync handle
    ///
    /// Not supported by the null renderer.
    ///
    /// @param handle The sync handle to resolve
    /// @param with_rgba Callback for RGBA data
    /// @return Error indicating unimplemented
    void resolve_sync_handle(
        std::unique_ptr<SyncHandle> handle,
        RgbaBufRead with_rgba
    ) override {
        // Would return Error in full implementation
    }

    /// Compile a PixelBender shader
    ///
    /// Not supported by the null renderer.
    ///
    /// @param shader The shader to compile
    /// @return Error indicating unimplemented
    std::unique_ptr<PixelBenderShaderHandle> compile_pixelbender_shader(
        PixelBenderShader shader
    ) override {
        return nullptr;  // Would return Error in full implementation
    }

    /// Create an empty texture
    ///
    /// Creates a null bitmap handle.
    ///
    /// @param width The texture width
    /// @param height The texture height
    /// @return A null bitmap handle
    std::unique_ptr<BitmapHandle> create_empty_texture(
        uint32_t width,
        uint32_t height
    ) override {
        return std::make_unique<BitmapHandle>(std::make_shared<NullBitmapHandleImpl>());
    }
};

} // namespace backend
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_BACKEND_NULL_H
