// C++ translation of core/src/avm2/filters.rs
//! AVM2 Filter conversion between SWF and AVM2 objects

#ifndef RUFFLE_CORE_AVM2_FILTERS_H
#define RUFFLE_CORE_AVM2_FILTERS_H

#include <vector>
#include <memory>
#include <any>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class TObject;
    template<typename T> class ArrayObject;
    template<typename T> class ClassObject;
    
    namespace globals {
        namespace slots {
            // Filter slot constants would be defined here
            namespace flash_filters_bevel_filter {
                constexpr int ANGLE = 0;
                constexpr int BLUR_X = 1;
                constexpr int BLUR_Y = 2;
                constexpr int DISTANCE = 3;
                constexpr int HIGHLIGHT_ALPHA = 4;
                constexpr int HIGHLIGHT_COLOR = 5;
                constexpr int KNOCKOUT = 6;
                constexpr int QUALITY = 7;
                constexpr int SHADOW_ALPHA = 8;
                constexpr int SHADOW_COLOR = 9;
                constexpr int STRENGTH = 10;
                constexpr int TYPE = 11;
            }
            namespace flash_filters_blur_filter {
                constexpr int BLUR_X = 0;
                constexpr int BLUR_Y = 1;
                constexpr int QUALITY = 2;
            }
            namespace flash_filters_color_matrix_filter {
                constexpr int _MATRIX = 0;
            }
            namespace flash_filters_convolution_filter {
                constexpr int MATRIX = 0;
                constexpr int ALPHA = 1;
                constexpr int BIAS = 2;
                constexpr int CLAMP = 3;
                constexpr int COLOR = 4;
                constexpr int DIVISOR = 5;
                constexpr int MATRIX_X = 6;
                constexpr int MATRIX_Y = 7;
                constexpr int PRESERVE_ALPHA = 8;
            }
            namespace flash_filters_displacement_map_filter {
                constexpr int ALPHA = 0;
                constexpr int COLOR = 1;
                constexpr int COMPONENT_X = 2;
                constexpr int COMPONENT_Y = 3;
                constexpr int MAP_POINT = 4;
                constexpr int MODE = 5;
                constexpr int SCALE_X = 6;
                constexpr int SCALE_Y = 7;
                constexpr int MAP_BITMAP = 8;
            }
            namespace flash_filters_drop_shadow_filter {
                constexpr int ALPHA = 0;
                constexpr int ANGLE = 1;
                constexpr int BLUR_X = 2;
                constexpr int BLUR_Y = 3;
                constexpr int COLOR = 4;
                constexpr int DISTANCE = 5;
                constexpr int HIDE_OBJECT = 6;
                constexpr int INNER = 7;
                constexpr int KNOCKOUT = 8;
                constexpr int QUALITY = 9;
                constexpr int STRENGTH = 10;
            }
            namespace flash_filters_glow_filter {
                constexpr int ALPHA = 0;
                constexpr int BLUR_X = 1;
                constexpr int BLUR_Y = 2;
                constexpr int COLOR = 3;
                constexpr int INNER = 4;
                constexpr int KNOCKOUT = 5;
                constexpr int QUALITY = 6;
                constexpr int STRENGTH = 7;
            }
            namespace flash_geom_point {
                constexpr int X = 0;
                constexpr int Y = 1;
            }
        }
    }
    
    namespace object {
        template<typename T> class TextureObject;
    }
}
namespace string {
    class WStr;
}
}
namespace render {
namespace filters {
    enum class DisplacementMapFilterMode;
    class Filter;
    class DisplacementMapFilter;
    class ShaderFilter;
    
