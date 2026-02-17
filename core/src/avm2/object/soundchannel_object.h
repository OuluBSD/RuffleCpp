// C++ translation of core/src/avm2/object/soundchannel_object.rs
//! Object representation for sounds

#ifndef RUFFLE_CORE_AVM2_OBJECT_SOUNDCHANNEL_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_SOUNDCHANNEL_OBJECT_H

#include <memory>
#include <format>
#include <cell>
#include <variant>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
    class UpdateContext;
    class SoundTransform;

    namespace backend {
        namespace audio {
            using SoundInstanceHandle = uint64_t; // Placeholder type
        }
    }

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
}
}

namespace ruffle {
namespace core {
namespace avm2 {

using backend::audio::SoundInstanceHandle;

/// Sound channel data - represents the state of a sound channel
enum class SoundChannelData {
    NotLoaded,
    Loaded
};

/// Detailed sound channel data structure
struct SoundChannelDataDetail {
    std::variant<
        std::monostate,  // NotLoaded state
        SoundInstanceHandle  // Loaded state
    > data;

    // For NotLoaded state
    std::optional<SoundTransform> sound_transform;
    bool should_stop = false;

    // For Loaded state
    SoundInstanceHandle sound_instance = 0;

    static SoundChannelDataDetail not_loaded(std::optional<SoundTransform> transform = std::nullopt) {
        SoundChannelDataDetail detail;
        detail.data = std::monostate{};
        detail.sound_transform = transform;
        detail.should_stop = false;
        return detail;
    }

    static SoundChannelDataDetail loaded(SoundInstanceHandle instance) {
        SoundChannelDataDetail detail;
        detail.data = instance;
        return detail;
    }

    bool is_loaded() const {
        return std::holds_alternative<SoundInstanceHandle>(data);
    }

    SoundInstanceHandle get_instance() const {
        if (auto* instance = std::get_if<SoundInstanceHandle>(&data)) {
            return *instance;
        }
        return 0;
    }
};

/// Weak reference to SoundChannelObject data
template<typename GC>
class SoundChannelObjectWeak;

/// Data structure for SoundChannelObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class SoundChannelObjectData {
public:
    SoundChannelObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// The sound this object holds.
    mutable std::cell::RefCell<SoundChannelDataDetail> sound_channel_data;

    /// Position of the last playing sound in milliseconds.
    mutable std::cell::Cell<double> position;

    // Friend classes for accessing private members
    friend class SoundChannelObject<GC>;
    friend class SoundChannelObjectWeak<GC>;
};

/// SoundChannelObject - AVM2 SoundChannel object representation
/// Represents a SoundChannel instance in ActionScript 3
template<typename GC>
class SoundChannelObject {
public:
    using DataType = SoundChannelObjectData<GC>;
    using WeakType = SoundChannelObjectWeak<GC>;

    SoundChannelObject() = default;
    explicit SoundChannelObject(Gc<GC, DataType> data) : data_(data) {}

    /// A class instance allocator that allocates SoundChannel objects.
    ///
    /// @param class_object The class object for SoundChannel
    /// @param activation The current activation
    /// @return New SoundChannelObject
    static Object<GC> sound_channel_allocator(
        ClassObject<GC> class_object,
        Activation<GC>* activation
    );

    /// Create an empty SoundChannel instance.
    ///
    /// @param activation The current activation
    /// @return New SoundChannelObject
    static SoundChannelObject empty(Activation<GC>* activation);

    /// Return the position of the playing sound in seconds.
    ///
    /// @param context The update context
    /// @return Position in seconds
    double position(UpdateContext<GC>* context) const;

    /// Get the sound instance handle if loaded.
    /// @return Sound instance handle or none
    std::optional<SoundInstanceHandle> instance() const;

    /// Set the sound instance.
    ///
    /// @param context The update context
    /// @param instance The sound instance handle
    void set_sound_instance(UpdateContext<GC>* context, SoundInstanceHandle instance);

    /// Get the sound transform.
    ///
    /// @param activation The current activation
    /// @return Sound transform or none
    std::optional<SoundTransform> sound_transform(Activation<GC>* activation) const;

    /// Set the sound transform.
    ///
    /// @param activation The current activation
    /// @param new_sound_transform The new sound transform
    void set_sound_transform(Activation<GC>* activation, SoundTransform new_sound_transform);

