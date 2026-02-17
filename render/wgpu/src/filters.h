// C++ translation of render/wgpu/src/filters.rs
// Filter rendering for wgpu

#ifndef RUFFLE_RENDER_WGPU_FILTERS_H
#define RUFFLE_RENDER_WGPU_FILTERS_H

#include <cstdint>
#include <cstddef>
#include <array>
#include <optional>
#include <memory>
#include <set>
#include <mutex>

// Forward declarations
namespace wgpu {
    class Device;
    class Texture;
    class TextureView;
    class CommandEncoder;
    class BindGroupLayout;
    struct VertexBufferLayout;
    struct TextureFormat;
}

namespace ruffle {
namespace render {
    enum class Filter;
}
}

namespace ruffle {
namespace render {
namespace wgpu {

// Forward declarations
struct Descriptors;
struct TexturePool;

// Forward declare filter types
namespace filters {
    struct BlurFilter;
    struct ColorMatrixFilter;
    struct ShaderFilter;
    struct GlowFilter;
    struct BevelFilter;
    struct DisplacementMapFilter;
}

/// Vertex for filter rendering
///
/// Contains position and UV coordinates.
struct FilterVertex {
    std::array<float, 2> position;
    std::array<float, 2> uv;
};

/// Vertex for filter rendering with blur offset
///
/// Contains position, source UV, and blur UV coordinates.
struct FilterVertexWithBlur {
    std::array<float, 2> position;
    std::array<float, 2> source_uv;
    std::array<float, 2> blur_uv;
};

/// Vertex for filter rendering with double blur (highlight and shadow)
///
/// Contains position, source UV, and two blur UV coordinates.
struct FilterVertexWithDoubleBlur {
    std::array<float, 2> position;
    std::array<float, 2> source_uv;
    std::array<float, 2> blur_uv_left;
    std::array<float, 2> blur_uv_right;
};

/// Vertex buffer layout for filter vertices
inline constexpr std::array<wgpu::VertexBufferLayout, 1> VERTEX_BUFFERS_DESCRIPTION_FILTERS = {{
    wgpu::VertexBufferLayout{
        .array_stride = sizeof(FilterVertex),
        .step_mode = wgpu::VertexStepMode::Vertex,
        .attributes = {
            wgpu::VertexAttribute{.format = wgpu::VertexFormat::Float32x2, .offset = 0, .shader_location = 0},
            wgpu::VertexAttribute{.format = wgpu::VertexFormat::Float32x2, .offset = 8, .shader_location = 1},
        }
    }
}};

/// Vertex buffer layout for filter vertices with blur
inline constexpr std::array<wgpu::VertexBufferLayout, 1> VERTEX_BUFFERS_DESCRIPTION_FILTERS_WITH_BLUR = {{
    wgpu::VertexBufferLayout{
        .array_stride = sizeof(FilterVertexWithBlur),
        .step_mode = wgpu::VertexStepMode::Vertex,
        .attributes = {
            wgpu::VertexAttribute{.format = wgpu::VertexFormat::Float32x2, .offset = 0, .shader_location = 0},
            wgpu::VertexAttribute{.format = wgpu::VertexFormat::Float32x2, .offset = 8, .shader_location = 1},
            wgpu::VertexAttribute{.format = wgpu::VertexFormat::Float32x2, .offset = 16, .shader_location = 2},
        }
    }
}};

/// Vertex buffer layout for filter vertices with double blur
inline constexpr std::array<wgpu::VertexBufferLayout, 1> VERTEX_BUFFERS_DESCRIPTION_FILTERS_WITH_DOUBLE_BLUR = {{
    wgpu::VertexBufferLayout{
        .array_stride = sizeof(FilterVertexWithDoubleBlur),
        .step_mode = wgpu::VertexStepMode::Vertex,
        .attributes = {
            wgpu::VertexAttribute{.format = wgpu::VertexFormat::Float32x2, .offset = 0, .shader_location = 0},
            wgpu::VertexAttribute{.format = wgpu::VertexFormat::Float32x2, .offset = 8, .shader_location = 1},
            wgpu::VertexAttribute{.format = wgpu::VertexFormat::Float32x2, .offset = 16, .shader_location = 2},
            wgpu::VertexAttribute{.format = wgpu::VertexFormat::Float32x2, .offset = 24, .shader_location = 3},
        }
    }
}};

/// Filter source texture information
///
/// Describes the source texture and region to apply filters to.
struct FilterSource {
    /// The source texture
    const wgpu::Texture* texture;

