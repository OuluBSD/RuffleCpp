// C++ translation of core/src/avm1/globals/xml_node.rs
// XMLNode class for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_XML_NODE_H
#define RUFFLE_CORE_AVM1_GLOBALS_XML_NODE_H

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
    struct SystemClass;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace xml_node {

/// Property declarations for XMLNode prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the XMLNode class
///
/// @param context The declaration context for creating the class
/// @param super_proto The prototype for the class
/// @return The created SystemClass
template<typename GC>
SystemClass<GC> create_class(DeclContext<GC>* context, Object<GC>* super_proto);

/// XMLNode constructor
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object being constructed
/// @param args Arguments (nodeType at index 0, optional value at index 1)
/// @return The constructed XMLNode object
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.appendChild
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (child node at index 0)
/// @return Value::Undefined
template<typename GC>
Value<GC> append_child(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.insertBefore
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (child node at index 0, insert point at index 1)
/// @return Value::Undefined
template<typename GC>
Value<GC> insert_before(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.cloneNode
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (deep flag at index 0)
/// @return Cloned XMLNode object or Value::Undefined
template<typename GC>
Value<GC> clone_node(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.getNamespaceForPrefix
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (prefix string at index 0)
/// @return Namespace URI string or Value::Null
template<typename GC>
Value<GC> get_namespace_for_prefix(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.getPrefixForNamespace
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (URI string at index 0)
/// @return Prefix string or Value::Null
template<typename GC>
Value<GC> get_prefix_for_namespace(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.hasChildNodes
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (unused)
/// @return Boolean indicating if node has children
template<typename GC>
Value<GC> has_child_nodes(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.removeNode
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> remove_node(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.toString
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (unused)
/// @return String representation of the node
template<typename GC>
Value<GC> to_string(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.localName property getter
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (unused)
/// @return Local name string or Value::Null
template<typename GC>
Value<GC> local_name(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.nodeName property getter
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (unused)
/// @return Node name string or Value::Null
template<typename GC>
Value<GC> node_name(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.nodeName and XMLNode.nodeValue property setter
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (new value at index 0)
/// @return Value::Undefined
template<typename GC>
Value<GC> set_node_value(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.nodeType property getter
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (unused)
/// @return Node type number or Value::Undefined
template<typename GC>
Value<GC> node_type(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.nodeValue property getter
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (unused)
/// @return Node value string or Value::Null
template<typename GC>
Value<GC> node_value(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.prefix property getter
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (unused)
/// @return Prefix string or Value::Null
template<typename GC>
Value<GC> prefix(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.childNodes property getter
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (unused)
/// @return Array of child nodes or Value::Undefined
template<typename GC>
Value<GC> child_nodes(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.firstChild property getter
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (unused)
/// @return First child node or Value::Null
template<typename GC>
Value<GC> first_child(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.lastChild property getter
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (unused)
/// @return Last child node or Value::Null
template<typename GC>
Value<GC> last_child(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.parentNode property getter
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (unused)
/// @return Parent node or Value::Null
template<typename GC>
Value<GC> parent_node(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.previousSibling property getter
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (unused)
/// @return Previous sibling node or Value::Null
template<typename GC>
Value<GC> previous_sibling(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.nextSibling property getter
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (unused)
/// @return Next sibling node or Value::Null
template<typename GC>
Value<GC> next_sibling(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.attributes property getter
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (unused)
/// @return Attributes object or Value::Undefined
template<typename GC>
Value<GC> attributes(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XMLNode.namespaceURI property getter
///
/// @param activation The current activation context
/// @param this_obj The XMLNode object
/// @param args Arguments (unused)
/// @return Namespace URI string or Value::Null
template<typename GC>
Value<GC> namespace_uri(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace xml_node
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_XML_NODE_H
