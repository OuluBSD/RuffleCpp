// C++ translation of desktop/src/gui/movie.rs
// Movie view rendering for desktop GUI

#ifndef RUFFLE_DESKTOP_GUI_MOVIE_H
#define RUFFLE_DESKTOP_GUI_MOVIE_H

#include <cstdint>
#include <memory>
#include <array>
#include <fmt/format.h>

// Forward declarations - wgpu types
namespace wgpu {
    class Device;
    class Queue;
    class ShaderModule;
    class BindGroupLayout;
    class RenderPipeline;
    class PipelineLayout;
    class Sampler;
    class Buffer;
    class Texture;
    class TextureView;
    class BindGroup;
    class RenderPassEncoder;

    struct TextureDescriptor;
    struct TextureViewDescriptor;
    struct BindGroupDescriptor;
    struct BindGroupEntry;
    struct BufferInitDescriptor;
    struct RenderPipelineDescriptor;
    struct VertexState;
    struct VertexBufferLayout;
    struct VertexAttribute;
    struct PrimitiveState;
    struct MultisampleState;
    struct FragmentState;
    struct ColorTargetState;
    struct ShaderModuleDescriptor;
    struct BindGroupLayoutDescriptor;
    struct BindGroupLayoutEntry;
    struct PipelineLayoutDescriptor;
    struct Extent3d;

    enum class TextureFormat;
    enum class TextureDimension;
    enum class TextureUsages;
    enum class FilterMode;
    enum class ShaderStages;
    enum class BindingType;
    enum class TextureSampleType;
    enum class TextureViewDimension;
    enum class SamplerBindingType;
    enum class VertexStepMode;
    enum class PrimitiveTopology;
    enum class FrontFace;
    enum class PolygonMode;
    enum class CullMode;
    enum class BufferUsages;
    enum class ColorWrites;
    enum class SurfaceError;

    class SubmissionIndex;

    namespace util {
        class DeviceExt;
    }
}

namespace ruffle {
namespace render_wgpu {
    namespace descriptors {
        class Descriptors;
    }
    namespace target {
        class RenderTarget;
        class RenderTargetFrame;
    }
}
}

namespace ruffle {
namespace desktop {
namespace gui {

/// Size of the top menu bar in pixels.
/// This is the offset at which the movie will be shown,
/// and added to the window size if trying to match a movie.
constexpr uint32_t MENU_HEIGHT = 24;

/// Get vertices for rendering the movie view
/// @param has_menu Whether the menu bar is visible
/// @param height Window height in pixels
/// @param scale_factor DPI scale factor
/// @return Array of 6 vertices (x, y, u, v format)
inline std::array<std::array<float, 4>, 6> get_vertices(
    bool has_menu,
    uint32_t height,
    double scale_factor
) {
    float top = 1.0f;
    if (has_menu) {
        float menu_height = static_cast<float>(MENU_HEIGHT) * static_cast<float>(scale_factor);
        top = 1.0f - ((menu_height / static_cast<float>(height)) * 2.0f);
    }

    // x y u v format (position and texture coordinates)
    return {{
        {{-1.0f, top,     0.0f, 0.0f}},  // tl (top-left)
        {{ 1.0f, top,     1.0f, 0.0f}},  // tr (top-right)
        {{ 1.0f, -1.0f,   1.0f, 1.0f}},  // br (bottom-right)
        {{ 1.0f, -1.0f,   1.0f, 1.0f}},  // br (duplicate for triangle)
        {{-1.0f, -1.0f,   0.0f, 1.0f}},  // bl (bottom-left)
        {{-1.0f, top,     0.0f, 0.0f}},  // tl (duplicate for triangle)
    }};
}

/// Renderer for the movie view
/// Manages the wgpu pipeline, sampler, and vertex buffer for blitting
class MovieViewRenderer {
private:
    wgpu::BindGroupLayout bind_group_layout_;
    wgpu::RenderPipeline pipeline_;
    wgpu::Sampler sampler_;
    wgpu::Buffer vertices_;

public:
    /// Create a new MovieViewRenderer
    ///
    /// @param device The wgpu device
    /// @param surface_format The surface texture format
    /// @param has_menu Whether the menu bar is visible
    /// @param height Window height in pixels
    /// @param scale_factor DPI scale factor
    MovieViewRenderer(
        const wgpu::Device& device,
        wgpu::TextureFormat surface_format,
        bool has_menu,
        uint32_t height,
        double scale_factor
    );

