// C++ translation of core/src/avm2/globals/namespace.rs
//! Namespace implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_NAMESPACE_H
#define RUFFLE_CORE_AVM2_GLOBALS_NAMESPACE_H

#include <vector>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class Namespace;
    template<typename T> class NamespaceObject;
    template<typename T> class AvmString;
    template<typename T> class Object;
    template<typename T> class QNameObject;

    namespace e4x {
        template<typename GC>
        bool is_xml_name(AvmString<GC> str);
    }
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {

/// Implements a custom constructor for Namespace
template<typename GC>
Value<GC> namespace_constructor(
    Activation<GC>* activation,
    const std::vector<Value<GC>>& args
);

/// Implements Namespace.prefix's getter
template<typename GC>
Value<GC> get_prefix(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Namespace.uri's getter
template<typename GC>
Value<GC> get_uri(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_NAMESPACE_H
