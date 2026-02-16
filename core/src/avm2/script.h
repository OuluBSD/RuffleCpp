// C++ translation of core/src/avm2/script.rs
//! Whole script representation

#ifndef RUFFLE_CORE_AVM2_SCRIPT_H
#define RUFFLE_CORE_AVM2_SCRIPT_H

#include <cstdint>
#include <vector>
#include <memory>
#include <optional>
#include <cell>

// Forward declarations
namespace ruffle {
namespace core {
    class UpdateContext;
    class AvmString;
    class SwfMovie;

    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class Value;
        template<typename T> class Error;
        template<typename T> class Class;
        template<typename T> class Method;
        template<typename T> class Object;
        template<typename T> class ScriptObject;
        template<typename T> class Domain;
        template<typename T> class ScopeChain;
        template<typename T> class VTable;
        template<typename T> class Multiname;
        template<typename T> class Namespace;
        template<typename T> class Avm2;
        template<typename T> class AvmAtom;

        namespace api_version {
            enum class ApiVersion;
        }
        using api_version::ApiVersion;

        enum class PlayerRuntime;
    }
}
}

// Placeholder for swf types
namespace swf {
namespace avm2 {
namespace types {
    class AbcFile;
    struct Method;
    struct Multiname;
    struct Namespace;
    struct Script;

    template<typename T>
    struct Index {
        uint32_t value;
    };
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

// Forward declaration
template<typename GC>
class TranslationUnit;

/// Data for TranslationUnit
/// Stores all loaded ABC items (classes, methods, scripts, etc.)
template<typename GC>
class TranslationUnitData {
public:
    /// The domain that all scripts export defs to
    Domain<GC> domain;

    /// The name from the original DoAbc2 tag, or std::nullopt if from DoAbc tag
    std::optional<AvmString<GC>> name;

    /// The ABC file that all data comes from
    std::shared_ptr<swf::avm2::types::AbcFile> abc;

    /// All classes loaded from the ABC's class list (lazy-loaded)
    std::vector<std::optional<Class<GC>>> classes;

    /// All methods loaded from the ABC's method list (lazy-loaded)
    std::vector<std::optional<Method<GC>>> methods;

    /// All scripts loaded from the ABC's scripts list (lazy-loaded)
    std::vector<std::optional<Script<GC>>> scripts;

    /// All strings loaded from the ABC's strings list (lazy-loaded, offset by 1)
    std::vector<std::optional<AvmAtom<GC>>> strings;

    /// All namespaces loaded from the ABC's namespace list (lazy-loaded, offset by 1)
    std::vector<std::optional<Namespace<GC>>> namespaces;

    /// All multinames loaded from the ABC's multiname list (lazy-loaded, offset by 1)
    std::vector<std::optional<Multiname<GC>>> multinames;

    /// The movie that this TranslationUnit was loaded from
    std::shared_ptr<SwfMovie> movie;
};

/// A loaded ABC file, with any loaded ABC items alongside it
///
/// A TranslationUnit is constructed when ABC loading begins, and it stores
/// all loaded ABC items (classes, methods, and scripts) as they are loaded.
/// Unit items are loaded lazily and retained for later retrieval.
template<typename GC>
class TranslationUnit {
public:
    /// Construct a new TranslationUnit for a given ABC file
    ///
    /// \param abc The ABC file to load
    /// \param domain The domain to execute within
    /// \param name The name from DoAbc2 tag, or std::nullopt
    /// \param movie The movie this unit belongs to
    static TranslationUnit from_abc(
        swf::avm2::types::AbcFile abc,
        Domain<GC> domain,
        std::optional<AvmString<GC>> name,
        std::shared_ptr<SwfMovie> movie
    );

    /// Load all classes from the ABC file
    void load_classes(Activation<GC>* activation);

    /// Manually set a loaded class in this TranslationUnit
    void set_class(size_t index, Class<GC> class_);

    /// Get the domain associated with this unit
    Domain<GC> domain() const { return domain_; }

    /// Get the name from DoAbc2 tag
    std::optional<AvmString<GC>> name() const { return name_; }