    // ShaderObject trait equivalent
    class ShaderObject {
    public:
        virtual ~ShaderObject() = default;
        virtual std::unique_ptr<ShaderObject> clone_box() const = 0;
        virtual bool equals(const ShaderObject* other) const = 0;
    };
}
}
}
namespace swf {
    struct Color;
    struct BevelFilter;
    struct BlurFilter;
    struct ColorMatrixFilter;
    struct ConvolutionFilter;
    struct DropShadowFilter;
    struct GlowFilter;
    struct GradientFilter;
    class Fixed8;
    class Fixed16;
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace filters {

/// Wrapper for AVM2 Object to be used as ShaderObject
class ObjectWrapper : public render::filters::ShaderObject {
public:
    ObjectWrapper(Object<>* obj);
    
    std::unique_ptr<render::filters::ShaderObject> clone_box() const override;
    bool equals(const render::filters::ShaderObject* other) const override;
    
private:
    Object<* root_;
};

/// Extension trait for Filter conversion
class FilterAvm2Ext {
public:
    /// Convert an AVM2 object to a Filter
    static render::filters::Filter from_avm2_object(
        Activation<>* activation,
        Object<>* object
    );
    
    /// Convert a Filter to an AVM2 object
    static Value<> as_avm2_object(
        Activation<>* activation,
        const render::filters::Filter& filter
    );
};

/// Convert AVM2 object to BevelFilter
render::filters::Filter avm2_to_bevel_filter(
    Activation<>* activation,
    Object<>* object
);

/// Convert BevelFilter to AVM2 object
Value<> bevel_filter_to_avm2(
    Activation<>* activation,
    const swf::BevelFilter& filter
);

/// Convert AVM2 object to BlurFilter
render::filters::Filter avm2_to_blur_filter(
    Activation<>* activation,
    Object<>* object
);

/// Convert BlurFilter to AVM2 object
Value<> blur_filter_to_avm2(
    Activation<>* activation,
    const swf::BlurFilter& filter
);

/// Convert AVM2 object to ColorMatrixFilter
render::filters::Filter avm2_to_color_matrix_filter(
    Activation<>* activation,
    Object<>* object
);

/// Convert ColorMatrixFilter to AVM2 object
Value<> color_matrix_filter_to_avm2(
    Activation<>* activation,
    const swf::ColorMatrixFilter& filter
);

/// Convert AVM2 object to ConvolutionFilter
render::filters::Filter avm2_to_convolution_filter(
    Activation<>* activation,
    Object<>* object
);

/// Convert ConvolutionFilter to AVM2 object
Value<> convolution_filter_to_avm2(
    Activation<>* activation,
    const swf::ConvolutionFilter& filter
);

/// Convert AVM2 object to DisplacementMapFilter
render::filters::Filter avm2_to_displacement_map_filter(
    Activation<>* activation,
    Object<>* object
);

/// Convert DisplacementMapFilter to AVM2 object
Value<> displacement_map_filter_to_avm2(
    Activation<>* activation,
    const render::filters::DisplacementMapFilter& filter
);

/// Convert AVM2 object to DropShadowFilter
render::filters::Filter avm2_to_drop_shadow_filter(
    Activation<>* activation,
    Object<>* object
);

/// Convert DropShadowFilter to AVM2 object
Value<> drop_shadow_filter_to_avm2(
    Activation<>* activation,
    const swf::DropShadowFilter& filter
);

/// Convert AVM2 object to GlowFilter
render::filters::Filter avm2_to_glow_filter(
    Activation<>* activation,
    Object<>* object
);

/// Convert GlowFilter to AVM2 object
Value<> glow_filter_to_avm2(
    Activation<>* activation,
    const swf::GlowFilter& filter
);

/// Convert AVM2 object to GradientFilter (Bevel or Glow)
render::filters::GradientFilter avm2_to_gradient_filter(
    Activation<>* activation,
    Object<>* object
);

/// Convert GradientFilter to AVM2 object
Value<> gradient_filter_to_avm2(
    Activation<>* activation,
    const swf::GradientFilter& filter,
    ClassObject<>* filter_class
);

/// Convert AVM2 object to ShaderFilter
render::filters::ShaderFilter avm2_to_shader_filter(
    Activation<>* activation,
    Object<>* object
);

/// Convert ShaderFilter to AVM2 object
Value<> shader_filter_to_avm2(
    Activation<>* activation,
    const render::filters::ShaderFilter& filter
);

} // namespace filters
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_FILTERS_H