    /// Stop the sound.
    ///
    /// @param activation The current activation
    void stop(Activation<GC>* activation);

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to SoundChannelObject
template<typename GC>
class SoundChannelObjectWeak {
public:
    SoundChannelObjectWeak() = default;
    explicit SoundChannelObjectWeak(GcWeak<GC, SoundChannelObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, SoundChannelObjectData<GC>> data_;
};

// Template method implementations

template<typename GC>
Object<GC> SoundChannelObject<GC>::sound_channel_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
) {
    auto base = ScriptObjectData<GC>::new_instance(class_object);

    auto data = Gc<GC, DataType>::create(
        activation->gc(),
        DataType{
            base,
            std::cell::RefCell<SoundChannelDataDetail>::new(
                SoundChannelDataDetail::not_loaded()
            ),
            std::cell::Cell<double>::new(0.0)
        }
    );

    return Object<GC>::from_soundchannel_object(SoundChannelObject<GC>(data));
}

template<typename GC>
SoundChannelObject<GC> SoundChannelObject<GC>::empty(Activation<GC>* activation) {
    auto class_object = activation->avm2()->classes()->soundchannel;
    auto base = ScriptObjectData<GC>::new_instance(class_object);

    return SoundChannelObject<GC>(Gc<GC, DataType>::create(
        activation->gc(),
        DataType{
            base,
            std::cell::RefCell<SoundChannelDataDetail>::new(
                SoundChannelDataDetail::not_loaded()
            ),
            std::cell::Cell<double>::new(0.0)
        }
    ));
}

template<typename GC>
double SoundChannelObject<GC>::position(UpdateContext<GC>* context) const {
    // The position is cached on read. This means that if the position isn't read until after
    // the sound has played, the position will be 0 (#9952).
    auto sound_channel_data = data_->sound_channel_data.borrow();
    if (sound_channel_data.is_loaded()) {
        if (auto pos = context->audio->get_sound_position(sound_channel_data.get_instance())) {
            data_->position.set(pos.value());
        }
    }

    return data_->position.get();
}

template<typename GC>
std::optional<SoundInstanceHandle> SoundChannelObject<GC>::instance() const {
    auto sound_channel_data = data_->sound_channel_data.borrow();
    if (sound_channel_data.is_loaded()) {
        return sound_channel_data.get_instance();
    }
    return std::nullopt;
}

template<typename GC>
void SoundChannelObject<GC>::set_sound_instance(UpdateContext<GC>* context, SoundInstanceHandle instance) {
    auto sound_channel_data = data_->sound_channel_data.borrow_mut();
    if (!sound_channel_data.is_loaded()) {
        // NotLoaded state
        if (sound_channel_data.sound_transform.has_value()) {
            context->set_local_sound_transform(instance, sound_channel_data.sound_transform.value());
        }

        if (sound_channel_data.should_stop) {
            context->stop_sound(instance);
        }

        sound_channel_data.data = instance;
    } else {
        // Already loaded - panic
        throw std::runtime_error("Tried to replace loaded sound instance");
    }
}

template<typename GC>
std::optional<SoundTransform> SoundChannelObject<GC>::sound_transform(Activation<GC>* activation) const {
    auto sound_channel_data = data_->sound_channel_data.borrow();
    if (!sound_channel_data.is_loaded()) {
        return sound_channel_data.sound_transform;
    } else {
        return activation->context->local_sound_transform(sound_channel_data.get_instance());
    }
}

template<typename GC>
void SoundChannelObject<GC>::set_sound_transform(Activation<GC>* activation, SoundTransform new_sound_transform) {
    auto sound_channel_data = data_->sound_channel_data.borrow_mut();
    if (!sound_channel_data.is_loaded()) {
        sound_channel_data.sound_transform = new_sound_transform;
    } else {
        activation->context->set_local_sound_transform(
            sound_channel_data.get_instance(),
            new_sound_transform
        );
    }
}

template<typename GC>
void SoundChannelObject<GC>::stop(Activation<GC>* activation) {
    auto sound_channel_data = data_->sound_channel_data.borrow_mut();
    if (!sound_channel_data.is_loaded()) {
        sound_channel_data.should_stop = true;
    } else {
        activation->context->stop_sound(sound_channel_data.get_instance());
    }
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::SoundChannelObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::SoundChannelObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "SoundChannelObject(ptr={:p})",
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_SOUNDCHANNEL_OBJECT_H
