/*
 * C++ header for AVM1 XML functionality
 * This replaces the functionality of core/src/avm1/xml.rs and core/src/avm1/xml/tree.rs
 */

#ifndef AVM1_XML_H
#define AVM1_XML_H

#include "avm1.h"
#include "avm1/value.h"
#include "avm1/object.h"
#include "avm1/activation.h"
#include "avm1/error.h"
#include "avm1/array_builder.h"
#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <list>
#include <unordered_map>

namespace ruffle {

// Constants for node types
constexpr uint8_t ELEMENT_NODE = 1;
constexpr uint8_t TEXT_NODE = 3;

// Forward declaration
class XmlNode;

// XML Node class representing a node in the XML tree
class XmlNode {
private:
    std::shared_ptr<Object> script_object_;
    std::shared_ptr<XmlNode> parent_;
    std::shared_ptr<XmlNode> prev_sibling_;
    std::shared_ptr<XmlNode> next_sibling_;
    uint8_t node_type_;
    std::optional<std::string> node_value_;
    std::shared_ptr<Object> attributes_;
    std::shared_ptr<Object> cached_child_nodes_;
    std::vector<std::shared_ptr<XmlNode>> children_;

public:
    // Constructor
    XmlNode(uint8_t node_type, std::optional<std::string> node_value = std::nullopt)
        : node_type_(node_type), node_value_(std::move(node_value)) {
        attributes_ = std::make_shared<Object>(nullptr); // Create object without prototype
    }

    // Static factory method to create a new XML node
    static std::shared_ptr<XmlNode> create(uint8_t node_type, 
                                          std::optional<std::string> node_value = std::nullopt) {
        return std::make_shared<XmlNode>(node_type, std::move(node_value));
    }

    // Get the parent, if this node has one
    std::shared_ptr<XmlNode> parent() const { return parent_; }

    // Set the parent of this node
    void set_parent(std::shared_ptr<XmlNode> parent) { parent_ = std::move(parent); }

    // Get the previous sibling, if this node has one
    std::shared_ptr<XmlNode> prev_sibling() const { return prev_sibling_; }

    // Set this node's previous sibling
    void set_prev_sibling(std::shared_ptr<XmlNode> new_prev) { prev_sibling_ = std::move(new_prev); }

    // Get the next sibling, if this node has one
    std::shared_ptr<XmlNode> next_sibling() const { return next_sibling_; }

    // Set this node's next sibling
    void set_next_sibling(std::shared_ptr<XmlNode> new_next) { next_sibling_ = std::move(new_next); }

    // Remove node from its current siblings list
    void disown_siblings() {
        if (prev_sibling_) {
            prev_sibling_->set_next_sibling(next_sibling_);
        }

        if (next_sibling_) {
            next_sibling_->set_prev_sibling(prev_sibling_);
        }

        set_prev_sibling(nullptr);
        set_next_sibling(nullptr);
    }

    // Add node to a new siblings list
    void adopt_siblings(std::shared_ptr<XmlNode> new_prev, 
                       std::shared_ptr<XmlNode> new_next) {
        if (new_prev) {
            new_prev->set_next_sibling(shared_from_this());
        }

        if (new_next) {
            new_next->set_prev_sibling(shared_from_this());
        }

        set_prev_sibling(std::move(new_prev));
        set_next_sibling(std::move(new_next));
    }

    // Remove node from this node's child list
    void orphan_child(std::shared_ptr<XmlNode> child) {
        auto it = std::find(children_.begin(), children_.end(), child);
        if (it != children_.end()) {
            children_.erase(it);
        }
    }

    // Insert child into the children list of this node
    void insert_child(size_t position, std::shared_ptr<XmlNode> child) {
        // Check for cyclic references
        if (is_ancestor(child)) {
            return; // Don't insert if it would create a cycle
        }

        // Remove from old parent if exists
        if (child->parent_ && child->parent_ != shared_from_this()) {
            child->parent_->orphan_child(child);
        }

        child->set_parent(shared_from_this());

        if (position <= children_.size()) {
            children_.insert(children_.begin() + position, child);
        } else {
            children_.push_back(child); // If position is out of bounds, append to end
        }

        // Set up sibling relationships
        std::shared_ptr<XmlNode> new_prev = (position > 0) ? children_[position - 1] : nullptr;
        std::shared_ptr<XmlNode> new_next = (position < children_.size() - 1) ? children_[position + 1] : nullptr;
        child->adopt_siblings(new_prev, new_next);
    }

    // Append a child element to the end of the child list
    void append_child(std::shared_ptr<XmlNode> child) {
        insert_child(children_.size(), std::move(child));
    }

