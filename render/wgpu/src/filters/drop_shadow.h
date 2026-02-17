// C++ translation of render/wgpu/src/filters/drop_shadow.rs
// Drop shadow filter implementation

#ifndef RUFFLE_RENDER_WGPU_FILTERS_DROP_SHADOW_H
#define RUFFLE_RENDER_WGPU_FILTERS_DROP_SHADOW_H

// Forward declarations
namespace wgpu {
    class CommandEncoder;
}

namespace wgpu {
namespace util {
    class StagingBelt;
}
}

namespace swf {
    struct DropShadowFilter;
}

namespace ruffle {
namespace render {
namespace wgpu {

class Descriptors;
class TexturePool;
class FilterSource;
class BlurFilter;
class GlowFilter;

namespace surface {
namespace target {
    class CommandTarget;
}
}

namespace filters {

/// Drop shadow filter
/// Drop shadow is just Glow with an offset.
/// None of this strictly needs to be a struct,
/// but it helps for code organisation + if we want to specialise the implementation in the future
class DropShadowFilter {
public:
    /// Apply the drop shadow filter
    /// 
    /// @param descriptors WebGPU descriptors
    /// @param texture_pool Texture pool for temporary textures
    /// @param draw_encoder Command encoder for recording draw commands
    /// @param staging_belt Staging belt for buffer uploads
    /// @param source Source filter data
    /// @param filter SWF drop shadow filter parameters
    /// @param blur_filter Blur filter instance
    /// @param glow_filter Glow filter instance
    /// @return Command target for the filtered result
    static surface::target::CommandTarget apply(
        const Descriptors* descriptors,
        TexturePool* texture_pool,
        wgpu::CommandEncoder* draw_encoder,
        wgpu::util::StagingBelt* staging_belt,
        const FilterSource* source,
        const swf::DropShadowFilter* filter,
        const BlurFilter* blur_filter,
        const GlowFilter* glow_filter
    );
};

} // namespace filters
} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_FILTERS_DROP_SHADOW_H
