/*
 * C++ header for AVM1 property functionality
 * This replaces the functionality of core/src/avm1/property.rs
 */

#ifndef AVM1_PROPERTY_H
#define AVM1_PROPERTY_H

#include "avm1.h"
#include "avm1/value.h"
#include "avm1/object.h"
#include <memory>
#include <string>
#include <optional>
#include <cstdint>

namespace ruffle {

// Bit flags for property attributes
enum class Attribute : uint16_t {
    DONT_ENUM     = 1 << 0,
    DONT_DELETE   = 1 << 1,
    READ_ONLY     = 1 << 2,

    VERSION_MASK  = 0x1FFF << 3,
    VERSION_5     = 0b0000_0000_0000_0000,
    VERSION_6     = 0b0000_0000_1000_0000,
    VERSION_7     = 0b0000_0101_0000_0000,
    VERSION_8     = 0b0001_0000_0000_0000,
    VERSION_9     = 0b0010_0000_0000_0000,
    VERSION_10    = 0b0100_0000_0000_0000
};

// Helper class for combining attributes
class AttributeFlags {
public:
    static constexpr Attribute combine(Attribute a, Attribute b) {
        return static_cast<Attribute>(static_cast<uint16_t>(a) | static_cast<uint16_t>(b));
    }

    static constexpr bool has_flag(Attribute flags, Attribute flag) {
        return (static_cast<uint16_t>(flags) & static_cast<uint16_t>(flag)) != 0;
    }

    static constexpr Attribute remove(Attribute flags, Attribute flag) {
        return static_cast<Attribute>(static_cast<uint16_t>(flags) & ~static_cast<uint16_t>(flag));
    }
};

// To check if a property is available in a specific SWF version, mask the property attributes
// against the entry in this array. If the result is non-zero, the property should be hidden.
constexpr uint16_t VERSION_MASKS[10] = {
    // SWFv4 and earlier: always hide
    // Shouldn't really be used because SWFv4 did not have much AS support.
    0b0111_1111_1111_1000,
    0b0111_1111_1111_1000,
    0b0111_1111_1111_1000,
    0b0111_1111_1111_1000,
    0b0111_1111_1111_1000,
    // SWFv5 and above
    0b0111_0100_1000_0000, // v5
    0b0111_0101_0000_0000, // v6
    0b0111_0000_0000_0000, // v7
    0b0110_0000_0000_0000, // v8
    0b0100_0000_0000_0000, // v9
};

// Property class for AVM1
class Property {
private:
    std::shared_ptr<Value> data_;
    std::shared_ptr<Object> getter_;
    std::optional<std::shared_ptr<Object>> setter_;
    Attribute attributes_;

public:
    // Constructor for stored properties
    Property(std::shared_ptr<Value> data, Attribute attributes)
        : data_(std::move(data)), getter_(nullptr), setter_(std::nullopt), attributes_(attributes) {}

    // Constructor for virtual properties
    Property(std::shared_ptr<Object> getter, 
             std::optional<std::shared_ptr<Object>> setter, 
             Attribute attributes)
        : data_(std::make_shared<Value>(Value::UNDEFINED)), 
          getter_(std::move(getter)), 
          setter_(std::move(setter)), 
          attributes_(attributes) {}

    // Create a new stored property
    static Property new_stored(std::shared_ptr<Value> data, Attribute attributes) {
        return Property(std::move(data), attributes);
    }

    // Create a new virtual property
    static Property new_virtual(std::shared_ptr<Object> getter, 
                               std::optional<std::shared_ptr<Object>> setter, 
                               Attribute attributes) {
        return Property(std::move(getter), std::move(setter), attributes);
    }

    // Getters
    std::shared_ptr<Value> data() const { return data_; }
    std::shared_ptr<Object> getter() const { return getter_; }
    std::optional<std::shared_ptr<Object>> setter() const { return setter_; }
    Attribute attributes() const { return attributes_; }

    // Store data on this property, ignoring virtual setters.
    // Read-only properties are not affected.
    void set_data(std::shared_ptr<Value> data) {
        if (is_overwritable()) {
            data_ = std::move(data);
            // Overwriting a property also clears SWF version requirements.
            attributes_ = AttributeFlags::remove(attributes_, Attribute::VERSION_MASK);
        }
    }

    // Make this property virtual by attaching a getter/setter to it.
    void set_virtual(std::shared_ptr<Object> getter, 
                    std::optional<std::shared_ptr<Object>> setter) {
        getter_ = std::move(getter);
        setter_ = std::move(setter);
    }

    // Re-define this property's attributes.
    void set_attributes(Attribute attributes) {
        attributes_ = attributes;
    }

    // Check property attributes
    bool is_enumerable() const {
        return !AttributeFlags::has_flag(attributes_, Attribute::DONT_ENUM);
    }

    bool can_delete() const {
        return !AttributeFlags::has_flag(attributes_, Attribute::DONT_DELETE);
    }

    bool is_overwritable() const {
        return !AttributeFlags::has_flag(attributes_, Attribute::READ_ONLY);
    }

    bool is_virtual() const {
        return getter_ != nullptr;
    }

    // Checks if this property is accessible in the given SWF version.
    // If `false`, the property should be returned as `undefined`.
    bool allow_swf_version(uint8_t swf_version) const {
        uint16_t mask = 0;
        if (swf_version < 10) {
            mask = VERSION_MASKS[swf_version];
        } else if (swf_version == 10) {
            mask = VERSION_MASKS[9]; // Use the last entry for version 10+
        } else {
            mask = VERSION_MASKS[5]; // Default to version 5 mask for very high versions
        }
        
        return (static_cast<uint16_t>(attributes_) & mask) == 0;
    }
};

} // namespace ruffle

#endif // AVM1_PROPERTY_H