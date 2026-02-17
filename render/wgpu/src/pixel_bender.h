// C++ translation of render/wgpu/src/pixel_bender.rs
//! Pixel Bender shader support for WebGPU

#ifndef RUFFLE_RENDER_WGPU_PIXEL_BENDER_H
#define RUFFLE_RENDER_WGPU_PIXEL_BENDER_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <optional>
#include <variant>
#include <cell>
#include <mutex>
#include <array>

// Forward declarations
namespace ruffle {
namespace render {
    class BitmapHandle;
    namespace error {
        class Error;
    }
    namespace pixel_bender {
        class PixelBenderParam;
        class PixelBenderShader;
        class PixelBenderShaderHandle;
        class PixelBenderShaderImpl;
        class PixelBenderType;
    }
    namespace pixel_bender_support {
        class ImageInputTexture;
        class PixelBenderShaderArgument;
    }
    namespace filters {
        class FilterSource;
    }
}
namespace wgpu {
    class BindGroupLayout;
    class PipelineLayout;
    class RenderPipeline;
    class ShaderModule;
    class Buffer;
    class Texture;
    class TextureView;
    class CommandEncoder;
    class BindGroup;
    class BindGroupEntry;
    class Descriptors;
}
}

namespace ruffle {
namespace render {
namespace wgpu {

/// Shader execution mode
enum class ShaderMode {
    ShaderJob,
    Filter
};

/// Pixel Bender shader implementation for WebGPU
class PixelBenderWgpuShader : public pixel_bender::PixelBenderShaderImpl {
public:
    /// Create a new Pixel Bender shader
    ///
    /// \param descriptors WebGPU descriptors
    /// \param shader Parsed Pixel Bender shader
    static std::unique_ptr<PixelBenderWgpuShader> create(
        const Descriptors* descriptors,
        pixel_bender::PixelBenderShader shader
    );

    /// Get the parsed shader
    const pixel_bender::PixelBenderShader& parsed_shader() const override;

    /// Get a render pipeline for the specified sample count and format
    const wgpu::RenderPipeline* get_pipeline(
        const Descriptors* descriptors,
        uint32_t samples,
        wgpu::TextureFormat format
    );

private:
    PixelBenderWgpuShader(
        wgpu::BindGroupLayout bind_group_layout,
        wgpu::PipelineLayout pipeline_layout,
        wgpu::ShaderModule vertex_shader,
        wgpu::ShaderModule fragment_shader,
        pixel_bender::PixelBenderShader shader,
        wgpu::Buffer float_parameters_buffer,
        uint64_t float_parameters_buffer_size,
        wgpu::Buffer int_parameters_buffer,
        uint64_t int_parameters_buffer_size,
        wgpu::Buffer zeroed_out_of_range_mode
    );

    wgpu::BindGroupLayout bind_group_layout_;
    wgpu::PipelineLayout pipeline_layout_;
    std::unordered_map<std::pair<uint32_t, wgpu::TextureFormat>, wgpu::RenderPipeline> pipelines_;
    wgpu::ShaderModule vertex_shader_;
    wgpu::ShaderModule fragment_shader_;
    pixel_bender::PixelBenderShader shader_;
    wgpu::Buffer float_parameters_buffer_;
    uint64_t float_parameters_buffer_size_;
    wgpu::Buffer int_parameters_buffer_;
    uint64_t int_parameters_buffer_size_;
    wgpu::Buffer zeroed_out_of_range_mode_;
    std::cell::RefCell<StagingBelt> staging_belt_;
};

/// Get the texture format for temporary textures based on channel count
///
/// \param channels Number of channels (1-4)
/// \return Appropriate wgpu texture format
wgpu::TextureFormat temporary_texture_format_for_channels(uint32_t channels);

/// Convert image input to wgpu texture
///
/// \param descriptors WebGPU descriptors
/// \param input Image input texture
/// \return Texture (may be borrowed or owned)
std::variant<const wgpu::Texture*, std::unique_ptr<wgpu::Texture>>
image_input_as_texture(const Descriptors* descriptors, const pixel_bender_support::ImageInputTexture& input);

/// Run a Pixel Bender shader
///
/// \param descriptors WebGPU descriptors
/// \param shader Compiled shader handle
/// \param mode Shader execution mode
/// \param arguments Shader arguments
/// \param target Target texture
/// \param render_command_encoder Command encoder for recording commands
/// \param color_attachment Optional color attachment
/// \param sample_count Sample count for MSAA
/// \param source Filter source geometry
/// \return Result or error
std::result::result<void, render::error::Error>
run_pixelbender_shader(
    const Descriptors* descriptors,
    const pixel_bender::PixelBenderShaderHandle& shader,
    ShaderMode mode,
    const std::vector<pixel_bender_support::PixelBenderShaderArgument>& arguments,
    const wgpu::Texture* target,
    wgpu::CommandEncoder* render_command_encoder,
    std::optional<wgpu::RenderPassColorAttachment> color_attachment,
    uint32_t sample_count,
    const render::filters::FilterSource& source
);

/// Helper to get cache holder from shader handle
const PixelBenderWgpuShader* as_cache_holder(const pixel_bender::PixelBenderShaderHandle& handle);

} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_PIXEL_BENDER_H
