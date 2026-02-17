// C++ translation of core/src/avm2/globals/reg_exp.rs
//! RegExp builtin and prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_REG_EXP_H
#define RUFFLE_CORE_AVM2_GLOBALS_REG_EXP_H

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class AvmString;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    template<typename GC>
    class Activation;
    template<typename GC>
    class RegExpObject;
    template<typename GC>
    class ArrayObject;
    template<typename GC>
    class FunctionObject;

    namespace avm2 {
        enum class RegExpFlags : uint8_t;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace reg_exp {

/// Implements RegExp's init method, which is called from the constructor
template<typename GC>
Value<GC> init(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements RegExp's call handler
template<typename GC>
Value<GC> call_handler(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements RegExp.dotall
template<typename GC>
Value<GC> get_dotall(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements RegExp.extended
template<typename GC>
Value<GC> get_extended(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements RegExp.global
template<typename GC>
Value<GC> get_global(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements RegExp.ignoreCase
template<typename GC>
Value<GC> get_ignore_case(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements RegExp.multiline
template<typename GC>
Value<GC> get_multiline(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements RegExp.lastIndex's getter
template<typename GC>
Value<GC> get_last_index(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements RegExp.lastIndex's setter
template<typename GC>
Value<GC> set_last_index(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements RegExp.source
template<typename GC>
Value<GC> get_source(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements RegExp.exec
template<typename GC>
Value<GC> exec(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements RegExp.test
template<typename GC>
Value<GC> test(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace reg_exp
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_REG_EXP_H
