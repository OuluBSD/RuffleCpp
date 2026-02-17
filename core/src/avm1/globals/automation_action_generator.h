// C++ translation of core/src/avm1/globals/automation_action_generator.rs
// Automation ActionGenerator for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_AUTOMATION_ACTION_GENERATOR_H
#define RUFFLE_CORE_AVM1_GLOBALS_AUTOMATION_ACTION_GENERATOR_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace automation_action_generator {

/// Property declarations for ActionGenerator prototype
extern const StaticDeclarations PROTO_DECLS;

/// Method indices for ActionGenerator methods
namespace method {
    constexpr uint16_t GENERATE_ACTION = 0;
    constexpr uint16_t GENERATE_ACTIONS = 1;
    constexpr uint16_t VALUE_OF = 3;
    constexpr uint16_t TO_STRING = 4;
}

/// Create the ActionGenerator class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created ActionGenerator class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// Implements ActionGenerator method dispatcher
///
/// Dispatches to the appropriate method based on index.
/// All methods are stubs that return Value::Undefined.
///
/// \param activation The current activation context
/// \param this_obj The ActionGenerator object
/// \param args Arguments for the method
/// \param index Method index to call
/// \return Value::Undefined
template<typename GC>
Value<GC> method(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count,
    uint16_t index
);

} // namespace automation_action_generator
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_AUTOMATION_ACTION_GENERATOR_H
