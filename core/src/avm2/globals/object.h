// C++ translation of core/src/avm2/globals/object.rs
//! Object builtin and prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_OBJECT_H
#define RUFFLE_CORE_AVM2_GLOBALS_OBJECT_H

#include <vector>
#include <string>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class ScriptObject;
    template<typename T> class AvmString;
    template<typename T> class Multiname;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace object {

/// Implements Object's custom constructor, called when ActionScript code runs
/// new Object(...) directly.
template<typename GC>
Value<GC> object_constructor(
    Activation<GC>* activation,
    const std::vector<Value<GC>>& args
);

/// Implements Object.prototype.toString
template<typename GC>
Value<GC> _to_string(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Object.prototype.hasOwnProperty
template<typename GC>
Value<GC> has_own_property(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Object.prototype.isPrototypeOf
template<typename GC>
Value<GC> is_prototype_of(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Object.prototype.propertyIsEnumerable
template<typename GC>
Value<GC> property_is_enumerable(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Object.prototype.setPropertyIsEnumerable
template<typename GC>
Value<GC> _set_property_is_enumerable(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace object
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_OBJECT_H
