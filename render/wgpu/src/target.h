// C++ translation of render/wgpu/src/target.rs
// Render target abstraction for wgpu

#ifndef RUFFLE_RENDER_WGPU_TARGET_H
#define RUFFLE_RENDER_WGPU_TARGET_H

#include <cstdint>
#include <memory>
#include <vector>

// Forward declarations
namespace wgpu {
    class Device;
    class Queue;
    class Surface;
    class Adapter;
    class Texture;
    class TextureView;
    class Buffer;
    struct SurfaceConfiguration;
    struct SurfaceError;
    struct Extent3d;
    struct TextureFormat;
    struct TextureDescriptor;
    struct CommandBuffer;
    using SubmissionIndex = uint64_t;
}

namespace ruffle {
namespace render {
    struct PixelRegion;
}
}

namespace ruffle {
namespace render {
namespace wgpu {

// Forward declarations
struct BufferDimensions;
template<typename T, typename D> class PoolEntry;

/// Error type for render target operations
using Error = std::runtime_error;

/// Frame interface for render targets
///
/// Represents a single frame from a render target.
/// Provides access to the texture view for rendering.
class RenderTargetFrame {
public:
    virtual ~RenderTargetFrame() = default;

    /// Get the texture view for this frame
    ///
    /// \return Reference to the texture view
    [[nodiscard]] virtual wgpu::TextureView* view() const = 0;

    /// Convert this frame into its texture view (consuming the frame)
    ///
    /// \return Unique pointer to the texture view
    [[nodiscard]] virtual std::unique_ptr<wgpu::TextureView> into_view() = 0;
};

/// Render target interface
///
/// Abstract interface for different types of render targets
/// (swap chain, texture, etc.).
template<typename FrameType>
class RenderTarget {
public:
    using Frame = FrameType;

    virtual ~RenderTarget() = default;

    /// Resize the render target
    ///
    /// \param device The wgpu device
    /// \param width New width in pixels
    /// \param height New height in pixels
    virtual void resize(wgpu::Device* device, uint32_t width, uint32_t height) = 0;

    /// Get the texture format
    ///
    /// \return The texture format
    [[nodiscard]] virtual wgpu::TextureFormat format() const = 0;

    /// Get the width
    ///
    /// \return Width in pixels
    [[nodiscard]] virtual uint32_t width() const = 0;

    /// Get the height
    ///
    /// \return Height in pixels
    [[nodiscard]] virtual uint32_t height() const = 0;

    /// Get the next frame for rendering
    ///
    /// \return The next frame, or error if unavailable
    virtual std::unique_ptr<Frame> get_next_texture() = 0;

    /// Submit command buffers for execution
    ///
    /// \param device The wgpu device
    /// \param queue The wgpu queue
    /// \param command_buffers Command buffers to submit
    /// \param frame The frame to present (for swap chain targets)
    /// \return Submission index
    virtual wgpu::SubmissionIndex submit(
        wgpu::Device* device,
        wgpu::Queue* queue,
        const std::vector<wgpu::CommandBuffer>& command_buffers,
        std::unique_ptr<Frame> frame
    ) = 0;
};

/// Swap chain render target frame
///
/// Represents a frame from a swap chain (window surface).
class SwapChainTargetFrame : public RenderTargetFrame {
private:
    /// The surface texture
    void* texture;  // wgpu::SurfaceTexture - placeholder

    /// The texture view
    std::unique_ptr<wgpu::TextureView> view_;

public:
    /// Constructor
    ///
    /// \param tex The surface texture
    /// \param tex_view The texture view
    SwapChainTargetFrame(void* tex, std::unique_ptr<wgpu::TextureView> tex_view)
        : texture(tex), view_(std::move(tex_view)) {}

    /// Get the texture view
    [[nodiscard]] wgpu::TextureView* view() const override {
        return view_.get();
    }

