// C++ translation of core/src/avm1/property.rs
//! User-defined properties for AVM1

#ifndef RUFFLE_CORE_AVM1_PROPERTY_H
#define RUFFLE_CORE_AVM1_PROPERTY_H

#include <cstdint>
#include <optional>
#include <format>
#include <array>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Object;
    template<typename T> class Value;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {

/// Attributes of properties in the AVM runtime.
/// The values are significant and should match the order used by object::as_set_prop_flags.
enum class Attribute : uint16_t {
    None = 0,
    DontEnum = 1 << 0,
    DontDelete = 1 << 1,
    ReadOnly = 1 << 2,

    VersionMask = 0x1FFF << 3,
    Version5 = 0b0000'0000'1000'0000,
    Version6 = 0b0000'0101'0000'0000,
    Version7 = 0b0001'0000'0000'0000,
    Version8 = 0b0010'0000'0000'0000,
    Version9 = 0b0100'0000'0000'0000,
};

/// Bitwise operations for Attribute enum
inline Attribute operator|(Attribute lhs, Attribute rhs) {
    return static_cast<Attribute>(static_cast<uint16_t>(lhs) | static_cast<uint16_t>(rhs));
}

inline Attribute operator&(Attribute lhs, Attribute rhs) {
    return static_cast<Attribute>(static_cast<uint16_t>(lhs) & static_cast<uint16_t>(rhs));
}

inline Attribute operator^(Attribute lhs, Attribute rhs) {
    return static_cast<Attribute>(static_cast<uint16_t>(lhs) ^ static_cast<uint16_t>(rhs));
}

inline Attribute operator~(Attribute attr) {
    return static_cast<Attribute>(~static_cast<uint16_t>(attr));
}

inline Attribute& operator|=(Attribute& lhs, Attribute rhs) {
    lhs = lhs | rhs;
    return lhs;
}

inline Attribute& operator&=(Attribute& lhs, Attribute rhs) {
    lhs = lhs & rhs;
    return lhs;
}

inline Attribute& operator^=(Attribute& lhs, Attribute rhs) {
    lhs = lhs ^ rhs;
    return lhs;
}

inline bool operator!(Attribute attr) {
    return attr == Attribute::None;
}

/// Version masks for checking property visibility by SWF version
/// To check if a property is available in a specific SWF version, mask the property attributes
/// against the entry in this array. If the result is non-zero, the property should be hidden.
constexpr std::array<uint16_t, 10> VERSION_MASKS = {
    // SWFv4 and earlier: always hide
    // Shouldn't really be used because SWFv4 did not have much AS support.
    0b0111'1111'1111'1000,
    0b0111'1111'1111'1000,
    0b0111'1111'1111'1000,
    0b0111'1111'1111'1000,
    0b0111'1111'1111'1000,
    // SWFv5 and above
    0b0111'0100'1000'0000, // v5
    0b0111'0101'0000'0000, // v6
    0b0111'0000'0000'0000, // v7
    0b0110'0000'0000'0000, // v8
    0b0100'0000'0000'0000, // v9
};

/// Property - Represents a user-defined property in AVM1
///
/// A property can be either:
/// - Stored: Has a direct value
/// - Virtual: Has getter/setter functions
template<typename GC>
class Property {
public:
    Property() = default;

    /// Create a stored property
    ///
    /// \param data The value to store
    /// \param attributes The property attributes
    /// \return New Property
    static Property new_stored(Value<GC> data, Attribute attributes) {
        Property prop;
        prop.data_ = data;
        prop.getter_ = std::nullopt;
        prop.setter_ = std::nullopt;
        prop.attributes_ = attributes;
        return prop;
    }

    /// Create a virtual property with getter/setter
    ///
    /// \param getter The getter function object
    /// \param setter Optional setter function object
    /// \param attributes The property attributes
    /// \return New Property
    static Property new_virtual(
        Object<GC>* getter,
        std::optional<Object<GC>*> setter,
        Attribute attributes
    ) {
        Property prop;
        prop.data_ = Value<GC>::undefined();
        prop.getter_ = std::make_optional(getter);
        prop.setter_ = setter;
        prop.attributes_ = attributes;
        return prop;
    }

    /// Get the stored data value
    /// \return The data value
    Value<GC> data() const {
        return data_;
    }

    /// Get the getter function
    /// \return The getter function, or nullopt if not virtual
    std::optional<Object<GC>*> getter() const {
        return getter_;
    }

    /// Get the setter function
    /// \return The setter function, or nullopt if not virtual or no setter
    std::optional<Object<GC>*> setter() const {
        return setter_;
    }

    /// Store data on this property, ignoring virtual setters.
    /// Read-only properties are not affected.
    ///
    /// \param data The new data value
    void set_data(Value<GC> data) {
        if (is_overwritable()) {
            data_ = data;
            // Overwriting a property also clears SWF version requirements.
            attributes_ = static_cast<Attribute>(
                static_cast<uint16_t>(attributes_) & ~static_cast<uint16_t>(Attribute::VersionMask)
            );
        }
    }

    /// Make this property virtual by attaching a getter/setter to it.
    ///
    /// \param getter The getter function object
    /// \param setter Optional setter function object
    void set_virtual(Object<GC>* getter, std::optional<Object<GC>*> setter) {
        getter_ = std::make_optional(getter);
        setter_ = setter;
    }

    /// Get the property attributes
    /// \return The attributes
    Attribute attributes() const {
        return attributes_;
    }

    /// Re-define this property's attributes.
    ///
    /// \param attributes The new attributes
    void set_attributes(Attribute attributes) {
        attributes_ = attributes;
    }

    /// Check if this property is enumerable
    /// \return true if enumerable, false if DONT_ENUM is set
    bool is_enumerable() const {
        return !(attributes_ & Attribute::DontEnum);
    }

    /// Check if this property can be deleted
    /// \return true if deletable, false if DONT_DELETE is set
    bool can_delete() const {
        return !(attributes_ & Attribute::DontDelete);
    }

    /// Check if this property is overwritable
    /// \return true if overwritable, false if READ_ONLY is set
    bool is_overwritable() const {
        return !(attributes_ & Attribute::ReadOnly);
    }

    /// Check if this property is virtual (has a getter)
    /// \return true if virtual, false if stored
    bool is_virtual() const {
        return getter_.has_value();
    }

    /// Check if this property is accessible in the given SWF version.
    /// If false, the property should be returned as undefined.
    ///
    /// \param swf_version The SWF version to check against
    /// \return true if accessible, false if hidden for this version
    bool allow_swf_version(uint8_t swf_version) const {
        uint16_t mask = (swf_version < VERSION_MASKS.size())
            ? VERSION_MASKS[swf_version]
            : 0;
        return (static_cast<uint16_t>(attributes_) & mask) == 0;
    }

private:
    Value<GC> data_;
    std::optional<Object<GC>*> getter_;
    std::optional<Object<GC>*> setter_;
    Attribute attributes_ = Attribute::None;
};

} // namespace avm1
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm1::Property<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm1::Property<GC>& prop, FormatContext& ctx) {
        return std::format_to(ctx.out(), "Property(data={}, getter={}, setter={}, attrs={:#x})",
            prop.data_,
            prop.getter_.has_value() ? "present" : "none",
            prop.setter_.has_value() ? "present" : "none",
            static_cast<uint16_t>(prop.attributes_));
    }
};

#endif // RUFFLE_CORE_AVM1_PROPERTY_H