    /// Update the vertex buffer when resolution changes
    ///
    /// @param descriptors The wgpu descriptors (for queue access)
    /// @param has_menu Whether the menu bar is visible
    /// @param height Window height in pixels
    /// @param scale_factor DPI scale factor
    void update_resolution(
        const ruffle::render_wgpu::descriptors::Descriptors& descriptors,
        bool has_menu,
        uint32_t height,
        double scale_factor
    ) const;

    /// Get the bind group layout
    const wgpu::BindGroupLayout& bind_group_layout() const {
        return bind_group_layout_;
    }

    /// Get the render pipeline
    const wgpu::RenderPipeline& pipeline() const {
        return pipeline_;
    }

    /// Get the sampler
    const wgpu::Sampler& sampler() const {
        return sampler_;
    }

    /// Get the vertex buffer
    const wgpu::Buffer& vertices() const {
        return vertices_;
    }
};

/// Movie view frame wrapper
class MovieViewFrame : public ruffle::render_wgpu::target::RenderTargetFrame {
private:
    wgpu::TextureView view_;

public:
    explicit MovieViewFrame(wgpu::TextureView view)
        : view_(std::move(view)) {}

    wgpu::TextureView into_view() override {
        return std::move(view_);
    }

    const wgpu::TextureView& view() const override {
        return view_;
    }
};

/// Movie view - displays the Flash movie content using wgpu
/// Implements RenderTarget for integration with the rendering system
class MovieView : public ruffle::render_wgpu::target::RenderTarget {
private:
    std::shared_ptr<MovieViewRenderer> renderer_;
    wgpu::Texture texture_;
    wgpu::BindGroup bind_group_;

public:
    using Frame = MovieViewFrame;

    /// Create a new MovieView
    ///
    /// @param renderer The movie view renderer
    /// @param device The wgpu device
    /// @param width Movie width in pixels
    /// @param height Movie height in pixels
    MovieView(
        std::shared_ptr<MovieViewRenderer> renderer,
        const wgpu::Device& device,
        uint32_t width,
        uint32_t height
    );

    /// Render the movie view to a render pass
    ///
    /// @param renderer The movie view renderer
    /// @param render_pass The render pass encoder
    void render(
        const MovieViewRenderer& renderer,
        wgpu::RenderPassEncoder* render_pass
    ) const;

    // RenderTarget implementation
    void resize(const wgpu::Device& device, uint32_t width, uint32_t height) override {
        *this = MovieView(renderer_, device, width, height);
    }

    wgpu::TextureFormat format() const override {
        return texture_.format();
    }

    uint32_t width() const override {
        return texture_.width();
    }

    uint32_t height() const override {
        return texture_.height();
    }

    Result<MovieViewFrame, wgpu::SurfaceError> get_next_texture() override {
        return MovieViewFrame(texture_.create_view(nullptr));
    }

    wgpu::SubmissionIndex submit(
        const wgpu::Device& device,
        const wgpu::Queue& queue,
        const std::vector<wgpu::CommandBuffer>& command_buffers,
        MovieViewFrame frame
    ) override {
        (void)device; // Unused
        return queue.submit(command_buffers);
    }

    /// Get the texture
    const wgpu::Texture& texture() const {
        return texture_;
    }

    /// Get the bind group
    const wgpu::BindGroup& bind_group() const {
        return bind_group_;
    }
};

} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_MOVIE_H
