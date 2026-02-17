// C++ translation of core/src/avm2/qname.rs
//! Qualified name

#ifndef RUFFLE_CORE_AVM2_QNAME_H
#define RUFFLE_CORE_AVM2_QNAME_H

#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <format>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC> class AvmString;
    template<typename GC> class WStr;
    template<typename GC> class WString;
    template<typename GC> class Mutation;
    template<typename GC> class UpdateContext;
    template<typename GC> class StringContext;

    namespace avm2 {
        template<typename GC> class Activation;
        template<typename GC> class Error;
        template<typename GC> class Namespace;
        template<typename GC> class Multiname;
        struct TranslationUnit;

        namespace api_version {
            enum class ApiVersion;
        }
    }

    namespace string {
        template<typename GC> using AvmString = ruffle::core::AvmString<GC>;
        template<typename GC> using WStr = ruffle::core::WStr<GC>;
        template<typename GC> using WString = ruffle::core::WString<GC>;
    }
}
}

namespace swf {
namespace avm2 {
namespace types {
    template<typename T>
    struct Index {
        uint32_t index;
    };
    struct Multiname;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Either type for returning different types
template<typename L, typename R>
class Either {
private:
    std::variant<L, R> data_;
    bool is_left_;

public:
    static Either<L, R> left(L value) {
        Either<L, R> result;
        result.data_ = std::move(value);
        result.is_left_ = true;
        return result;
    }

    static Either<L, R> right(R value) {
        Either<L, R> result;
        result.data_ = std::move(value);
        result.is_left_ = false;
        return result;
    }

    bool is_left() const { return is_left_; }
    bool is_right() const { return !is_left_; }

    const L& left() const { return std::get<L>(data_); }
    const R& right() const { return std::get<R>(data_); }
};

/// Qualified name.
/// NOTE: this struct doesn't actually directly correspond to an AVM2 QName concept.
/// Currently, we mostly use this struct simply to wrap a name+namespace pair,
/// and try to reduce its future use if possible.
template<typename GC>
class QName {
private:
    Namespace<GC> ns_;
    AvmString<GC> name_;

public:
    QName() = default;

    QName(Namespace<GC> ns, AvmString<GC> name)
        : ns_(ns), name_(name) {}

    /// Create a new QName
    static QName<GC> new_(Namespace<GC> ns, AvmString<GC> name) {
        return QName<GC>(ns, name);
    }

    /// Pull a `QName` from the multiname pool.
    ///
    /// This function returns an Err if the multiname is not a `QName`.
    static std::variant<QName<GC>, Error<GC>> from_abc_multiname(
        Activation<GC>* activation,
        TranslationUnit<GC> translation_unit,
        swf::avm2::types::Index<swf::avm2::types::Multiname> multiname_index
    );

    /// Constructs a `QName` from a fully qualified name.
    ///
    /// A fully qualified name can be any of the following formats:
    /// NAMESPACE::LOCAL_NAME
    /// NAMESPACE.LOCAL_NAME (Where the LAST dot is used to split the namespace & local_name)
    /// LOCAL_NAME (Use the public namespace)
    ///
    /// If no namespace is specified, the public namespace with the highest version
    /// will be used. Otherwise, the namespace with the root API version will be
    /// used.
    ///
    /// This does *not* handle `Vector.<SomeTypeParam>` - use `get_defined_value_handling_vector` for that
    static QName<GC> from_qualified_name(
        AvmString<GC> name,
        UpdateContext<GC>* context
    );

    /// Converts this `QName` to a fully qualified name.
    AvmString<GC> to_qualified_name(Mutation<GC>* mc) const {
        auto result = to_qualified_name_no_mc();
        if (result.is_left()) {
            return result.left();
        } else {
            return AvmString<GC>::new_(mc, result.right());
        }
    }