    /// Top-left point of the region
    std::pair<uint32_t, uint32_t> point;

    /// Size of the region
    std::pair<uint32_t, uint32_t> size;

    /// Create a filter source for an entire texture
    ///
    /// \param texture The source texture
    /// \return FilterSource covering the entire texture
    static FilterSource for_entire_texture(const wgpu::Texture* texture) {
        return FilterSource{
            .texture = texture,
            .point = {0, 0},
            .size = {texture->getWidth(), texture->getHeight()}
        };
    }

    /// Get vertices for the filter region
    ///
    /// \return Array of 4 FilterVertex for the region
    std::array<FilterVertex, 4> vertices() const;

    /// Get vertices with blur offset
    ///
    /// \param blur_offset The blur offset in pixels
    /// \return Array of 4 FilterVertexWithBlur
    std::array<FilterVertexWithBlur, 4> vertices_with_blur_offset(
        std::pair<float, float> blur_offset
    ) const;

    /// Get vertices with highlight and shadow blur offsets
    ///
    /// \param blur_offset The blur offset in pixels
    /// \return Array of 4 FilterVertexWithDoubleBlur
    std::array<FilterVertexWithDoubleBlur, 4> vertices_with_highlight_and_shadow(
        std::pair<float, float> blur_offset
    ) const;
};

/// Filter rendering resources
///
/// Contains all the filter types and their resources.
class Filters {
public:
    /// Blur filter
    std::unique_ptr<filters::BlurFilter> blur;

    /// Color matrix filter
    std::unique_ptr<filters::ColorMatrixFilter> color_matrix;

    /// Shader filter
    std::unique_ptr<filters::ShaderFilter> shader;

    /// Glow filter
    std::unique_ptr<filters::GlowFilter> glow;

    /// Bevel filter
    std::unique_ptr<filters::BevelFilter> bevel;

    /// Displacement map filter
    std::unique_ptr<filters::DisplacementMapFilter> displacement_map;

    /// Default constructor
    Filters() = default;

    /// Create filters
    ///
    /// \param device The wgpu device
    explicit Filters(wgpu::Device* device);

    /// Apply a filter to a source texture
    ///
    /// \param descriptors The render descriptors
    /// \param draw_encoder The command encoder
    /// \param texture_pool The texture pool for temporary textures
    /// \param staging_belt The staging belt for buffer uploads
    /// \param source The filter source
    /// \param filter The filter to apply
    /// \return The command target (may be nullopt for unsupported filters)
    std::optional<void*> apply(
        Descriptors* descriptors,
        wgpu::CommandEncoder* draw_encoder,
        TexturePool* texture_pool,
        void* staging_belt,
        const FilterSource& source,
        ruffle::render::Filter filter
    );
};

/// Warned filters set for one-time warnings
class WarnedFilters {
private:
    mutable std::mutex mutex_;
    std::set<std::string> warned_;

public:
    /// Insert a filter name and return true if it was newly inserted
    ///
    /// \param name The filter name
    /// \return True if this is the first time warning about this filter
    bool insert(const std::string& name) {
        std::lock_guard<std::mutex> lock(mutex_);
        return warned_.insert(name).second;
    }
};

/// Global warned filters instance
extern WarnedFilters g_warned_filters;

} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_FILTERS_H
