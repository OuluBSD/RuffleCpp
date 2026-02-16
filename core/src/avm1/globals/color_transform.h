// C++ translation of core/src/avm1/globals/color_transform.rs
// flash.geom.ColorTransform object for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_COLOR_TRANSFORM_H
#define RUFFLE_CORE_AVM1_GLOBALS_COLOR_TRANSFORM_H

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
namespace string {
    template<typename T> class AvmString;
}
}
namespace swf {
    struct ColorTransform;
    class Fixed8;
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace color_transform {

/// ColorTransformObject class for AVM1 ColorTransform objects
///
/// Stores the color transform properties with Cell-based mutability.
template<typename GC>
class ColorTransformObject {
public:
    /// Creates an identity color transform (multipliers=1.0, offsets=0.0)
    static ColorTransformObject identity();

    /// Constructs a ColorTransform object with the given transform
    ///
    /// \param activation The current activation context
    /// \param color_transform The SWF color transform to use
    /// \return The constructed ColorTransform object
    static Value<GC> construct(
        Activation<GC>* activation,
        const swf::ColorTransform& color_transform
    );

    /// Casts a Value to a ColorTransformObject
    ///
    /// \param value The value to cast
    /// \return The ColorTransformObject, or nullptr if not a ColorTransform
    static ColorTransformObject* cast(const Value<GC>& value);

    /// Converts to SWF ColorTransform
    ///
    /// \return The equivalent SWF ColorTransform
    swf::ColorTransform to_swf() const;

    // Getters
    double red_multiplier() const;
    double green_multiplier() const;
    double blue_multiplier() const;
    double alpha_multiplier() const;
    double red_offset() const;
    double green_offset() const;
    double blue_offset() const;
    double alpha_offset() const;

    // Setters
    void set_red_multiplier(double value);
    void set_green_multiplier(double value);
    void set_blue_multiplier(double value);
    void set_alpha_multiplier(double value);
    void set_red_offset(double value);
    void set_green_offset(double value);
    void set_blue_offset(double value);
    void set_alpha_offset(double value);

private:
    double red_multiplier_;
    double green_multiplier_;
    double blue_multiplier_;
    double alpha_multiplier_;
    double red_offset_;
    double green_offset_;
    double blue_offset_;
    double alpha_offset_;
};

/// Property declarations for ColorTransform prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the ColorTransform class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created ColorTransform class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// ColorTransform constructor
///
/// Initializes the ColorTransform object with the provided parameters.
/// Can be called with 8 numeric parameters or a ColorTransform object.
///
/// \param activation The current activation context
/// \param this_obj The ColorTransform object being constructed
/// \param args Arguments (rMult, gMult, bMult, aMult, rOff, gOff, bOff, aOff) or ColorTransform
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.alphaMultiplier getter
///
/// \param activation The current activation context
/// \param this_obj The ColorTransform object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The alpha multiplier value
template<typename GC>
Value<GC> get_alpha_multiplier(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.alphaMultiplier setter
///
/// \param activation The current activation context
/// \param this_obj The ColorTransform object
/// \param args Arguments (new alpha multiplier value)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> set_alpha_multiplier(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.redMultiplier getter
template<typename GC>
Value<GC> get_red_multiplier(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.redMultiplier setter
template<typename GC>
Value<GC> set_red_multiplier(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.greenMultiplier getter
template<typename GC>
Value<GC> get_green_multiplier(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.greenMultiplier setter
template<typename GC>
Value<GC> set_green_multiplier(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.blueMultiplier getter
template<typename GC>
Value<GC> get_blue_multiplier(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.blueMultiplier setter
template<typename GC>
Value<GC> set_blue_multiplier(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.alphaOffset getter
template<typename GC>
Value<GC> get_alpha_offset(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.alphaOffset setter
template<typename GC>
Value<GC> set_alpha_offset(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.redOffset getter
template<typename GC>
Value<GC> get_red_offset(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.redOffset setter
template<typename GC>
Value<GC> set_red_offset(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.greenOffset getter
template<typename GC>
Value<GC> get_green_offset(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.greenOffset setter
template<typename GC>
Value<GC> set_green_offset(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.blueOffset getter
template<typename GC>
Value<GC> get_blue_offset(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.blueOffset setter
template<typename GC>
Value<GC> set_blue_offset(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.rgb getter
///
/// Gets the RGB color value (offsets only, multipliers ignored).
///
/// \param activation The current activation context
/// \param this_obj The ColorTransform object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return RGB value as integer
template<typename GC>
Value<GC> get_rgb(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.rgb setter
///
/// Sets the RGB offsets and clears multipliers.
///
/// \param activation The current activation context
/// \param this_obj The ColorTransform object
/// \param args Arguments (RGB value as integer)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> set_rgb(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.toString method
///
/// Returns a string representation of the ColorTransform.
///
/// \param activation The current activation context
/// \param this_obj The ColorTransform object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return String representation
template<typename GC>
Value<GC> to_string(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ColorTransform.concat method
///
/// Concatenates another ColorTransform with this one.
///
/// \param activation The current activation context
/// \param this_obj The ColorTransform object
/// \param args Arguments (other ColorTransform to concat)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> concat(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace color_transform
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_COLOR_TRANSFORM_H
