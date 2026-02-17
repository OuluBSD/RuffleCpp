// C++ translation of core/src/avm1.rs
// AVM1 module declarations

#ifndef RUFFLE_CORE_AVM1_H
#define RUFFLE_CORE_AVM1_H

// Standard includes
#include <string>
#include <memory>

// Forward declarations for AVM1 submodules
namespace ruffle {
namespace core {
namespace avm1 {

// Submodule namespaces (defined in respective headers)
namespace activation {
    class Activation;
    struct ActivationIdentifier;
}

namespace callable_value {
    // Callable value handling
}

namespace clamp {
    // Clamping utilities
}

namespace debug {
    class VariableDumper;
}

namespace error {
    class Error;
}

namespace flv {
    // FLV value extensions
}

namespace fscommand {
    // FSCommand handling
}

namespace globals {
    namespace array {
        class ArrayBuilder;
    }
    namespace context_menu {
        // Context menu state
    }
    namespace sound {
        // Sound functions
    }
}

namespace object {
    class NativeObject;
    class Object;
    using ObjectHandle = std::shared_ptr<Object>;
    using ObjectPtr = Object*;
}

namespace property {
    class Attribute;
}

namespace property_map {
    class PropertyMap;
}

namespace runtime {
    class Avm1;
}

namespace value {
    class Value;
}

namespace function {
    enum class ExecutionReason {
        Normal,
        Return,
        Throw,
        Break,
        Continue
    };
}

// Re-exports from submodules
using Activation = activation::Activation;
using ActivationIdentifier = activation::ActivationIdentifier;
using VariableDumper = debug::VariableDumper;
using Error = error::Error;
using ExecutionReason = function::ExecutionReason;
using ArrayBuilder = globals::array::ArrayBuilder;
using NativeObject = object::NativeObject;
using Object = object::Object;
using ObjectHandle = object::ObjectHandle;
using ObjectPtr = object::ObjectPtr;
using Attribute = property::Attribute;
using PropertyMap = property_map::PropertyMap;
using Avm1 = runtime::Avm1;
using Value = value::Value;

} // namespace avm1
} // namespace core
} // namespace ruffle

// AVM warning macro
#define AVM_WARN(activation, ...) \
    do { \
        if constexpr (false) { /* AVm debug feature */ \
            /* Debug logging with activation id */ \
        } else { \
            /* Standard warning logging */ \
        } \
    } while (0)

// AVM error macro
#define AVM_ERROR(activation, ...) \
    do { \
        if constexpr (false) { /* AVM debug feature */ \
            /* Debug logging with activation id */ \
        } else { \
            /* Standard error logging */ \
        } \
    } while (0)

// AVM stub macro for tracking unimplemented features
#define AVM1_STUB(activation, class_name, ...) \
    do { \
        /* Track stub encounter for unimplemented features */ \
    } while (0)

#endif // RUFFLE_CORE_AVM1_H
