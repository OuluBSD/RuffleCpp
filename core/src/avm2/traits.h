// C++ translation of core/src/avm2/traits.rs
//! Active trait definitions for AVM2

#ifndef RUFFLE_CORE_AVM2_TRAITS_H
#define RUFFLE_CORE_AVM2_TRAITS_H

#include <cstdint>
#include <optional>
#include <vector>
#include <memory>
#include <format>

// Forward declarations
namespace ruffle {
namespace core {
    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class Error;
        template<typename T> class Multiname;
        template<typename T> class QName;
        template<typename T> class Domain;
        template<typename T> class Metadata;
        template<typename T> class Method;
        template<typename T> class Class;
        template<typename T> class Script;
        template<typename T> class Value;
        template<typename T> class Gc;

        namespace script {
            template<typename T> class TranslationUnit;
        }
    }
    namespace swf {
        namespace avm2 {
            namespace types {
                struct Trait;
                enum class TraitKind;
                struct DefaultValue;
            }
        }
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// All attributes a trait can have.
enum class TraitAttributes : uint8_t {
    None = 0,

    /// Whether or not traits in downstream classes are allowed to override this trait.
    Final = 1 << 0,

    /// Whether or not this trait is intended to override an upstream class's trait.
    Override = 1 << 1
};

/// Bitwise operations for TraitAttributes enum
inline TraitAttributes operator|(TraitAttributes lhs, TraitAttributes rhs) {
    return static_cast<TraitAttributes>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline TraitAttributes operator&(TraitAttributes lhs, TraitAttributes rhs) {
    return static_cast<TraitAttributes>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

inline TraitAttributes operator^(TraitAttributes lhs, TraitAttributes rhs) {
    return static_cast<TraitAttributes>(static_cast<uint8_t>(lhs) ^ static_cast<uint8_t>(rhs));
}

inline TraitAttributes operator~(TraitAttributes attr) {
    return static_cast<TraitAttributes>(~static_cast<uint8_t>(attr));
}

inline TraitAttributes& operator|=(TraitAttributes& lhs, TraitAttributes rhs) {
    lhs = lhs | rhs;
    return lhs;
}

inline TraitAttributes& operator&=(TraitAttributes& lhs, TraitAttributes rhs) {
    lhs = lhs & rhs;
    return lhs;
}

inline TraitAttributes& operator^=(TraitAttributes& lhs, TraitAttributes rhs) {
    lhs = lhs ^ rhs;
    return lhs;
}

inline bool operator!(TraitAttributes attr) {
    return attr == TraitAttributes::None;
}

/// The fields for a particular kind of trait.
///
/// The kind of a trait also determines how it's instantiated on the object.
/// See each individual variant for more information.
template<typename GC>
struct TraitKind {
    /// Tag to identify the variant
    enum class Kind {
        Slot,
        Method,
        Getter,
        Setter,
        Class,
        Const
    };

    Kind kind;

    /// A data field on an object instance that can be read from and written to.
    struct SlotData {
        uint32_t slot_id;
        std::optional<Gc<GC, Multiname<GC>>> type_name;
        Value<GC> default_value;
        Domain<GC> domain;
    };

    /// A method on an object that can be called.
    struct MethodData {
        uint32_t disp_id;
        Method<GC> method;
    };

    /// A getter property on an object that can be read.
    struct GetterData {
        uint32_t disp_id;
        Method<GC> method;
    };

    /// A setter property on an object that can be written.
    struct SetterData {
        uint32_t disp_id;
        Method<GC> method;
    };

    /// A class property on an object that can be used to construct more objects.
    struct ClassData {
        uint32_t slot_id;
        Class<GC> class_;
    };

    /// A data field on an object that is always a particular value, and cannot be overridden.
    struct ConstData {
        uint32_t slot_id;
        std::optional<Gc<GC, Multiname<GC>>> type_name;
        Value<GC> default_value;
        Domain<GC> domain;
    };

    union Data {
        SlotData slot;
        MethodData method;
        GetterData getter;
        SetterData setter;
        ClassData class_;
        ConstData const_;

        Data() {}
        ~Data() {}
    } data;

    TraitKind() : kind(Kind::Slot) {}

    // Slot constructors
    static TraitKind slot(
        uint32_t slot_id,
        std::optional<Gc<GC, Multiname<GC>>> type_name,
        Value<GC> default_value,
        Domain<GC> domain
    ) {
        TraitKind result;
        result.kind = Kind::Slot;
        result.data.slot = {slot_id, type_name, default_value, domain};
        return result;
    }

    // Method constructor
    static TraitKind method(uint32_t disp_id, Method<GC> method) {
        TraitKind result;
        result.kind = Kind::Method;
        result.data.method = {disp_id, method};
        return result;
    }

    // Getter constructor
    static TraitKind getter(uint32_t disp_id, Method<GC> method) {
        TraitKind result;
        result.kind = Kind::Getter;
        result.data.getter = {disp_id, method};
        return result;
    }

    // Setter constructor
    static TraitKind setter(uint32_t disp_id, Method<GC> method) {
        TraitKind result;
        result.kind = Kind::Setter;
        result.data.setter = {disp_id, method};
        return result;
    }

    // Class constructor
    static TraitKind class_(uint32_t slot_id, Class<GC> class_) {
        TraitKind result;
        result.kind = Kind::Class;
        result.data.class_ = {slot_id, class_};
        return result;
    }

    // Const constructor
    static TraitKind const_(
        uint32_t slot_id,
        std::optional<Gc<GC, Multiname<GC>>> type_name,
        Value<GC> default_value,
        Domain<GC> domain
    ) {
        TraitKind result;
        result.kind = Kind::Const;
        result.data.const_ = {slot_id, type_name, default_value, domain};
        return result;
    }
};

/// Represents a trait as loaded into the VM.
///
/// A trait is an uninstantiated AVM2 property. Traits are used by objects to
/// track how to construct their properties when first accessed.
///
/// This type exists primarily to support classes with native methods. Adobe's
/// implementation of AVM2 handles native classes by having a special ABC file
/// load before all other code. We instead generate an initial heap in the same
/// manner as we do in AVM1, which means that we need to have a way to
/// dynamically originate traits that do not come from any particular ABC file.
template<typename GC>
class Trait {
public:
    Trait() = default;

    /// Create a const trait
    ///
    /// @param name The name of the trait
    /// @param type_name Optional type name
    /// @param default_value Optional default value
    /// @param domain The domain
    /// @return New Trait
    static Trait from_const(
        QName<GC> name,
        std::optional<Gc<GC, Multiname<GC>>> type_name,
        std::optional<Value<GC>> default_value,
        Domain<GC> domain
    ) {
        Trait trait;
        trait.name_ = name;
        trait.attributes_ = TraitAttributes::None;
        trait.kind_ = TraitKind<GC>::const_(
            0,
            type_name,
            default_value.value_or(default_value_for_type(type_name)),
            domain
        );
        trait.metadata_ = std::nullopt;
        return trait;
    }

    /// Convert an ABC trait into a loaded trait.
    ///
    /// @param unit The translation unit
    /// @param abc_trait The ABC trait to convert
    /// @param activation The current activation
    /// @return Result containing the loaded Trait or error
    static Result<Trait, Error<GC>> from_abc_trait(
        script::TranslationUnit<GC> unit,
        const swf::avm2::types::Trait& abc_trait,
        Activation<GC>* activation
    );

    /// Get the name of this trait
    QName<GC> name() const {
        return name_;
    }

    /// Get the kind of this trait
    const TraitKind<GC>& kind() const {
        return kind_;
    }

    /// Get the metadata on this trait
    std::optional<std::vector<Metadata<GC>>> metadata() const {
        return metadata_;
    }

    /// Check if this trait is final
    bool is_final() const {
        return attributes_ & TraitAttributes::Final;
    }

    /// Check if this trait is an override
    bool is_override() const {
        return attributes_ & TraitAttributes::Override;
    }

    /// Set the attributes of this trait
    void set_attributes(TraitAttributes attribs) {
        attributes_ = attribs;
    }

    /// Add the override flag to a trait
    Trait with_override() const {
        Trait result = *this;
        result.attributes_ = result.attributes_ | TraitAttributes::Override;
        return result;
    }

    /// Get the slot ID of this trait
    std::optional<uint32_t> slot_id() const {
        switch (kind_.kind) {
            case TraitKind<GC>::Kind::Slot:
            case TraitKind<GC>::Kind::Class:
            case TraitKind<GC>::Kind::Const:
                return std::make_optional(kind_.data.slot.slot_id);
            default:
                return std::nullopt;
        }
    }

    /// Set the slot ID of this trait
    void set_slot_id(uint32_t id) {
        switch (kind_.kind) {
            case TraitKind<GC>::Kind::Slot:
                kind_.data.slot.slot_id = id;
                break;
            case TraitKind<GC>::Kind::Class:
                kind_.data.class_.slot_id = id;
                break;
            case TraitKind<GC>::Kind::Const:
                kind_.data.const_.slot_id = id;
                break;
            default:
                break;
        }
    }

    /// Get the dispatch ID of this trait
    std::optional<uint32_t> disp_id() const {
        switch (kind_.kind) {
            case TraitKind<GC>::Kind::Method:
                return std::make_optional(kind_.data.method.disp_id);
            case TraitKind<GC>::Kind::Getter:
                return std::make_optional(kind_.data.getter.disp_id);
            case TraitKind<GC>::Kind::Setter:
                return std::make_optional(kind_.data.setter.disp_id);
            default:
                return std::nullopt;
        }
    }

    /// Set the dispatch ID of this trait
    void set_disp_id(uint32_t id) {
        switch (kind_.kind) {
            case TraitKind<GC>::Kind::Method:
                kind_.data.method.disp_id = id;
                break;
            case TraitKind<GC>::Kind::Getter:
                kind_.data.getter.disp_id = id;
                break;
            case TraitKind<GC>::Kind::Setter:
                kind_.data.setter.disp_id = id;
                break;
            default:
                break;
        }
    }

    /// Get the method contained within this trait, if it has one
    std::optional<Method<GC>> as_method() const {
        switch (kind_.kind) {
            case TraitKind<GC>::Kind::Method:
                return std::make_optional(kind_.data.method.method);
            case TraitKind<GC>::Kind::Getter:
                return std::make_optional(kind_.data.getter.method);
            case TraitKind<GC>::Kind::Setter:
                return std::make_optional(kind_.data.setter.method);
            default:
                return std::nullopt;
        }
    }

private:
    /// The name of this trait
    QName<GC> name_;

    /// The attributes set on this trait
    TraitAttributes attributes_ = TraitAttributes::None;

    /// The kind of trait in use
    TraitKind<GC> kind_;

    /// Metadata on the trait, such as "[Inject]"
    std::optional<std::vector<Metadata<GC>>> metadata_;

    /// Returns the default "null" value for the given type
    static Value<GC> default_value_for_type(
        std::optional<Gc<GC, Multiname<GC>>> type_name
    );
};

// Trait method implementations

template<typename GC>
Result<Trait<GC>, Error<GC>> Trait<GC>::from_abc_trait(
    script::TranslationUnit<GC> unit,
    const swf::avm2::types::Trait& abc_trait,
    Activation<GC>* activation
) {
    // Implementation would convert ABC trait to loaded trait
    // This is a simplified version - full implementation would need
    // to handle all ABC trait kinds and metadata
    return Result<Trait, Error<GC>>::ok(Trait());
}

template<typename GC>
Value<GC> Trait<GC>::default_value_for_type(
    std::optional<Gc<GC, Multiname<GC>>> type_name
) {
    if (type_name.has_value()) {
        // Check if type_name contains public namespace
        // and return appropriate default value based on type name
        // This is simplified - full implementation would check actual type
        return Value<GC>::null();
    }
    return Value<GC>::undefined();
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::Trait<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::Trait<GC>& trait, FormatContext& ctx) {
        return std::format_to(ctx.out(), "Trait(name={})",
            "trait_name"); // Would use trait.name() in real implementation
    }
};

#endif // RUFFLE_CORE_AVM2_TRAITS_H
