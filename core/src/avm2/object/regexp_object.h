// C++ translation of core/src/avm2/object/regexp_object.rs
//! Object representation for regexp

#ifndef RUFFLE_CORE_AVM2_OBJECT_REGEXP_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_REGEXP_OBJECT_H

#include <memory>
#include <cstdint>

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
    template<typename T> class Error;
    template<typename T> class RegExp;
    class Mutation;
}
namespace string {
    template<typename T> class WString;
    class WStr;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to RegExpObject data
template<typename GC>
class RegExpObjectWeak;

/// Data structure for RegExpObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class RegExpObjectData {
public:
    RegExpObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// The regular expression
    RegExp<GC> regexp;

    // Friend classes for accessing private members
    friend class RegExpObject<GC>;
    friend class RegExpObjectWeak<GC>;
};

/// RegExpObject - AVM2 RegExp object representation
/// Represents a RegExp instance in ActionScript
template<typename GC>
class RegExpObject {
public:
    using DataType = RegExpObjectData<GC>;
    using WeakType = RegExpObjectWeak<GC>;

    RegExpObject() = default;
    explicit RegExpObject(Gc<GC, DataType> data) : data_(data) {}

    /// Get the regular expression (const reference)
    const RegExp<GC>& regexp() const {
        return data_->regexp;
    }

    /// Get the regular expression (mutable reference)
    RegExp<GC>& regexp_mut() {
        return data_->regexp;
    }

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to RegExpObject
template<typename GC>
class RegExpObjectWeak {
public:
    RegExpObjectWeak() = default;
    explicit RegExpObjectWeak(GcWeak<GC, RegExpObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, RegExpObjectData<GC>> data_;
};

/// RegExp object allocator function
/// @param class_obj The class object for RegExp
/// @param activation The current activation context
/// @return Result containing the new RegExpObject or error
template<typename GC>
Object<GC> reg_exp_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation);

// Template method implementations
template<typename GC>
Object<GC> reg_exp_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation) {
    auto base = ScriptObjectData<GC>::create(class_obj);
    auto data = Gc<GC, DataType>::create(activation->gc(), DataType{base});
    data->regexp = RegExp<GC>::create(activation->gc(), string::WString::from_utf8(""));
    return Object<GC>(RegExpObject<GC>(data));
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_REGEXP_OBJECT_H
