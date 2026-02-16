// C++ translation of render/src/filters.rs
// Filter types for rendering effects

#ifndef RUFFLE_RENDER_FILTERS_H
#define RUFFLE_RENDER_FILTERS_H

#include <memory>
#include <vector>
#include <optional>
#include <cstdint>
#include <any>

// Forward declarations
namespace ruffle {
namespace render {
    class BitmapHandle;
    class PixelBenderShaderHandle;
    struct PixelBenderShaderArgument;
}
namespace swf {
    struct BevelFilter;
    struct BlurFilter;
    struct ColorMatrixFilter;
    struct ConvolutionFilter;
    struct DropShadowFilter;
    struct GlowFilter;
    struct GradientFilter;
    template<typename T>
    struct Rectangle;
    struct Twips;
    struct Color;
}
}

namespace ruffle {
namespace render {

/// Displacement map filter component selection
enum class DisplacementMapFilterComponent {
    Alpha,
    Blue,
    Green,
    Red,
};

/// Displacement map filter mode
enum class DisplacementMapFilterMode {
    Clamp,
    Color,
    Ignore,
    Wrap,  // Default
};

/// Displacement map filter for pixel displacement effects
struct DisplacementMapFilter {
    swf::Color color;
    uint8_t component_x;
    uint8_t component_y;
    std::optional<BitmapHandle> map_bitmap;
    std::pair<int32_t, int32_t> map_point;
    DisplacementMapFilterMode mode;
    float scale_x;
    float scale_y;
    float viewscale_x;
    float viewscale_y;

    /// Default constructor
    DisplacementMapFilter()
        : component_x(0)
        , component_y(0)
        , map_point{0, 0}
        , mode(DisplacementMapFilterMode::Wrap)
        , scale_x(0.0f)
        , scale_y(0.0f)
        , viewscale_x(0.0f)
        , viewscale_y(0.0f)
    {}

    /// Scale the filter by the given factors
    void scale(float x, float y) {
        viewscale_x *= x;
        viewscale_y *= y;
    }

    /// Calculate the destination rectangle for this filter
    /// @param source_rect The source rectangle to transform
    /// @return The destination rectangle
    swf::Rectangle<swf::Twips> calculate_dest_rect(const swf::Rectangle<swf::Twips>& source_rect) const {
        // [NA] TODO: This *appears* to be correct, but I'm not entirely sure why Flash does this.
        // This is commented out for now because Flash actually might need us to resize the texture *after* we make it,
        // which is unsupported in our current architecture as of time of writing.
        return source_rect;
    }

    /// Equality comparison
    bool operator==(const DisplacementMapFilter& other) const = default;
};

/// Abstract base class for shader objects
/// This is the C++ equivalent of the Rust ShaderObject trait
class ShaderObject {
public:
    virtual ~ShaderObject() = default;

    /// Clone the shader object
    virtual std::unique_ptr<ShaderObject> clone_box() const = 0;

    /// Check equality with another shader object
    virtual bool equals(const ShaderObject& other) const = 0;
};

/// Shader filter for Pixel Bender shader effects
struct ShaderFilter {
    int32_t bottom_extension;
    int32_t left_extension;
    int32_t right_extension;
    int32_t top_extension;

    /// The AVM2 `flash.display.Shader` object that we extracted
    /// the `shader` and `shader_args` fields from. This is used when
    /// we reconstruct a `ShaderFilter` object in the AVM2 `DisplayObject.filters`
    /// (Flash re-uses the same object)
    std::unique_ptr<ShaderObject> shader_object;

    PixelBenderShaderHandle shader;
    std::vector<PixelBenderShaderArgument> shader_args;

    /// Default constructor
    ShaderFilter()
        : bottom_extension(0)
        , left_extension(0)
        , right_extension(0)
        , top_extension(0)
    {}

    /// Equality comparison
    bool operator==(const ShaderFilter& other) const {
        return bottom_extension == other.bottom_extension
            && left_extension == other.left_extension
            && right_extension == other.right_extension
            && top_extension == other.top_extension
            && shader_object && other.shader_object && shader_object->equals(*other.shader_object)
            && shader == other.shader
            && shader_args == other.shader_args;
    }
};

/// Filter enumeration for rendering effects
/// This is the main filter type that can hold any kind of filter
class Filter {
public:
    /// Filter variant types
    enum class Type {
        Bevel,
        Blur,
        ColorMatrix,
        Convolution,
        DisplacementMap,
        DropShadow,
        Glow,
        GradientBevel,
        GradientGlow,
        Shader,
    };

private:
    Type type_;

