// C++ translation of core/src/avm2/globals/flash/net/net_stream.rs
//! flash.net.NetStream implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_NET_STREAM_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_NET_STREAM_H

#include <vector>
#include <optional>
#include <string>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class Object;

    namespace object {
        template<typename GC>
        using netstream_allocator = /* would come from object module */ void;
    }
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace net {
namespace net_stream {

/// Re-export netstream_allocator from object module
template<typename GC>
using netstream_allocator = ruffle::core::avm2::object::netstream_allocator<GC>;

/// Implements NetStream.bytesLoaded getter
template<typename GC>
Value<GC> get_bytes_loaded(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements NetStream.bytesTotal getter
template<typename GC>
Value<GC> get_bytes_total(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements NetStream.play
template<typename GC>
Value<GC> play(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements NetStream.pause
template<typename GC>
Value<GC> pause(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements NetStream.resume
template<typename GC>
Value<GC> resume(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements NetStream.togglePause
template<typename GC>
Value<GC> toggle_pause(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements NetStream.client getter
template<typename GC>
Value<GC> get_client(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements NetStream.client setter
template<typename GC>
Value<GC> set_client(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements NetStream.seek
template<typename GC>
Value<GC> seek(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements NetStream.time getter
template<typename GC>
Value<GC> get_time(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace net_stream
} // namespace net
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_NET_STREAM_H
