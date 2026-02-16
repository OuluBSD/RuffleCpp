// C++ translation of core/src/avm2/object/dictionary_object.rs
//! Object representation for flash.utils.Dictionary

#ifndef RUFFLE_CORE_AVM2_OBJECT_DICTIONARY_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_DICTIONARY_OBJECT_H

#include <memory>
#include <unordered_map>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class ScriptObjectData;
    template<typename T> class TObject;
    template<typename T> class Gc;
    template<typename T> class GcWeak;
    template<typename T> class ClassObject;
    template<typename T> class Object;
    template<typename T> class Value;
    template<typename T> class Mutation;
    template<typename T> class AvmString;
    template<typename T> class DynamicKey;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to DictionaryObject data
template<typename GC>
class DictionaryObjectWeak;

/// Data structure for DictionaryObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class DictionaryObjectData {
public:
    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// Get the values map for reading
    const std::unordered_map<DynamicKey<GC>, Value<GC>>& values() const { return values_; }

    /// Get the values map for modification
    std::unordered_map<DynamicKey<GC>, Value<GC>>& values_mut() { return values_; }

    /// Get value at index for enumeration
    Value<GC> value_at(size_t index) const;

private:
    std::unordered_map<DynamicKey<GC>, Value<GC>> values_;

    // Friend classes for accessing private members
    friend class DictionaryObject<GC>;
    friend class DictionaryObjectWeak<GC>;
};

/// DictionaryObject - AVM2 Dictionary object representation
/// Allows associations between objects and values using object keys
template<typename GC>
class DictionaryObject {
public:
    using DataType = DictionaryObjectData<GC>;
    using WeakType = DictionaryObjectWeak<GC>;

    DictionaryObject() = default;
    explicit DictionaryObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create a new DictionaryObject
    static DictionaryObject create(ClassObject<GC> class_obj, Activation<GC>* activation);

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

    /// Retrieve a value in the dictionary's object space
    Value<GC> get_property_by_object(Object<GC> key) const;

    /// Set a value in the dictionary's object space
    void set_property_by_object(Object<GC> key, Value<GC> value, Mutation<GC>* mc);

    /// Delete a value from the dictionary's object space
    void delete_property_by_object(Object<GC> key, Mutation<GC>* mc);

    /// Check if the dictionary has a property by object key
    bool has_property_by_object(Object<GC> key) const;

    /// Get enumerant value at index
    Value<GC> get_enumerant_value(uint32_t index, Activation<GC>* activation) const;

    /// Set local property is enumerable (no-op for Dictionary)
    void set_local_property_is_enumerable(Mutation<GC>* mc, AvmString<GC> name, bool is_enumerable);

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to DictionaryObject
template<typename GC>
class DictionaryObjectWeak {
public:
    DictionaryObjectWeak() = default;
    explicit DictionaryObjectWeak(GcWeak<GC, DictionaryObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, DictionaryObjectData<GC>> data_;
};

// Template method implementations
template<typename GC>
DictionaryObject<GC> DictionaryObject<GC>::create(ClassObject<GC> class_obj, Activation<GC>* activation) {
    auto base = ScriptObjectData<GC>::create(class_obj);
    auto data = Gc<GC, DataType>::create(activation->gc(), DataType{base});
    return DictionaryObject<GC>(data);
}

template<typename GC>
Value<GC> DictionaryObject<GC>::get_property_by_object(Object<GC> key) const {
    auto it = data_->values().find(DynamicKey<GC>::object(key));
    if (it != data_->values().end()) {
        return it->second;
    }
    return Value<GC>::undefined();
}

template<typename GC>
void DictionaryObject<GC>::set_property_by_object(Object<GC> key, Value<GC> value, Mutation<GC>* mc) {
    // In real implementation, this would use gc_arena barrier unlocking
    data_->values_mut()[DynamicKey<GC>::object(key)] = value;
}

template<typename GC>
void DictionaryObject<GC>::delete_property_by_object(Object<GC> key, Mutation<GC>* mc) {
    data_->values_mut().erase(DynamicKey<GC>::object(key));
}

template<typename GC>
bool DictionaryObject<GC>::has_property_by_object(Object<GC> key) const {
    return data_->values().find(DynamicKey<GC>::object(key)) != data_->values().end();
}

template<typename GC>
Value<GC> DictionaryObject<GC>::get_enumerant_value(uint32_t index, Activation<GC>* activation) const {
    // Would iterate through values map in real implementation
    return Value<GC>::undefined();
}

template<typename GC>
void DictionaryObject<GC>::set_local_property_is_enumerable(Mutation<GC>* mc, AvmString<GC> name, bool is_enumerable) {
    // Calling setPropertyIsEnumerable on a Dictionary has no effect
    // Stringified properties are always enumerable
}

template<typename GC>
Value<GC> DictionaryObjectData<GC>::value_at(size_t index) const {
    // Would iterate through values map in real implementation
    return Value<GC>::undefined();
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_DICTIONARY_OBJECT_H
