// C++ translation of core/src/avm2/globals/flash/system/application_domain.rs
//! flash.system.ApplicationDomain class

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_APPLICATION_DOMAIN_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_APPLICATION_DOMAIN_H

#include <vector>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class DomainObject;
    template<typename T> class VectorObject;
    template<typename T> class AvmString;
    template<typename T> class ByteArrayObject;
    namespace parameters {
        class ParametersExt;
    }
    namespace vector {
        template<typename T> class VectorStorage;
    }
    class Domain;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace system {
namespace application_domain {

using parameters::ParametersExt;
using vector::VectorStorage;

/// Implements flash.system.ApplicationDomain's init method, which
/// is called from the constructor
///
/// @param activation The current activation
/// @param this_value The ApplicationDomain object being initialized
/// @param args Constructor arguments (optional parent domain)
/// @return Undefined value or error
template<typename GC>
Value<GC> init(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements ApplicationDomain.currentDomain static property getter
///
/// Returns the current application domain
///
/// @param activation The current activation
/// @param this_value Unused (static property)
/// @param args Unused
/// @return The current application domain as DomainObject
template<typename GC>
Value<GC> get_current_domain(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements ApplicationDomain.parentDomain property getter
///
/// Returns the parent domain, or null if this is the playerglobals domain
///
/// @param activation The current activation
/// @param this_value The ApplicationDomain object
/// @param args Unused
/// @return The parent domain as DomainObject, or null
template<typename GC>
Value<GC> get_parent_domain(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements ApplicationDomain.getDefinition method
///
/// Gets a definition by name from the application domain
///
/// @param activation The current activation
/// @param this_value The ApplicationDomain object
/// @param args Method arguments (definitionName: String)
/// @return The definition value, or Undefined
template<typename GC>
Value<GC> get_definition(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements ApplicationDomain.hasDefinition method
///
/// Checks if a definition exists in the application domain
///
/// @param activation The current activation
/// @param this_value The ApplicationDomain object
/// @param args Method arguments (definitionName: String)
/// @return true if the definition exists, false otherwise
template<typename GC>
Value<GC> has_definition(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements ApplicationDomain.getQualifiedDefinitionNames method
///
/// Returns an array of qualified names of all definitions in the domain.
/// NOTE: Normally only available in Flash Player 11.3+.
///
/// @param activation The current activation
/// @param this_value The ApplicationDomain object
/// @param args Unused
/// @return Vector of qualified definition names
template<typename GC>
Value<GC> get_qualified_definition_names(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements ApplicationDomain.domainMemory property setter
///
/// Sets the domain's memory (ByteArray)
///
/// @param activation The current activation
/// @param this_value The ApplicationDomain object
/// @param args Method arguments (ByteArray or null)
/// @return Undefined value or error
template<typename GC>
Value<GC> set_domain_memory(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements ApplicationDomain.domainMemory property getter
///
/// Gets the domain's memory (ByteArray)
///
/// @param activation The current activation
/// @param this_value The ApplicationDomain object
/// @param args Unused
/// @return The domain memory as ByteArray, or null if default
template<typename GC>
Value<GC> get_domain_memory(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace application_domain
} // namespace system
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

// Template method implementations

template<typename GC>
Value<GC> application_domain::init(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_value.as_object();
    if (!this_obj.has_value()) {
        return Value<GC>::undefined();
    }

    auto parent_domain = [&]() -> Domain<GC> {
        if (auto domain_obj = args.size() > 0 ? args[0].as_object() : std::optional<Object<GC>>{}) {
            if (domain_obj.has_value()) {
                return domain_obj->as_application_domain()
                    .expect("Invalid parent domain");
            }
        }
        return activation->avm2()->playerglobals_domain();
    }();

    auto target_domain = this_obj->as_application_domain()
        .expect("Invalid target domain");

    auto fresh_domain = Domain<GC>::movie_domain(activation->context, parent_domain);
    target_domain.init_domain(activation->gc(), fresh_domain);

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> application_domain::get_current_domain(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
) {
    auto caller_domain = activation->caller_domain()
        .expect("Missing caller domain in ApplicationDomain.currentDomain");

    return Value<GC>::from_object(DomainObject<GC>::from_domain(activation, caller_domain));
}

template<typename GC>
Value<GC> application_domain::get_parent_domain(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_value.as_object();
    if (!this_obj.has_value()) {
        return Value<GC>::null();
    }

    if (auto appdomain = this_obj->as_application_domain()) {
        if (auto parent_domain = appdomain->parent_domain()) {
            if (parent_domain->is_playerglobals_domain(activation->avm2())) {
                return Value<GC>::null();
            }
            return Value<GC>::from_object(DomainObject<GC>::from_domain(activation, parent_domain));
        }
    }

    return Value<GC>::null();
}

template<typename GC>
Value<GC> application_domain::get_definition(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_value.as_object();
    if (!this_obj.has_value()) {
        return Value<GC>::undefined();
    }

    if (auto appdomain = this_obj->as_application_domain()) {
        auto name = ParametersExt::get_string_non_null(activation, args, 0, "definitionName");
        return appdomain->get_defined_value_handling_vector(activation, name);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> application_domain::has_definition(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_value.as_object();
    if (!this_obj.has_value()) {
        return Value<GC>::undefined();
    }

    if (auto appdomain = this_obj->as_application_domain()) {
        auto name = ParametersExt::try_get_string(args, 0);
        if (!name.has_value()) {
            return Value<GC>::from_bool(false);
        }

        auto result = appdomain->has_defined_value_handling_vector(activation, name.value());
        return Value<GC>::from_bool(result);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> application_domain::get_qualified_definition_names(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_value.as_object();
    if (!this_obj.has_value()) {
        return Value<GC>::undefined();
    }

    if (auto appdomain = this_obj->as_application_domain()) {
        // NOTE: According to the docs of 'getQualifiedDefinitionNames',
        // it is able to throw a 'SecurityError' if "The definition belongs
        // to a domain to which the calling code does not have access."
        //
        // We do not implement this.

        std::vector<Value<GC>> values;
        for (const auto& name : appdomain->get_defined_names()) {
            if (!name.namespace_().is_private()) {
                values.push_back(Value<GC>::from_string(
                    name.to_qualified_name(activation->gc())
                ));
            }
        }

        auto storage = VectorStorage<GC>::from_values(
            values,
            false,
            activation->avm2()->class_defs()->string
        );

        auto name_vector = VectorObject<GC>::from_vector(storage, activation);

        return Value<GC>::from_object(name_vector);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> application_domain::set_domain_memory(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_value.as_object();
    if (!this_obj.has_value()) {
        return Value<GC>::undefined();
    }

    if (auto appdomain = this_obj->as_application_domain()) {
        if (args.size() > 0 && args[0].is_object()) {
            auto obj = args[0].as_object();
            if (obj.has_value()) {
                appdomain->set_domain_memory(
                    activation,
                    std::make_optional(obj->as_bytearray_object().expect("Expected ByteArray"))
                )?;
            } else {
                appdomain->set_domain_memory(activation, std::nullopt)?;
            }
        } else {
            appdomain->set_domain_memory(activation, std::nullopt)?;
        }
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> application_domain::get_domain_memory(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_value.as_object();
    if (!this_obj.has_value()) {
        return Value<GC>::undefined();
    }

    if (auto appdomain = this_obj->as_application_domain()) {
        if (appdomain->is_default_domain_memory()) {
            return Value<GC>::null();
        } else {
            Object<GC> bytearray_object = appdomain->domain_memory();
            return Value<GC>::from_object(bytearray_object);
        }
    }

    return Value<GC>::undefined();
}

} // namespace application_domain
} // namespace system
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_APPLICATION_DOMAIN_H
