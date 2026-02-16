// C++ translation of core/src/avm1/globals/color_matrix_filter.rs
// flash.filters.ColorMatrixFilter object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_COLOR_MATRIX_FILTER_H
#define RUFFLE_CORE_AVM1_GLOBALS_COLOR_MATRIX_FILTER_H

#include <cstdint>
#include <array>

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
    struct ColorMatrixFilter;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace color_matrix_filter {

/// ColorMatrixFilter data structure
///
/// Holds the 4x5 color transformation matrix.
template<typename GC>
class ColorMatrixFilter {
public:
    /// Create a new ColorMatrixFilter
    ///
    /// \param activation The current activation context
    /// \param args Arguments containing the initial matrix
    static ColorMatrixFilter<GC> create(
        Activation<GC>* activation,
        const Value<GC>* args,
        size_t arg_count
    );

    /// Create from a SWF ColorMatrixFilter
    ///
    /// \param filter The SWF filter to convert from
    static ColorMatrixFilter<GC> from_filter(const swf::ColorMatrixFilter& filter);

    /// Duplicate this filter
    ColorMatrixFilter<GC> duplicate() const;

    /// Get the matrix as an array value
    ///
    /// \param activation The current activation context
    /// \return Array containing the 20 matrix values
    Value<GC> matrix(Activation<GC>* activation) const;

    /// Set the matrix from an array value
    ///
    /// \param activation The current activation context
    /// \param value The array value containing matrix values
    void set_matrix(Activation<GC>* activation, const Value<GC>* value);

    /// Convert to SWF ColorMatrixFilter
    swf::ColorMatrixFilter filter() const;
};

/// Method indices for ColorMatrixFilter
namespace method {
    constexpr uint16_t CONSTRUCTOR = 0;
    constexpr uint16_t GET_MATRIX = 1;
    constexpr uint16_t SET_MATRIX = 2;
}

/// Property declarations for ColorMatrixFilter prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the ColorMatrixFilter class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created ColorMatrixFilter class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// ColorMatrixFilter method handler
///
/// Handles all ColorMatrixFilter methods based on the index.
///
/// \param activation The current activation context
/// \param this_obj The ColorMatrixFilter object
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

} // namespace color_matrix_filter
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_COLOR_MATRIX_FILTER_H
