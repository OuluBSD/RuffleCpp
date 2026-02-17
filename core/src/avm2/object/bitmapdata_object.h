// C++ translation of core/src/avm2/object/bitmapdata_object.rs
//! Object representation for BitmapData

#ifndef RUFFLE_CORE_AVM2_OBJECT_BITMAPDATA_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_BITMAPDATA_OBJECT_H

#include <memory>
#include <cstdint>
#include <format>

// Forward declarations
namespace ruffle {
namespace core {
    class WString;
    template<typename GC> class UpdateContext;

    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class ScriptObjectData;
        template<typename T> class TObject;
        template<typename T> class Gc;
        template<typename T> class GcWeak;
        template<typename T> class ClassObject;
        template<typename T> class Object;
        template<typename T> class Error;
        template<typename T> class Mutation;
    }
    namespace bitmap {
        namespace bitmap_data {
            template<typename T> class BitmapData;
        }
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to BitmapDataObject data
template<typename GC>
class BitmapDataObjectWeak;

/// Data structure for BitmapDataObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class BitmapDataObjectData {
public:
    BitmapDataObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// The BitmapData this object wraps
    bitmap::bitmap_data::BitmapData<GC> bitmap_data;

    // Friend classes for accessing private members
    friend class BitmapDataObject<GC>;
    friend class BitmapDataObjectWeak<GC>;
};

/// BitmapDataObject - AVM2 BitmapData object representation
/// Represents a BitmapData instance in ActionScript 3
template<typename GC>
class BitmapDataObject {
public:
    using DataType = BitmapDataObjectData<GC>;
    using WeakType = BitmapDataObjectWeak<GC>;

    BitmapDataObject() = default;
    explicit BitmapDataObject(Gc<GC, DataType> data) : data_(data) {}

    /// Construct a BitmapDataObject from a BitmapData and class
    ///
    /// This is *not* used when explicitly constructing a BitmapData
    /// instance from ActionScript (e.g. `new BitmapData(100, 100)`,
    /// or `new MyBitmapDataSubclass(100, 100)`).
    ///
    /// Instead, this is used when constructing a `Bitmap` object,
    /// (from ActionScript or from the timeline), or when we need
    /// to produce a new BitmapData object from a `BitmapData` method
    /// like `clone()`
    ///
    /// @param mc The mutation context
    /// @param bitmap_data The BitmapData to wrap
    /// @param class_obj The class object
    /// @return New BitmapDataObject
    static BitmapDataObject from_bitmap_data_and_class(
        Mutation<GC>* mc,
        bitmap::bitmap_data::BitmapData<GC> bitmap_data,
        ClassObject<GC> class_obj
    );

    /// Construct a BitmapDataObject for a given BitmapData
    /// The resulting object will have the BitmapData class
    ///
    /// @param context The update context
    /// @param bitmap_data The BitmapData to wrap
    /// @return New BitmapDataObject
    static BitmapDataObject from_bitmap_data(
        UpdateContext<GC>* context,
        bitmap::bitmap_data::BitmapData<GC> bitmap_data
    );

    /// Get the BitmapData
    /// @return The BitmapData
    bitmap::bitmap_data::BitmapData<GC> get_bitmap_data() const {
        return data_->bitmap_data;
    }

    /// Initialize the BitmapData association
    ///
    /// This should only be called to initialize the association between an AVM
    /// object and its associated bitmap data. This association should not be
    /// reinitialized later.
    ///
    /// @param mc The mutation context
    /// @param new_bitmap The new BitmapData to associate
    void init_bitmap_data(Mutation<GC>* mc, bitmap::bitmap_data::BitmapData<GC> new_bitmap);

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to BitmapDataObject
template<typename GC>
class BitmapDataObjectWeak {
public:
    BitmapDataObjectWeak() = default;
    explicit BitmapDataObjectWeak(GcWeak<GC, BitmapDataObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, BitmapDataObjectData<GC>> data_;
};

// Template method implementations

template<typename GC>
BitmapDataObject<GC> BitmapDataObject<GC>::from_bitmap_data_and_class(
    Mutation<GC>* mc,
    bitmap::bitmap_data::BitmapData<GC> bitmap_data,
    ClassObject<GC> class_obj
) {
    auto base = ScriptObjectData<GC>::create(class_obj);
    auto data = Gc<GC, DataType>::create(
        mc,
        DataType{base, bitmap_data}
    );
    auto instance = BitmapDataObject<GC>(data);

    bitmap_data.init_object2(mc, instance);

    return instance;
}

template<typename GC>
BitmapDataObject<GC> BitmapDataObject<GC>::from_bitmap_data(
    UpdateContext<GC>* context,
    bitmap::bitmap_data::BitmapData<GC> bitmap_data
) {
    auto bitmapdata_class = context->avm2()->classes()->bitmapdata;
    return from_bitmap_data_and_class(context->gc(), bitmap_data, bitmapdata_class);
}

template<typename GC>
void BitmapDataObject<GC>::init_bitmap_data(Mutation<GC>* mc, bitmap::bitmap_data::BitmapData<GC> new_bitmap) {
    data_->bitmap_data = new_bitmap;
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::BitmapDataObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::BitmapDataObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "BitmapDataObject(ptr={})",
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_BITMAPDATA_OBJECT_H