    // Storage for each filter type
    std::optional<swf::BevelFilter> bevel_filter_;
    std::optional<swf::BlurFilter> blur_filter_;
    std::optional<swf::ColorMatrixFilter> color_matrix_filter_;
    std::optional<swf::ConvolutionFilter> convolution_filter_;
    std::optional<DisplacementMapFilter> displacement_map_filter_;
    std::optional<swf::DropShadowFilter> drop_shadow_filter_;
    std::optional<swf::GlowFilter> glow_filter_;
    std::optional<swf::GradientFilter> gradient_bevel_filter_;
    std::optional<swf::GradientFilter> gradient_glow_filter_;
    std::optional<ShaderFilter> shader_filter_;

public:
    /// Create a BevelFilter variant
    static Filter bevel(const swf::BevelFilter& filter) {
        Filter f;
        f.type_ = Type::Bevel;
        f.bevel_filter_ = filter;
        return f;
    }

    /// Create a BlurFilter variant
    static Filter blur(const swf::BlurFilter& filter) {
        Filter f;
        f.type_ = Type::Blur;
        f.blur_filter_ = filter;
        return f;
    }

    /// Create a ColorMatrixFilter variant
    static Filter color_matrix(const swf::ColorMatrixFilter& filter) {
        Filter f;
        f.type_ = Type::ColorMatrix;
        f.color_matrix_filter_ = filter;
        return f;
    }

    /// Create a ConvolutionFilter variant
    static Filter convolution(const swf::ConvolutionFilter& filter) {
        Filter f;
        f.type_ = Type::Convolution;
        f.convolution_filter_ = filter;
        return f;
    }

    /// Create a DisplacementMapFilter variant
    static Filter displacement_map(const DisplacementMapFilter& filter) {
        Filter f;
        f.type_ = Type::DisplacementMap;
        f.displacement_map_filter_ = filter;
        return f;
    }

    /// Create a DropShadowFilter variant
    static Filter drop_shadow(const swf::DropShadowFilter& filter) {
        Filter f;
        f.type_ = Type::DropShadow;
        f.drop_shadow_filter_ = filter;
        return f;
    }

    /// Create a GlowFilter variant
    static Filter glow(const swf::GlowFilter& filter) {
        Filter f;
        f.type_ = Type::Glow;
        f.glow_filter_ = filter;
        return f;
    }

    /// Create a GradientBevelFilter variant
    static Filter gradient_bevel(const swf::GradientFilter& filter) {
        Filter f;
        f.type_ = Type::GradientBevel;
        f.gradient_bevel_filter_ = filter;
        return f;
    }

    /// Create a GradientGlowFilter variant
    static Filter gradient_glow(const swf::GradientFilter& filter) {
        Filter f;
        f.type_ = Type::GradientGlow;
        f.gradient_glow_filter_ = filter;
        return f;
    }

    /// Create a ShaderFilter variant
    static Filter shader(const ShaderFilter& filter) {
        Filter f;
        f.type_ = Type::Shader;
        f.shader_filter_ = filter;
        return f;
    }

    /// Get the filter type
    Type type() const { return type_; }

    /// Accessors for each filter type
    const swf::BevelFilter* as_bevel() const { return bevel_filter_ ? &*bevel_filter_ : nullptr; }
    const swf::BlurFilter* as_blur() const { return blur_filter_ ? &*blur_filter_ : nullptr; }
    const swf::ColorMatrixFilter* as_color_matrix() const { return color_matrix_filter_ ? &*color_matrix_filter_ : nullptr; }
    const swf::ConvolutionFilter* as_convolution() const { return convolution_filter_ ? &*convolution_filter_ : nullptr; }
    const DisplacementMapFilter* as_displacement_map() const { return displacement_map_filter_ ? &*displacement_map_filter_ : nullptr; }
    const swf::DropShadowFilter* as_drop_shadow() const { return drop_shadow_filter_ ? &*drop_shadow_filter_ : nullptr; }
    const swf::GlowFilter* as_glow() const { return glow_filter_ ? &*glow_filter_ : nullptr; }
    const swf::GradientFilter* as_gradient_bevel() const { return gradient_bevel_filter_ ? &*gradient_bevel_filter_ : nullptr; }
    const swf::GradientFilter* as_gradient_glow() const { return gradient_glow_filter_ ? &*gradient_glow_filter_ : nullptr; }
    const ShaderFilter* as_shader() const { return shader_filter_ ? &*shader_filter_ : nullptr; }

