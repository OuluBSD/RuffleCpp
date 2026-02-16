// C++ translation of core/src/avm1/globals/system_security.rs
// System.security object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_SYSTEM_SECURITY_H
#define RUFFLE_CORE_AVM1_GLOBALS_SYSTEM_SECURITY_H

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
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace system_security {

/// Property declarations for System.security object
extern const StaticDeclarations OBJECT_DECLS;

/// Create the System.security object
///
/// \param context The declaration context for creating the object
/// \return The created System.security object
template<typename GC>
Object<GC> create(
    DeclContext<GC>* context
);

/// Implements System.security.allowDomain method
///
/// \param activation The current activation context
/// \param this_obj The System.security object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Bool(true) if argument provided - stub implementation
template<typename GC>
Value<GC> allow_domain(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.security.allowInsecureDomain method
///
/// \param activation The current activation context
/// \param this_obj The System.security object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Undefined - stub implementation
template<typename GC>
Value<GC> allow_insecure_domain(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.security.loadPolicyFile method
///
/// \param activation The current activation context
/// \param this_obj The System.security object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Undefined - stub implementation
template<typename GC>
Value<GC> load_policy_file(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.security.escapeDomain method
///
/// \param activation The current activation context
/// \param this_obj The System.security object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Undefined - stub implementation
template<typename GC>
Value<GC> escape_domain(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.security.sandboxType property getter
///
/// Returns the sandbox type of the current movie.
///
/// \param activation The current activation context
/// \param this_obj The System.security object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return String representing the sandbox type
template<typename GC>
Value<GC> get_sandbox_type(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.security.chooseLocalSwfPath property getter
///
/// \param activation The current activation context
/// \param this_obj The System.security object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Undefined - stub implementation
template<typename GC>
Value<GC> get_choose_local_swf_path(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.security.PolicyFileResolver method
///
/// \param activation The current activation context
/// \param this_obj The System.security object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Undefined - stub implementation
template<typename GC>
Value<GC> policy_file_resolver(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace system_security
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_SYSTEM_SECURITY_H
