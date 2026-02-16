// C++ translation of render/wgpu/src/globals.rs
// Global uniform buffers for wgpu rendering

#ifndef RUFFLE_RENDER_WGPU_GLOBALS_H
#define RUFFLE_RENDER_WGPU_GLOBALS_H

#include <array>
#include <cstdint>

// Forward declarations
namespace wgpu {
    class BindGroup;
    class Buffer;
    class Device;
    class BindGroupLayout;
}

namespace ruffle {
namespace render {
namespace wgpu {

/// Uniform data for global transforms
///
/// Contains the view matrix used for transforming coordinates
/// from screen space to normalized device coordinates.
struct GlobalsUniform {
    /// 4x4 view matrix for coordinate transformation
    std::array<std::array<float, 4>, 4> view_matrix;

    /// Default constructor - creates identity-like matrix
    /// The matrix is set up to transform from viewport coordinates
    /// to normalized device coordinates (-1 to 1 range).
    GlobalsUniform() : view_matrix{} {}

    /// Constructor from viewport dimensions
    ///
    /// \param viewport_width Width of the viewport in pixels
    /// \param viewport_height Height of the viewport in pixels
    GlobalsUniform(uint32_t viewport_width, uint32_t viewport_height) {
        const float inv_half_width = 1.0f / (static_cast<float>(viewport_width) / 2.0f);
        const float inv_half_height = 1.0f / (static_cast<float>(viewport_height) / 2.0f);

        view_matrix = {{
            {inv_half_width, 0.0f, 0.0f, 0.0f},
            {0.0f, -inv_half_height, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {-1.0f, 1.0f, 0.0f, 1.0f},
        }};
    }
};

/// Global uniform buffer wrapper
///
/// Manages the wgpu bind group and buffer for global uniforms.
/// The buffer contains the view matrix used for coordinate transformations.
class Globals {
private:
    /// The bind group for accessing global uniforms in shaders
    wgpu::BindGroup* bind_group;

    /// The underlying buffer (kept alive by the bind group)
    wgpu::Buffer* buffer;

public:
    /// Constructor
    ///
    /// Creates the uniform buffer and bind group for global transforms.
    ///
    /// \param device The wgpu device to create resources on
    /// \param layout The bind group layout defining the buffer binding
    /// \param viewport_width Width of the viewport in pixels
    /// \param viewport_height Height of the viewport in pixels
    Globals(
        wgpu::Device* device,
        wgpu::BindGroupLayout* layout,
        uint32_t viewport_width,
        uint32_t viewport_height
    );

    /// Destructor
    ~Globals();

    /// Get the bind group for use in render passes
    ///
    /// \return Pointer to the bind group containing the uniform buffer
    [[nodiscard]] wgpu::BindGroup* bind_group() const {
        return bind_group;
    }

    /// Get the underlying buffer
    ///
    /// \return Pointer to the uniform buffer
    [[nodiscard]] wgpu::Buffer* buffer() const {
        return buffer;
    }
};

} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_GLOBALS_H
