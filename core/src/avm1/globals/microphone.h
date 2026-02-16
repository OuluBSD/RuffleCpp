// C++ translation of core/src/avm1/globals/microphone.rs
// Microphone object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_MICROPHONE_H
#define RUFFLE_CORE_AVM1_GLOBALS_MICROPHONE_H

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace microphone {

/// Property declarations for Microphone prototype
extern const StaticDeclarations PROTO_DECLS;

/// Property declarations for Microphone object (static methods/properties)
extern const StaticDeclarations OBJECT_DECLS;

/// Create the Microphone class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created Microphone class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// Implements Microphone.get method
///
/// Returns null when there's no microphone available.
///
/// \param activation The current activation context
/// \param this_obj The Microphone object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Null - stub implementation
template<typename GC>
Value<GC> get(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Microphone.names property getter
///
/// \param activation The current activation context
/// \param this_obj The Microphone object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Empty array - stub implementation
template<typename GC>
Value<GC> get_names(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Microphone.setSilenceLevel method
///
/// \param activation The current activation context
/// \param this_obj The Microphone object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Undefined - stub implementation
template<typename GC>
Value<GC> set_silence_level(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Microphone.setRate method
///
/// \param activation The current activation context
/// \param this_obj The Microphone object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Undefined - stub implementation
template<typename GC>
Value<GC> set_rate(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Microphone.setGain method
///
/// \param activation The current activation context
/// \param this_obj The Microphone object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Undefined - stub implementation
template<typename GC>
Value<GC> set_gain(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Microphone.setUseEchoSuppression method
///
/// \param activation The current activation context
/// \param this_obj The Microphone object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Undefined - stub implementation
template<typename GC>
Value<GC> set_use_echo_suppression(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Microphone.setCodec method
///
/// \param activation The current activation context
/// \param this_obj The Microphone object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Undefined - stub implementation
template<typename GC>
Value<GC> set_codec(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Microphone.setFramesPerPacket method
///
/// \param activation The current activation context
/// \param this_obj The Microphone object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Undefined - stub implementation
template<typename GC>
Value<GC> set_frames_per_packet(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Microphone.setEncodeQuality method
///
/// \param activation The current activation context
/// \param this_obj The Microphone object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Value::Undefined - stub implementation
template<typename GC>
Value<GC> set_encode_quality(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace microphone
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_MICROPHONE_H
