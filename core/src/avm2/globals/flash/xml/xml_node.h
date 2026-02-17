// C++ translation of core/src/avm2/globals/flash/xml/xml_node.rs
// flash.xml.XMLNode native methods

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_XML_XML_NODE_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_XML_XML_NODE_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class AvmString;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace xml {
namespace xml_node {

/// Implements flash.xml.XMLNode._escape_xml
/// Escapes special XML characters in a string
///
/// @param activation The current activation context
/// @param this_value The this value (unused)
/// @param args Function arguments (expects string at index 0)
/// @return Escaped string if special characters found, otherwise original string
template<typename GC>
Value<GC> escape_xml(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace xml_node
} // namespace xml
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_XML_XML_NODE_H
