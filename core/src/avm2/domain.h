// C++ translation of core/src/avm2/domain.rs
//! Application Domains

#ifndef RUFFLE_CORE_AVM2_DOMAIN_H
#define RUFFLE_CORE_AVM2_DOMAIN_H

#include <memory>
#include <vector>
#include <optional>
#include <cstdint>

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
    class QName;
    template<typename GC>
    class Class;
    template<typename GC>
    class Script;
    template<typename GC>
    class ByteArrayObject;
    template<typename GC>
    class Avm2;
    template<typename GC>
    class PropertyMap;
    class UpdateContext;

    namespace gc_arena {
        template<typename T>
        class Gc;
        template<typename T>
        class GcWeak;
        template<typename T>
        class Lock;
        template<typename T>
        class OnceLock;
        template<typename T>
        class RefLock;
        class Mutation;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

using gc_arena::Gc;
using gc_arena::GcWeak;
using gc_arena::Lock;
using gc_arena::OnceLock;
using gc_arena::RefLock;
using gc_arena::Mutation;

/// Minimum domain memory length
constexpr size_t MIN_DOMAIN_MEMORY_LENGTH = 1024;

/// Forward declaration of Domain
template<typename GC>
class Domain;

/// Weak version of the Domain struct, used to mark children of a Domain
/// when the egui feature is enabled
template<typename GC>
class DomainWeak;

/// Internal data for Domain
template<typename GC>
struct DomainData {
    RefLock<PropertyMap<GC, Script<GC>>> defs;
    RefLock<PropertyMap<GC, Class<GC>>> classes;

    /// The parent domain.
    std::optional<Domain<GC>> parent;

    /// The bytearray used for storing domain memory
    Lock<std::optional<ByteArrayObject<GC>>> domain_memory;

    OnceLock<ByteArrayObject<GC>> default_domain_memory;

#ifdef EGUI_FEATURE
    /// All children of this domain. This is intended exclusively for
    /// use with `debug_ui`
    std::vector<DomainWeak<GC>> children;
#endif
};

/// Represents a set of scripts and movies that share traits across different
/// script-global scopes.
template<typename GC>
class Domain {
private:
    Gc<DomainData<GC>> data_;

    /// Get mutable reference to domain data
    DomainData<GC>& cell_mut(Mutation<GC>* mc) {
        return data_->write(mc);
    }

    /// Get immutable reference to domain data
    const DomainData<GC>& cell() const {
        return data_->read();
    }

public:
    /// Create a new domain with no parent.
    ///
    /// This is intended exclusively for creating the player globals domain,
    /// and stage domain, which are created before ByteArray is available.
    ///
    /// Note: the global domain will be created without valid domain memory.
    /// You must initialize domain memory later on after the ByteArray class is
    /// instantiated but before user code runs.
    static Domain uninitialized_domain(
        Mutation<GC>* mc,
        std::optional<Domain<GC>> parent
    ) {
        DomainData<GC> data;
        data.parent = parent;
        // domain_memory and default_domain_memory are left uninitialized

        Domain<GC> domain(Gc<DomainData<GC>>::new(mc, data));

#ifdef EGUI_FEATURE
        if (parent.has_value()) {
            parent->cell_mut(mc).children.push_back(DomainWeak<GC>(GcWeak<DomainData<GC>>::downgrade(domain.data_)));
        }
#endif

        return domain;
    }

    /// Create a new domain with a given parent.
    ///
    /// This function must not be called before the player globals have been
    /// fully allocated.
    static Domain movie_domain(
        UpdateContext<GC>* context,
        Domain<GC> parent
    ) {
        auto domain_memory = create_default_domain_memory(context);

        DomainData<GC> data;
        data.parent = Some(parent);
        data.domain_memory = Lock::new(Some(domain_memory));
        data.default_domain_memory = OnceLock::from(domain_memory);

        Domain<GC> domain(Gc<DomainData<GC>>::new(context->gc(), data));

#ifdef EGUI_FEATURE
        parent.cell_mut(context->gc()).children.push_back(
            DomainWeak<GC>(GcWeak<DomainData<GC>>::downgrade(domain.data_))
        );
#endif

        return domain;
    }

    /// Get the parent of this domain
    std::optional<Domain<GC>> parent_domain() const {
        return data_->parent;
    }

    /// Determine if something has been defined within the current domain (including parents)
    bool has_definition(QName<GC> name) const {
        if (cell().defs->read().contains_key(name)) {
            return true;
        }

        if (data_->parent.has_value()) {
            return data_->parent->has_definition(name);
        }

        return false;
    }

    /// Determine if a class has been defined within the current domain (including parents)
    bool has_class(QName<GC> name) const {
        if (cell().classes->read().contains_key(name)) {
            return true;
        }

        if (data_->parent.has_value()) {
            return data_->parent->has_class(name);
        }

        return false;
    }

    /// Resolve a Multiname and return the script that provided it.
    ///
    /// If a name does not exist or cannot be resolved, no script or name will
    /// be returned.
    std::optional<std::pair<QName<GC>, Script<GC>>> get_defining_script(
        const Multiname<GC>* multiname
    ) const {
        if (auto name = multiname->local_name()) {
            if (auto result = cell().defs->read().get_with_ns_for_multiname(multiname)) {
                auto qname = QName<GC>::new(result->namespace_, name);
                return std::make_pair(qname, result->script);
            }
        }

        if (data_->parent.has_value()) {
            return data_->parent->get_defining_script(multiname);
        }

        return std::nullopt;
    }

    /// Resolve a Multiname and return the script that provided it.
    ///
    /// If a name does not exist or cannot be resolved, an error will be thrown.
    Error<GC> find_defining_script(
        Activation<GC>* activation,
        const Multiname<GC>* multiname
    ) const {
        if (auto result = get_defining_script(multiname)) {
            return Error<GC>::ok(*result);
        }
        return make_error_1065(activation, multiname);
    }

    /// Retrieve a value from this domain.
    Value<GC> get_defined_value(
        Activation<GC>* activation,
        QName<GC> name
    ) const {
        auto result = find_defining_script(activation, &name.into());
        if (result.is_error()) {
            return Value<GC>::undefined();
        }

        auto [qname, script] = result.unwrap();
        auto globals = script.globals(activation->context());

        return Value<GC>::from(globals).get_property(&qname.into(), activation);
    }

    /// Retrieve a value from this domain, with special handling for 'Vector.<SomeType>'.
    /// This is used by `getQualifiedClassName`, `ApplicationDomain.getDefinition`, and `ApplicationDomain.hasDefinition`.
    Value<GC> get_defined_value_handling_vector(
        Activation<GC>* activation,
        AvmString<GC> name
    ) const {
        // Special-case lookups of `Vector.<SomeType>` - these get internally converted
        // to a lookup of `Vector`, a lookup of `SomeType`, and `vector_class.apply(some_type_class)`
        if (auto type_name = vector_parameter_from_name(activation->gc(), name)) {
            auto vector_class = activation->avm2()->classes().generic_vector;
            auto parameter_value = get_defined_value_handling_vector(activation, *type_name);

            return vector_class.apply(activation, {parameter_value}).into();
        }

        // If we're not hitting the special-case, just call `get_defined_value`
        auto qname = QName<GC>::from_qualified_name(name, activation->context());
        return get_defined_value(activation, qname);
    }

    bool has_defined_value_handling_vector(
        Activation<GC>* activation,
        AvmString<GC> name
    ) const {
        if (auto type_name = vector_parameter_from_name(activation->gc(), name)) {
            // avmplus just checks if the type parameter exists, so we do the same
            return has_defined_value_handling_vector(activation, *type_name);
        } else {
            auto qname = QName<GC>::from_qualified_name(name, activation->context());
            return get_defining_script(&qname.into()).has_value();
        }
    }

    std::vector<QName<GC>> get_defined_names() const {
        std::vector<QName<GC>> result;
        for (const auto& [name, namespace_, script] : cell().defs->read().iter()) {
            result.push_back(QName<GC>::new(namespace_, name));
        }
        return result;
    }

    /// Export a definition from a script into the current application domain.
    ///
    /// This does nothing if the definition already exists in this domain or a parent.
    void export_definition(
        QName<GC> name,
        Script<GC> script,
        Mutation<GC>* mc
    ) {
        if (has_definition(name)) {
            return;
        }

        cell_mut(mc).defs->write().insert(name, script);
    }

    /// Export a class into the current application domain.
    ///
    /// This does nothing if the definition already exists in this domain or a parent.
    void export_class(
        QName<GC> export_name,
        Class<GC> class_,
        Mutation<GC>* mc
    ) {
        if (has_class(export_name)) {
            return;
        }
        cell_mut(mc).classes->write().insert(export_name, class_);
    }

    bool is_default_domain_memory() const {
        auto domain_memory_ptr = domain_memory().as_ptr();
        auto default_domain_memory_ptr = data_->default_domain_memory.get()->as_ptr();
        return domain_memory_ptr == default_domain_memory_ptr;
    }

    ByteArrayObject<GC> domain_memory() const {
        return *data_->domain_memory.read();
    }

    void set_domain_memory(
        Activation<GC>* activation,
        std::optional<ByteArrayObject<GC>> domain_memory
    ) -> Result<void, Error<GC>> {
        auto memory = domain_memory.value_or(
            *data_->default_domain_memory.get()
        );

        if (memory.storage().len() < MIN_DOMAIN_MEMORY_LENGTH) {
            return make_error_1504(activation);
        }

        data_->write(mc).domain_memory.write().set(Some(memory));
        return Ok();
    }

    /// Allocate the default domain memory for this domain, if it does not
    /// already exist.
    ///
    /// This function is only necessary to be called for domains created via
    /// `global_domain`. It will panic on already fully-initialized domains.
    void init_default_domain_memory(UpdateContext<GC>* context) {
        auto memory = create_default_domain_memory(context);

        auto write = data_->write(context->gc());
        if (write.default_domain_memory.set(memory).is_err()) {
            panic("Already initialized domain memory!");
        }
        write.domain_memory.set(Some(memory));
    }

    const Domain<GC>* as_ptr() const {
        return Gc<DomainData<GC>>::as_ptr(data_);
    }

    bool operator==(const Domain<GC>& other) const {
        return Gc<DomainData<GC>>::as_ptr(data_) == Gc<DomainData<GC>>::as_ptr(other.data_);
    }

private:
    explicit Domain(Gc<DomainData<GC>> data) : data_(data) {}

    static ByteArrayObject<GC> create_default_domain_memory(UpdateContext<GC>* context) {
        std::vector<uint8_t> initial_data(MIN_DOMAIN_MEMORY_LENGTH, 0);
        auto storage = ByteArrayStorage::from_vec(context, initial_data);
        return ByteArrayObject<GC>::from_storage(context, storage);
    }
};

/// Given a class name such as `Vector.<int>`, returns the Vector type
/// parameter (`int`), or `None` if the class name does not represent a
/// parametrized Vector class (e.g. `flash.display::MovieClip`).
template<typename GC>
std::optional<AvmString<GC>> vector_parameter_from_name(
    Mutation<GC>* mc,
    AvmString<GC> name
) {
    using WStr = ruffle::core::WStr;

    if ((name.starts_with(WStr::from_units(b"__AS3__.vec::Vector.<")) ||
         name.starts_with(WStr::from_units(b"Vector.<"))) &&
        name.ends_with(WStr::from_units(b">"))) {

        if (auto start = name.find(WStr::from_units(b".<"))) {
            return Some(AvmString<GC>::new(
                mc,
                &name[*start + 2 .. name.len() - 1]
            ));
        }
    }

    return std::nullopt;
}

/// Weak reference to a Domain
template<typename GC>
class DomainWeak {
private:
    GcWeak<DomainData<GC>> data_;

public:
    explicit DomainWeak(GcWeak<DomainData<GC>> data) : data_(data) {}

    std::optional<Domain<GC>> upgrade(Mutation<GC>* mc) const {
        if (auto strong = GcWeak<DomainData<GC>>::upgrade(data_, mc)) {
            return Some(Domain<GC>(strong));
        }
        return std::nullopt;
    }
};

/// Tag type for Domain pointer
struct DomainPtr {};

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_DOMAIN_H