    // Remove this node from its parent
    void remove_node() {
        if (parent_) {
            parent_->orphan_child(shared_from_this());
            disown_siblings();
            set_parent(nullptr);
        }
    }

    // Returns the type of this node as an integer
    uint8_t node_type() const { return node_type_; }

    // Returns the tag name of this element, if any
    std::optional<std::string> node_name() const {
        if (node_type_ == ELEMENT_NODE) {
            return node_value_;
        }
        return std::nullopt;
    }

    // Returns the local name (after colon) of this element
    std::optional<std::string> local_name() const {
        if (auto name = node_name()) {
            size_t pos = name->find(':');
            if (pos != std::string::npos && pos + 1 < name->length()) {
                return name->substr(pos + 1);
            }
            return *name;
        }
        return std::nullopt;
    }

    // Returns the prefix (before colon) of this element
    std::optional<std::string> prefix() const {
        if (auto name = node_name()) {
            size_t pos = name->find(':');
            if (pos != std::string::npos && pos + 1 < name->length()) {
                return name->substr(0, pos);
            }
            return std::string(); // Empty string for no prefix
        }
        return std::nullopt;
    }

    // Returns the node value of this node, if any
    std::optional<std::string> node_value() const {
        if (node_type_ == ELEMENT_NODE) {
            return std::nullopt;
        } else {
            return node_value_;
        }
    }

    // Set the node value
    void set_node_value(std::string value) {
        node_value_ = std::move(value);
    }

    // Returns the number of children of the current tree node
    size_t children_len() const { return children_.size(); }

    // Get the position of a child of this node
    std::optional<size_t> child_position(std::shared_ptr<XmlNode> child) const {
        for (size_t i = 0; i < children_.size(); ++i) {
            if (children_[i] == child) {
                return i;
            }
        }
        return std::nullopt;
    }

    // Checks if child is a direct descendant of this node
    bool has_child(std::shared_ptr<XmlNode> child) const {
        return child->parent_ == shared_from_this();
    }

    // Retrieve a given child by index
    std::shared_ptr<XmlNode> get_child_by_index(size_t index) const {
        if (index < children_.size()) {
            return children_[index];
        }
        return nullptr;
    }

    // Returns an iterator that yields child nodes
    const std::vector<std::shared_ptr<XmlNode>>& children() const { return children_; }

    // Returns an iterator that yields ancestor nodes (including itself)
    std::vector<std::shared_ptr<XmlNode>> ancestors() const {
        std::vector<std::shared_ptr<XmlNode>> result;
        std::shared_ptr<XmlNode> current = shared_from_this();
        while (current) {
            result.push_back(current);
            current = current->parent();
        }
        return result;
    }

    // Check if a node is an ancestor of this node
    bool is_ancestor(std::shared_ptr<XmlNode> node) const {
        std::shared_ptr<XmlNode> current = parent_;
        while (current) {
            if (current == node) {
                return true;
            }
            current = current->parent_;
        }
        return false;
    }

    // Get the already-instantiated script object from the current node
    std::shared_ptr<Object> get_script_object() const { return script_object_; }

    // Introduce this node to a new script object
    void introduce_script_object(std::shared_ptr<Object> new_object) {
        if (script_object_) {
            throw std::runtime_error("An attempt was made to change the already-established link between script object and XML node. This has been denied and is likely a bug.");
        }
        script_object_ = std::move(new_object);
    }

    // Obtain the script object for a given XML tree node, constructing a new
    // script object if one does not exist
    std::shared_ptr<Object> script_object(std::shared_ptr<Activation> activation) {
        if (script_object_) {
            return script_object_;
        }

        // Create a new XML node object
        auto xml_node_proto = activation->context()->prototypes().xml_node_constructor;
        auto prototype = xml_node_proto->get("prototype", activation);
        auto object = std::make_shared<Object>(&activation->context()->strings, prototype);
        
        introduce_script_object(object);
        object->set_native(activation->context()->gc(), NativeObject(object));
        
        return object;
    }

    // Obtain the script object for a given XML tree node's attributes
    std::shared_ptr<Object> attributes() const { return attributes_; }

    // Gets a lazy-created .childNodes array
    std::shared_ptr<Object> get_or_init_cached_child_nodes(std::shared_ptr<Activation> activation) {
        if (cached_child_nodes_) {
            return cached_child_nodes_;
        }

        auto array = ArrayBuilder::empty(activation);
        cached_child_nodes_ = array;
        refresh_cached_child_nodes(activation);
        return array;
    }