    /// Get the underlying ABC file
    std::shared_ptr<swf::avm2::types::AbcFile> abc() const { return abc_; }

    /// Get the movie this unit belongs to
    std::shared_ptr<SwfMovie> movie() const { return movie_; }

    /// Get the API version for this unit
    ApiVersion api_version(Avm2<GC>* avm2) const;

    /// Load a method from the ABC file
    Method<GC> load_method(
        swf::avm2::types::Index<swf::avm2::types::Method> method_index,
        bool is_function,
        Activation<GC>* activation
    );

    /// Load a class from the ABC file
    Class<GC> load_class(uint32_t class_index, Activation<GC>* activation);

    /// Load a script from the ABC file
    Script<GC> load_script(uint32_t script_index, Activation<GC>* activation);

    /// Get a script by index (if already loaded)
    std::optional<Script<GC>> get_script(size_t index) const;

    /// Load a string from the ABC's constant pool
    /// Returns std::nullopt for string index 0
    std::optional<AvmAtom<GC>> pool_string_option(
        uint32_t string_index
    );

    /// Load a string from the ABC's constant pool
    /// String index 0 returns the empty string
    AvmAtom<GC> pool_string(uint32_t string_index);

    /// Retrieve a namespace from the constant pool
    /// Index 0 is treated as an error
    Namespace<GC> pool_namespace(
        Activation<GC>* activation,
        swf::avm2::types::Index<swf::avm2::types::Namespace> ns_index
    );

    /// Retrieve a multiname from the constant pool
    /// The name can have a lazy component
    Multiname<GC> pool_maybe_uninitialized_multiname(
        Activation<GC>* activation,
        swf::avm2::types::Index<swf::avm2::types::Multiname> multiname_index
    );

    /// Retrieve a static multiname from the constant pool
    /// Index 0 is treated as an error
    Multiname<GC> pool_multiname_static(
        Activation<GC>* activation,
        swf::avm2::types::Index<swf::avm2::types::Multiname> multiname_index
    );

    /// Retrieve a static multiname, returning std::nullopt for index 0
    std::optional<Multiname<GC>> pool_multiname_static_any(
        Activation<GC>* activation,
        swf::avm2::types::Index<swf::avm2::types::Multiname> multiname_index
    );

private:
    explicit TranslationUnit(TranslationUnitData<GC> data)
        : data_(std::make_shared<TranslationUnitData<GC>>(data)) {}

    std::shared_ptr<TranslationUnitData<GC>> data_;
};

/// A loaded Script from an ABC file
template<typename GC>
class Script {
public:
    /// Construct a script from a TranslationUnit and its script index
    static Script from_abc_index(
        TranslationUnit<GC> unit,
        uint32_t script_index,
        Domain<GC> domain,
        Activation<GC>* activation
    );

    /// Return the entrypoint for the script and the scope it should run in
    std::tuple<Method<GC>, Object<GC>, Domain<GC>> init() const;

    /// Get the domain associated with this script
    Domain<GC> domain() const { return domain_; }

    /// Get the TranslationUnit this script was loaded from
    TranslationUnit<GC> translation_unit() const { return translation_unit_; }

    /// Get the global class for this script
    Class<GC> global_class() const;

    /// Return the global scope for the script
    /// Initializes the script if not already initialized
    Object<GC> globals(UpdateContext* context);

private:
    /// Create the globals object for a script
    static Object<GC> create_globals_object(
        TranslationUnit<GC> unit,
        const swf::avm2::types::Script& script,
        Domain<GC> domain,
        Method<GC> init_method,
        Activation<GC>* activation
    );

    Script(
        Object<GC> globals,
        Domain<GC> domain,
        Method<GC> init,
        TranslationUnit<GC> translation_unit
    )
        : globals_(globals)
        , domain_(domain)
        , init_(init)
        , initialized_(false)
        , translation_unit_(translation_unit)
    {}

    Object<GC> globals_;
    Domain<GC> domain_;
    Method<GC> init_;
    bool initialized_;
    TranslationUnit<GC> translation_unit_;
};

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_SCRIPT_H
