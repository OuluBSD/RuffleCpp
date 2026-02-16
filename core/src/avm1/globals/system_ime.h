// C++ translation of core/src/avm1/globals/system_ime.rs
// System.IME class for input method editor

#ifndef RUFFLE_CORE_AVM1_GLOBALS_SYSTEM_IME_H
#define RUFFLE_CORE_AVM1_GLOBALS_SYSTEM_IME_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Object;
    template<typename T> class Value;
    template<typename T> class Error;
    struct DeclContext;
    struct StaticDeclarations;
    namespace globals {
        namespace as_broadcaster {
            template<typename T> struct BroadcasterFunctions;
        }
    }
}
namespace string {
    template<typename T> class AvmString;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace system_ime {

/// IME conversion mode constants
namespace conversion_mode {
    constexpr const char* UNKNOWN = "UNKNOWN";
    constexpr const char* KOREAN = "KOREAN";
    constexpr const char* JAPANESE_KATAKANA_HALF = "JAPANESE_KATAKANA_HALF";
    constexpr const char* JAPANESE_KATAKANA_FULL = "JAPANESE_KATAKANA_FULL";
    constexpr const char* JAPANESE_HIRAGANA = "JAPANESE_HIRAGANA";
    constexpr const char* CHINESE = "CHINESE";
    constexpr const char* ALPHANUMERIC_HALF = "ALPHANUMERIC_HALF";
    constexpr const char* ALPHANUMERIC_FULL = "ALPHANUMERIC_FULL";
}

// Property declarations for System.IME object
extern const StaticDeclarations OBJECT_DECLS;

/// Create the System.IME object
template<typename GC>
Object<GC>* create(
    DeclContext<GC>* context,
    globals::as_broadcaster::BroadcasterFunctions<GC> broadcaster_functions,
    Object<GC>* array_proto
);

/// System.IME.onIMEComposition handler
template<typename GC>
Value<GC> on_ime_composition(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.IME.doConversion
template<typename GC>
Value<GC> do_conversion(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.IME.getConversionMode
template<typename GC>
Value<GC> get_conversion_mode(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.IME.getEnabled
template<typename GC>
Value<GC> get_enabled(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.IME.setCompositionString
template<typename GC>
Value<GC> set_composition_string(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.IME.setConversionMode
template<typename GC>
Value<GC> set_conversion_mode(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.IME.setEnabled
template<typename GC>
Value<GC> set_enabled(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace system_ime
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_SYSTEM_IME_H
