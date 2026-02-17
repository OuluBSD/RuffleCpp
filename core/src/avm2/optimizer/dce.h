// C++ translation of core/src/avm2/optimizer/dce.rs
//! AVM2 Optimizer - Dead Code Elimination

#ifndef RUFFLE_CORE_AVM2_OPTIMIZER_DCE_H
#define RUFFLE_CORE_AVM2_OPTIMIZER_DCE_H

#include <vector>
#include <unordered_set>
#include <cstddef>
#include <cell>

namespace ruffle {
namespace core {
namespace avm2 {

// Forward declarations
template<typename GC>
class Op;

/// Eliminate dead code from bytecode
///
/// Remove jumps and the code that they jump over when the code inside the
/// jump is unreachable.
///
/// \param ops The bytecode operations to optimize (modified in place)
/// \param jump_targets Set of indices that are jump targets
template<typename GC>
void eliminate_dead_code(
    std::vector<std::cell::RefCell<Op<GC>>>& ops,
    const std::unordered_set<size_t>& jump_targets
);

// Template implementation

template<typename GC>
void eliminate_dead_code(
    std::vector<std::cell::RefCell<Op<GC>>>& ops,
    const std::unordered_set<size_t>& jump_targets
) {
    // Remove jumps and the code that they jump over when the code inside the
    // jump is unreachable
    for (size_t i = 0; i < ops.size(); ++i) {
        auto& op_cell = ops[i];
        auto op = op_cell.get();

        if (op.is_jump()) {
            size_t offset = op.get_offset();
            bool is_reachable = false;

            for (size_t pos = i; pos < offset; ++pos) {
                if (jump_targets.count(pos) > 0) {
                    is_reachable = true;
                    break;
                }
            }

            if (!is_reachable && i < offset) {
                for (size_t j = i; j < offset; ++j) {
                    // Set all unreachable ops to Nop, including the Jump op
                    ops[j].set(Op<GC>::nop());
                }
            }
        } else if (op.is_pop_jump()) {
            size_t offset = op.get_offset();
            bool is_reachable = false;

            for (size_t pos = i; pos < offset; ++pos) {
                if (jump_targets.count(pos) > 0) {
                    is_reachable = true;
                    break;
                }
            }

            if (!is_reachable && i < offset) {
                for (size_t j = i; j < offset; ++j) {
                    // Set all unreachable ops to Nop, including the Jump op
                    ops[j].set(Op<GC>::nop());
                }
                // PopJump works exactly like `Jump`, but it sets the jumping op
                // (`PopJump` in this case) to `Pop` instead of `Nop`.
                ops[i].set(Op<GC>::pop());
            }
        }
    }
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OPTIMIZER_DCE_H
