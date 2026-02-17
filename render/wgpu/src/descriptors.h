// C++ translation of render/wgpu/src/descriptors.rs
// WGPU device and resource descriptors

#ifndef RUFFLE_RENDER_WGPU_DESCRIPTORS_H
#define RUFFLE_RENDER_WGPU_DESCRIPTORS_H

#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <string>
#include <format>

// Forward declarations
namespace wgpu {
    class Instance;
    class Adapter;
    class Device;
    class Queue;
    struct Limits;
    class RenderPipeline;
    class PipelineLayout;
    class BindGroupLayout;
    class ShaderModule;
    class TextureView;
    class Buffer;
    struct TextureFormat;
    struct VertexBufferLayout;
    enum class Backend : uint8_t;
    struct PipelineLayoutDescriptor;
    struct RenderPipelineDescriptor;
    struct VertexState;
    struct FragmentState;
    struct PrimitiveState;
    struct MultisampleState;
    struct ColorTargetState;
    struct BlendState;
    struct PipelineCompilationOptions;
}

namespace ruffle {
namespace render {
namespace wgpu {

// Forward declarations
struct BitmapSamplers;
struct BindLayouts;
struct Shaders;
struct Pipelines;
struct Filters;
struct PosVertex;
struct PosColorVertex;
struct FilterVertex;
struct TextureTransforms;

/// Quad geometry for rendering
///
/// Contains vertex and index buffers for drawing quads.
struct Quad {
    /// Vertex buffer for position-only vertices
    wgpu::Buffer* vertices_pos;

    /// Vertex buffer for position+color vertices
    wgpu::Buffer* vertices_pos_color;

    /// Vertex buffer for filter vertices (with UVs)
    wgpu::Buffer* filter_vertices;

    /// Index buffer for triangle list (6 indices)
    wgpu::Buffer* indices;

    /// Index buffer for line drawing (2 indices)
    wgpu::Buffer* indices_line;

    /// Index buffer for line rectangle (5 indices)
    wgpu::Buffer* indices_line_rect;

    /// Texture transforms uniform buffer
    wgpu::Buffer* texture_transforms;

    /// Default constructor
    Quad() = default;

    /// Create quad geometry
    ///
    /// \param device The wgpu device
    Quad(wgpu::Device* device);
};

/// Hash map key for pipeline caching
struct PipelineKey {
    uint32_t sample_count;
    wgpu::TextureFormat format;

    bool operator==(const PipelineKey& other) const {
        return sample_count == other.sample_count && format == other.format;
    }
};

/// Hash function for PipelineKey
struct PipelineKeyHash {
    size_t operator()(const PipelineKey& key) const {
        return std::hash<uint32_t>()(key.sample_count) ^
               (std::hash<uint32_t>()(static_cast<uint32_t>(key.format)) << 1);
    }
};

/// WGPU Descriptors
///
/// Contains all the wgpu resources needed for rendering:
/// - Instance, adapter, device, queue
/// - Samplers, bind layouts, shaders
/// - Pipeline caches
/// - Filter resources
class Descriptors {
public:
    /// WGPU instance
    wgpu::Instance* wgpu_instance;

    /// WGPU adapter
    wgpu::Adapter* adapter;

    /// WGPU device
    wgpu::Device* device;

    /// Device limits
    wgpu::Limits limits;

    /// Backend type
    wgpu::Backend backend;

    /// WGPU queue
    wgpu::Queue* queue;

    /// Bitmap samplers
    std::unique_ptr<BitmapSamplers> bitmap_samplers;

    /// Bind group layouts
    std::unique_ptr<BindLayouts> bind_layouts;

    /// Quad geometry
    Quad quad;

    /// Shaders
    std::unique_ptr<Shaders> shaders;

    /// Filters
    std::unique_ptr<Filters> filters;

private:
    /// Copy pipeline cache (sample_count, format) -> pipeline
    mutable std::mutex copy_pipeline_mutex_;
    std::unordered_map<PipelineKey, wgpu::RenderPipeline*, PipelineKeyHash> copy_pipeline_;

    /// Copy sRGB pipeline cache
    mutable std::mutex copy_srgb_pipeline_mutex_;
    std::unordered_map<PipelineKey, wgpu::RenderPipeline*, PipelineKeyHash> copy_srgb_pipeline_;

    /// Pipelines cache
    mutable std::mutex pipelines_mutex_;
    std::unordered_map<PipelineKey, std::shared_ptr<Pipelines>, PipelineKeyHash> pipelines_;

public:
    /// Default constructor
    Descriptors() = default;

    /// Create descriptors
    ///
    /// \param instance The WGPU instance
    /// \param adapter The WGPU adapter
    /// \param device The WGPU device
    /// \param queue The WGPU queue
    Descriptors(
        wgpu::Instance* instance,
        wgpu::Adapter* adapter,
        wgpu::Device* device,
        wgpu::Queue* queue
    );

    /// Get or create copy sRGB pipeline
    ///
    /// \param format The texture format
    /// \param msaa_sample_count The MSAA sample count
    /// \return The render pipeline
    wgpu::RenderPipeline* copy_srgb_pipeline(
        wgpu::TextureFormat format,
        uint32_t msaa_sample_count
    );

    /// Get or create copy pipeline
    ///
    /// \param format The texture format
    /// \param msaa_sample_count The MSAA sample count
    /// \return The render pipeline
    wgpu::RenderPipeline* copy_pipeline(
        wgpu::TextureFormat format,
        uint32_t msaa_sample_count
    );

    /// Get or create pipelines
    ///
    /// \param msaa_sample_count The MSAA sample count
    /// \param format The texture format
    /// \return Shared pointer to pipelines
    std::shared_ptr<Pipelines> pipelines(
        uint32_t msaa_sample_count,
        wgpu::TextureFormat format
    );
};

// Vertex buffer descriptions
extern const std::array<wgpu::VertexBufferLayout, 1> VERTEX_BUFFERS_DESCRIPTION_POS;

} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_DESCRIPTORS_H
