// C++ translation of core/src/avm1/globals/blur_filter.rs
// flash.filters.BlurFilter object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_BLUR_FILTER_H
#define RUFFLE_CORE_AVM1_GLOBALS_BLUR_FILTER_H

#include <cstdint>

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
    struct BlurFilter;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace blur_filter {

/// BlurFilter data structure
///
/// Holds the blur filter properties (blurX, blurY, quality).
template<typename GC>
class BlurFilter {
public:
    /// Create a new BlurFilter
    ///
    /// \param activation The current activation context
    /// \param args Arguments containing the initial properties
    static BlurFilter<GC> create(
        Activation<GC>* activation,
        const Value<GC>* args,
        size_t arg_count
    );

    /// Create from a SWF BlurFilter
    ///
    /// \param filter The SWF filter to convert from
    static BlurFilter<GC> from_filter(const swf::BlurFilter& filter);

    /// Duplicate this filter
    BlurFilter<GC> duplicate() const;

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

    /// Get the quality value
    int32_t quality() const;

    /// Set the quality value
    ///
    /// \param activation The current activation context
    /// \param value The new quality value (clamped to 0-15)
    void set_quality(Activation<GC>* activation, const Value<GC>* value);

    /// Convert to SWF BlurFilter
    swf::BlurFilter filter() const;
};

/// Method indices for BlurFilter
namespace method {
    constexpr uint16_t CONSTRUCTOR = 0;
    constexpr uint16_t GET_BLUR_X = 1;
    constexpr uint16_t SET_BLUR_X = 2;
    constexpr uint16_t GET_BLUR_Y = 3;
    constexpr uint16_t SET_BLUR_Y = 4;
    constexpr uint16_t GET_QUALITY = 5;
    constexpr uint16_t SET_QUALITY = 6;
}

/// Property declarations for BlurFilter prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the BlurFilter class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created BlurFilter class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// BlurFilter method handler
///
/// Handles all BlurFilter methods based on the index.
///
/// \param activation The current activation context
/// \param this_obj The BlurFilter object
/// \param args Arguments for the method
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

} // namespace blur_filter
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_BLUR_FILTER_H
