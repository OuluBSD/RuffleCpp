// C++ translation of core/src/avm1/globals/system_product.rs
// System.Product class

#ifndef RUFFLE_CORE_AVM1_GLOBALS_SYSTEM_PRODUCT_H
#define RUFFLE_CORE_AVM1_GLOBALS_SYSTEM_PRODUCT_H

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
namespace system_product {

/// Property declarations for System.Product prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the System.Product class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created System.Product class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// System.Product constructor
///
/// Sets the 'name' property on the instance.
///
/// \param activation The current activation context
/// \param this_obj The System.Product object
/// \param args Arguments passed to the constructor (name)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.Product.isRunning method
///
/// \param activation The current activation context
/// \param this_obj The System.Product object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Bool(false) - stub implementation
template<typename GC>
Value<GC> is_running(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.Product.isInstalled method
///
/// \param activation The current activation context
/// \param this_obj The System.Product object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Bool(false) - stub implementation
template<typename GC>
Value<GC> is_installed(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.Product.launch method
///
/// \param activation The current activation context
/// \param this_obj The System.Product object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Undefined - stub implementation
template<typename GC>
Value<GC> launch(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.Product.download method
///
/// \param activation The current activation context
/// \param this_obj The System.Product object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Undefined - stub implementation
template<typename GC>
Value<GC> download(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.Product.validate method
///
/// \param activation The current activation context
/// \param this_obj The System.Product object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Undefined - stub implementation
template<typename GC>
Value<GC> validate(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.Product.installedVersion method
///
/// \param activation The current activation context
/// \param this_obj The System.Product object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Undefined - stub implementation
template<typename GC>
Value<GC> installed_version(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace system_product
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_SYSTEM_PRODUCT_H
