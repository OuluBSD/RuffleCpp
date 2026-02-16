// C++ translation of core/src/avm1/globals/accessibility.rs
// Accessibility class

#ifndef RUFFLE_CORE_AVM1_GLOBALS_ACCESSIBILITY_H
#define RUFFLE_CORE_AVM1_GLOBALS_ACCESSIBILITY_H

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
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace accessibility {

// Method indices for Accessibility
namespace method {
    constexpr uint16_t IS_ACTIVE = 0;
    constexpr uint16_t SEND_EVENT = 1;
    constexpr uint16_t UPDATE_PROPERTIES = 2;
}

// Property declarations for Accessibility object
extern const StaticDeclarations OBJECT_DECLS;

/// Create the Accessibility object
template<typename GC>
Object<GC>* create(DeclContext<GC>* context);

/// Accessibility method handler
template<typename GC>
Value<GC> method(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count,
    uint16_t index
);

} // namespace accessibility
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_ACCESSIBILITY_H
