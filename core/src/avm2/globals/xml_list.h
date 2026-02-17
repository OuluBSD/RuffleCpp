// C++ translation of core/src/avm2/globals/xml_list.rs
//! XMLList builtin and prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_XML_LIST_H
#define RUFFLE_CORE_AVM2_GLOBALS_XML_LIST_H

#include <vector>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class AvmString;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    template<typename GC>
    class Activation;
    template<typename GC>
    class XmlListObject;
    template<typename GC>
    class XmlObject;
    template<typename GC>
    class Object;
    template<typename GC>
    class Multiname;

    namespace avm2 {
        namespace e4x {
            template<typename GC>
            class E4XNode;
            template<typename GC>
            class E4XNodeKind;
            template<typename GC>
            class E4XOrXml;
        }
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace xml_list {

/// Check if children have complex content
template<typename GC>
bool has_complex_content_inner(const std::vector<e4x::E4XOrXml<GC>>& children);

/// Check if children have simple content
template<typename GC>
bool has_simple_content_inner(const std::vector<e4x::E4XOrXml<GC>>& children);

/// Implements XMLList's init method, which is called from the constructor
template<typename GC>
Value<GC> init(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements XMLList's call handler
template<typename GC>
Value<GC> call_handler(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// ECMA-357 13.5.4.11 XMLList.prototype.elements([name])
template<typename GC>
Value<GC> elements(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.hasComplexContent()
template<typename GC>
Value<GC> has_complex_content(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.hasSimpleContent()
template<typename GC>
Value<GC> has_simple_content(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.toString()
template<typename GC>
Value<GC> to_string(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.toXMLString()
template<typename GC>
Value<GC> to_xml_string(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.length()
template<typename GC>
Value<GC> length(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.child(propertyName)
template<typename GC>
Value<GC> child(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.children()
template<typename GC>
Value<GC> children(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// ECMA-357 13.5.4.8 XMLList.prototype.contains(value)
template<typename GC>
Value<GC> contains(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.copy()
template<typename GC>
Value<GC> copy(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.attribute(attributeName)
template<typename GC>
Value<GC> attribute(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.attributes()
template<typename GC>
Value<GC> attributes(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.descendants([name])
template<typename GC>
Value<GC> descendants(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// ECMA-357 13.5.4.20 XMLList.prototype.text()
template<typename GC>
Value<GC> text(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.comments()
template<typename GC>
Value<GC> comments(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// ECMA-357 13.5.4.17 XMLList.prototype.parent()
template<typename GC>
Value<GC> parent(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.processingInstructions([name])
template<typename GC>
Value<GC> processing_instructions(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.normalize()
template<typename GC>
Value<GC> normalize(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// XML proxy methods - delegate to single child XML object

/// XMLList.prototype.addNamespace(ns)
template<typename GC>
Value<GC> add_namespace(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.appendChild(child)
template<typename GC>
Value<GC> append_child(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.childIndex()
template<typename GC>
Value<GC> child_index(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.inScopeNamespaces()
template<typename GC>
Value<GC> in_scope_namespaces(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.insertChildAfter(child1, child2)
template<typename GC>
Value<GC> insert_child_after(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.insertChildBefore(child1, child2)
template<typename GC>
Value<GC> insert_child_before(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.localName()
template<typename GC>
Value<GC> local_name(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.name()
template<typename GC>
Value<GC> name(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.namespaceDeclarations()
template<typename GC>
Value<GC> namespace_declarations(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.nodeKind()
template<typename GC>
Value<GC> node_kind(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.prependChild(value)
template<typename GC>
Value<GC> prepend_child(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.removeNamespace(ns)
template<typename GC>
Value<GC> remove_namespace(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.replace(propertyName, value)
template<typename GC>
Value<GC> replace(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.setChildren(value)
template<typename GC>
Value<GC> set_children(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.setLocalName(name)
template<typename GC>
Value<GC> set_local_name(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.setName(name)
template<typename GC>
Value<GC> set_name(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.setNamespace(ns)
template<typename GC>
Value<GC> set_namespace(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// XMLList.prototype.namespace(hasPrefix, prefix) - special case
template<typename GC>
Value<GC> namespace_internal_impl(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace xml_list
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_XML_LIST_H