    /// Convert to texture view
    [[nodiscard]] std::unique_ptr<wgpu::TextureView> into_view() override {
        return std::move(view_);
    }
};

/// Swap chain render target
///
/// Renders to a window surface using wgpu swap chain.
class SwapChainTarget : public RenderTarget<SwapChainTargetFrame> {
private:
    /// The window surface
    wgpu::Surface* window_surface;

    /// Surface configuration
    wgpu::SurfaceConfiguration surface_config;

public:
    /// Constructor
    ///
    /// Creates a swap chain target for rendering to a window.
    ///
    /// \param surface The wgpu surface
    /// \param adapter The wgpu adapter
    /// \param width Initial width
    /// \param height Initial height
    /// \param device The wgpu device
    SwapChainTarget(
        wgpu::Surface* surface,
        wgpu::Adapter* adapter,
        uint32_t width,
        uint32_t height,
        wgpu::Device* device
    );

    /// Resize the swap chain
    void resize(wgpu::Device* device, uint32_t width, uint32_t height) override;

    /// Get the texture format
    [[nodiscard]] wgpu::TextureFormat format() const override;

    /// Get the width
    [[nodiscard]] uint32_t width() const override;

    /// Get the height
    [[nodiscard]] uint32_t height() const override;

    /// Get the next frame
    std::unique_ptr<SwapChainTargetFrame> get_next_texture() override;

    /// Submit command buffers
    wgpu::SubmissionIndex submit(
        wgpu::Device* device,
        wgpu::Queue* queue,
        const std::vector<wgpu::CommandBuffer>& command_buffers,
        std::unique_ptr<SwapChainTargetFrame> frame
    ) override;
};

/// Buffer ownership type
///
/// Represents either a borrowed buffer from a pool or an owned buffer.
enum class MaybeOwnedBufferType {
    Borrowed,
    Owned,
};

/// Buffer info for texture readback
///
/// Contains buffer and copy region information for reading back texture data.
struct TextureBufferInfo {
    /// The buffer (owned or borrowed)
    wgpu::Buffer* buffer;

    /// Buffer dimensions
    BufferDimensions* dimensions;

    /// The region of the texture to copy
    PixelRegion copy_area;
};

/// Texture render target
///
/// Renders to an off-screen texture that can be read back.
class TextureTarget : public RenderTarget<RenderTargetFrame> {
private:
    /// Texture size
    wgpu::Extent3d size;

    /// The texture
    wgpu::Texture* texture;

    /// Texture format
    wgpu::TextureFormat format;

    /// Optional buffer for readback
    TextureBufferInfo* buffer;

public:
    /// Constructor
    ///
    /// Creates a texture render target.
    ///
    /// \param device The wgpu device
    /// \param width Texture width
    /// \param height Texture height
    TextureTarget(wgpu::Device* device, uint32_t width, uint32_t height);

    /// Destructor
    ~TextureTarget();

    /// Resize the texture
    void resize(wgpu::Device* device, uint32_t width, uint32_t height) override;

    /// Get the texture format
    [[nodiscard]] wgpu::TextureFormat format() const override;

    /// Get the width
    [[nodiscard]] uint32_t width() const override;

    /// Get the height
    [[nodiscard]] uint32_t height() const override;

    /// Get the next frame
    std::unique_ptr<RenderTargetFrame> get_next_texture() override;

    /// Submit command buffers
    wgpu::SubmissionIndex submit(
        wgpu::Device* device,
        wgpu::Queue* queue,
        const std::vector<wgpu::CommandBuffer>& command_buffers,
        std::unique_ptr<RenderTargetFrame> frame
    ) override;

    /// Get the texture
    ///
    /// \return Pointer to the texture
    [[nodiscard]] wgpu::Texture* get_texture() const {
        return texture;
    }

    /// Take ownership of the buffer
    ///
    /// \return The buffer info, or nullptr if no buffer
    TextureBufferInfo* take_buffer();
};

} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_TARGET_H
