// C++ translation of core/src/avm2/globals/q_name.rs
//! QName implementation - qualified names for AVM2

#ifndef RUFFLE_CORE_AVM2_GLOBALS_Q_NAME_H
#define RUFFLE_CORE_AVM2_GLOBALS_Q_NAME_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class QNameObject;
    template<typename T> class Namespace;
    template<typename T> class AvmString;
    enum class ApiVersion;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace q_name {

/// Call handler for QName constructor
///
/// Implements the QName constructor logic:
/// 1. If Namespace is not specified and Type(Name) is Object and Name.[[Class]] == "QName"
///    a. Return Name
/// 2. Otherwise, create and return a new QName object exactly as if the QName
///    constructor had been called with the same arguments
template<typename GC>
Value<GC> call_handler(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements a custom constructor for QName
///
/// Handles various argument combinations:
/// - No arguments: creates QName with empty local name and no namespace
/// - One argument: if QName, copies it; otherwise uses as local name with public namespace
/// - Two arguments: first is namespace, second is local name
template<typename GC>
Value<GC> q_name_constructor(
    Activation<GC>* activation,
    const std::vector<Value<GC>>& args
);

/// Implements QName.localName's getter
///
/// Returns the local name part of the QName
template<typename GC>
Value<GC> get_local_name(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements QName.uri's getter
///
/// Returns the URI of the QName's namespace, or null if no namespace
template<typename GC>
Value<GC> get_uri(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements QName.AS3::toString and QName.prototype.toString
///
/// Returns the string representation of the QName (namespace::localName format)
template<typename GC>
Value<GC> to_string(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace q_name
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_Q_NAME_H
