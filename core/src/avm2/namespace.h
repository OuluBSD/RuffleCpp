// C++ translation of core/src/avm2/namespace.rs
// AVM2 Namespace representation

#ifndef RUFFLE_CORE_AVM2_NAMESPACE_H
#define RUFFLE_CORE_AVM2_NAMESPACE_H

#include <memory>
#include <string>
#include <optional>
#include <variant>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class AvmString;
    template<typename GC>
    class AvmAtom;
    class StringContext;

    namespace avm2 {
        enum class ApiVersion;
        template<typename GC>
        class Activation;
        template<typename GC>
        class TranslationUnit;
        template<typename GC>
        class Error;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Internal namespace data variants
template<typename GC>
struct NamespaceData {
    using AvmAtom = ruffle::core::AvmAtom<GC>;
    using ApiVersion = ruffle::core::avm2::ApiVersion;

    struct NamespaceVariant {
        AvmAtom name;
        ApiVersion version;
    };

    struct PackageInternalVariant {
        AvmAtom name;
    };

    struct ProtectedVariant {
        AvmAtom name;
    };

    struct ExplicitVariant {
        AvmAtom name;
    };

    struct StaticProtectedVariant {
        AvmAtom name;
    };

    struct PrivateVariant {
        AvmAtom name;
    };

    std::variant<
        NamespaceVariant,
        PackageInternalVariant,
        ProtectedVariant,
        ExplicitVariant,
        StaticProtectedVariant,
        PrivateVariant
    > data;

    // Helper accessors
    bool is_namespace() const {
        return std::holds_alternative<NamespaceVariant>(data);
    }

    bool is_package_internal() const {
        return std::holds_alternative<PackageInternalVariant>(data);
    }

    bool is_protected() const {
        return std::holds_alternative<ProtectedVariant>(data);
    }

    bool is_explicit() const {
        return std::holds_alternative<ExplicitVariant>(data);
    }

    bool is_static_protected() const {
        return std::holds_alternative<StaticProtectedVariant>(data);
    }

    bool is_private() const {
        return std::holds_alternative<PrivateVariant>(data);
    }
};

/// Represents the name of a namespace in AVM2.
template<typename GC>
class Namespace {
private:
    using AvmAtom = ruffle::core::AvmAtom<GC>;
    using AvmString = ruffle::core::AvmString<GC>;
    using ApiVersion = ruffle::core::avm2::ApiVersion;
    using NamespaceData = ruffle::core::avm2::NamespaceData<GC>;

    // `nullptr` represents the wildcard namespace `Namespace::any()`.
    std::shared_ptr<NamespaceData> data;

public:
    /// Create an empty/any namespace
    static Namespace any() {
        return Namespace(nullptr);
    }

    /// Create a package namespace
    static Namespace package(
        AvmString package_name,
        ApiVersion api_version,
        StringContext<GC>& context
    );

    /// Create an internal namespace
    static Namespace internal(
        AvmString package_name,
        StringContext<GC>& context
    );

    /// Check if this is the public namespace (empty name)
    bool is_public() const {
        if (!data) return false;
        if (!data->is_namespace()) return false;
        // Would need to check if name is empty
        return false; // Placeholder
    }

    /// Check if this is any namespace variant (ignoring specific type)
    bool is_public_ignoring_ns() const {
        return data && data->is_namespace();
    }

    /// Check if this is the wildcard namespace
    bool is_any() const {
        return data == nullptr;
    }

    /// Check if this is a private namespace
    bool is_private() const {
        return data && data->is_private();
    }

    /// Check if this is a namespace variant
    bool is_namespace() const {
        return data && data->is_namespace();
    }

    /// Get the URI as an optional AvmString
    std::optional<AvmString> as_uri_opt() const;

    /// Get the string value of this namespace
    AvmString as_uri(StringContext<GC>& context) const {
        auto opt = as_uri_opt();
        if (opt) {
            return *opt;
        }
        return context.empty();
    }

    /// Compares two namespaces, requiring that their versions match exactly.
    bool exact_version_match(const Namespace& other) const {
        if (data.get() == other.data.get()) {
            return true;
        }
        if (is_private() || other.is_private()) {
            return false;
        }
        // Would need proper data comparison
        return data == other.data;
    }

    /// Compares this namespace to another, considering version compatibility.
    bool matches_ns(const Namespace& other) const {
        if (exact_version_match(other)) {
            return true;
        }
        // Would need proper version comparison logic
        return false;
    }

    /// Check if this namespace matches the given API version
    bool matches_api_version(ApiVersion match_version) const;

private:
    explicit Namespace(std::shared_ptr<NamespaceData> d) : data(d) {}
};

/// Common namespaces used in the AVM.
template<typename GC>
class CommonNamespaces {
private:
    using Namespace = ruffle::core::avm2::Namespace<GC>;
    using ApiVersion = ruffle::core::avm2::ApiVersion;

    static constexpr size_t PUBLIC_LEN = static_cast<size_t>(ApiVersion::VM_INTERNAL) + 1;

    std::array<Namespace, PUBLIC_LEN> public_namespaces;
    Namespace as3;
    Namespace vector_internal;

public:
    CommonNamespaces(StringContext<GC>& context);

    /// The public namespace, versioned with `ApiVersion::AllVersions`.
    Namespace public_all() const {
        return public_namespaces[static_cast<size_t>(ApiVersion::AllVersions)];
    }

    /// The public namespace, versioned with `ApiVersion::VM_INTERNAL`.
    Namespace public_vm_internal() const {
        return public_namespaces[static_cast<size_t>(ApiVersion::VM_INTERNAL)];
    }

    /// Get public namespace for a specific version
    Namespace public_for(ApiVersion version) const {
        return public_namespaces[static_cast<size_t>(version)];
    }
};

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_NAMESPACE_H
