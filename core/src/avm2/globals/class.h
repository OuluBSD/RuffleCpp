// C++ translation of core/src/avm2/globals/class.rs
// AVM2 Class builtin/prototype implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_CLASS_H
#define RUFFLE_CORE_AVM2_GLOBALS_CLASS_H

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class ClassObject;
    template<typename T> class Object;
    template<typename T> class Value;
    template<typename T> class Error;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {

/// Allocator for the Class class
/// Always returns an error (error 1115)
template<typename GC>
Object<GC> class_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation
);

/// Get the prototype of a class
/// Returns the class's prototype if the value is a ClassObject, otherwise Undefined
template<typename GC>
Value<GC> get_prototype(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args
);

} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_CLASS_H