    /// Like `to_qualified_name`, but avoids the need for a `Mutation`
    /// by returning `Either::Right(wstring)` when it would otherwise
    /// be necessary to allocate a new `AvmString`.
    ///
    /// This method is intended for contexts like `Debug` impls where
    /// a `Mutation` is not available. Normally, you should
    /// use `to_qualified_name`
    Either<AvmString<GC>, WString<GC>> to_qualified_name_no_mc() const {
        auto name = local_name();
        auto ns_uri = namespace_().as_uri_opt();

        if (ns_uri.has_value() && !ns_uri->is_empty()) {
            WString<GC> buf = WString<GC>::from(*ns_uri);
            buf.push_str(WStr<GC>::from_units(b"::"));
            buf.push_str(&name);
            return Either<AvmString<GC>, WString<GC>>::right(std::move(buf));
        } else {
            return Either<AvmString<GC>, WString<GC>>::left(name);
        }
    }

    /// Like `to_qualified_name`, but uses a `.` instead of `::` separate
    /// the namespace and local name. This matches the output produced by
    /// Flash Player in error messages
    AvmString<GC> to_qualified_name_err_message(Mutation<GC>* mc) const {
        auto name = local_name();
        auto ns_uri = namespace_().as_uri_opt();

        if (ns_uri.has_value() && !ns_uri->is_empty()) {
            WString<GC> buf = WString<GC>::from(*ns_uri);
            buf.push_char(u'.');
            buf.push_str(&name);
            return AvmString<GC>::new_(mc, buf);
        } else {
            return name;
        }
    }

    /// Get the local name
    AvmString<GC> local_name() const {
        return name_;
    }

    /// Get the namespace
    Namespace<GC> namespace_() const {
        return ns_;
    }

    /// Get the string value of this QName, including the namespace URI.
    AvmString<GC> as_uri(Mutation<GC>* mc) const {
        auto ns_uri = ns_.as_uri_opt();
        
        if (!ns_uri.has_value() || ns_uri->is_empty()) {
            return name_;
        }

        WString<GC> uri = WString<GC>::from(*ns_uri);
        uri.push_str(WStr<GC>::from_units(b"::"));
        uri.push_str(&name_);
        return AvmString<GC>::new_(mc, uri);
    }

    /// Equality operator
    bool operator==(const QName<GC>& other) const {
        // Implemented by hand to enforce order of comparisons for perf
        return name_ == other.name_ && ns_.exact_version_match(other.ns_);
    }

    bool operator!=(const QName<GC>& other) const {
        return !(*this == other);
    }
};

// Template method implementations

template<typename GC>
std::variant<QName<GC>, Error<GC>> QName<GC>::from_abc_multiname(
    Activation<GC>* activation,
    TranslationUnit<GC> translation_unit,
    swf::avm2::types::Index<swf::avm2::types::Multiname> multiname_index
) {
    // This would call Multiname::from_abc_index in real implementation
    // For now, return a placeholder
    return Error<GC>();
}

template<typename GC>
QName<GC> QName<GC>::from_qualified_name(
    AvmString<GC> name,
    UpdateContext<GC>* context
) {
    // Try to split by :: or .
    // This is a simplified implementation
    auto parts = name.rsplit_once(WStr<GC>::from_units(b"::"));
    if (!parts.has_value()) {
        parts = name.rsplit_once(WStr<GC>::from_units(b"."));
    }

    if (parts.has_value()) {
        auto [package_name, local_name] = parts.value();
        auto interned_package = context->strings().intern_wstr(package_name);
        auto api_version = context->avm2().root_api_version();

        return QName<GC>(
            Namespace<GC>::package(interned_package, api_version, &context->strings()),
            AvmString<GC>::new_(context->gc(), local_name)
        );
    } else {
        return QName<GC>(
            context->avm2().namespaces().public_for(api_version::ApiVersion::VM_INTERNAL),
            name
        );
    }
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::QName<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::QName<GC>& qname, FormatContext& ctx) {
        auto result = qname.to_qualified_name_no_mc();
        if (result.is_left()) {
            return std::format_to(ctx.out(), "{}", result.left().to_utf8_lossy());
        } else {
            return std::format_to(ctx.out(), "{}", result.right().to_utf8_lossy());
        }
    }
};

#endif // RUFFLE_CORE_AVM2_QNAME_H
