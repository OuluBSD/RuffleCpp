// C++ translation of core/src/avm2/multiname.rs
//! Multiname - A name which could be resolved in one or more potential namespaces

#ifndef RUFFLE_CORE_AVM2_MULTINAME_H
#define RUFFLE_CORE_AVM2_MULTINAME_H

#include <vector>
#include <optional>
#include <memory>
#include <cstdint>
#include <variant>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC> class AvmString;
    template<typename GC> class WString;
    template<typename GC> class Mutation;
    template<typename GC> class StringContext;
    template<typename GC> class Gc;

    namespace avm2 {
        template<typename GC> class Activation;
        template<typename GC> class Error;
        template<typename GC> class Namespace;
        template<typename GC> class QName;
        template<typename GC> class Value;
        template<typename GC> class Object;
        struct TranslationUnit;

        namespace swf {
            namespace avm2 {
                namespace types {
                    template<typename T> struct Index;
                    struct Multiname;
                    struct NamespaceSet;
                }
            }
        }
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Namespace set - can be single or multiple namespaces
template<typename GC>
class NamespaceSet {
private:
    std::variant<Namespace<GC>, std::shared_ptr<std::vector<Namespace<GC>>>> data_;

public:
    NamespaceSet() = default;

    static NamespaceSet<GC> single(Namespace<GC> ns) {
        NamespaceSet<GC> result;
        result.data_ = ns;
        return result;
    }

    static NamespaceSet<GC> multiple(std::vector<Namespace<GC>> set, Mutation<GC>* mc) {
        NamespaceSet<GC> result;
        if (set.size() == 1) {
            result.data_ = set[0];
        } else {
            result.data_ = std::make_shared<std::vector<Namespace<GC>>>(std::move(set));
        }
        return result;
    }

    size_t len() const {
        if (std::holds_alternative<Namespace<GC>>(data_)) {
            return 1;
        } else {
            const auto& vec = std::get<std::shared_ptr<std::vector<Namespace<GC>>>>(data_);
            return vec->size();
        }
    }

    std::optional<Namespace<GC>> get(size_t index) const {
        if (std::holds_alternative<Namespace<GC>>(data_)) {
            if (index == 0) {
                return std::get<Namespace<GC>>(data_);
            }
            return std::nullopt;
        } else {
            const auto& vec = std::get<std::shared_ptr<std::vector<Namespace<GC>>>>(data_);
            if (index < vec->size()) {
                return (*vec)[index];
            }
            return std::nullopt;
        }
    }

    const std::vector<Namespace<GC>>& as_slice() const {
        static const std::vector<Namespace<GC>> empty;
        if (std::holds_alternative<Namespace<GC>>(data_)) {
            // Return a static single-element vector
            static thread_local std::vector<Namespace<GC>> single_vec;
            single_vec.clear();
            single_vec.push_back(std::get<Namespace<GC>>(data_));
            return single_vec;
        } else {
            return *std::get<std::shared_ptr<std::vector<Namespace<GC>>>>(data_);
        }
    }
};

/// Multiname flags
enum class MultinameFlags : uint8_t {
    None = 0,
    
    /// Whether the namespace needs to be read at runtime before use.
    /// This should only be set when lazy-initialized in Activation.
    HasLazyNs = 1 << 0,
    
    /// Whether the name needs to be read at runtime before use
    /// This should only be set when lazy-initialized in Activation.
    HasLazyName = 1 << 1,
    
    /// Whether this was a 'MultinameA' - used for XML attribute lookups
    Attribute = 1 << 2,

    /// Represents the XML concept of "qualified name".
    /// This also distinguishes a QName(x, y) from Multiname(x, [y])
    /// Basically, marks multinames that come from multinames of kind `(RT)QName(L)(A)`
    IsQname = 1 << 3,

    /// Whether this multiname was initially a Multiname, MultinameA,
    /// MultinameL, or MultinameLA.
    HasMultipleNs = 1 << 4
};

/// Combine MultinameFlags
inline MultinameFlags operator|(MultinameFlags a, MultinameFlags b) {
    return static_cast<MultinameFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline MultinameFlags operator&(MultinameFlags a, MultinameFlags b) {
    return static_cast<MultinameFlags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

inline MultinameFlags& operator|=(MultinameFlags& a, MultinameFlags b) {
    a = a | b;
    return a;
}

/// A `Multiname` consists of a name which could be resolved in one or more
/// potential namespaces.
///
/// All unresolved names are of the form `Multiname`, and the name resolution
/// process consists of searching each name space for a given name.
///
/// The existence of a `name` of `None` indicates the `Any` name.
template<typename GC>
class Multiname {
private:
    /// The list of namespaces that satisfy this multiname.
    NamespaceSet<GC> ns_;

    /// The local name that satisfies this multiname. If `None`, then this
    /// multiname is satisfied by any name in the namespace.
    std::optional<AvmString<GC>> name_;

    /// The type parameter required to satisfy this multiname. If None, then
    /// this multiname does not have a type parameter. If Some(None), then
    /// this multiname uses the Any type parameter (`*`).
    std::optional<std::optional<Gc<GC, Multiname<GC>>>> param_;

    MultinameFlags flags_;

public:
    Multiname() : flags_(MultinameFlags::None) {}

    Multiname(
        NamespaceSet<GC> ns,
        std::optional<AvmString<GC>> name,
        std::optional<std::optional<Gc<GC, Multiname<GC>>>> param,
        MultinameFlags flags
    ) : ns_(ns), name_(name), param_(param), flags_(flags) {}

    /// Check if has lazy namespace
    bool has_lazy_ns() const {
        return (flags_ & MultinameFlags::HasLazyNs) != MultinameFlags::None;
    }

    /// Check if has lazy name
    bool has_lazy_name() const {
        return (flags_ & MultinameFlags::HasLazyName) != MultinameFlags::None;
    }

    /// Check if has lazy component
    bool has_lazy_component() const {
        return has_lazy_ns() || has_lazy_name();
    }

    /// Check if is attribute
    bool is_attribute() const {
        return (flags_ & MultinameFlags::Attribute) != MultinameFlags::None;
    }

    /// Set is attribute flag
    void set_is_attribute(bool is_attribute) {
        if (is_attribute) {
            flags_ |= MultinameFlags::Attribute;
        } else {
            flags_ = flags_ & ~MultinameFlags::Attribute;
        }
    }

    /// Check if is qname
    bool is_qname() const {
        return (flags_ & MultinameFlags::IsQname) != MultinameFlags::None;
    }

    /// Set is qname flag
    void set_is_qname(bool is_qname) {
        if (is_qname) {
            flags_ |= MultinameFlags::IsQname;
        } else {
            flags_ = flags_ & ~MultinameFlags::IsQname;
        }
    }

    /// Check if has multiple namespaces
    bool has_multiple_ns() const {
        return (flags_ & MultinameFlags::HasMultipleNs) != MultinameFlags::None;
    }

    /// Read a namespace set from the ABC constant pool
    static std::variant<NamespaceSet<GC>, Error<GC>> abc_namespace_set(
        Activation<GC>* activation,
        TranslationUnit<GC> translation_unit,
        swf::avm2::types::Index<swf::avm2::types::NamespaceSet> namespace_set_index
    );

    /// Create from ABC index
    static std::variant<Multiname<GC>, Error<GC>> from_abc_index(
        Activation<GC>* activation,
        TranslationUnit<GC> translation_unit,
        swf::avm2::types::Index<swf::avm2::types::Multiname> multiname_index
    );

    /// Fill with runtime parameters
    std::variant<Multiname<GC>, Error<GC>> fill_with_runtime_params(
        Activation<GC>* activation
    ) const;

    /// Indicates the any type (any name in any namespace).
    static Multiname<GC> any() {
        return Multiname<GC>(
            NamespaceSet<GC>::single(Namespace<GC>::any()),
            std::nullopt,
            std::nullopt,
            MultinameFlags::None
        );
    }

    /// Indicates the any attribute type (any attribute in any namespace).
    static Multiname<GC> any_attribute() {
        return Multiname<GC>(
            NamespaceSet<GC>::single(Namespace<GC>::any()),
            std::nullopt,
            std::nullopt,
            MultinameFlags::Attribute
        );
    }

    /// Create a new Multiname
    static Multiname<GC> new_(Namespace<GC> ns, AvmString<GC> name) {
        return Multiname<GC>(
            NamespaceSet<GC>::single(ns),
            name,
            std::nullopt,
            MultinameFlags::None
        );
    }

    /// Creates a new Multiname with the `MultinameFlags::ATTRIBUTE` flag.
    static Multiname<GC> attribute(Namespace<GC> ns, AvmString<GC> name) {
        return Multiname<GC>(
            NamespaceSet<GC>::single(ns),
            name,
            std::nullopt,
            MultinameFlags::Attribute
        );
    }

    /// Get namespace set
    const std::vector<Namespace<GC>>& namespace_set() const {
        return ns_.as_slice();
    }

    /// Get local name
    std::optional<AvmString<GC>> local_name() const {
        return name_;
    }

    /// Check if contains public namespace
    bool contains_public_namespace() const {
        const auto& ns_vec = namespace_set();
        for (const auto& ns : ns_vec) {
            if (ns.is_public()) {
                return true;
            }
        }
        return false;
    }

    /// Check if has explicit namespace
    bool has_explicit_namespace() const {
        if (ns_.len() == 1) {
            auto ns = ns_.get(0);
            if (ns.has_value()) {
                return ns->is_namespace() && !ns->is_public();
            }
        }
        return false;
    }

    /// Check if has non-empty namespace
    bool has_nonempty_namespace() const {
        if (ns_.len() == 1) {
            auto ns = ns_.get(0);
            if (ns.has_value()) {
                return !ns->is_public();
            }
        }
        return ns_.len() > 1;
    }

    /// Get explicit namespace
    std::optional<AvmString<GC>> explicit_namespace() const {
        if (ns_.len() == 1) {
            auto ns = ns_.get(0);
            if (ns.has_value() && ns->is_namespace() && !ns->is_public()) {
                return ns->as_uri_opt();
            }
        }
        return std::nullopt;
    }

    /// Indicates if this multiname matches any type.
    bool is_any_name() const {
        return !name_.has_value();
    }

    /// Indicates if this multiname matches any namespace.
    bool is_any_namespace() const {
        if (ns_.len() == 1) {
            auto ns = ns_.get(0);
            if (ns.has_value()) {
                return ns->is_any();
            }
        }
        // NamespaceSet::Multiple should not have any Any namespaces in it
        return false;
    }

    /// Determine if this multiname matches a given QName.
    bool contains_name(const QName<GC>& name) const {
        bool ns_match = false;
        const auto& ns_vec = namespace_set();
        for (const auto& ns : ns_vec) {
            if (ns.is_any() || ns.matches_ns(name.namespace_())) {
                ns_match = true;
                break;
            }
        }

        bool name_match = true;
        if (name_.has_value()) {
            name_match = name_.value() == name.local_name();
        }

        return ns_match && name_match;
    }

    /// Whether this multiname is valid for dynamic lookups, such as `array[3]`.
    bool valid_dynamic_name() const {
        return contains_public_namespace() && !is_attribute();
    }

    /// List the parameters that the selected class must match.
    std::optional<std::optional<Gc<GC, Multiname<GC>>>> param() const {
        return param_;
    }

    /// Convert to qualified name
    AvmString<GC> to_qualified_name(Mutation<GC>* mc) const;

    /// Like `to_qualified_name`, but returns `*` if `is_any()` is true.
    /// This is used by `describeType`
    AvmString<GC> to_qualified_name_or_star(StringContext<GC>* context) const;

    /// Convert to URI format
    AvmString<GC> as_uri(StringContext<GC>* context) const;

    /// Set namespace set
    void set_ns(NamespaceSet<GC> ns) {
        ns_ = std::move(ns);
    }

    /// Set single namespace
    void set_single_namespace(Namespace<GC> namespace_) {
        ns_ = NamespaceSet<GC>::single(namespace_);
    }

    /// Set local name
    void set_local_name(AvmString<GC> name) {
        name_ = name;
    }

    /// Convert from QName
    static Multiname<GC> from_qname(QName<GC> q) {
        return Multiname<GC>(
            NamespaceSet<GC>::single(q.namespace_()),
            q.local_name(),
            std::nullopt,
            MultinameFlags::None
        );
    }
};

// Template method implementations

template<typename GC>
std::variant<NamespaceSet<GC>, Error<GC>> Multiname<GC>::abc_namespace_set(
    Activation<GC>* activation,
    TranslationUnit<GC> translation_unit,
    swf::avm2::types::Index<swf::avm2::types::NamespaceSet> namespace_set_index
) {
    // Placeholder implementation
    // Real implementation would read from ABC constant pool
    return Error<GC>();
}

template<typename GC>
std::variant<Multiname<GC>, Error<GC>> Multiname<GC>::from_abc_index(
    Activation<GC>* activation,
    TranslationUnit<GC> translation_unit,
    swf::avm2::types::Index<swf::avm2::types::Multiname> multiname_index
) {
    // Placeholder implementation
    // Real implementation would read from ABC constant pool and construct Multiname
    return Error<GC>();
}

template<typename GC>
std::variant<Multiname<GC>, Error<GC>> Multiname<GC>::fill_with_runtime_params(
    Activation<GC>* activation
) const {
    // Placeholder implementation
    // Real implementation would pop values from stack and fill in lazy components
    return Error<GC>();
}

template<typename GC>
AvmString<GC> Multiname<GC>::to_qualified_name(Mutation<GC>* mc) const {
    WString<GC> uri;
    
    std::optional<AvmString<GC>> ns_str;
    if (ns_.len() == 1) {
        auto ns = ns_.get(0);
        if (ns.has_value()) {
            if (ns->is_any()) {
                ns_str = AvmString<GC>::from_units(b"*");
            } else {
                ns_str = ns->as_uri_opt();
            }
        }
    }

    if (ns_str.has_value() && !ns_str->is_empty()) {
        uri.push_str(&ns_str->as_wstr());
    }

    if (name_.has_value()) {
        if (!uri.is_empty()) {
            uri.push_str(WStr<GC>::from_units(b"::"));
        } else if (!param_.has_value()) {
            // Special-case this to avoid allocating.
            return name_.value();
        }
        uri.push_str(&name_->as_wstr());
    } else {
        uri.push_str(WStr<GC>::from_units(b"::*"));
    }

    if (param_.has_value()) {
        uri.push_str(WStr<GC>::from_units(b".<"));
        if (param_.value().has_value()) {
            uri.push_str(&param_.value().value()->to_qualified_name(mc)->as_wstr());
        } else {
            uri.push_str(WStr<GC>::from_units(b"*"));
        }
        uri.push_str(WStr<GC>::from_units(b">"));
    }

    return AvmString<GC>::new_(mc, uri);
}

template<typename GC>
AvmString<GC> Multiname<GC>::to_qualified_name_or_star(StringContext<GC>* context) const {
    if (is_any_name()) {
        return AvmString<GC>::from_units(b"*");
    }
    return to_qualified_name(context->gc());
}

template<typename GC>
AvmString<GC> Multiname<GC>::as_uri(StringContext<GC>* context) const {
    std::optional<AvmString<GC>> ns_str;
    if (ns_.len() == 1) {
        auto ns = ns_.get(0);
        if (ns.has_value()) {
            if (ns->is_any()) {
                ns_str = AvmString<GC>::from_units(b"*");
            } else {
                ns_str = ns->as_uri_opt();
            }
        }
    }

    if (!ns_str.has_value() || ns_str->is_empty()) {
        // Special-case this to avoid allocating.
        return name_.value_or(AvmString<GC>::from_units(b"*"));
    }

    WString<GC> uri = WString<GC>::from(&ns_str->as_wstr());
    uri.push_str(WStr<GC>::from_units(b"::"));
    
    if (name_.has_value()) {
        uri.push_str(&name_->as_wstr());
    } else {
        uri.push_str(WStr<GC>::from_units(b"*"));
    }

    return AvmString<GC>::new_(context->gc(), uri);
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_MULTINAME_H
