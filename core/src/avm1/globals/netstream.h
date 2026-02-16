// C++ translation of core/src/avm1/globals/netstream.rs
// NetStream class implementation

#ifndef RUFFLE_CORE_AVM1_GLOBALS_NETSTREAM_H
#define RUFFLE_CORE_AVM1_GLOBALS_NETSTREAM_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Object;
    template<typename T> class Value;
    template<typename T> class Error;
    enum class NativeObject;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
}
namespace streams {
    template<typename T> class NetStream;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace netstream {

// Property declarations for NetStream prototype
extern const StaticDeclarations PROTO_DECLS;

/// NetStream constructor
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Create the NetStream class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// NetStream.bufferLength getter
template<typename GC>
Value<GC> get_buffer_length(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.bufferTime getter
template<typename GC>
Value<GC> get_buffer_time(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.bytesLoaded getter
template<typename GC>
Value<GC> get_bytes_loaded(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.bytesTotal getter
template<typename GC>
Value<GC> get_bytes_total(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.play
template<typename GC>
Value<GC> play(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.pause
template<typename GC>
Value<GC> pause(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.seek
template<typename GC>
Value<GC> seek(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.setBufferTime
template<typename GC>
Value<GC> set_buffer_time(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.time getter
template<typename GC>
Value<GC> get_time(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.publish
template<typename GC>
Value<GC> publish(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.play2
template<typename GC>
Value<GC> play2(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.receiveAudio
template<typename GC>
Value<GC> receive_audio(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.receiveVideo
template<typename GC>
Value<GC> receive_video(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.onPeerConnect
template<typename GC>
Value<GC> on_peer_connect(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.close
template<typename GC>
Value<GC> close(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.attachAudio
template<typename GC>
Value<GC> attach_audio(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.attachVideo
template<typename GC>
Value<GC> attach_video(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.send
template<typename GC>
Value<GC> send(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.getInfo
template<typename GC>
Value<GC> get_info(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.checkPolicyFile
template<typename GC>
Value<GC> check_policy_file(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.maxPauseBufferTime
template<typename GC>
Value<GC> max_pause_buffer_time(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// NetStream.backBufferTime
template<typename GC>
Value<GC> back_buffer_time(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace netstream
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_NETSTREAM_H
