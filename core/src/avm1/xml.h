// C++ translation of core/src/avm1/xml.rs
// Garbage-collectable XML DOM impl

#ifndef RUFFLE_CORE_AVM1_XML_H
#define RUFFLE_CORE_AVM1_XML_H

// Submodule declarations
namespace ruffle {
namespace core {
namespace avm1 {
namespace xml {

// Submodule namespaces
namespace iterators {
    template<typename GC> class ChildIter;
}

namespace tree {
    template<typename GC> class XmlNode;
}

// Re-exports from tree module
using XmlNode = tree::XmlNode;

// Node type constants
constexpr uint8_t ELEMENT_NODE = 1;
constexpr uint8_t TEXT_NODE = 3;

} // namespace xml
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_XML_H
