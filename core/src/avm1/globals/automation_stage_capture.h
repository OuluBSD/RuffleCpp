// C++ translation of core/src/avm1/globals/automation_stage_capture.rs
// flash.automation.StageCapture class

#ifndef RUFFLE_CORE_AVM1_GLOBALS_AUTOMATION_STAGE_CAPTURE_H
#define RUFFLE_CORE_AVM1_GLOBALS_AUTOMATION_STAGE_CAPTURE_H

#include <cstdint>

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
namespace automation_stage_capture {

// Method indices for StageCapture
namespace method {
    constexpr uint16_t CAPTURE = 0;
    constexpr uint16_t CANCEL = 1;
    constexpr uint16_t GET_FILE_NAME_BASE = 2;
    constexpr uint16_t SET_FILE_NAME_BASE = 3;
    constexpr uint16_t GET_CLIP_RECT = 4;
    constexpr uint16_t SET_CLIP_RECT = 5;
    constexpr uint16_t LISTEN_FOR_STAGE_CAPTURE = 6;
    constexpr uint16_t VALUE_OF = 7;
    constexpr uint16_t TO_STRING = 8;
    constexpr uint16_t CONSTRUCTOR = 100;
}

// Property declarations for StageCapture prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the StageCapture class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created StageCapture class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// StageCapture method handler
///
/// Handles all methods for the StageCapture class.
/// All methods are currently stubs that return Value::Undefined.
///
/// \param activation The current activation context
/// \param this_obj The StageCapture object
/// \param args Arguments passed to the method
/// \param index The method index to call
/// \return Value::Undefined for all stub methods, or this_obj for constructor
template<typename GC>
Value<GC> method(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count,
    uint16_t index
);

} // namespace automation_stage_capture
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_AUTOMATION_STAGE_CAPTURE_H
