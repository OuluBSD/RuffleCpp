// C++ translation of core/src/avm2/object/date_object.rs
//! Object representation for Date objects

#ifndef RUFFLE_CORE_AVM2_OBJECT_DATE_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_DATE_OBJECT_H

#include <memory>
#include <optional>
#include <chrono>

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
    template<typename T> class Mutation;
    template<typename T> class UpdateContext;
    template<typename T> class AvmString;
    enum class Hint : uint8_t;
}
}
}

// For DateTime<Utc> equivalent
namespace chrono {
    template<typename T> class date_time;
    struct utc;
}

namespace ruffle {
namespace core {
namespace avm2 {

/// DateTime type alias for UTC time
using DateTimeUtc = std::chrono::time_point<std::chrono::system_clock>;

/// Weak reference to DateObject data
template<typename GC>
class DateObjectWeak;

/// Data structure for DateObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class DateObjectData {
public:
    DateObjectData() : date_time_(std::nullopt) {}

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// Get the date time value
    std::optional<DateTimeUtc> date_time() const { return date_time_; }

    /// Set the date time value
    void set_date_time(const std::optional<DateTimeUtc>& value) { date_time_ = value; }

private:
    std::optional<DateTimeUtc> date_time_;

    // Friend classes for accessing private members
    friend class DateObject<GC>;
    friend class DateObjectWeak<GC>;
};

/// DateObject - AVM2 Date object representation
/// Represents a date/time value in ActionScript
template<typename GC>
class DateObject {
public:
    using DataType = DateObjectData<GC>;
    using WeakType = DateObjectWeak<GC>;

    DateObject() = default;
    explicit DateObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create a new DateObject from a date time
    static Object<GC> from_date_time(UpdateContext<GC>* context, const DateTimeUtc& date_time);

    /// Create a DateObject for prototype
    static Object<GC> for_prototype(UpdateContext<GC>* context, ClassObject<GC> date_class);

    /// Create a new DateObject with allocator
    static DateObject create(ClassObject<GC> class_obj, Activation<GC>* activation);

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

    /// Get the date time value
    std::optional<DateTimeUtc> date_time() const {
        return data_->date_time();
    }

    /// Set the date time value
    void set_date_time(const std::optional<DateTimeUtc>& value) {
        data_->set_date_time(value);
    }

    /// Get the default hint for type conversion
    Hint default_hint() const {
        return Hint::String;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to DateObject
template<typename GC>
class DateObjectWeak {
public:
    DateObjectWeak() = default;
    explicit DateObjectWeak(GcWeak<GC, DateObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, DateObjectData<GC>> data_;
};

// Template method implementations
template<typename GC>
DateObject<GC> DateObject<GC>::create(ClassObject<GC> class_obj, Activation<GC>* activation) {
    auto base = ScriptObjectData<GC>::create(class_obj);
    auto data = Gc<GC, DataType>::create(activation->gc(), DataType{base});
    return DateObject<GC>(data);
}

template<typename GC>
Object<GC> DateObject<GC>::from_date_time(UpdateContext<GC>* context, const DateTimeUtc& date_time) {
    // Get the Date class from context
    // auto class_obj = context->avm2()->classes()->date;
    auto base = ScriptObjectData<GC>::create(/* class_obj */);
    auto data = Gc<GC, DataType>::create(context->gc(), DataType{base});
    data->set_date_time(date_time);
    return Object<GC>(DateObject<GC>(data));
}

template<typename GC>
Object<GC> DateObject<GC>::for_prototype(UpdateContext<GC>* context, ClassObject<GC> date_class) {
    auto object_class = context->avm2()->classes()->object;
    auto base = ScriptObjectData<GC>::custom_new(
        date_class.inner_class_definition(),
        object_class.prototype(),
        date_class.instance_vtable()
    );

    auto data = Gc<GC, DataType>::create(context->gc(), DataType{base});
    return Object<GC>(DateObject<GC>(data));
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_DATE_OBJECT_H
