// C++ translation of core/src/avm1/globals/matrix.rs
// flash.geom.Matrix object for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_MATRIX_H
#define RUFFLE_CORE_AVM1_GLOBALS_MATRIX_H

#include <vector>
#include <cstdint>
#include <memory>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {

namespace string {
    class AvmString;
}

namespace avm1 {

template<typename GC>
class Activation;
template<typename GC>
class Error;
template<typename GC>
class Object;
template<typename GC>
class Value;

namespace property_decl {
    template<typename GC>
    class DeclContext;
    template<typename GC>
    class StaticDeclarations;
    template<typename GC>
    struct SystemClass;
}

} // namespace avm1

namespace wstr {
    class WStr;
}

} // namespace core
} // namespace ruffle

namespace swf {
struct Twips;
}

namespace ruffle_render {
namespace matrix {
    struct Matrix;
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace matrix {

/// Property declarations for Matrix prototype
extern const avm1::property_decl::StaticDeclarations PROTO_DECLS;

/// Create the Matrix class
/// @param context The declaration context
/// @param super_proto The superclass prototype
/// @return SystemClass containing the Matrix class
template<typename GC>
avm1::property_decl::SystemClass<GC> create_class(
    avm1::property_decl::DeclContext<GC>* context,
    avm1::Object<GC> super_proto
);

/// Convert a Value to a Matrix
/// @param value The value to convert
/// @param activation The current activation
/// @return The resulting Matrix
template<typename GC>
ruffle_render::matrix::Matrix value_to_matrix(
    Value<GC> value,
    Activation<GC>* activation
);

/// Convert a gradient object to a Matrix
/// @param object The gradient object
/// @param activation The current activation
/// @return The resulting Matrix
template<typename GC>
ruffle_render::matrix::Matrix gradient_object_to_matrix(
    Object<GC> object,
    Activation<GC>* activation
);

/// Convert an object to a Matrix
/// @param object The object to convert
/// @param activation The current activation
/// @return The resulting Matrix
template<typename GC>
ruffle_render::matrix::Matrix object_to_matrix(
    Object<GC> object,
    Activation<GC>* activation
);

/// Convert an object to a Matrix, using identity if properties are missing
/// @param object The object to convert
/// @param activation The current activation
/// @return The resulting Matrix (identity if properties missing)
template<typename GC>
ruffle_render::matrix::Matrix object_to_matrix_or_default(
    Object<GC> object,
    Activation<GC>* activation
);

/// Convert a Matrix to a Value
/// @param matrix The matrix to convert
/// @param activation The current activation
/// @return The resulting Value
template<typename GC>
Value<GC> matrix_to_value(
    const ruffle_render::matrix::Matrix& matrix,
    Activation<GC>* activation
);

/// Apply a Matrix to an object's properties
/// @param matrix The matrix to apply
/// @param object The object to modify
/// @param activation The current activation
template<typename GC>
void apply_matrix_to_object(
    const ruffle_render::matrix::Matrix& matrix,
    Object<GC> object,
    Activation<GC>* activation
);

/// Matrix constructor
/// @param activation The current activation
/// @param this_obj The this object
/// @param args Function arguments [a, b, c, d, tx, ty]
/// @return Undefined
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Set matrix to identity
/// @param activation The current activation
/// @param this_obj The this object
/// @param args Unused
/// @return Undefined
template<typename GC>
Value<GC> identity(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Clone the matrix
/// @param activation The current activation
/// @param this_obj The this object
/// @param args Unused
/// @return New Matrix object
template<typename GC>
Value<GC> clone(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Scale the matrix
/// @param activation The current activation
/// @param this_obj The this object
/// @param args [scaleX, scaleY]
/// @return Undefined
template<typename GC>
Value<GC> scale(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Rotate the matrix
/// @param activation The current activation
/// @param this_obj The this object
/// @param args [angle]
/// @return Undefined
template<typename GC>
Value<GC> rotate(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Translate the matrix
/// @param activation The current activation
/// @param this_obj The this object
/// @param args [translateX, translateY]
/// @return Undefined
template<typename GC>
Value<GC> translate(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Concatenate another matrix
/// @param activation The current activation
/// @param this_obj The this object
/// @param args [other]
/// @return Undefined
template<typename GC>
Value<GC> concat(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Invert the matrix
/// @param activation The current activation
/// @param this_obj The this object
/// @param args Unused
/// @return Undefined
template<typename GC>
Value<GC> invert(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Create a box matrix
/// @param activation The current activation
/// @param this_obj The this object
/// @param args [scaleX, scaleY, rotation, translateX, translateY]
/// @return Undefined
template<typename GC>
Value<GC> create_box(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Create a gradient box matrix
/// @param activation The current activation
/// @param this_obj The this object
/// @param args [width, height, rotation, translateX, translateY]
/// @return Undefined
template<typename GC>
Value<GC> create_gradient_box(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Transform a point by the matrix
/// @param activation The current activation
/// @param this_obj The this object
/// @param args [point]
/// @return New Point object
template<typename GC>
Value<GC> transform_point(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Delta transform a point (without translation)
/// @param activation The current activation
/// @param this_obj The this object
/// @param args [point]
/// @return New Point object
template<typename GC>
Value<GC> delta_transform_point(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Convert matrix to string
/// @param activation The current activation
/// @param this_obj The this object
/// @param args Unused
/// @return String representation
template<typename GC>
Value<GC> to_string(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

// Template method implementations

template<typename GC>
avm1::property_decl::SystemClass<GC> create_class(
    avm1::property_decl::DeclContext<GC>* context,
    avm1::Object<GC> super_proto
) {
    auto class_obj = context->class(constructor, super_proto);
    context->define_properties_on(class_obj.proto, PROTO_DECLS(context));
    return class_obj;
}

template<typename GC>
ruffle_render::matrix::Matrix value_to_matrix(
    Value<GC> value,
    Activation<GC>* activation
) {
    auto obj = value.coerce_to_object_or_bare(activation);
    
    auto a = obj->get(string::AvmString::from_utf8(activation->gc(), "a"), activation)
        ->coerce_to_f64(activation) as f32;
    auto b = obj->get(string::AvmString::from_utf8(activation->gc(), "b"), activation)
        ->coerce_to_f64(activation) as f32;
    auto c = obj->get(string::AvmString::from_utf8(activation->gc(), "c"), activation)
        ->coerce_to_f64(activation) as f32;
    auto d = obj->get(string::AvmString::from_utf8(activation->gc(), "d"), activation)
        ->coerce_to_f64(activation) as f32;
    auto tx = swf::Twips::from_pixels(
        obj->get(string::AvmString::from_utf8(activation->gc(), "tx"), activation)
            ->coerce_to_f64(activation)
    );
    auto ty = swf::Twips::from_pixels(
        obj->get(string::AvmString::from_utf8(activation->gc(), "ty"), activation)
            ->coerce_to_f64(activation)
    );

    return ruffle_render::matrix::Matrix{a, b, c, d, tx, ty};
}

template<typename GC>
ruffle_render::matrix::Matrix gradient_object_to_matrix(
    Object<GC> object,
    Activation<GC>* activation
) {
    auto matrix_type = object->get(
        string::AvmString::from_utf8(activation->gc(), "matrixType"),
        activation
    )->coerce_to_string(activation);

    if (matrix_type == string::AvmString::from_utf8(activation->gc(), "box")) {
        auto width = object->get(
            string::AvmString::from_utf8(activation->gc(), "w"),
            activation
        )->coerce_to_f64(activation);
        
        auto height = object->get(
            string::AvmString::from_utf8(activation->gc(), "h"),
            activation
        )->coerce_to_f64(activation);
        
        auto rotation = object->get(
            string::AvmString::from_utf8(activation->gc(), "r"),
            activation
        )->coerce_to_f64(activation);
        
        auto tx = object->get(
            string::AvmString::from_utf8(activation->gc(), "x"),
            activation
        )->coerce_to_f64(activation);
        
        auto ty = object->get(
            string::AvmString::from_utf8(activation->gc(), "y"),
            activation
        )->coerce_to_f64(activation);

        return ruffle_render::matrix::Matrix::create_gradient_box(
            static_cast<float>(width),
            static_cast<float>(height),
            static_cast<float>(rotation),
            swf::Twips::from_pixels(tx),
            swf::Twips::from_pixels(ty)
        );
    } else {
        // TODO: You can also pass a 3x3 matrix here
        return object_to_matrix(object, activation);
    }
}

template<typename GC>
ruffle_render::matrix::Matrix object_to_matrix(
    Object<GC> object,
    Activation<GC>* activation
) {
    auto a = object->get(
        string::AvmString::from_utf8(activation->gc(), "a"),
        activation
    )->coerce_to_f64(activation) as f32;
    
    auto b = object->get(
        string::AvmString::from_utf8(activation->gc(), "b"),
        activation
    )->coerce_to_f64(activation) as f32;
    
    auto c = object->get(
        string::AvmString::from_utf8(activation->gc(), "c"),
        activation
    )->coerce_to_f64(activation) as f32;
    
    auto d = object->get(
        string::AvmString::from_utf8(activation->gc(), "d"),
        activation
    )->coerce_to_f64(activation) as f32;
    
    auto tx = swf::Twips::from_pixels(
        object->get(
            string::AvmString::from_utf8(activation->gc(), "tx"),
            activation
        )->coerce_to_f64(activation)
    );
    
    auto ty = swf::Twips::from_pixels(
        object->get(
            string::AvmString::from_utf8(activation->gc(), "ty"),
            activation
        )->coerce_to_f64(activation)
    );

    return ruffle_render::matrix::Matrix{a, b, c, d, tx, ty};
}

template<typename GC>
ruffle_render::matrix::Matrix object_to_matrix_or_default(
    Object<GC> object,
    Activation<GC>* activation
) {
    // These lookups do not search the prototype chain and ignore virtual properties
    auto a_val = object->get_local_stored(
        string::AvmString::from_utf8(activation->gc(), "a"),
        activation
    );
    auto b_val = object->get_local_stored(
        string::AvmString::from_utf8(activation->gc(), "b"),
        activation
    );
    auto c_val = object->get_local_stored(
        string::AvmString::from_utf8(activation->gc(), "c"),
        activation
    );
    auto d_val = object->get_local_stored(
        string::AvmString::from_utf8(activation->gc(), "d"),
        activation
    );
    auto tx_val = object->get_local_stored(
        string::AvmString::from_utf8(activation->gc(), "tx"),
        activation
    );
    auto ty_val = object->get_local_stored(
        string::AvmString::from_utf8(activation->gc(), "ty"),
        activation
    );

    if (a_val.has_value() && b_val.has_value() && c_val.has_value() &&
        d_val.has_value() && tx_val.has_value() && ty_val.has_value()) {
        
        auto a = a_val->coerce_to_f64(activation) as f32;
        auto b = b_val->coerce_to_f64(activation) as f32;
        auto c = c_val->coerce_to_f64(activation) as f32;
        auto d = d_val->coerce_to_f64(activation) as f32;
        auto tx = swf::Twips::from_pixels(tx_val->coerce_to_f64(activation));
        auto ty = swf::Twips::from_pixels(ty_val->coerce_to_f64(activation));
        
        return ruffle_render::matrix::Matrix{a, b, c, d, tx, ty};
    }

    return ruffle_render::matrix::Matrix::IDENTITY;
}

template<typename GC>
Value<GC> matrix_to_value(
    const ruffle_render::matrix::Matrix& matrix,
    Activation<GC>* activation
) {
    std::vector<Value<GC>> args = {
        Value<GC>::from_f64(matrix.a),
        Value<GC>::from_f64(matrix.b),
        Value<GC>::from_f64(matrix.c),
        Value<GC>::from_f64(matrix.d),
        Value<GC>::from_f64(matrix.tx.to_pixels()),
        Value<GC>::from_f64(matrix.ty.to_pixels())
    };

    auto constructor = activation->prototypes()->matrix_constructor;
    auto object = constructor->construct(activation, args);
    return object;
}

template<typename GC>
void apply_matrix_to_object(
    const ruffle_render::matrix::Matrix& matrix,
    Object<GC> object,
    Activation<GC>* activation
) {
    object->set(
        string::AvmString::from_utf8(activation->gc(), "a"),
        Value<GC>::from_f64(matrix.a),
        activation
    );
    object->set(
        string::AvmString::from_utf8(activation->gc(), "b"),
        Value<GC>::from_f64(matrix.b),
        activation
    );
    object->set(
        string::AvmString::from_utf8(activation->gc(), "c"),
        Value<GC>::from_f64(matrix.c),
        activation
    );
    object->set(
        string::AvmString::from_utf8(activation->gc(), "d"),
        Value<GC>::from_f64(matrix.d),
        activation
    );
    object->set(
        string::AvmString::from_utf8(activation->gc(), "tx"),
        Value<GC>::from_f64(matrix.tx.to_pixels()),
        activation
    );
    object->set(
        string::AvmString::from_utf8(activation->gc(), "ty"),
        Value<GC>::from_f64(matrix.ty.to_pixels()),
        activation
    );
}

template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
) {
    if (args.empty()) {
        identity(activation, this_obj, args);
    } else {
        if (args.size() > 0) {
            this_obj->set(
                string::AvmString::from_utf8(activation->gc(), "a"),
                args[0],
                activation
            );
        }
        if (args.size() > 1) {
            this_obj->set(
                string::AvmString::from_utf8(activation->gc(), "b"),
                args[1],
                activation
            );
        }
        if (args.size() > 2) {
            this_obj->set(
                string::AvmString::from_utf8(activation->gc(), "c"),
                args[2],
                activation
            );
        }
        if (args.size() > 3) {
            this_obj->set(
                string::AvmString::from_utf8(activation->gc(), "d"),
                args[3],
                activation
            );
        }
        if (args.size() > 4) {
            this_obj->set(
                string::AvmString::from_utf8(activation->gc(), "tx"),
                args[4],
                activation
            );
        }
        if (args.size() > 5) {
            this_obj->set(
                string::AvmString::from_utf8(activation->gc(), "ty"),
                args[5],
                activation
            );
        }
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> identity(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>&
) {
    this_obj->set(
        string::AvmString::from_utf8(activation->gc(), "d"),
        Value<GC>::from_f64(1.0),
        activation
    );
    this_obj->set(
        string::AvmString::from_utf8(activation->gc(), "a"),
        Value<GC>::from_f64(1.0),
        activation
    );
    this_obj->set(
        string::AvmString::from_utf8(activation->gc(), "c"),
        Value<GC>::from_f64(0.0),
        activation
    );
    this_obj->set(
        string::AvmString::from_utf8(activation->gc(), "b"),
        Value<GC>::from_f64(0.0),
        activation
    );
    this_obj->set(
        string::AvmString::from_utf8(activation->gc(), "ty"),
        Value<GC>::from_f64(0.0),
        activation
    );
    this_obj->set(
        string::AvmString::from_utf8(activation->gc(), "tx"),
        Value<GC>::from_f64(0.0),
        activation
    );
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> clone(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>&
) {
    std::vector<Value<GC>> args = {
        this_obj->get(string::AvmString::from_utf8(activation->gc(), "a"), activation),
        this_obj->get(string::AvmString::from_utf8(activation->gc(), "b"), activation),
        this_obj->get(string::AvmString::from_utf8(activation->gc(), "c"), activation),
        this_obj->get(string::AvmString::from_utf8(activation->gc(), "d"), activation),
        this_obj->get(string::AvmString::from_utf8(activation->gc(), "tx"), activation),
        this_obj->get(string::AvmString::from_utf8(activation->gc(), "ty"), activation)
    };

    auto constructor = activation->prototypes()->matrix_constructor;
    auto cloned = constructor->construct(activation, args);
    return cloned;
}

template<typename GC>
Value<GC> scale(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
) {
    auto scale_x = args.size() > 0 
        ? args[0].coerce_to_f64(activation)
        : Value<GC>::undefined().coerce_to_f64(activation);
    auto scale_y = args.size() > 1
        ? args[1].coerce_to_f64(activation)
        : Value<GC>::undefined().coerce_to_f64(activation);

    auto matrix = ruffle_render::matrix::Matrix::scale(
        static_cast<float>(scale_x),
        static_cast<float>(scale_y)
    );
    matrix *= object_to_matrix(this_obj, activation);
    apply_matrix_to_object(matrix, this_obj, activation);

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> rotate(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
) {
    auto angle = args.size() > 0
        ? args[0].coerce_to_f64(activation)
        : Value<GC>::undefined().coerce_to_f64(activation);

    auto matrix = ruffle_render::matrix::Matrix::rotate(static_cast<float>(angle));
    matrix *= object_to_matrix(this_obj, activation);
    apply_matrix_to_object(matrix, this_obj, activation);

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> translate(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
) {
    auto translate_x = args.size() > 0
        ? args[0].coerce_to_f64(activation)
        : Value<GC>::undefined().coerce_to_f64(activation);
    auto translate_y = args.size() > 1
        ? args[1].coerce_to_f64(activation)
        : Value<GC>::undefined().coerce_to_f64(activation);

    auto matrix = ruffle_render::matrix::Matrix::translate(
        swf::Twips::from_pixels(translate_x),
        swf::Twips::from_pixels(translate_y)
    );
    matrix *= object_to_matrix(this_obj, activation);
    apply_matrix_to_object(matrix, this_obj, activation);

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> concat(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
) {
    auto matrix = object_to_matrix(this_obj, activation);
    auto other = value_to_matrix(
        args.size() > 0 ? args[0] : Value<GC>::undefined(),
        activation
    );
    matrix = other * matrix;
    apply_matrix_to_object(matrix, this_obj, activation);

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> invert(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>&
) {
    // FIXME:
    // 1) `invert` and other Matrix methods need to operate on `f64`, not with `ruffle_render::Matrix`.
    // 2) If non-invertible, we are always setting to an identity matrix. But Flash only return identity
    //    if `c != 0 && b != 0`? Otherwise it results in a matrix with infinities.
    
    auto matrix = object_to_matrix(this_obj, activation);
    auto inverted = matrix.inverse().value_or(ruffle_render::matrix::Matrix::IDENTITY);
    apply_matrix_to_object(inverted, this_obj, activation);

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> create_box(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
) {
    auto scale_x = args.size() > 0
        ? args[0].coerce_to_f64(activation)
        : Value<GC>::undefined().coerce_to_f64(activation);
    auto scale_y = args.size() > 1
        ? args[1].coerce_to_f64(activation)
        : Value<GC>::undefined().coerce_to_f64(activation);
    
    // [NA] Docs say rotation is optional and defaults to 0, but that's wrong?
    auto rotation = args.size() > 2
        ? args[2].coerce_to_f64(activation)
        : Value<GC>::undefined().coerce_to_f64(activation);
    
    auto translate_x = args.size() > 3
        ? args[3].coerce_to_f64(activation)
        : 0.0;
    auto translate_y = args.size() > 4
        ? args[4].coerce_to_f64(activation)
        : 0.0;

    auto matrix = ruffle_render::matrix::Matrix::create_box_with_rotation(
        static_cast<float>(scale_x),
        static_cast<float>(scale_y),
        static_cast<float>(rotation),
        swf::Twips::from_pixels(translate_x),
        swf::Twips::from_pixels(translate_y)
    );
    apply_matrix_to_object(matrix, this_obj, activation);

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> create_gradient_box(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
) {
    auto width = args.size() > 0
        ? args[0].coerce_to_f64(activation)
        : Value<GC>::undefined().coerce_to_f64(activation);
    auto height = args.size() > 1
        ? args[1].coerce_to_f64(activation)
        : Value<GC>::undefined().coerce_to_f64(activation);
    
    auto rotation = args.size() > 2
        ? args[2].coerce_to_f64(activation)
        : 0.0;
    auto translate_x = args.size() > 3
        ? args[3].coerce_to_f64(activation)
        : 0.0;
    auto translate_y = args.size() > 4
        ? args[4].coerce_to_f64(activation)
        : 0.0;

    auto matrix = ruffle_render::matrix::Matrix::create_gradient_box(
        static_cast<float>(width),
        static_cast<float>(height),
        static_cast<float>(rotation),
        swf::Twips::from_pixels(translate_x),
        swf::Twips::from_pixels(translate_y)
    );
    apply_matrix_to_object(matrix, this_obj, activation);

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> transform_point(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
) {
    auto matrix = object_to_matrix(this_obj, activation);
    auto point = globals::point::value_to_point(
        args.size() > 0 ? args[0] : Value<GC>::undefined(),
        activation
    );

    auto x = point.first * matrix.a + point.second * matrix.c + matrix.tx.to_pixels();
    auto y = point.first * matrix.b + point.second * matrix.d + matrix.ty.to_pixels();
    
    auto object = globals::point::point_to_object({x, y}, activation);
    return object;
}

template<typename GC>
Value<GC> delta_transform_point(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
) {
    auto matrix = object_to_matrix(this_obj, activation);
    auto point = globals::point::value_to_point(
        args.size() > 0 ? args[0] : Value<GC>::undefined(),
        activation
    );

    auto x = point.first * matrix.a + point.second * matrix.c;
    auto y = point.first * matrix.b + point.second * matrix.d;
    
    auto object = globals::point::point_to_object({x, y}, activation);
    return object;
}

template<typename GC>
Value<GC> to_string(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>&
) {
    auto a = this_obj->get(string::AvmString::from_utf8(activation->gc(), "a"), activation);
    auto b = this_obj->get(string::AvmString::from_utf8(activation->gc(), "b"), activation);
    auto c = this_obj->get(string::AvmString::from_utf8(activation->gc(), "c"), activation);
    auto d = this_obj->get(string::AvmString::from_utf8(activation->gc(), "d"), activation);
    auto tx = this_obj->get(string::AvmString::from_utf8(activation->gc(), "tx"), activation);
    auto ty = this_obj->get(string::AvmString::from_utf8(activation->gc(), "ty"), activation);

    auto str = string::AvmString::from_utf8(
        activation->gc(),
        string_format("(a={}, b={}, c={}, d={}, tx={}, ty={})",
            a.coerce_to_string(activation)->to_string(),
            b.coerce_to_string(activation)->to_string(),
            c.coerce_to_string(activation)->to_string(),
            d.coerce_to_string(activation)->to_string(),
            tx.coerce_to_string(activation)->to_string(),
            ty.coerce_to_string(activation)->to_string()
        )
    );
    
    return str.into();
}

} // namespace matrix
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_MATRIX_H
