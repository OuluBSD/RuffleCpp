// C++ translation of core/src/avm2/globals/avmplus.rs
// avmplus.describeTypeJSON implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_AVMPLUS_H
#define RUFFLE_CORE_AVM2_GLOBALS_AVMPLUS_H

#include <vector>
#include <cstdint>
#include <memory>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {

namespace string {
    class AvmString;
}

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
class ArrayObject;
template<typename GC>
class ScriptObject;
template<typename GC>
class Class;
template<typename GC>
class ClassObject;
template<typename GC>
class Method;
template<typename GC>
class Multiname;
template<typename GC>
class Namespace;
template<typename GC>
class UpdateContext;

namespace metadata {
    template<typename GC>
    class Metadata;
}

} // namespace avm2

namespace context {
    template<typename GC>
    class UpdateContext;
}

} // namespace core
} // namespace ruffle

namespace gc_arena {
    template<typename T>
    class Gc;
    template<typename GC>
    class Mutation;
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace avmplus {

using gc_arena::Gc;
using gc_arena::Mutation;

/// Bitflags for describeTypeJSON
class DescribeTypeFlags {
private:
    uint32_t bits_;

public:
    enum Flag : uint32_t {
        HIDE_NSURI_METHODS      = 1 << 0,
        INCLUDE_BASES           = 1 << 1,
        INCLUDE_INTERFACES      = 1 << 2,
        INCLUDE_VARIABLES       = 1 << 3,
        INCLUDE_ACCESSORS       = 1 << 4,
        INCLUDE_METHODS         = 1 << 5,
        INCLUDE_METADATA        = 1 << 6,
        INCLUDE_CONSTRUCTOR     = 1 << 7,
        INCLUDE_TRAITS          = 1 << 8,
        USE_ITRAITS             = 1 << 9,
        HIDE_OBJECT             = 1 << 10
    };

    DescribeTypeFlags() : bits_(0) {}
    explicit DescribeTypeFlags(uint32_t bits) : bits_(bits) {}

    bool contains(Flag flag) const {
        return (bits_ & static_cast<uint32_t>(flag)) != 0;
    }

    void set(Flag flag) {
        bits_ |= static_cast<uint32_t>(flag);
    }

    void clear(Flag flag) {
        bits_ &= ~static_cast<uint32_t>(flag);
    }

    uint32_t bits() const { return bits_; }

    static std::optional<DescribeTypeFlags> from_bits(uint32_t bits) {
        // Validate that only known flags are set
        constexpr uint32_t VALID_FLAGS = 
            HIDE_NSURI_METHODS | INCLUDE_BASES | INCLUDE_INTERFACES |
            INCLUDE_VARIABLES | INCLUDE_ACCESSORS | INCLUDE_METHODS |
            INCLUDE_METADATA | INCLUDE_CONSTRUCTOR | INCLUDE_TRAITS |
            USE_ITRAITS | HIDE_OBJECT;
        
        if ((bits & ~VALID_FLAGS) == 0) {
            return DescribeTypeFlags(bits);
        }
        return std::nullopt;
    }
};

/// Implements `avmplus.describeTypeJSON`
/// @param activation The current activation
/// @param this_val The this value (ignored)
/// @param args Function arguments [value, flags]
/// @return JSON object describing the type
template<typename GC>
Value<GC> describe_type_json(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Get the instance class for describeType, supporting Null/Undefined/Integer
/// @param activation The current activation
/// @param value The value to get the class for
/// @return The class definition
template<typename GC>
Class<GC> instance_class_describe_type(
    Activation<GC>* activation,
    Value<GC> value
);

/// Describe the internal body of a class
/// @param context The update context
/// @param class_def The class definition
/// @param flags The describe type flags
/// @return Object containing traits
template<typename GC>
Object<GC> describe_internal_body(
    UpdateContext<GC>* context,
    Class<GC> class_def,
    DescribeTypeFlags flags
);

/// Display a name, handling None as "*"
/// @param context The string context
/// @param name The optional multiname
/// @return The display name
template<typename GC>
string::AvmString display_name(
    UpdateContext<GC>* context,
    const std::optional<Gc<Multiname<GC>>>& name
);

/// Write method parameters to an array
/// @param method The method to write parameters for
/// @param context The update context
/// @return Array of parameter objects
template<typename GC>
ArrayObject<GC> write_params(
    Method<GC> method,
    UpdateContext<GC>* context
);

/// Write metadata to an array
/// @param metadata_object The array to write to
/// @param trait_metadata The metadata to write
/// @param context The update context
template<typename GC>
void write_metadata(
    ArrayObject<GC> metadata_object,
    const std::vector<metadata::Metadata<GC>>& trait_metadata,
    UpdateContext<GC>* context
);

// Template method implementations

template<typename GC>
Value<GC> describe_type_json(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto flags_value = args.size() > 1 ? args[1].coerce_to_u32(activation) : 0;
    auto flags = DescribeTypeFlags::from_bits(flags_value);
    
    if (!flags.has_value()) {
        return Value<GC>::from_error(activation, "Invalid flags");
    }

    auto value = args.size() > 0 ? args[0] : Value<GC>::undefined();
    auto class_def = instance_class_describe_type(activation, value);
    
    auto object = ScriptObject<GC>::new_object(activation->context);

    auto used_class_def = class_def;
    if (flags->contains(DescribeTypeFlags::USE_ITRAITS)) {
        auto i_class = used_class_def.i_class();
        if (i_class.has_value()) {
            used_class_def = i_class.value();
        } else {
            return Value<GC>::null();
        }
    }

    auto qualified_name = used_class_def
        .dollar_removed_name(activation->gc())
        .to_qualified_name(activation->gc());

    object->set_dynamic_property(
        activation->context->strings.from_utf8("name"),
        qualified_name.into(),
        activation->gc()
    );

    object->set_dynamic_property(
        activation->context->strings.from_utf8("isDynamic"),
        Value<GC>::from_bool(!used_class_def.is_sealed()),
        activation->gc()
    );

    object->set_dynamic_property(
        activation->context->strings.from_utf8("isFinal"),
        Value<GC>::from_bool(used_class_def.is_final()),
        activation->gc()
    );

    bool is_static = value.as_object()
        .and_then([](auto o) { return o->as_class_object(); })
        .has_value();

    object->set_dynamic_property(
        activation->context->strings.from_utf8("isStatic"),
        Value<GC>::from_bool(is_static),
        activation->gc()
    );

    auto traits = describe_internal_body(activation->context, used_class_def, *flags);
    
    if (flags->contains(DescribeTypeFlags::INCLUDE_TRAITS)) {
        object->set_dynamic_property(
            activation->context->strings.from_utf8("traits"),
            traits.into(),
            activation->gc()
        );
    } else {
        object->set_dynamic_property(
            activation->context->strings.from_utf8("traits"),
            Value<GC>::null(),
            activation->gc()
        );
    }

    return object.into();
}

template<typename GC>
Class<GC> instance_class_describe_type(
    Activation<GC>* activation,
    Value<GC> value
) {
    auto class_defs = activation->avm2()->class_defs();

    auto normalized = value.normalize();
    if (normalized.is_null()) {
        return class_defs.null;
    } else if (normalized.is_undefined()) {
        return class_defs.void_;
    } else if (normalized.is_integer()) {
        return class_defs.int_;
    } else {
        return value.instance_class(activation);
    }
}

template<typename GC>
Object<GC> describe_internal_body(
    UpdateContext<GC>* context,
    Class<GC> class_def,
    DescribeTypeFlags flags
) {
    auto mc = context->gc();
    auto strings = &context->strings;

    auto traits = ScriptObject<GC>::new_object(context);

    auto bases = ArrayObject<GC>::empty(context);
    auto interfaces = ArrayObject<GC>::empty(context);
    auto variables = ArrayObject<GC>::empty(context);
    auto accessors = ArrayObject<GC>::empty(context);
    auto methods = ArrayObject<GC>::empty(context);

    // Set up bases array
    if (flags.contains(DescribeTypeFlags::INCLUDE_BASES)) {
        traits->set_dynamic_property(
            strings->from_utf8("bases"),
            bases.into(),
            mc
        );
    } else {
        traits->set_dynamic_property(
            strings->from_utf8("bases"),
            Value<GC>::null(),
            mc
        );
    }

    // Set up interfaces array
    if (flags.contains(DescribeTypeFlags::INCLUDE_INTERFACES)) {
        traits->set_dynamic_property(
            strings->from_utf8("interfaces"),
            interfaces.into(),
            mc
        );
    } else {
        traits->set_dynamic_property(
            strings->from_utf8("interfaces"),
            Value<GC>::null(),
            mc
        );
    }

    // Set up variables array
    if (flags.contains(DescribeTypeFlags::INCLUDE_VARIABLES)) {
        traits->set_dynamic_property(
            strings->from_utf8("variables"),
            variables.into(),
            mc
        );
    } else {
        traits->set_dynamic_property(
            strings->from_utf8("variables"),
            Value<GC>::null(),
            mc
        );
    }

    // Set up accessors array
    if (flags.contains(DescribeTypeFlags::INCLUDE_ACCESSORS)) {
        traits->set_dynamic_property(
            strings->from_utf8("accessors"),
            accessors.into(),
            mc
        );
    } else {
        traits->set_dynamic_property(
            strings->from_utf8("accessors"),
            Value<GC>::null(),
            mc
        );
    }

    // Set up methods array
    if (flags.contains(DescribeTypeFlags::INCLUDE_METHODS)) {
        traits->set_dynamic_property(
            strings->from_utf8("methods"),
            methods.into(),
            mc
        );
    } else {
        traits->set_dynamic_property(
            strings->from_utf8("methods"),
            Value<GC>::null(),
            mc
        );
    }

    // Get mutable storage for arrays
    auto bases_array = bases->storage_mut(mc);
    auto interfaces_array = interfaces->storage_mut(mc);
    auto variables_array = variables->storage_mut(mc);
    auto accessors_array = accessors->storage_mut(mc);
    auto methods_array = methods->storage_mut(mc);

    // Fill bases array
    if (flags.contains(DescribeTypeFlags::INCLUDE_BASES)) {
        auto current_super_class = class_def.super_class();
        while (current_super_class.has_value()) {
            auto super_class = current_super_class.value();
            auto super_name = super_class.name()->to_qualified_name(mc);
            bases_array->push(super_name.into());
            current_super_class = super_class.super_class();
        }
    }

    // Fill interfaces array
    if (flags.contains(DescribeTypeFlags::INCLUDE_INTERFACES)) {
        for (const auto& interface : class_def.all_interfaces()) {
            auto interface_name = interface.name()->to_qualified_name(mc);
            interfaces_array->push(interface_name.into());
        }
    }

    // Build skip_ns list for HIDE_NSURI_METHODS behavior
    std::vector<Namespace<GC>> skip_ns;
    auto super_vtable = class_def.super_class().map([](auto c) { return c.vtable(); });
    
    if (super_vtable.has_value()) {
        for (const auto& [_, ns, prop] : super_vtable->resolved_traits().iter()) {
            if (!ns.as_uri(&context->strings).empty()) {
                if (prop.is_method()) {
                    bool found = false;
                    for (const auto& other_ns : skip_ns) {
                        if (other_ns.exact_version_match(ns)) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        skip_ns.push_back(ns);
                    }
                }
            }
        }
    }

    // Iterate over vtable traits
    auto vtable = class_def.vtable();
    for (const auto& [prop_name, ns, prop] : vtable.resolved_traits().iter()) {
        if (!ns.is_public_ignoring_ns()) continue;
        if (!ns.matches_api_version(context->avm2.root_api_version)) continue;

        if (flags.contains(DescribeTypeFlags::HIDE_NSURI_METHODS)) {
            bool should_skip = false;
            for (const auto& other_ns : skip_ns) {
                if (ns.exact_version_match(other_ns)) {
                    should_skip = true;
                    break;
                }
            }
            if (should_skip) continue;
        }

        auto uri = ns.as_uri_opt().filter([](const auto& u) { return !u.empty(); });

        // Handle different property types
        if (prop.is_slot() || prop.is_const_slot()) {
            if (!flags.contains(DescribeTypeFlags::INCLUDE_VARIABLES)) continue;

            auto prop_class_name = vtable.slot_class_name(&context->strings, prop.slot_id());
            auto access = prop.is_const_slot() 
                ? strings->from_utf8("readonly")
                : strings->from_utf8("readwrite");

            auto trait_metadata = vtable.get_metadata_for_slot(prop.slot_id());

            auto variable = ScriptObject<GC>::new_object(context);
            variable->set_dynamic_property(strings->from_utf8("name"), prop_name.into(), mc);
            variable->set_dynamic_property(strings->from_utf8("type"), prop_class_name.into(), mc);
            variable->set_dynamic_property(strings->from_utf8("access"), access.into(), mc);
            variable->set_dynamic_property(
                strings->from_utf8("uri"),
                uri.map_or(Value<GC>::null(), [](const auto& u) { return u.into(); }),
                mc
            );
            variable->set_dynamic_property(strings->from_utf8("metadata"), Value<GC>::null(), mc);

            if (flags.contains(DescribeTypeFlags::INCLUDE_METADATA)) {
                auto metadata_object = ArrayObject<GC>::empty(context);
                if (trait_metadata.has_value()) {
                    write_metadata(metadata_object, trait_metadata.value(), context);
                }
                variable->set_dynamic_property(
                    strings->from_utf8("metadata"),
                    metadata_object.into(),
                    mc
                );
            }

            variables_array->push(variable.into());
        }
        else if (prop.is_method()) {
            if (!flags.contains(DescribeTypeFlags::INCLUDE_METHODS)) continue;

            auto method = vtable.get_method(prop.disp_id())
                .value_or([]{ /* panic */ return Method<GC>(); }());
            
            auto declared_by = method.bound_class()
                .value_or([]{ /* panic */ return Class<GC>(); }());

            // Skip methods that exist in interfaces
            bool in_interface = false;
            for (const auto& interface : declared_by.all_interfaces()) {
                if (interface.vtable().has_trait(&Multiname<GC>::new(ns, prop_name))) {
                    in_interface = true;
                    break;
                }
            }
            if (in_interface) continue;

            auto return_type_name = display_name(context, method.return_type());

            if (flags.contains(DescribeTypeFlags::HIDE_OBJECT) &&
                declared_by == context->avm2.class_defs().object) {
                continue;
            }

            auto declared_by_name = declared_by.dollar_removed_name(mc).to_qualified_name(mc);
            auto trait_metadata = vtable.get_metadata_for_disp(prop.disp_id());

            auto method_obj = ScriptObject<GC>::new_object(context);
            method_obj->set_dynamic_property(strings->from_utf8("name"), prop_name.into(), mc);
            method_obj->set_dynamic_property(
                strings->from_utf8("returnType"),
                return_type_name.into(),
                mc
            );
            method_obj->set_dynamic_property(
                strings->from_utf8("declaredBy"),
                declared_by_name.into(),
                mc
            );
            method_obj->set_dynamic_property(
                strings->from_utf8("uri"),
                uri.map_or(Value<GC>::null(), [](const auto& u) { return u.into(); }),
                mc
            );

            auto params = write_params(method, context);
            method_obj->set_dynamic_property(
                strings->from_utf8("parameters"),
                params.into(),
                mc
            );
            method_obj->set_dynamic_property(strings->from_utf8("metadata"), Value<GC>::null(), mc);

            if (flags.contains(DescribeTypeFlags::INCLUDE_METADATA)) {
                auto metadata_object = ArrayObject<GC>::empty(context);
                if (trait_metadata.has_value()) {
                    write_metadata(metadata_object, trait_metadata.value(), context);
                }
                method_obj->set_dynamic_property(
                    strings->from_utf8("metadata"),
                    metadata_object.into(),
                    mc
                );
            }

            methods_array->push(method_obj.into());
        }
        else if (prop.is_virtual()) {
            if (!flags.contains(DescribeTypeFlags::INCLUDE_ACCESSORS)) continue;

            auto [get, set] = prop.virtual_methods();
            auto access = get.has_value() && set.has_value() ? strings->from_utf8("readwrite")
                : get.has_value() ? strings->from_utf8("readonly")
                : set.has_value() ? strings->from_utf8("writeonly")
                : []{ /* unreachable */ return string::AvmString(); }();

            // Get type from getter or setter
            auto [method_type, defining_class] = get.has_value()
                ? [&]() -> std::pair<std::optional<Gc<Multiname<GC>>>, Class<GC>> {
                    auto get_method = vtable.get_method(*get)
                        .value_or([]{ /* panic */ return Method<GC>(); }());
                    auto bound_class = get_method.bound_class()
                        .value_or([]{ /* panic */ return Class<GC>(); }());
                    return {get_method.return_type(), bound_class};
                }()
                : [&]() -> std::pair<std::optional<Gc<Multiname<GC>>>, Class<GC>> {
                    auto set_method = vtable.get_method(*set)
                        .value_or([]{ /* panic */ return Method<GC>(); }());
                    auto bound_class = set_method.bound_class()
                        .value_or([]{ /* panic */ return Class<GC>(); }());
                    return {set_method.signature()[0].param_type_name, bound_class};
                }();

            // Skip if exists in interface
            bool in_interface = false;
            for (const auto& interface : defining_class.all_interfaces()) {
                if (interface.vtable().has_trait(&Multiname<GC>::new(ns, prop_name))) {
                    in_interface = true;
                    break;
                }
            }
            if (in_interface) continue;

            auto accessor_type = display_name(context, method_type);
            auto declared_by = defining_class.dollar_removed_name(mc).to_qualified_name(mc);

            auto accessor_obj = ScriptObject<GC>::new_object(context);
            accessor_obj->set_dynamic_property(strings->from_utf8("name"), prop_name.into(), mc);
            accessor_obj->set_dynamic_property(strings->from_utf8("access"), access.into(), mc);
            accessor_obj->set_dynamic_property(strings->from_utf8("type"), accessor_type.into(), mc);
            accessor_obj->set_dynamic_property(
                strings->from_utf8("declaredBy"),
                declared_by.into(),
                mc
            );
            accessor_obj->set_dynamic_property(
                strings->from_utf8("uri"),
                uri.map_or(Value<GC>::null(), [](const auto& u) { return u.into(); }),
                mc
            );

            auto metadata_object = ArrayObject<GC>::empty(context);
            if (get.has_value()) {
                if (auto metadata = vtable.get_metadata_for_disp(*get)) {
                    write_metadata(metadata_object, *metadata, context);
                }
            }
            if (set.has_value()) {
                if (auto metadata = vtable.get_metadata_for_disp(*set)) {
                    write_metadata(metadata_object, *metadata, context);
                }
            }

            if (flags.contains(DescribeTypeFlags::INCLUDE_METADATA) &&
                metadata_object.storage().length() > 0) {
                accessor_obj->set_dynamic_property(
                    strings->from_utf8("metadata"),
                    metadata_object.into(),
                    mc
                );
            } else {
                accessor_obj->set_dynamic_property(
                    strings->from_utf8("metadata"),
                    Value<GC>::null(),
                    mc
                );
            }

            accessors_array->push(accessor_obj.into());
        }
    }

    // Handle constructor
    auto constructor = class_def.instance_init();
    if (constructor.has_value() && 
        !constructor->signature().is_empty() &&
        flags.contains(DescribeTypeFlags::INCLUDE_CONSTRUCTOR)) {
        auto params = write_params(*constructor, context);
        traits->set_dynamic_property(
            strings->from_utf8("constructor"),
            params.into(),
            mc
        );
    } else {
        traits->set_dynamic_property(
            strings->from_utf8("constructor"),
            Value<GC>::null(),
            mc
        );
    }

    // Handle metadata
    if (flags.contains(DescribeTypeFlags::INCLUDE_METADATA)) {
        // STUB: with top-level metadata
        auto metadata_object = ArrayObject<GC>::empty(context);
        traits->set_dynamic_property(
            strings->from_utf8("metadata"),
            metadata_object.into(),
            mc
        );
    } else {
        traits->set_dynamic_property(
            strings->from_utf8("metadata"),
            Value<GC>::null(),
            mc
        );
    }

    return traits;
}

template<typename GC>
string::AvmString display_name(
    UpdateContext<GC>* context,
    const std::optional<Gc<Multiname<GC>>>& name
) {
    if (name.has_value()) {
        return name.value()->to_qualified_name_or_star(&context->strings);
    }
    return context->strings.from_utf8("*");
}

template<typename GC>
ArrayObject<GC> write_params(
    Method<GC> method,
    UpdateContext<GC>* context
) {
    auto mc = context->gc();
    auto params = ArrayObject<GC>::empty(context);
    auto params_array = params->storage_mut(mc);

    for (const auto& param : method.signature()) {
        auto param_type_name = display_name(context, param.param_type_name);
        auto optional = param.default_value.has_value();
        
        auto param_obj = ScriptObject<GC>::new_object(context);
        param_obj->set_dynamic_property(
            context->strings.from_utf8("type"),
            param_type_name.into(),
            mc
        );
        param_obj->set_dynamic_property(
            context->strings.from_utf8("optional"),
            Value<GC>::from_bool(optional),
            mc
        );
        params_array->push(param_obj.into());
    }

    return params;
}

template<typename GC>
void write_metadata(
    ArrayObject<GC> metadata_object,
    const std::vector<metadata::Metadata<GC>>& trait_metadata,
    UpdateContext<GC>* context
) {
    auto metadata_array = metadata_object->storage_mut(context->gc());

    for (const auto& single_trait : trait_metadata) {
        metadata_array->push(single_trait.as_json_object(context).into());
    }
}

} // namespace avmplus
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_AVMPLUS_H
