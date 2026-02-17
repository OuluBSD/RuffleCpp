// C++ translation of core/src/avm1/xml/tree.rs
//! XML Tree structure

#ifndef RUFFLE_CORE_AVM1_XML_TREE_H
#define RUFFLE_CORE_AVM1_XML_TREE_H

#include <cstdint>
#include <memory>
#include <vector>
#include <optional>
#include <string>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Value;
    template<typename T> class Object;
    template<typename T> class NativeObject;
    template<typename T> class ArrayBuilder;
    struct Attribute;
    
    namespace xml {
        struct XmlNodeData;
        class XmlNode;
        
        namespace iterators {
            class ChildIter;
        }
    }
}
namespace string {
    template<typename T> class AvmString;
    template<typename T> class StringContext;
    class WStr;
    class WString;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace xml {
namespace tree {

/// Node type constant for element nodes
constexpr uint8_t ELEMENT_NODE = 1;

/// Node type constant for text nodes
constexpr uint8_t TEXT_NODE = 3;

/// Represents a node in the XML tree
class XmlNode {
public:
    /// Construct a new XML node
    /// @param node_type The type of node (ELEMENT_NODE or TEXT_NODE)
    /// @param node_value The node value (tag name for elements, text content for text nodes)
    static XmlNode new_node(uint8_t node_type, std::optional<string::AvmString<>> node_value);
    
    /// Construct an XML Element node from a quick_xml BytesStart event
    /// @param activation The current activation
    /// @param bs The BytesStart event from quick_xml
    /// @param id_map The ID map object for tracking element IDs
    static XmlNode from_start_event(
        Activation<>* activation,
        const void* bs, // quick_xml::events::BytesStart
        Object<> id_map
    );
    
    /// Get the parent node, if this node has one
    std::optional<XmlNode> parent() const;
    
    /// Get the previous sibling, if this node has one
    std::optional<XmlNode> prev_sibling() const;
    
    /// Get the next sibling, if this node has one
    std::optional<XmlNode> next_sibling() const;
    
    /// Remove node from its current siblings list
    void disown_siblings();
    
    /// Add node to a new siblings list
    void adopt_siblings(std::optional<XmlNode> new_prev, std::optional<XmlNode> new_next);
    
    /// Remove node from this node's child list
    void orphan_child(XmlNode child);
    
    /// Insert a child element into the children list at the given position
    /// @param position The position to insert the child
    /// @param child The child node to insert
    void insert_child(size_t position, XmlNode child);
    
    /// Append a child element to the end of the child list
    void append_child(XmlNode child);
    
    /// Remove this node from its parent
    void remove_node();
    
    /// Returns the type of this node as an integer
    uint8_t node_type() const;
    
    /// Returns the tag name of this element, if any
    std::optional<string::AvmString<>> node_name() const;
    
    /// Get the local name (part after colon in qualified name)
    std::optional<string::AvmString<>> local_name();
    
    /// Get the prefix (part before colon in qualified name)
    std::optional<string::AvmString<>> prefix(string::StringContext<>& context);
    
    /// Returns the node value of this node, if any
    std::optional<string::AvmString<>> node_value() const;
    
    /// Set the node value
    void set_node_value(string::AvmString<> value);
    
    /// Returns the number of children of the current tree node
    size_t children_len() const;
    
    /// Get the position of a child of this node
    std::optional<size_t> child_position(XmlNode child) const;
    
    /// Checks if child is a direct descendant of self
    bool has_child(XmlNode child) const;
    
    /// Retrieve a given child by index
    std::optional<XmlNode> get_child_by_index(size_t index) const;
    
    /// Returns an iterator that yields child nodes
    // iterators::ChildIter children() const;
    
    /// Returns an iterator that yields ancestor nodes (including itself)
    // core::iter::successors ancestors() const;
    
    /// Introduce this node to a new script object
    /// @param new_object The script object to associate with this node
    void introduce_script_object(Object<> new_object);
    
    /// Obtain the script object for a given XML tree node
    /// Constructs a new script object if one does not exist
    Object<> script_object(Activation<>* activation);
    
    /// Obtain the script object for a given XML tree node's attributes
    Object<> attributes() const;
    
    /// Gets a lazy-created .childNodes array
    Object<> get_or_init_cached_child_nodes(Activation<>* activation);
    
    /// Refreshes the .childNodes array. Call this after every child list mutation
    void refresh_cached_child_nodes(Activation<>* activation);
    
    /// Create a duplicate copy of this node
    /// @param deep If true, clone the entire subtree
    XmlNode duplicate(bool deep) const;
    
    /// Look up the URI for the given prefix
    /// XML namespaces are determined by xmlns: namespace attributes
    std::optional<Value<>> lookup_namespace_uri(const string::WStr& prefix) const;
    
    /// Convert the given node to a string of UTF-8 encoded XML
    string::WString into_string(Activation<>* activation);
    
private:
    /// Set the parent of this node
    void set_parent(std::optional<XmlNode> parent);
    
    /// Set this node's previous sibling
    void set_prev_sibling(std::optional<XmlNode> new_prev);
    
    /// Set this node's next sibling
    void set_next_sibling(std::optional<XmlNode> new_next);
    
    /// Returns a mutable reference to child nodes
    std::vector<XmlNode>& children_mut();
    
    /// Get the already-instantiated script object from the current node
    std::optional<Object<>> get_script_object() const;
    
    /// Write the contents of this node, including its children, to the given string
    void write_node_to_string(Activation<>* activation, string::WString& result);
    
    // Internal data pointer (GC-managed in Rust, would need smart pointer in C++)
    // This is a simplified representation
    std::shared_ptr<XmlNodeData> data_;
};

} // namespace tree
} // namespace xml
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_XML_TREE_H
