// C++ translation of core/src/avm2/object/soundtransform_object.rs
//! Object representation for SoundTransform objects

#ifndef RUFFLE_CORE_AVM2_OBJECT_SOUNDTRANSFORM_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_SOUNDTRANSFORM_OBJECT_H

#include <memory>
#include <cstdint>
#include <format>

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
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to SoundTransformObject data
template<typename GC>
class SoundTransformObjectWeak;

/// Data structure for SoundTransformObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class SoundTransformObjectData {
public:
    SoundTransformObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// Sound transformation matrix values
    double left_to_left = 0.0;
    double left_to_right = 0.0;
    double right_to_left = 0.0;
    double right_to_right = 0.0;

    /// Volume level
    double volume = 0.0;

    // Friend classes for accessing private members
    friend class SoundTransformObject<GC>;
    friend class SoundTransformObjectWeak<GC>;
};

/// SoundTransformObject - AVM2 SoundTransform object representation
/// Represents a SoundTransform instance in ActionScript
/// 
/// SoundTransform controls audio mixing between left and right channels
/// and overall volume. The matrix values represent:
/// - left_to_left: How much of the left input goes to the left output
/// - left_to_right: How much of the left input goes to the right output
/// - right_to_left: How much of the right input goes to the left output
/// - right_to_right: How much of the right input goes to the right output
template<typename GC>
class SoundTransformObject {
public:
    using DataType = SoundTransformObjectData<GC>;
    using WeakType = SoundTransformObjectWeak<GC>;

    SoundTransformObject() = default;
    explicit SoundTransformObject(Gc<GC, DataType> data) : data_(data) {}

    /// Get the left-to-left mixing value
    /// @return The left-to-left mixing coefficient (0.0 to 1.0)
    double left_to_left() const {
        return data_->left_to_left;
    }

    /// Set the left-to-left mixing value
    /// @param value The new left-to-left mixing coefficient
    void set_left_to_left(double value) {
        data_->left_to_left = value;
    }

    /// Get the left-to-right mixing value
    /// @return The left-to-right mixing coefficient (0.0 to 1.0)
    double left_to_right() const {
        return data_->left_to_right;
    }

    /// Set the left-to-right mixing value
    /// @param value The new left-to-right mixing coefficient
    void set_left_to_right(double value) {
        data_->left_to_right = value;
    }

    /// Get the right-to-left mixing value
    /// @return The right-to-left mixing coefficient (0.0 to 1.0)
    double right_to_left() const {
        return data_->right_to_left;
    }

    /// Set the right-to-left mixing value
    /// @param value The new right-to-left mixing coefficient
    void set_right_to_left(double value) {
        data_->right_to_left = value;
    }

    /// Get the right-to-right mixing value
    /// @return The right-to-right mixing coefficient (0.0 to 1.0)
    double right_to_right() const {
        return data_->right_to_right;
    }

    /// Set the right-to-right mixing value
    /// @param value The new right-to-right mixing coefficient
    void set_right_to_right(double value) {
        data_->right_to_right = value;
    }

    /// Get the volume level
    /// @return The volume level (0.0 to 1.0)
    double volume() const {
        return data_->volume;
    }

    /// Set the volume level
    /// @param value The new volume level
    void set_volume(double value) {
        data_->volume = value;
    }

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to SoundTransformObject
template<typename GC>
class SoundTransformObjectWeak {
public:
    SoundTransformObjectWeak() = default;
    explicit SoundTransformObjectWeak(GcWeak<GC, SoundTransformObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, SoundTransformObjectData<GC>> data_;
};

/// SoundTransform object allocator function
/// 
/// Creates a new SoundTransform object with default values (all zeros)
/// 
/// @param class_obj The class object for SoundTransform
/// @param activation The current activation context
/// @return Result containing the new SoundTransformObject or error
template<typename GC>
Object<GC> sound_transform_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation);

// Template method implementations

template<typename GC>
Object<GC> sound_transform_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation) {
    auto base = ScriptObjectData<GC>::create(class_obj);
    auto data = Gc<GC, SoundTransformObjectData<GC>>::create(
        activation->gc(),
        SoundTransformObjectData<GC>{
            .base = base,
            .left_to_left = 0.0,
            .left_to_right = 0.0,
            .right_to_left = 0.0,
            .right_to_right = 0.0,
            .volume = 0.0
        }
    );
    return Object<GC>(SoundTransformObject<GC>(data));
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::SoundTransformObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::SoundTransformObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), 
            "SoundTransformObject(ptr={}, L->L={:.2}, L->R={:.2}, R->L={:.2}, R->R={:.2}, vol={:.2})",
            static_cast<const void*>(obj.data_.get()),
            obj.left_to_left(), obj.left_to_right(),
            obj.right_to_left(), obj.right_to_right(),
            obj.volume());
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_SOUNDTRANSFORM_OBJECT_H
