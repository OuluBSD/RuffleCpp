// C++ translation of core/src/avm1/globals/automation_configuration.rs
// flash.automation.Configuration class

#ifndef RUFFLE_CORE_AVM1_GLOBALS_AUTOMATION_CONFIGURATION_H
#define RUFFLE_CORE_AVM1_GLOBALS_AUTOMATION_CONFIGURATION_H

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
namespace automation_configuration {

// Method indices for Configuration
namespace method {
    constexpr uint16_t GET_TEST_AUTOMATION_CONFIGURATION = 0;
    constexpr uint16_t GET_DEVICE_CONFIGURATION = 1;
    constexpr uint16_t SET_DEVICE_CONFIGURATION = 2;
    constexpr uint16_t VALUE_OF = 4;
    constexpr uint16_t TO_STRING = 5;
}

// Property declarations for Configuration prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the Configuration class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created Configuration class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// Configuration method handler
///
/// Handles all methods for the Configuration class.
/// All methods are currently stubs that return Value::Undefined.
///
/// \param activation The current activation context
/// \param this_obj The Configuration object
/// \param args Arguments passed to the method
/// \param index The method index to call
/// \return Value::Undefined for all stub methods
template<typename GC>
Value<GC> method(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count,
    uint16_t index
);

} // namespace automation_configuration
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_AUTOMATION_CONFIGURATION_H
