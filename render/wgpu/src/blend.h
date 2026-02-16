// C++ translation of render/wgpu/src/blend.rs
// Blend mode handling for wgpu rendering

#ifndef RUFFLE_RENDER_WGPU_BLEND_H
#define RUFFLE_RENDER_WGPU_BLEND_H

#include <cstdint>

// Forward declarations
namespace wgpu {
    struct BlendState;
    struct Color;
}

namespace ruffle {
namespace render {
    struct RenderBlendMode;
}
}

namespace swf {
    enum class BlendMode : uint8_t;
}

namespace ruffle {
namespace pixel_bender {
    using PixelBenderShaderHandle = void*; // Placeholder for shader handle
}
}

namespace ruffle {
namespace render {
namespace wgpu {

/// Complex blend modes that require shader computation
///
/// These blend modes cannot be expressed with simple wgpu blend states
/// and require custom shader code to compute the blend result.
enum class ComplexBlend : uint8_t {
    Multiply,    // Can't be trivial, 0 alpha is special case
    Lighten,     // Might be trivial but can't reproduce the right colors
    Darken,      // Might be trivial but can't reproduce the right colors
    Difference,  // Can't be trivial, relies on abs operation
    Invert,      // May be trivial using a constant (hard with premultiplied alpha)
    Alpha,       // Can't be trivial, requires layer tracking
    Erase,       // Can't be trivial, requires layer tracking
    Overlay,     // Can't be trivial, big math expression
    HardLight,   // Can't be trivial, big math expression
};

/// Trivial blend modes that can use wgpu blend states
///
/// These blend modes can be expressed with simple wgpu blend state
/// configuration without requiring custom shader code.
enum class TrivialBlend : uint8_t {
    Normal,
    Add,
    Subtract,
    Screen,
};

/// Blend type classification
///
/// Classifies blend modes into trivial (can use blend states),
/// complex (require shaders), or custom shader blends.
enum class BlendType {
    /// Trivial blends can be expressed with just a "draw bitmap" with blend states
    Trivial,

    /// Complex blends require a shader to express
    Complex,

    /// Invoke a custom PixelBender shader
    Shader,
};

/// Helper functions for TrivialBlend
namespace trivial_blend {
    /// Get the wgpu blend state for a trivial blend mode
    ///
    /// out = <src_factor> * src <operation> <dst_factor> * dst
    ///
    /// \param blend The trivial blend mode
    /// \return The corresponding wgpu BlendState
    wgpu::BlendState blend_state(TrivialBlend blend);
}

/// Helper functions for BlendType
namespace blend_type {
    /// Convert a RenderBlendMode to a BlendType
    ///
    /// Classifies the blend mode as trivial, complex, or shader-based.
    ///
    /// \param mode The render blend mode to classify
    /// \return The corresponding BlendType
    BlendType from(const ruffle::render::RenderBlendMode& mode);

    /// Get the default clear color for blend operations
    ///
    /// \return Transparent color (RGBA: 0, 0, 0, 0)
    wgpu::Color default_color();
}

} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_BLEND_H
