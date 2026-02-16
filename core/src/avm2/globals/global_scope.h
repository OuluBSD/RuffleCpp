// C++ translation of core/src/avm2/globals/global_scope.rs
//! global_scope implementation - provides the global scope object for scripts

#ifndef RUFFLE_CORE_AVM2_GLOBALS_GLOBAL_SCOPE_H
#define RUFFLE_CORE_AVM2_GLOBALS_GLOBAL_SCOPE_H

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class ScriptObject;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace global_scope {

/// Create the global scope object for a script
/// This is the object that serves as the global scope for ActionScript code
template<typename GC>
Object<GC> create_global_scope(
    Activation<GC>* activation
);

} // namespace global_scope
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_GLOBAL_SCOPE_H
