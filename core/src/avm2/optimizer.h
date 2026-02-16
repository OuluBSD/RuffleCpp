// C++ translation of core/src/avm2/optimizer.rs
//! AVM2 Optimizer

#ifndef RUFFLE_CORE_AVM2_OPTIMIZER_H
#define RUFFLE_CORE_AVM2_OPTIMIZER_H

#include <cstdint>
#include <vector>
#include <unordered_set>

namespace ruffle {
namespace core {
namespace avm2 {

// Forward declarations
template<typename GC>
class Activation;
template<typename GC>
class Method;
template<typename GC>
class Op;
template<typename GC>
class Error;

struct Exception {
    // Placeholder for exception structure
};

struct ResolvedParamConfig {
    // Placeholder for resolved parameter config
};

/// Submodules for optimizer passes
namespace blocks {
    // Basic block analysis
}

namespace dce {
    // Dead code elimination
}

namespace nop_remover {
    // NOP instruction removal
}

namespace peephole {
    // Peephole optimization
}

namespace type_aware {
    // Type-aware optimization
}

/// Run all the optimizer passes on the given code.
///
/// This method should be run regardless of whether or not the "disable AVM2
/// optimizer" player option is on. It will not perform observable optimizations
/// if the "disable AVM2 optimizer" player option is off.
///
/// \param activation The current activation context
/// \param method The method being optimized
/// \param code The bytecode operations to optimize (modified in place)
/// \param method_exceptions Exceptions that may be thrown by the method
/// \param resolved_parameters Resolved parameter configurations
/// \param jump_targets Set of jump target indices
/// \return Result indicating success or an error
template<typename GC>
void optimize(
    Activation<GC>* activation,
    Method<GC> method,
    std::vector<Op<GC>>& code,
    std::vector<Exception<GC>>& method_exceptions,
    const std::vector<ResolvedParamConfig<GC>>& resolved_parameters,
    std::unordered_set<size_t> jump_targets
);

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OPTIMIZER_H
