// C++ translation of core/src/avm2/object/qname_object.rs
//! Boxed QNames

#ifndef RUFFLE_CORE_AVM2_OBJECT_QNAME_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_QNAME_OBJECT_H

#include <memory>
#include <optional>
#include <format>
#include <cell>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class AvmString;
    template<typename GC>
    class Activation;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    template<typename GC>
    class Multiname;
    template<typename GC>
    class Namespace;
    template<typename GC>
    class ScriptObjectData;
    template<typename GC>
    class TObject;
    class StringContext;

    namespace gc_arena {
        template<typename T>
        class Gc;
        template<typename T>
        class GcWeak;
        class Mutation;
        template<typename T>
        class RefLock;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

using gc_arena::Gc;
using gc_arena::GcWeak;
using gc_arena::Mutation;
using gc_arena::RefLock;

/// Weak reference to QNameObject data
template<typename GC>
class QNameObjectWeak;

/// Data structure for QNameObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class QNameObjectData {
public:
    QNameObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// The Multiname this object is associated with.
    RefLock<Multiname<GC>> name;

    // Friend classes for accessing private members
    friend class QNameObject<GC>;
    friend class QNameObjectWeak<GC>;
};

/// An Object which represents a boxed QName.
template<typename GC>
class QNameObject {
public:
    using DataType = QNameObjectData<GC>;
    using WeakType = QNameObjectWeak<GC>;

    QNameObject() = default;
    explicit QNameObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create an empty QName object
    /// @param activation The current activation
    /// @return New QNameObject with empty Multiname
    static QNameObject new_empty(Activation<GC>* activation);

    /// Box a Multiname into an object.
    /// @param activation The current activation
    /// @param name The Multiname to box
    /// @return New QNameObject containing the Multiname
    static QNameObject from_name(Activation<GC>* activation, Multiname<GC> name);

    /// Get the associated Multiname
    /// @return Reference to the Multiname
    const Multiname<GC>& name() const {
        return data_->name.get();
    }

    /// Set the namespace of the QName
    /// @param mc The mutation context for GC barrier
    /// @param namespace The new namespace
    void set_namespace(Mutation<GC>* mc, Namespace<GC> namespace_);

    /// Set the local name of the QName
    /// @param mc The mutation context for GC barrier
    /// @param local The new local name
    void set_local_name(Mutation<GC>* mc, AvmString<GC> local);

    /// Get the local name of the QName
    /// @param context The string context
    /// @return The local name, or "*" if not set
    AvmString<GC> local_name(StringContext<GC>* context) const;

    /// Set whether this is a QName (vs other QName-like types)
    /// @param mc The mutation context for GC barrier
    /// @param is_qname Whether this is a QName
    void set_is_qname(Mutation<GC>* mc, bool is_qname);

    /// Get the URI of the QName's namespace
    /// @param context The string context
    /// @return The URI, or nullopt if any namespace
    std::optional<AvmString<GC>> uri(StringContext<GC>* context) const;

    /// Check if this QName has any namespace
    /// @return true if the namespace is "any"
    bool is_any_namespace() const;

    /// Initialize the QName with a new Multiname
    /// @param mc The mutation context for GC barrier
    /// @param name The new Multiname
    void init_name(Mutation<GC>* mc, Multiname<GC> name);

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to QNameObject
template<typename GC>
class QNameObjectWeak {
public:
    QNameObjectWeak() = default;
    explicit QNameObjectWeak(GcWeak<GC, QNameObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, QNameObjectData<GC>> data_;
};

// Template method implementations

template<typename GC>
QNameObject<GC> QNameObject<GC>::new_empty(Activation<GC>* activation) {
    auto class_object = activation->avm2()->classes()->qname;
    auto base = ScriptObjectData<GC>::new_instance(class_object);

    auto data = Gc<GC, DataType>::create(
        activation->gc(),
        DataType{
            base,
            RefLock<Multiname<GC>>::new(Multiname<GC>::any())
        }
    );

    return QNameObject<GC>(data);
}

template<typename GC>
QNameObject<GC> QNameObject<GC>::from_name(Activation<GC>* activation, Multiname<GC> name) {
    auto class_object = activation->avm2()->classes()->qname;
    auto base = ScriptObjectData<GC>::new_instance(class_object);

    auto data = Gc<GC, DataType>::create(
        activation->gc(),
        DataType{
            base,
            RefLock<Multiname<GC>>::new(name)
        }
    );

    return QNameObject<GC>(data);
}

template<typename GC>
void QNameObject<GC>::set_namespace(Mutation<GC>* mc, Namespace<GC> namespace_) {
    auto& name_ref = const_cast<RefLock<Multiname<GC>>&>(data_->name);
    name_ref.borrow_mut().set_single_namespace(namespace_);
}

template<typename GC>
void QNameObject<GC>::set_local_name(Mutation<GC>* mc, AvmString<GC> local) {
    auto& name_ref = const_cast<RefLock<Multiname<GC>>&>(data_->name);
    name_ref.borrow_mut().set_local_name(local);
}

template<typename GC>
AvmString<GC> QNameObject<GC>::local_name(StringContext<GC>* context) const {
    const auto& name = data_->name.borrow();
    return name.local_name().value_or(context->empty());
}

template<typename GC>
void QNameObject<GC>::set_is_qname(Mutation<GC>* mc, bool is_qname) {
    auto& name_ref = const_cast<RefLock<Multiname<GC>>&>(data_->name);
    name_ref.borrow_mut().set_is_qname(is_qname);
}

template<typename GC>
std::optional<AvmString<GC>> QNameObject<GC>::uri(StringContext<GC>* context) const {
    const auto& name = data_->name.borrow();

    if (name.is_any_namespace()) {
        return std::nullopt;
    } else if (name.namespace_set().size() > 1) {
        return context->empty();
    } else {
        const auto& ns_set = name.namespace_set();
        if (!ns_set.empty()) {
            return ns_set[0].as_uri_opt();
        }
        return std::nullopt;
    }
}

template<typename GC>
bool QNameObject<GC>::is_any_namespace() const {
    return data_->name.borrow().is_any_namespace();
}

template<typename GC>
void QNameObject<GC>::init_name(Mutation<GC>* mc, Multiname<GC> name) {
    auto& name_ref = const_cast<RefLock<Multiname<GC>>&>(data_->name);
    *name_ref.borrow_mut() = name;
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::QNameObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::QNameObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "QNameObject(ptr={:p})",
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_QNAME_OBJECT_H
