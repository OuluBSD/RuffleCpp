// C++ translation of core/src/avm2/object/xml_object.rs
//! Object representation for XML objects

#ifndef RUFFLE_CORE_AVM2_OBJECT_XML_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_XML_OBJECT_H

#include <vector>
#include <optional>
#include <memory>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename GC>
    class Activation;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    template<typename GC>
    class Object;
    template<typename GC>
    class ClassObject;
    template<typename GC>
    class NamespaceObject;
    template<typename GC>
    class XmlListObject;
    template<typename GC>
    class AvmString;
    template<typename GC>
    class Multiname;
    template<typename GC>
    class FunctionArgs;

    namespace e4x {
        template<typename GC>
        class E4XNamespace;
        template<typename GC>
        class E4XNode;
        enum class E4XNodeKind;
    }

    namespace object {
        template<typename GC>
        class ScriptObjectData;
        template<typename GC>
        class TObject;
    }
}

namespace gc_arena {
    template<typename T>
    class Gc;
    template<typename T>
    class GcWeak;
    class Mutation;
    namespace lock {
        template<typename T>
        class Lock;
    }
}

namespace common {
    namespace utils {
        template<typename T>
        concept HasPrefixField;
    }
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace object {

using gc_arena::Gc;
using gc_arena::GcWeak;
using gc_arena::Mutation;
using gc_arena::lock::Lock;

/// Forward declaration of XmlListObject for E4XOrXml
template<typename GC>
class XmlListObject;

/// Union type for E4X or XML
template<typename GC>
struct E4XOrXml {
    // Implementation would use std::variant in C++17+
    // For now, using separate fields
    e4x::E4XNode<GC> e4x_node;
    XmlListObject<GC> xml_list;
    
    bool is_e4x() const { return e4x_node != nullptr; }
};

/// XmlOrXmlListObject union
template<typename GC>
struct XmlOrXmlListObject {
    enum class Type { Xml, XmlList };
    Type type;
    
    Object<GC> xml_obj;
    XmlListObject<GC> xml_list_obj;
    
    static XmlOrXmlListObject<GC> xml(Object<GC> obj) {
        return XmlOrXmlListObject<GC>{Type::Xml, obj, XmlListObject<GC>()};
    }
};

/// Internal data structure for XmlObject
template<typename GC>
struct XmlObjectData {
    /// Base script object
    ScriptObjectData<GC> base;
    
    /// XML node
    Lock<e4x::E4XNode<GC>> node;
};

/// Weak reference to XmlObject
template<typename GC>
class XmlObjectWeak {
public:
    XmlObjectWeak(GcWeak<GC, XmlObjectData<GC>> weak) : weak_(weak) {}
    
    bool is_alive() const { return weak_.is_alive(); }
    Object<GC> upgrade() const;
    
private:
    GcWeak<GC, XmlObjectData<GC>> weak_;
};

/// XmlObject template class representing XML instances
template<typename GC>
class XmlObject : public TObject<GC> {
public:
    using GcContext = Mutation;
    using DataType = XmlObjectData<GC>;
    
    XmlObject(Gc<GC, DataType> data) : data_(data) {}
    
    /// Create a new XML object from a node
    static XmlObject<GC> new_xml(
        e4x::E4XNode<GC> node,
        Activation<GC>* activation
    );
    
    /// Get the child list for a given name
    XmlListObject<GC> get_child_list(
        Activation<GC>* activation,
        const Multiname<GC>* name
    );
    
    /// Implements XML.prototype.child(propertyName)
    XmlListObject<GC> child(
        const Multiname<GC>* name,
        Activation<GC>* activation
    );
    
    /// Get elements matching the given name
    XmlListObject<GC> elements(
        const Multiname<GC>* name,
        Activation<GC>* activation
    );
    
    /// Get the length (number of children)
    std::optional<size_t> length() const;
    
    /// Set the node
    void set_node(GcContext* mc, e4x::E4XNode<GC> node);
    
    /// Get the local name
    std::optional<AvmString<GC>> local_name() const;
    
    /// Get the namespace object
    NamespaceObject<GC> namespace_object(
        Activation<GC>* activation,
        const std::vector<e4x::E4XNamespace<GC>>& in_scope_ns
    );
    
    /// Check if this matches the given name
    bool matches_name(const Multiname<GC>* multiname) const;
    
    /// Get the underlying node
    e4x::E4XNode<GC> node() const { return data_->node.get(); }
    
    /// Create a deep copy
    XmlObject<GC> deep_copy(Activation<GC>* activation);
    
    /// Convert to XML string
    AvmString<GC> as_xml_string(Activation<GC>* activation);
    
    /// Check equality with another value
    bool equals(const Value<GC>* other, Activation<GC>* activation);
    
    /// Abstract equality comparison
    bool abstract_eq(const Value<GC>* other, Activation<GC>* activation);
    
    // TObject interface
    ScriptObjectData<GC>* gc_base() override {
        return &data_->base;
    }
    
    std::optional<XmlListObject<GC>> xml_descendants(
        Activation<GC>* activation,
        const Multiname<GC>* multiname
    ) override;
    
    Value<GC> get_property_local(
        const Multiname<GC>* name,
        Activation<GC>* activation
    ) override;
    
    Value<GC> call_property_local(
        const Multiname<GC>* multiname,
        FunctionArgs<GC> arguments,
        Activation<GC>* activation
    ) override;
    
    bool has_own_property(const Multiname<GC>* name) override;
    
    bool has_property_via_in(
        Activation<GC>* activation,
        const Multiname<GC>* name
    ) override;
    
    bool has_own_property_string(
        AvmString<GC> name,
        Activation<GC>* activation
    ) override;
    
    void set_property_local(
        const Multiname<GC>* name,
        Value<GC> value,
        Activation<GC>* activation
    ) override;
    
    uint32_t get_next_enumerant(uint32_t last_index) override;
    
    Value<GC> get_enumerant_value(uint32_t index) override;
    
    Value<GC> get_enumerant_name(uint32_t index) override;
    
    bool delete_property_local(
        Activation<GC>* activation,
        const Multiname<GC>* name
    ) override;
    
    /// Get weak reference
    XmlObjectWeak<GC> weak() const {
        return XmlObjectWeak<GC>(GcWeak<GC, DataType>(data_));
    }
    
private:
    Gc<GC, DataType> data_;
};

// ============================================================================
// Template Implementations
// ============================================================================

template<typename GC>
XmlObject<GC> XmlObject<GC>::new_xml(
    e4x::E4XNode<GC> node,
    Activation<GC>* activation
) {
    auto data = Gc<GC, DataType>::new_gc(
        activation->gc(),
        DataType{
            ScriptObjectData<GC>::new_instance(activation->avm2().classes().xml),
            Lock<e4x::E4XNode<GC>>(node)
        }
    );
    return XmlObject<GC>(data);
}

template<typename GC>
XmlListObject<GC> XmlObject<GC>::get_child_list(
    Activation<GC>* activation,
    const Multiname<GC>* name
) {
    std::vector<E4XOrXml<GC>> matched_children;
    
    auto node_kind = node().kind();
    if (node_kind->is_element()) {
        auto [children, attributes] = node_kind->as_element();
        auto search_children = name->is_attribute() ? attributes : children;
        
        for (const auto& child : search_children) {
            if (child->matches_name(name)) {
                matched_children.push_back(E4XOrXml<GC>{child, XmlListObject<GC>()});
            }
        }
    }
    
    auto list = XmlListObject<GC>::new_with_children(
        activation,
        matched_children,
        std::optional(XmlOrXmlListObject<GC>::xml(Object<GC>(this))),
        std::optional(name->clone())
    );
    
    if (list.length() > 0) {
        list.set_dirty_flag();
    }
    
    return list;
}

template<typename GC>
XmlListObject<GC> XmlObject<GC>::child(
    const Multiname<GC>* name,
    Activation<GC>* activation
) {
    // Check if name is a numeric index
    if (auto local_name = name->local_name()) {
        if (auto index = local_name->parse<unsigned int>()) {
            std::vector<E4XOrXml<GC>> result;
            
            auto node_kind = node().kind();
            if (node_kind->is_element()) {
                auto [children, _attributes] = node_kind->as_element();
                if (index.value() < children.size()) {
                    result.push_back(E4XOrXml<GC>{children[index.value()], XmlListObject<GC>()});
                }
            }
            
            auto list = XmlListObject<GC>::new_with_children(
                activation,
                result,
                std::nullopt,
                std::nullopt
            );
            
            if (list.length() > 0) {
                list.set_dirty_flag();
            }
            
            return list;
        }
    }
    
    return get_child_list(activation, name);
}

template<typename GC>
XmlListObject<GC> XmlObject<GC>::elements(
    const Multiname<GC>* name,
    Activation<GC>* activation
) {
    std::vector<E4XOrXml<GC>> children;
    
    auto node_kind = node().kind();
    if (node_kind->is_element()) {
        auto [node_children, _attributes] = node_kind->as_element();
        
        for (const auto& child : node_children) {
            if (child->is_element() && child->matches_name(name)) {
                children.push_back(E4XOrXml<GC>{child, XmlListObject<GC>()});
            }
        }
    }
    
    auto list = XmlListObject<GC>::new_with_children(
        activation,
        children,
        std::optional(XmlOrXmlListObject<GC>::xml(Object<GC>(this))),
        std::nullopt
    );
    
    if (list.length() > 0) {
        list.set_dirty_flag();
    }
    
    return list;
}

template<typename GC>
std::optional<size_t> XmlObject<GC>::length() const {
    return node().length();
}

template<typename GC>
void XmlObject<GC>::set_node(GcContext* mc, e4x::E4XNode<GC> node) {
    data_->node.set(node);
}

template<typename GC>
std::optional<AvmString<GC>> XmlObject<GC>::local_name() const {
    return node().local_name();
}

template<typename GC>
NamespaceObject<GC> XmlObject<GC>::namespace_object(
    Activation<GC>* activation,
    const std::vector<e4x::E4XNamespace<GC>>& in_scope_ns
) {
    return node()
        ->get_namespace(activation->strings(), in_scope_ns)
        ->as_namespace_object(activation);
}

template<typename GC>
bool XmlObject<GC>::matches_name(const Multiname<GC>* multiname) const {
    return node()->matches_name(multiname);
}

template<typename GC>
XmlObject<GC> XmlObject<GC>::deep_copy(Activation<GC>* activation) {
    auto node_copy = node()->deep_copy(activation->gc());
    return new_xml(node_copy, activation);
}

template<typename GC>
AvmString<GC> XmlObject<GC>::as_xml_string(Activation<GC>* activation) {
    return node()->xml_to_xml_string(activation);
}

template<typename GC>
bool XmlObject<GC>::equals(const Value<GC>* other, Activation<GC>* activation) {
    auto other_xml = other->as_object()->as_xml_object();
    if (!other_xml.has_value()) {
        return false;
    }
    
    if (Object<GC>::ptr_eq(*this, other_xml.value())) {
        return true;
    }
    
    return node()->equals(other_xml.value()->node());
}

template<typename GC>
bool XmlObject<GC>::abstract_eq(const Value<GC>* other, Activation<GC>* activation) {
    if (auto obj = other->as_object()) {
        if (auto xml_obj = obj->as_xml_object()) {
            // Both are XML types
            if ((node()->is_text() || node()->is_attribute()) && xml_obj->node()->has_simple_content()) {
                return node()->xml_to_string(activation) == xml_obj->node()->xml_to_string(activation);
            }
            if ((xml_obj->node()->is_text() || xml_obj->node()->is_attribute()) && node()->has_simple_content()) {
                return node()->xml_to_string(activation) == xml_obj->node()->xml_to_string(activation);
            }
            return equals(other, activation);
        }
    }
    
    // Check if this has simple content
    if (node()->has_simple_content()) {
        return node()->xml_to_string(activation) == other->coerce_to_string(activation);
    }
    
    return false;
}

template<typename GC>
std::optional<XmlListObject<GC>> XmlObject<GC>::xml_descendants(
    Activation<GC>* activation,
    const Multiname<GC>* multiname
) {
    auto handled_name = e4x::handle_input_multiname(multiname->clone(), activation);
    std::vector<E4XOrXml<GC>> descendants;
    
    node()->descendants(&handled_name, &descendants);
    
    auto list = XmlListObject<GC>::new_with_children(
        activation,
        descendants,
        std::nullopt,
        std::nullopt
    );
    
    if (list.length() > 0) {
        list.set_dirty_flag();
    }
    
    return list;
}

template<typename GC>
Value<GC> XmlObject<GC>::get_property_local(
    const Multiname<GC>* name,
    Activation<GC>* activation
) {
    // Only supported numerical index is 0
    if (!name->has_explicit_namespace()) {
        if (auto local_name = name->local_name()) {
            if (auto index = local_name->parse<unsigned int>()) {
                if (index.value() == 0) {
                    return Value<GC>::from_object(Object<GC>(this));
                } else {
                    return Value<GC>::undefined();
                }
            }
        }
    }
    
    auto handled_name = e4x::handle_input_multiname(name->clone(), activation);
    auto list = get_child_list(activation, &handled_name);
    return Value<GC>::from_object(Object<GC>(list));
}

template<typename GC>
Value<GC> XmlObject<GC>::call_property_local(
    const Multiname<GC>* multiname,
    FunctionArgs<GC> arguments,
    Activation<GC>* activation
) {
    auto proto = this->proto();
    if (!proto.has_value()) {
        return Value<GC>::undefined();
    }
    
    auto method = Value<GC>::from_object(*proto)->get_property(multiname, activation);
    
    // If method doesn't exist and we have simple content, coerce to string
    if (method.is_undefined()) {
        auto prop = get_property_local(multiname, activation);
        if (auto list = prop.as_object()->as_xml_list_object()) {
            if (list->length() == 0 && node()->has_simple_content()) {
                auto receiver = Value<GC>::from_string(node()->xml_to_string(activation));
                return receiver.call_property(multiname, arguments, activation);
            }
        }
    }
    
    return method.call(activation, Value<GC>::from_object(Object<GC>(this)), arguments);
}

template<typename GC>
bool XmlObject<GC>::has_own_property(const Multiname<GC>* name) {
    if (node()->has_property(name)) {
        return true;
    }
    return data_->base.has_own_dynamic_property(name);
}

template<typename GC>
bool XmlObject<GC>::has_property_via_in(
    Activation<GC>* activation,
    const Multiname<GC>* name
) {
    auto handled_name = e4x::handle_input_multiname(name->clone(), activation);
    return has_own_property(&handled_name);
}

template<typename GC>
bool XmlObject<GC>::has_own_property_string(
    AvmString<GC> name,
    Activation<GC>* activation
) {
    auto multiname = e4x::string_to_multiname(activation, name);
    return has_own_property(&multiname);
}

template<typename GC>
void XmlObject<GC>::set_property_local(
    const Multiname<GC>* name,
    Value<GC> value,
    Activation<GC>* activation
) {
    auto handled_name = e4x::handle_input_multiname(name->clone(), activation);
    
    // Throw TypeError if name is numeric
    if (auto local_name = handled_name.local_name()) {
        if (local_name->parse<unsigned int>().has_value()) {
            // Error 1087
            return;
        }
    }
    
    // Return if not element
    if (!node()->is_element()) {
        return;
    }
    
    // Process value
    Value<GC> processed_value;
    if (auto xml = value.as_object()->as_xml_object()) {
        if (xml->node()->is_attribute() || xml->node()->is_text()) {
            processed_value = Value<GC>::from_string(value.coerce_to_string(activation));
        } else {
            processed_value = Value<GC>::from_object(Object<GC>(xml->deep_copy(activation)));
        }
    } else if (auto list = value.as_object()->as_xml_list_object()) {
        if (list->length() == 1) {
            auto xml = list->xml_object_child(0, activation);
            if (xml->node()->is_attribute() || xml->node()->is_text()) {
                processed_value = Value<GC>::from_string(value.coerce_to_string(activation));
            } else {
                processed_value = Value<GC>::from_object(Object<GC>(list->deep_copy(activation)));
            }
        } else {
            processed_value = Value<GC>::from_object(Object<GC>(list->deep_copy(activation)));
        }
    } else {
        processed_value = Value<GC>::from_string(value.coerce_to_string(activation));
    }
    
    // Handle attribute
    if (handled_name.is_attribute()) {
        AvmString<GC> attr_value;
        if (auto list = processed_value.as_object()->as_xml_list_object()) {
            string::WString out;
            if (list->length() != 0) {
                out.append(list->children()[0]->node()->xml_to_string(activation).as_wstr());
                for (size_t i = 1; i < list->length(); ++i) {
                    out.push_back(' ');
                    out.append(list->children()[i]->node()->xml_to_string(activation).as_wstr());
                }
            }
            attr_value = AvmString<GC>::new_string(activation->gc(), out);
        } else {
            attr_value = processed_value.coerce_to_string(activation);
        }
        
        delete_property_local(activation, &handled_name);
        
        if (auto local = handled_name.local_name()) {
            auto ns = handled_name.explicit_namespace()
                ? std::optional(e4x::E4XNamespace<GC>::new_uri(*handled_name.explicit_namespace()))
                : std::nullopt;
            
            auto new_attr = e4x::E4XNode<GC>::attribute(
                activation->gc(),
                ns,
                *local,
                attr_value,
                std::optional(node())
            );
            
            node()->add_attribute(new_attr);
        }
        return;
    }
    
    // Handle element
    auto is_valid_name = handled_name.local_name()
        ? e4x::is_xml_name(*handled_name.local_name())
        : false;
    
    if (!is_valid_name && !handled_name.is_any_name()) {
        return;
    }
    
    bool primitive_assign = !processed_value.as_object()->as_xml_list_object().has_value()
        && !processed_value.as_object()->as_xml_object().has_value()
        && !handled_name.is_any_name();
    
    auto index = node()->remove_matching_children(activation->gc(), &handled_name);
    
    if (index.has_value()) {
        auto [idx, node_val] = index.value();
        node()->insert_at(activation->gc(), idx, node_val);
    } else {
        auto idx = node()->length().value_or(0);
        node()->insert_at(activation->gc(), idx, e4x::E4XNode<GC>::dummy(activation->gc()));
        
        if (primitive_assign) {
            auto elem_node = e4x::E4XNode<GC>::element(
                activation->gc(),
                e4x::namespace_for_multiname(&handled_name, activation),
                handled_name.local_name().value_or(AvmString<GC>::empty()),
                std::optional(node())
            );
            node()->replace(idx, XmlObject<GC>::new_xml(elem_node, activation), activation);
        }
    }
    
    if (primitive_assign) {
        node()->remove_all_children_at(index.value_or(0).first, activation->gc());
        auto val = processed_value.coerce_to_string(activation);
        if (!val.is_empty()) {
            node()->replace(index.value_or(0).first, processed_value, activation);
        }
    } else {
        node()->replace(index.value_or(0).first, processed_value, activation);
    }
}

template<typename GC>
uint32_t XmlObject<GC>::get_next_enumerant(uint32_t last_index) {
    return last_index == 0 ? 1 : 0;
}

template<typename GC>
Value<GC> XmlObject<GC>::get_enumerant_value(uint32_t index) {
    if (index == 1) {
        return Value<GC>::from_object(Object<GC>(this));
    }
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> XmlObject<GC>::get_enumerant_name(uint32_t index) {
    if (index == 1) {
        return Value<GC>::from_int(0);
    }
    return Value<GC>::null();
}

template<typename GC>
bool XmlObject<GC>::delete_property_local(
    Activation<GC>* activation,
    const Multiname<GC>* name
) {
    auto handled_name = e4x::handle_input_multiname(name->clone(), activation);
    
    // Note: Flash doesn't actually throw for numeric indices
    if (auto local_name = handled_name.local_name()) {
        if (local_name->parse<unsigned int>().has_value()) {
            return true;
        }
    }
    
    // Handle attribute
    if (handled_name.is_attribute()) {
        node()->remove_attributes_matching(&handled_name);
        return true;
    }
    
    // Handle element
    node()->remove_children_matching(&handled_name);
    return true;
}

// XmlObjectWeak implementation
template<typename GC>
Object<GC> XmlObjectWeak<GC>::upgrade() const {
    if (auto data = weak_.upgrade()) {
        return Object<GC>(XmlObject<GC>(data));
    }
    return Object<GC>::null();
}

} // namespace object
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_XML_OBJECT_H
