// C++ translation of core/src/avm1/globals/boolean.rs
// Boolean class implementation

#ifndef RUFFLE_CORE_AVM1_GLOBALS_BOOLEAN_H
#define RUFFLE_CORE_AVM1_GLOBALS_BOOLEAN_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Object;
    template<typename T> class Value;
    template<typename T> class Error;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
    enum class NativeObject;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace boolean {

// Property declarations for Boolean prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the Boolean class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// Boolean constructor
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Boolean function (called as function, not constructor)
template<typename GC>
Value<GC> function(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Boolean.prototype.toString
template<typename GC>
Value<GC> to_string(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Boolean.prototype.valueOf
template<typename GC>
Value<GC> value_of(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace boolean
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_BOOLEAN_H