    /// Scale the filter by the given factors
    void scale(float x, float y) {
        switch (type_) {
            case Type::Bevel:
                if (bevel_filter_) { /* bevel_filter_->scale(x, y); */ }
                break;
            case Type::Blur:
                if (blur_filter_) { /* blur_filter_->scale(x, y); */ }
                break;
            case Type::DropShadow:
                if (drop_shadow_filter_) { /* drop_shadow_filter_->scale(x, y); */ }
                break;
            case Type::Glow:
                if (glow_filter_) { /* glow_filter_->scale(x, y); */ }
                break;
            case Type::GradientBevel:
                if (gradient_bevel_filter_) { /* gradient_bevel_filter_->scale(x, y); */ }
                break;
            case Type::GradientGlow:
                if (gradient_glow_filter_) { /* gradient_glow_filter_->scale(x, y); */ }
                break;
            case Type::DisplacementMap:
                if (displacement_map_filter_) { displacement_map_filter_->scale(x, y); }
                break;
            default:
                break;
        }
    }

    /// Calculate the destination rectangle for this filter
    /// @param source_rect The source rectangle to transform
    /// @return The destination rectangle
    swf::Rectangle<swf::Twips> calculate_dest_rect(const swf::Rectangle<swf::Twips>& source_rect) const {
        switch (type_) {
            case Type::Blur:
                if (blur_filter_) { /* return blur_filter_->calculate_dest_rect(source_rect); */ }
                break;
            case Type::Glow:
                if (glow_filter_) { /* return glow_filter_->calculate_dest_rect(source_rect); */ }
                break;
            case Type::DropShadow:
                if (drop_shadow_filter_) { /* return drop_shadow_filter_->calculate_dest_rect(source_rect); */ }
                break;
            case Type::Bevel:
                if (bevel_filter_) { /* return bevel_filter_->calculate_dest_rect(source_rect); */ }
                break;
            case Type::DisplacementMap:
                if (displacement_map_filter_) { return displacement_map_filter_->calculate_dest_rect(source_rect); }
                break;
            default:
                break;
        }
        return source_rect;
    }

    /// Check if this filter is impotent (has no effect)
    /// @return true if the filter has no effect
    bool impotent() const {
        // TODO: There's more cases here, find them!
        switch (type_) {
            case Type::Blur:
                if (blur_filter_) { /* return blur_filter_->impotent(); */ }
                break;
            case Type::ColorMatrix:
                if (color_matrix_filter_) { /* return color_matrix_filter_->impotent(); */ }
                break;
            default:
                break;
        }
        return false;
    }

    /// Equality comparison
    bool operator==(const Filter& other) const {
        if (type_ != other.type_) return false;
        switch (type_) {
            case Type::Bevel: return bevel_filter_ == other.bevel_filter_;
            case Type::Blur: return blur_filter_ == other.blur_filter_;
            case Type::ColorMatrix: return color_matrix_filter_ == other.color_matrix_filter_;
            case Type::Convolution: return convolution_filter_ == other.convolution_filter_;
            case Type::DisplacementMap: return displacement_map_filter_ == other.displacement_map_filter_;
            case Type::DropShadow: return drop_shadow_filter_ == other.drop_shadow_filter_;
            case Type::Glow: return glow_filter_ == other.glow_filter_;
            case Type::GradientBevel: return gradient_bevel_filter_ == other.gradient_bevel_filter_;
            case Type::GradientGlow: return gradient_glow_filter_ == other.gradient_glow_filter_;
            case Type::Shader: return shader_filter_ == other.shader_filter_;
        }
        return false;
    }
};

/// Default filter - a color matrix filter that does nothing
inline Filter default_filter() {
    // A default colormatrix is a filter that essentially does nothing,
    // making it a useful default in situations that we need a dummy filter
    return Filter::color_matrix(swf::ColorMatrixFilter{});
}

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_FILTERS_H