    // Refreshes the .childNodes array. Call this after every child list mutation
    void refresh_cached_child_nodes(std::shared_ptr<Activation> activation) {
        if (cached_child_nodes_) {
            cached_child_nodes_->set_length(activation, 0);
            for (size_t i = 0; i < children_.size(); ++i) {
                auto child_obj = children_[i]->script_object(activation);
                cached_child_nodes_->set_element(activation, static_cast<int32_t>(i), child_obj->as_value());
            }
        }
    }

    // Create a duplicate copy of this node
    std::shared_ptr<XmlNode> duplicate(bool deep) const {
        auto attributes = std::make_shared<Object>(nullptr);
        // Copy attributes from this node
        for (const auto& [key, value] : attributes_->get_properties()) {
            attributes->define_value(key, value, 0); // 0 for empty attributes
        }

        auto clone = std::make_shared<XmlNode>(node_type_, node_value_);
        clone->attributes_ = attributes;

        if (deep) {
            for (const auto& child : children_) {
                clone->append_child(child->duplicate(deep));
            }
        }

        return clone;
    }

    // Look up the URI for the given prefix
    std::optional<std::shared_ptr<Value>> lookup_namespace_uri(const std::string& prefix) const {
        for (const auto& node : ancestors()) {
            // Iterate attributes looking for xmlns attributes
            for (const auto& [key, value] : node->attributes_->get_properties()) {
                if (key.substr(0, 5) == "xmlns") {
                    // The empty prefix matches every attribute that starts with "xmlns"
                    std::string namespace_part = key.length() > 5 ? key.substr(6) : ""; // Skip "xmlns:"
                    if (prefix.empty() || (namespace_part == prefix)) {
                        return value;
                    }
                }
            }
        }

        return std::nullopt;
    }

    // Convert the given node to a string of UTF-8 encoded XML
    std::string to_string(std::shared_ptr<Activation> activation) const {
        std::string result;
        write_node_to_string(activation, result);
        return result;
    }

private:
    // Write the contents of this node, including its children, to the given string
    void write_node_to_string(std::shared_ptr<Activation> activation, std::string& result) const {
        if (node_type_ == ELEMENT_NODE) {
            if (node_value_) {
                result += "<" + *node_value_;

                // Add attributes
                for (const auto& [key, value] : attributes_->get_properties()) {
                    std::string value_str = value->coerce_to_string(activation);
                    // Escape the value
                    std::string escaped_value = escape_xml(value_str);

                    result += " " + key + "=\"" + escaped_value + "\"";
                }

                if (children_.empty()) {
                    result += " />";
                } else {
                    result += ">";
                    for (const auto& child : children_) {
                        child->write_node_to_string(activation, result);
                    }
                    result += "</" + *node_value_ + ">";
                }
            } else {
                // If no tag name, just process children
                for (const auto& child : children_) {
                    child->write_node_to_string(activation, result);
                }
            }
        } else {
            // Text node
            if (node_value_) {
                result += escape_xml(*node_value_);
            }
        }
    }

    // Helper function to escape XML
    static std::string escape_xml(const std::string& input) {
        std::string result;
        for (char c : input) {
            switch (c) {
                case '&': result += "&amp;"; break;
                case '<': result += "&lt;"; break;
                case '>': result += "&gt;"; break;
                case '\"': result += "&quot;"; break;
                case '\'': result += "&apos;"; break;
                default: result += c; break;
            }
        }
        return result;
    }

public:
    // Enable shared_from_this
    std::shared_ptr<XmlNode> shared_from_this() {
        return std::static_pointer_cast<XmlNode>(shared_from_this_base());
    }

private:
    // Base class for shared_from_this
    class SharedFromThisBase {
    public:
        virtual ~SharedFromThisBase() = default;
        virtual std::shared_ptr<XmlNode> shared_from_this_base() = 0;
    };
};

// Helper class to implement shared_from_this
class XmlNodeWithSharedFromThis : public XmlNode, public std::enable_shared_from_this<XmlNodeWithSharedFromThis> {
public:
    XmlNodeWithSharedFromThis(uint8_t node_type, std::optional<std::string> node_value = std::nullopt)
        : XmlNode(node_type, std::move(node_value)) {}

    std::shared_ptr<XmlNode> shared_from_this_base() override {
        return std::static_pointer_cast<XmlNode>(shared_from_this());
    }
};

// Factory function to create XmlNode with shared_from_this support
inline std::shared_ptr<XmlNode> create_xml_node(uint8_t node_type, 
                                               std::optional<std::string> node_value = std::nullopt) {
    return std::make_shared<XmlNodeWithSharedFromThis>(node_type, std::move(node_value));
}

} // namespace ruffle

#endif // AVM1_XML_H