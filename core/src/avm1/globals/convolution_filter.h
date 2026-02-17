// C++ translation of core/src/avm1/globals/convolution_filter.rs
// flash.filters.ConvolutionFilter object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_CONVOLUTION_FILTER_H
#define RUFFLE_CORE_AVM1_GLOBALS_CONVOLUTION_FILTER_H

#include <cstdint>
#include <memory>
#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class ArrayBuilder;
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
    struct ConvolutionFilter;
    struct ConvolutionFilterFlags;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace convolution_filter {

/// ConvolutionFilter data structure
///
/// Holds the convolution filter properties (matrixX, matrixY, matrix, divisor, bias, etc.).
template<typename GC>
class ConvolutionFilterData {
public:
    uint8_t matrix_x;
    uint8_t matrix_y;
    std::vector<float> matrix;
    float divisor;
    float bias;
    bool preserve_alpha;
    bool clamp;
    swf::Color color;

    /// Resize the matrix to match matrix_x * matrix_y
    void resize_matrix() {
        auto new_len = static_cast<size_t>(matrix_x) * static_cast<size_t>(matrix_y);
        if (new_len > matrix.size()) {
            matrix.resize(new_len, 0.0f);
        }
    }

    /// Set matrix_x and resize matrix
    void set_matrix_x(uint8_t matrix_x) {
        this->matrix_x = matrix_x;
        resize_matrix();
    }

    /// Set matrix_y and resize matrix
    void set_matrix_y(uint8_t matrix_y) {
        this->matrix_y = matrix_y;
        resize_matrix();
    }

    /// Default constructor
    ConvolutionFilterData()
        : matrix_x(0)
        , matrix_y(0)
        , matrix()
        , divisor(1.0f)
        , bias(0.0f)
        , preserve_alpha(true)
        , clamp(true)
        , color{0, 0, 0, 0}  // Color::from_rgba(0)
    {}
};

/// ConvolutionFilter wrapper
///
/// AVM1 convolution filter object wrapper around ConvolutionFilterData.
template<typename GC>
class ConvolutionFilter {
public:
    /// Create a new ConvolutionFilter
    ///
    /// \param activation The current activation context
    /// \param args Arguments containing the initial properties
    /// \return Result containing the new ConvolutionFilter or an error
    static ConvolutionFilter<GC> create(
        Activation<GC>* activation,
        const Value<GC>* args,
        size_t arg_count
    );

    /// Create from a SWF ConvolutionFilter
    ///
    /// \param filter The SWF filter to convert from
    static ConvolutionFilter<GC> from_filter(const swf::ConvolutionFilter& filter);

    /// Duplicate this filter
    ConvolutionFilter<GC> duplicate() const;

    /// Get the matrixX value
    uint8_t matrix_x() const;

    /// Set the matrixX value
    ///
    /// \param activation The current activation context
    /// \param value The new matrixX value (clamped to 0-15)
    void set_matrix_x(Activation<GC>* activation, const Value<GC>* value);

    /// Get the matrixY value
    uint8_t matrix_y() const;

    /// Set the matrixY value
    ///
    /// \param activation The current activation context
    /// \param value The new matrixY value (clamped to 0-15)
    void set_matrix_y(Activation<GC>* activation, const Value<GC>* value);

    /// Get the matrix as an Array
    ///
    /// \param activation The current activation context
    /// \return Array containing the matrix values
    ArrayBuilder<GC> matrix(Activation<GC>* activation) const;

    /// Set the matrix from an Array
    ///
    /// \param activation The current activation context
    /// \param value The new matrix array
    void set_matrix(Activation<GC>* activation, const Value<GC>* value);

    /// Get the divisor value
    float divisor() const;

    /// Set the divisor value
    ///
    /// \param activation The current activation context
    /// \param value The new divisor value
    void set_divisor(Activation<GC>* activation, const Value<GC>* value);

    /// Get the bias value
    float bias() const;

    /// Set the bias value
    ///
    /// \param activation The current activation context
    /// \param value The new bias value
    void set_bias(Activation<GC>* activation, const Value<GC>* value);

    /// Get the preserveAlpha flag
    bool preserve_alpha() const;

    /// Set the preserveAlpha flag
    ///
    /// \param activation The current activation context
    /// \param value The new preserveAlpha value
    void set_preserve_alpha(Activation<GC>* activation, const Value<GC>* value);

    /// Get the clamp flag
    bool clamp() const;

    /// Set the clamp flag
    ///
    /// \param activation The current activation context
    /// \param value The new clamp value
    void set_clamp(Activation<GC>* activation, const Value<GC>* value);

    /// Get the color value
    swf::Color color() const;

    /// Set the color value
    ///
    /// \param activation The current activation context
    /// \param value The new color value
    void set_color(Activation<GC>* activation, const Value<GC>* value);

    /// Set the alpha value
    ///
    /// \param activation The current activation context
    /// \param value The new alpha value (0.0-1.0, clamped)
    void set_alpha(Activation<GC>* activation, const Value<GC>* value);

    /// Convert to SWF ConvolutionFilter
    swf::ConvolutionFilter filter() const;

private:
    std::shared_ptr<ConvolutionFilterData<GC>> data_;
};

/// Method indices for ConvolutionFilter
namespace method {
    constexpr uint16_t CONSTRUCTOR = 0;
    constexpr uint16_t GET_MATRIX_X = 1;
    constexpr uint16_t SET_MATRIX_X = 2;
    constexpr uint16_t GET_MATRIX_Y = 3;
    constexpr uint16_t SET_MATRIX_Y = 4;
    constexpr uint16_t GET_MATRIX = 5;
    constexpr uint16_t SET_MATRIX = 6;
    constexpr uint16_t GET_DIVISOR = 7;
    constexpr uint16_t SET_DIVISOR = 8;
    constexpr uint16_t GET_BIAS = 9;
    constexpr uint16_t SET_BIAS = 10;
    constexpr uint16_t GET_PRESERVE_ALPHA = 11;
    constexpr uint16_t SET_PRESERVE_ALPHA = 12;
    constexpr uint16_t GET_CLAMP = 13;
    constexpr uint16_t SET_CLAMP = 14;
    constexpr uint16_t GET_COLOR = 15;
    constexpr uint16_t SET_COLOR = 16;
    constexpr uint16_t GET_ALPHA = 17;
    constexpr uint16_t SET_ALPHA = 18;
}

/// Property declarations for ConvolutionFilter prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the ConvolutionFilter class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created ConvolutionFilter class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// ConvolutionFilter method handler
///
/// Handles all ConvolutionFilter methods based on the index.
///
/// \param activation The current activation context
/// \param this_obj The ConvolutionFilter object
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

} // namespace convolution_filter
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_CONVOLUTION_FILTER_H
