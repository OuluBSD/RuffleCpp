// C++ translation of core/src/avm1/globals/drop_shadow_filter.rs
// flash.filters.DropShadowFilter object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_DROP_SHADOW_FILTER_H
#define RUFFLE_CORE_AVM1_GLOBALS_DROP_SHADOW_FILTER_H

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
    struct DropShadowFilter;
    class Fixed8;
    class Fixed16;
    struct DropShadowFilterFlags;
}
namespace gc_arena {
    template<typename T> class Gc;
    template<typename T> class Mutation;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace drop_shadow_filter {

/// DropShadowFilter data structure
///
/// Holds the drop shadow filter properties (distance, angle, color, quality,
/// inner, knockout, blurX, blurY, strength, hideObject).
template<typename GC>
class DropShadowFilterData {
public:
    double distance;
    double angle;  // In radians
    swf::Color color;
    int32_t quality;
    bool inner;
    bool knockout;
    double blur_x;
    double blur_y;
    uint16_t strength;  // Stored as fixed-point (multiplied by 256)
    bool hide_object;

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
    DropShadowFilterData()
        : distance(4.0)
        , angle(0.785398163)  // ~45 degrees in radians
        , color{0, 0, 0, 255}  // Color::BLACK
        , quality(1)
        , inner(false)
        , knockout(false)
        , blur_x(4.0)
        , blur_y(4.0)
        , strength(1 << 8)
        , hide_object(false)
    {}
};

/// DropShadowFilter wrapper
///
/// AVM1 drop shadow filter object wrapper around DropShadowFilterData.
template<typename GC>
class DropShadowFilter {
public:
    /// Create a new DropShadowFilter
    ///
    /// @param activation The current activation context
    /// @param args Arguments containing the initial properties
    /// @return Result containing the new DropShadowFilter or an error
    static DropShadowFilter<GC> create(
        Activation<GC>* activation,
        const Value<GC>* args,
        size_t arg_count
    );

    /// Create from a SWF DropShadowFilter
    ///
    /// @param gc_context The GC mutation context
    /// @param filter The SWF filter to convert from
    static DropShadowFilter<GC> from_filter(
        gc_arena::Mutation<GC>* gc_context,
        const swf::DropShadowFilter& filter
    );

    /// Duplicate this filter
    ///
    /// @param gc_context The GC mutation context
    DropShadowFilter<GC> duplicate(gc_arena::Mutation<GC>* gc_context) const;

    /// Get the distance value
    double distance() const;

    /// Set the distance value
    ///
    /// @param activation The current activation context
    /// @param value The new distance value
    void set_distance(Activation<GC>* activation, const Value<GC>* value);

    /// Get the angle value in degrees
    double angle() const;

    /// Set the angle value
    ///
    /// @param activation The current activation context
    /// @param value The new angle value in degrees (will be converted to radians)
    void set_angle(Activation<GC>* activation, const Value<GC>* value);

    /// Get the color value (RGB only)
    int32_t color() const;

    /// Set the color value
    ///
    /// @param activation The current activation context
    /// @param value The new color value
    void set_color(Activation<GC>* activation, const Value<GC>* value);

    /// Get the alpha value (0.0-1.0)
    double alpha() const;

    /// Set the alpha value
    ///
    /// @param activation The current activation context
    /// @param value The new alpha value (0.0-1.0)
    void set_alpha(Activation<GC>* activation, const Value<GC>* value);

    /// Get the quality value
    int32_t quality() const;

    /// Set the quality value
    ///
    /// @param activation The current activation context
    /// @param value The new quality value (clamped to 0-15)
    void set_quality(Activation<GC>* activation, const Value<GC>* value);

    /// Get the inner flag
    bool inner() const;

    /// Set the inner flag
    ///
    /// @param activation The current activation context
    /// @param value The new inner value
    void set_inner(Activation<GC>* activation, const Value<GC>* value);

    /// Get the knockout flag
    bool knockout() const;

    /// Set the knockout flag
    ///
    /// @param activation The current activation context
    /// @param value The new knockout value
    void set_knockout(Activation<GC>* activation, const Value<GC>* value);

    /// Get the blurX value
    double blur_x() const;

    /// Set the blurX value
    ///
    /// @param activation The current activation context
    /// @param value The new blurX value (clamped to 0.0-255.0)
    void set_blur_x(Activation<GC>* activation, const Value<GC>* value);

    /// Get the blurY value
    double blur_y() const;

    /// Set the blurY value
    ///
    /// @param activation The current activation context
    /// @param value The new blurY value (clamped to 0.0-255.0)
    void set_blur_y(Activation<GC>* activation, const Value<GC>* value);

    /// Get the strength value
    double strength() const;

    /// Set the strength value
    ///
    /// @param activation The current activation context
    /// @param value The new strength value
    void set_strength(Activation<GC>* activation, const Value<GC>* value);

    /// Get the hideObject flag
    bool hide_object() const;

    /// Set the hideObject flag
    ///
    /// @param activation The current activation context
    /// @param value The new hideObject value
    void set_hide_object(Activation<GC>* activation, const Value<GC>* value);

    /// Convert to SWF DropShadowFilter
    swf::DropShadowFilter filter() const;

private:
    DropShadowFilter(gc_arena::Gc<DropShadowFilterData<GC>> data)
        : data_(data) {}

    gc_arena::Gc<DropShadowFilterData<GC>> data_;
};

/// Method indices for DropShadowFilter
namespace method {
    constexpr uint16_t CONSTRUCTOR = 0;
    constexpr uint16_t GET_DISTANCE = 1;
    constexpr uint16_t SET_DISTANCE = 2;
    constexpr uint16_t GET_ANGLE = 3;
    constexpr uint16_t SET_ANGLE = 4;
    constexpr uint16_t GET_COLOR = 5;
    constexpr uint16_t SET_COLOR = 6;
    constexpr uint16_t GET_ALPHA = 7;
    constexpr uint16_t SET_ALPHA = 8;
    constexpr uint16_t GET_QUALITY = 9;
    constexpr uint16_t SET_QUALITY = 10;
    constexpr uint16_t GET_INNER = 11;
    constexpr uint16_t SET_INNER = 12;
    constexpr uint16_t GET_KNOCKOUT = 13;
    constexpr uint16_t SET_KNOCKOUT = 14;
    constexpr uint16_t GET_BLUR_X = 15;
    constexpr uint16_t SET_BLUR_X = 16;
    constexpr uint16_t GET_BLUR_Y = 17;
    constexpr uint16_t SET_BLUR_Y = 18;
    constexpr uint16_t GET_STRENGTH = 19;
    constexpr uint16_t SET_STRENGTH = 20;
    constexpr uint16_t GET_HIDE_OBJECT = 21;
    constexpr uint16_t SET_HIDE_OBJECT = 22;
}

/// Property declarations for DropShadowFilter prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the DropShadowFilter class
///
/// @param context The declaration context for creating the class
/// @param super_proto The prototype object for the superclass
/// @return A SystemClass containing the created DropShadowFilter class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// DropShadowFilter method handler
///
/// Handles all DropShadowFilter methods based on the index.
///
/// @param activation The current activation context
/// @param this_obj The DropShadowFilter object
/// @param args Arguments for the method
/// @param arg_count Number of arguments
/// @param index The method index to call
/// @return The result of the method call
template<typename GC>
Value<GC> method(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count,
    uint16_t index
);

} // namespace drop_shadow_filter
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_DROP_SHADOW_FILTER_H
