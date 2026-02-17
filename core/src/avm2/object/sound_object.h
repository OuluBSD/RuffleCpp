// C++ translation of core/src/avm2/object/sound_object.rs
//! Object representation for sounds

#ifndef RUFFLE_CORE_AVM2_OBJECT_SOUND_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_SOUND_OBJECT_H

#include <cstdint>
#include <vector>
#include <optional>
#include <memory>
#include <variant>
#include <cell>
#include <mutex>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class AvmString;
    template<typename GC>
    class Activation;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    template<typename GC>
    class EventObject;
    template<typename GC>
    class ScriptObjectData;
    template<typename GC>
    class TObject;
    template<typename GC>
    class ClassObject;
    template<typename GC>
    class Object;
    template<typename GC>
    class SoundChannelObject;
    template<typename GC>
    class UpdateContext;
    template<typename GC>
    class Mutation;

    namespace backend {
        namespace audio {
            using SoundHandle = uint64_t; // Placeholder type
        }
    }

    namespace display_object {
        struct SoundTransform;
    }

    namespace gc_arena {
        template<typename T>
        class Gc;
        template<typename T>
        class GcWeak;
        template<typename T>
        class Rootable;
        class Mutation;
        template<typename T>
        class DynamicRoot;
        template<typename T>
        class Lock;
        template<typename T>
        class RefLock;
    }

    namespace swf {
        struct SoundInfo;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

using gc_arena::Gc;
using gc_arena::GcWeak;
using gc_arena::Mutation;
using gc_arena::DynamicRoot;
using gc_arena::Rootable;
using gc_arena::Lock;
using gc_arena::RefLock;
using backend::audio::SoundHandle;

/// Loading state of the sound.
enum class SoundLoadingState {
    New,
    Loading,
    Loaded
};

/// Queued play information for sounds not yet loaded
template<typename GC>
struct QueuedPlay {
    swf::SoundInfo sound_info;
    std::optional<display_object::SoundTransform> sound_transform;
    SoundChannelObject<GC> sound_channel;
    double position;
};

/// Sound data - either not loaded or loaded
template<typename GC>
class SoundData {
public:
    struct NotLoaded {
        std::vector<QueuedPlay<GC>> queued_plays;
    };

    struct Loaded {
        SoundHandle sound;
    };

    using Variant = std::variant<NotLoaded, Loaded>;
    Variant data;

    SoundData() : data(NotLoaded{}) {}
};

/// Weak reference to SoundObject data
template<typename GC>
class SoundObjectWeak;

/// Handle for SoundObject that can be stashed across GC mutations
template<typename GC>
class SoundObjectHandle;

/// Data structure for SoundObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class SoundObjectData {
public:
    SoundObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    // Loading state of the sound
    std::cell::Cell<SoundLoadingState> loading_state;

    // The sound this object holds
    RefLock<SoundData<GC>> sound_data;

    // ID3Info Object
    Lock<std::optional<Object<GC>>> id3;

    // Friend classes for accessing private members
    friend class SoundObject<GC>;
    friend class SoundObjectWeak<GC>;
    friend class SoundObjectHandle<GC>;
};

/// A class instance allocator that allocates Sound objects.
/// @param class_object The class object for Sound
/// @param activation The current activation
/// @return New SoundObject
template<typename GC>
Object<GC> sound_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
);

/// SoundObject - Object representation for sounds
template<typename GC>
class SoundObject {
public:
    using DataType = SoundObjectData<GC>;
    using WeakType = SoundObjectWeak<GC>;
    using HandleType = SoundObjectHandle<GC>;

    SoundObject() = default;
    explicit SoundObject(Gc<GC, DataType> data) : data_(data) {}

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

    /// Get the sound handle if loaded
    std::optional<SoundHandle> sound_handle() const {
        const auto& sound_data = data_->sound_data.get();
        if (std::holds_alternative<typename SoundData<GC>::Loaded>(sound_data.data)) {
            return std::get<typename SoundData<GC>::Loaded>(sound_data.data).sound;
        }
        return std::nullopt;
    }

    /// Get the loading state
    SoundLoadingState loading_state() const {
        return data_->loading_state.get();
    }

    /// Set the loading state
    void set_loading_state(SoundLoadingState value) {
        data_->loading_state.set(value);
    }

    /// Play the sound, queuing if not loaded
    /// @param queued The queued play information
    /// @param activation The current activation
    /// @return true if a SoundChannel should be returned
    bool play(QueuedPlay<GC> queued, Activation<GC>* activation);

