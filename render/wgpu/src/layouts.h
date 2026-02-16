// C++ translation of render/wgpu/src/layouts.rs
// Bind group layouts for wgpu rendering

#ifndef RUFFLE_RENDER_WGPU_LAYOUTS_H
#define RUFFLE_RENDER_WGPU_LAYOUTS_H

// Forward declarations
namespace wgpu {
    class Device;
    class BindGroupLayout;
}

namespace ruffle {
namespace render {
namespace wgpu {

// Forward declarations
struct GlobalsUniform;
struct Transforms;
struct TextureTransforms;
struct GradientUniforms;

/// Bind group layouts for wgpu rendering
///
/// Contains all the bind group layouts needed for the rendering pipeline.
/// Each layout defines the structure of a bind group used in shader stages.
class BindLayouts {
private:
    /// Globals bind group layout
    /// Contains the global view matrix uniform buffer
    wgpu::BindGroupLayout* globals;

    /// Transforms bind group layout
    /// Contains per-draw transform uniform buffers with dynamic offsets
    wgpu::BindGroupLayout* transforms;

    /// Bitmap bind group layout
    /// Contains texture transform buffer, texture, and sampler
    wgpu::BindGroupLayout* bitmap;

    /// Gradient bind group layout
    /// Contains gradient uniforms, texture, and sampler
    wgpu::BindGroupLayout* gradient;

    /// Blend bind group layout
    /// Contains blend textures and sampler for complex blend modes
    wgpu::BindGroupLayout* blend;

    /// Alpha mask bind group layout
    /// Contains alpha mask textures and sampler
    wgpu::BindGroupLayout* alpha_mask;

public:
    /// Default constructor
    BindLayouts();

    /// Destructor
    ~BindLayouts();

    /// Create all bind group layouts
    ///
    /// Creates all the bind group layouts needed for the rendering pipeline.
    /// Each layout corresponds to a specific type of resource binding.
    ///
    /// \param device The wgpu device to create layouts on
    void create(wgpu::Device* device);

    /// Get the globals bind group layout
    ///
    /// \return Pointer to the globals bind group layout
    [[nodiscard]] wgpu::BindGroupLayout* get_globals() const {
        return globals;
    }

    /// Get the transforms bind group layout
    ///
    /// \return Pointer to the transforms bind group layout
    [[nodiscard]] wgpu::BindGroupLayout* get_transforms() const {
        return transforms;
    }

    /// Get the bitmap bind group layout
    ///
    /// \return Pointer to the bitmap bind group layout
    [[nodiscard]] wgpu::BindGroupLayout* get_bitmap() const {
        return bitmap;
    }

    /// Get the gradient bind group layout
    ///
    /// \return Pointer to the gradient bind group layout
    [[nodiscard]] wgpu::BindGroupLayout* get_gradient() const {
        return gradient;
    }

    /// Get the blend bind group layout
    ///
    /// \return Pointer to the blend bind group layout
    [[nodiscard]] wgpu::BindGroupLayout* get_blend() const {
        return blend;
    }

    /// Get the alpha mask bind group layout
    ///
    /// \return Pointer to the alpha mask bind group layout
    [[nodiscard]] wgpu::BindGroupLayout* get_alpha_mask() const {
        return alpha_mask;
    }
};

} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_LAYOUTS_H
