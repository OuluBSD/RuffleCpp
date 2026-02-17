// C++ translation of core/src/avm2/globals/flash/media/sound.rs
//! `flash.media.Sound` builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_MEDIA_SOUND_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_MEDIA_SOUND_H

#include <vector>
#include <optional>
#include <memory>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class Activation;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    template<typename GC>
    class EventObject;
    template<typename GC>
    class ClassObject;
    template<typename GC>
    class Object;
    template<typename GC>
    class SoundObject;
    template<typename GC>
    class SoundChannelObject;
    template<typename GC>
    class UpdateContext;
    template<typename GC>
    class ByteArrayObject;
    template<typename GC>
    class AvmString;

    namespace backend {
        namespace navigator {
            struct Request;
        }
    }

    namespace character {
        enum class Character;
    }

    namespace display_object {
        struct SoundTransform;
    }

    namespace gc_arena {
        template<typename T>
        class Gc;
        class Mutation;
    }

    namespace loader {
        // Sound loading future
    }
}
}

namespace swf {
struct SoundInfo;
enum class SoundEvent;
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace media {
namespace sound {

using gc_arena::Gc;
using gc_arena::Mutation;

// Re-export the allocator from sound_object
using ruffle::core::avm2::sound_allocator;

/// Implements `flash.media.Sound`'s 'init' method, which is called from the constructor.
template<typename GC>
Value<GC> init(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `Sound.bytesTotal`
template<typename GC>
Value<GC> get_bytes_total(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `Sound.bytesLoaded`
template<typename GC>
Value<GC> get_bytes_loaded(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `Sound.isBuffering`
template<typename GC>
Value<GC> get_is_buffering(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `Sound.isURLInaccessible`
template<typename GC>
Value<GC> get_is_url_inaccessible(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `Sound.url`
template<typename GC>
Value<GC> get_url(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `Sound.length`
template<typename GC>
Value<GC> get_length(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `Sound.play`
template<typename GC>
Value<GC> play(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// `Sound.extract`
template<typename GC>
Value<GC> extract(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// `Sound.close`
template<typename GC>
Value<GC> close(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// `Sound.load`
template<typename GC>
Value<GC> load(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// `Sound.loadCompressedDataFromByteArray`
template<typename GC>
Value<GC> load_compressed_data_from_byte_array(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// `Sound.loadPCMFromByteArray`
template<typename GC>
Value<GC> load_pcm_from_byte_array(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `Sound.id3`
template<typename GC>
Value<GC> get_id3(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

// Template method implementations

template<typename GC>
Value<GC> init(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    if (auto sound_object = this_val.as_object()->as_sound_object()) {
        auto class_def = this_val.instance_class(activation);

        if (auto symbol_info = activation->context->library->avm2_class_registry()->class_symbol(class_def)) {
            auto [movie, symbol] = symbol_info.value();
            
            if (auto character = activation->context->library
                ->library_for_movie_mut(movie)
                ->character_by_id(symbol)) {
                if (character->is_sound()) {
                    sound_object->set_sound(activation->context, character->as_sound());
                } else {
                    // tracing::warn!("Attempted to construct subclass of Sound, {}, which is associated with non-Sound character {}",
                    //     class_def.name().local_name(), symbol);
                }
            }
        }
    }

    if (args.try_get_object(0).has_value()) {
        this_val.call_method(sound_methods::LOAD, args, activation);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_bytes_total(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto sound = this_obj->as_sound_object()) {
        if (auto sound_handle = sound->sound_handle()) {
            if (auto length = activation->context->audio.get_sound_size(sound_handle.value())) {
                return Value<GC>::from_int(static_cast<int32_t>(length.value()));
            }
        }
        return Value<GC>::from_int(0);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_bytes_loaded(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // This should have a different value from bytesTotal when the sound is loading.
    // STUB: We do not yet support network-loaded sounds.
    return get_bytes_total(activation, this_val, args);
}

template<typename GC>
Value<GC> get_is_buffering(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // STUB: We do not yet support network-loaded sounds.
    return Value<GC>::from_bool(false);
}

template<typename GC>
Value<GC> get_is_url_inaccessible(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // STUB: We do not yet support network-loaded sounds.
    return Value<GC>::from_bool(false);
}

template<typename GC>
Value<GC> get_url(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // STUB: We do not yet support network-loaded sounds.
    return Value<GC>::null();
}

template<typename GC>
Value<GC> get_length(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto sound = this_obj->as_sound_object()) {
        if (auto sound_handle = sound->sound_handle()) {
            if (auto duration = activation->context->audio.get_sound_duration(sound_handle.value())) {
                return Value<GC>::from_number(duration.value());
            }
        }
        return Value<GC>::from_number(0.0);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> play(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto sound_object = this_obj->as_sound_object()) {
        auto position = args.get_f64(0);
        auto num_loops = args.get_i32(1);
        auto sound_transform = args.try_get_object(2);

        auto in_sample = position > 0.0 
            ? std::optional(static_cast<uint32_t>(position / 1000.0 * 44100.0))
            : std::nullopt;

        auto sound_info = swf::SoundInfo{
            swf::SoundEvent::Start,
            in_sample,
            std::nullopt,
            static_cast<uint16_t>(std::max(num_loops, 1)),
            std::nullopt
        };

        std::optional<display_object::SoundTransform> st;
        if (sound_transform.has_value()) {
            st = display_object::SoundTransform::from_avm2_object(sound_transform.value());
        }

        auto sound_channel = SoundChannelObject<GC>::empty(activation);

        auto queued_play = QueuedPlay<GC>{
            position,
            sound_info,
            st,
            sound_channel
        };
        
        if (sound_object->play(queued_play, activation)) {
            return Value<GC>::from_object(sound_channel.into());
        }
        
        // If we start playing a loaded sound with an invalid position,
        // this method returns null
        return Value<GC>::null();
    }

    return Value<GC>::null();
}

template<typename GC>
Value<GC> extract(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // STUB
    auto bytearray = args.try_get_object(0);
    auto length = args.get_f64(1);

    if (bytearray.has_value()) {
        if (auto ba = bytearray.value()->as_bytearray_mut()) {
            std::vector<uint8_t> zeros(static_cast<size_t>(std::ceil(length)), 0);
            ba->write_bytes(zeros);
        }
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> close(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // STUB
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> load(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();
    auto sound = this_obj->as_sound_object().value();
    
    if (sound->loading_state() != SoundLoadingState::New) {
        // Return error 2037
        return Value<GC>::undefined(); // Placeholder for error
    }

    auto url_request = args.try_get_object(0);
    // FP ignores calls of load(null)
    if (!url_request.has_value()) {
        return Value<GC>::undefined();
    }

    constexpr uint32_t URL_SLOT = 0; // url_request_slots::_URL placeholder
    auto url = url_request.value()->get_slot(URL_SLOT).coerce_to_string(activation).value();

    // TODO: context parameter currently unused.
    auto sound_context = args.try_get_object(1);
    if (sound_context.has_value()) {
        // STUB: with context
    }

    auto future = loader::load_sound_avm2(
        activation->context,
        sound,
        backend::navigator::Request::get(url->to_string())
    );
    activation->context->navigator.spawn_future(future);
    sound->set_loading_state(SoundLoadingState::Loading);

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> load_compressed_data_from_byte_array(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();
    auto sound = this_obj->as_sound_object().value();
    
    if (sound->loading_state() == SoundLoadingState::Loaded) {
        return Value<GC>::undefined();
    }

    auto bytearray = args.get_object(activation, 0, "bytes").value();
    auto bytes_length = args.get_u32(1);
    auto ba = bytearray->as_bytearray().value();

    auto bytes = ba->read_bytes(static_cast<size_t>(bytes_length));
    if (!bytes.has_value()) {
        // This is the error Flash throws (error 2084)
        return Value<GC>::undefined(); // Placeholder for error
    }

    auto handle = activation->context->audio.register_mp3(bytes.value());
    if (!handle.has_value()) {
        return Value<GC>::undefined(); // Placeholder for error
    }

    auto progress_evt = EventObject<GC>::progress_event(
        activation, 
        "progress", 
        static_cast<uint32_t>(bytes.value().size()), 
        static_cast<uint32_t>(bytes.value().size())
    );

    Avm2<GC>::dispatch_event(activation->context, progress_evt, this_obj);

    sound->read_and_call_id3_event(activation, bytes.value());
    sound->set_sound(activation->context, handle.value());

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> load_pcm_from_byte_array(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();
    auto sound = this_obj->as_sound_object().value();
    
    if (sound->loading_state() == SoundLoadingState::Loaded) {
        return Value<GC>::undefined();
    }

    // TODO: Add proper implementation.
    // The following line ensures proper behavior when calling load multiple times.
    sound->set_loading_state(SoundLoadingState::Loaded);
    
    // STUB
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_id3(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto id3 = this_obj->as_sound_object()->id3()) {
        return Value<GC>::from_object(id3.value());
    }
    
    return Value<GC>::null();
}

} // namespace sound
} // namespace media
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_MEDIA_SOUND_H