    /// Set the sound handle when loaded
    /// @param context The update context
    /// @param sound The sound handle
    void set_sound(UpdateContext<GC>* context, SoundHandle sound);

    /// Get the ID3 object
    std::optional<Object<GC>> id3() const {
        return data_->id3.get();
    }

    /// Set the ID3 object
    void set_id3(Mutation<GC>* mc, std::optional<Object<GC>> id3);

    /// Read ID3 data from bytes and dispatch id3 event
    void read_and_call_id3_event(Activation<GC>* activation, const std::vector<uint8_t>& bytes);

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to SoundObject
template<typename GC>
class SoundObjectWeak {
public:
    SoundObjectWeak() = default;
    explicit SoundObjectWeak(GcWeak<GC, SoundObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, SoundObjectData<GC>> data_;
};

/// Handle for stashing SoundObject across GC mutations
template<typename GC>
class SoundObjectHandle {
private:
    DynamicRoot<Rootable<SoundObjectData<GC>>> root_;

public:
    /// Stash a SoundObject for later retrieval
    static SoundObjectHandle stash(UpdateContext<GC>* context, SoundObject<GC> obj) {
        return SoundObjectHandle{context->dynamic_root.stash(context->gc(), obj.data_)};
    }

    /// Fetch a previously stashed SoundObject
    SoundObject<GC> fetch(UpdateContext<GC>* context) const {
        return SoundObject<GC>{context->dynamic_root.fetch(&root_)};
    }
};

// Template method implementations

template<typename GC>
Object<GC> sound_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
) {
    auto base = ScriptObjectData<GC>::new_instance(class_object);

    auto data = Gc<GC, SoundObjectData<GC>>::create(
        activation->gc(),
        SoundObjectData<GC>{
            base,
            std::cell::Cell<SoundLoadingState>(SoundLoadingState::New),
            RefLock<SoundData<GC>>(SoundData<GC>{}),
            Lock<std::optional<Object<GC>>>(std::nullopt)
        }
    );

    return Object<GC>::from_sound_object(SoundObject<GC>(data));
}

template<typename GC>
bool SoundObject<GC>::play(QueuedPlay<GC> queued, Activation<GC>* activation) {
    auto& sound_data = data_->sound_data.get();
    
    if (std::holds_alternative<typename SoundData<GC>::NotLoaded>(sound_data.data)) {
        auto& not_loaded = std::get<typename SoundData<GC>::NotLoaded>(sound_data.data);
        
        // Avoid queuing more unloaded sounds than the maximum allowed
        constexpr size_t MAX_SOUNDS = 256; // AudioManager::MAX_SOUNDS placeholder
        if (not_loaded.queued_plays.size() >= MAX_SOUNDS) {
            // tracing::warn!("Sound.play: too many unloaded sounds queued");
            return false;
        }

        not_loaded.queued_plays.push_back(queued);
        return true; // We don't know the length yet, return the SoundChannel
    } else {
        auto& loaded = std::get<typename SoundData<GC>::Loaded>(sound_data.data);
        return play_queued(queued, loaded.sound, activation->context);
    }
}

template<typename GC>
void SoundObject<GC>::set_sound(UpdateContext<GC>* context, SoundHandle sound) {
    auto& sound_data = data_->sound_data.get();
    
    if (std::holds_alternative<typename SoundData<GC>::NotLoaded>(sound_data.data)) {
        auto& not_loaded = std::get<typename SoundData<GC>::NotLoaded>(sound_data.data);
        
        for (auto& queued : not_loaded.queued_plays) {
            play_queued(queued, sound, context);
        }
        
        sound_data.data = typename SoundData<GC>::Loaded{sound};
    } else {
        // Should not happen - panic in debug
        // panic!("Tried to replace sound");
    }
    
    set_loading_state(SoundLoadingState::Loaded);
}

template<typename GC>
void SoundObject<GC>::set_id3(Mutation<GC>* mc, std::optional<Object<GC>> id3) {
    data_->id3.set(id3);
}

template<typename GC>
void SoundObject<GC>::read_and_call_id3_event(Activation<GC>* activation, const std::vector<uint8_t>& bytes) {
    // Stub - ID3 parsing would go here
    // This would create an ID3Info object and set its slots
    // Then dispatch an "id3" event
}

// Helper function for playing queued sounds
template<typename GC>
bool play_queued(QueuedPlay<GC> queued, SoundHandle sound, UpdateContext<GC>* context) {
    // Check if position is valid
    // Start the sound
    // Attach the sound channel
    return true;
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::SoundObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::SoundObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "SoundObject(ptr={:p})",
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_SOUND_OBJECT_H
