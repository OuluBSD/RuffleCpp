// C++ translation of core/src/avm1/globals/glow_filter.rs
// flash.filters.GlowFilter object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_GLOW_FILTER_H
#define RUFFLE_CORE_AVM1_GLOBALS_GLOW_FILTER_H

#include <cstdint>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class NativeObject;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
}
}
namespace swf {
    struct Color;
    struct GlowFilter;
    class Fixed8;
    class Fixed16;
    struct GlowFilterFlags;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace glow_filter {

/// GlowFilter data structure
///
/// Holds the glow filter properties (color, quality, inner, knockout, blurX, blurY, strength).
template<typename GC>
class GlowFilterData {
public:
    swf::Color color;
    int32_t quality;
    bool inner;
    bool knockout;
    double blur_x;
    double blur_y;
    uint16_t strength;  // Stored as fixed-point (multiplied by 256)

    /// Get strength as a double (divided by 256)
    double get_strength() const {
        return static_cast<double>(strength) / 256.0;
    }

    /// Set strength from a double value
    void set_strength(double strength_val) {
        auto strength_int = static_cast<uint16_t>(strength_val * 256.0);
        strength = std::clamp(strength_int, static_cast<uint16_t>(0), static_cast<uint16_t>(0xFF00));
    }

    /// Default constructor
    GlowFilterData()
        : color{255, 0, 0, 255}  // Color::RED
        , quality(1)
        , inner(false)
        , knockout(false)
        , blur_x(6.0)
        , blur_y(6.0)
        , strength(2 << 8)
    {}
};

/// GlowFilter wrapper
///
/// AVM1 glow filter object wrapper around GlowFilterData.
template<typename GC>
class GlowFilter {
public:
    /// Create a new GlowFilter
    ///
    /// \param activation The current activation context
    /// \param args Arguments containing the initial properties
    /// \return Result containing the new GlowFilter or an error
    static GlowFilter<GC> create(
        Activation<GC>* activation,
        const Value<GC>* args,
        size_t arg_count
    );

    /// Create from a SWF GlowFilter
    ///
    /// \param filter The SWF filter to convert from
    static GlowFilter<GC> from_filter(const swf::GlowFilter& filter);

    /// Duplicate this filter
    GlowFilter<GC> duplicate() const;

    /// Get the color value (RGB only)
    int32_t color() const;

    /// Set the color value
    ///
    /// \param activation The current activation context
    /// \param value The new color value
    void set_color(Activation<GC>* activation, const Value<GC>* value);

    /// Get the alpha value (0.0-1.0)
    double alpha() const;

    /// Set the alpha value
    ///
    /// \param activation The current activation context
    /// \param value The new alpha value (0.0-1.0)
    void set_alpha(Activation<GC>* activation, const Value<GC>* value);

    /// Get the quality value
    int32_t quality() const;

    /// Set the quality value
    ///
    /// \param activation The current activation context
    /// \param value The new quality value (clamped to 0-15)
    void set_quality(Activation<GC>* activation, const Value<GC>* value);

    /// Get the inner flag
    bool inner() const;

    /// Set the inner flag
    ///
    /// \param activation The current activation context
    /// \param value The new inner value
    void set_inner(Activation<GC>* activation, const Value<GC>* value);

    /// Get the knockout flag
    bool knockout() const;

    /// Set the knockout flag
    ///
    /// \param activation The current activation context
    /// \param value The new knockout value
    void set_knockout(Activation<GC>* activation, const Value<GC>* value);

    /// Get the blurX value
    double blur_x() const;

    /// Set the blurX value
    ///
    /// \param activation The current activation context
    /// \param value The new blurX value (clamped to 0.0-255.0)
    void set_blur_x(Activation<GC>* activation, const Value<GC>* value);

    /// Get the blurY value
    double blur_y() const;

    /// Set the blurY value
    ///
    /// \param activation The current activation context
    /// \param value The new blurY value (clamped to 0.0-255.0)
    void set_blur_y(Activation<GC>* activation, const Value<GC>* value);

    /// Get the strength value
    double strength() const;

    /// Set the strength value
    ///
    /// \param activation The current activation context
    /// \param value The new strength value
    void set_strength(Activation<GC>* activation, const Value<GC>* value);

    /// Convert to SWF GlowFilter
    swf::GlowFilter filter() const;

private:
    std::shared_ptr<GlowFilterData<GC>> data_;
};

/// Method indices for GlowFilter
namespace method {
    constexpr uint16_t CONSTRUCTOR = 0;
    constexpr uint16_t GET_COLOR = 1;
    constexpr uint16_t SET_COLOR = 2;
    constexpr uint16_t GET_ALPHA = 3;
    constexpr uint16_t SET_ALPHA = 4;
    constexpr uint16_t GET_QUALITY = 5;
    constexpr uint16_t SET_QUALITY = 6;
    constexpr uint16_t GET_INNER = 7;
    constexpr uint16_t SET_INNER = 8;
    constexpr uint16_t GET_KNOCKOUT = 9;
    constexpr uint16_t SET_KNOCKOUT = 10;
    constexpr uint16_t GET_BLUR_X = 11;
    constexpr uint16_t SET_BLUR_X = 12;
    constexpr uint16_t GET_BLUR_Y = 13;
    constexpr uint16_t SET_BLUR_Y = 14;
    constexpr uint16_t GET_STRENGTH = 15;
    constexpr uint16_t SET_STRENGTH = 16;
}

/// Property declarations for GlowFilter prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the GlowFilter class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created GlowFilter class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// GlowFilter method handler
///
/// Handles all GlowFilter methods based on the index.
///
/// \param activation The current activation context
/// \param this_obj The GlowFilter object
/// \param args Arguments for the method
/// \param arg_count Number of arguments
/// \param index The method index to call
/// \return The result of the method call
template<typename GC>
Value<GC> method(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count,
    uint16_t index
);

} // namespace glow_filter
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_GLOW_FILTER_H
