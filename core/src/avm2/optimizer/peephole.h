// C++ translation of core/src/avm2/optimizer/peephole.rs
//! AVM2 Optimizer - Peephole Optimization

#ifndef RUFFLE_CORE_AVM2_OPTIMIZER_PEEPHOLE_H
#define RUFFLE_CORE_AVM2_OPTIMIZER_PEEPHOLE_H

#include <vector>
#include <unordered_set>
#include <cstddef>
#include <cell>
#include <optional>
#include <utility>

namespace ruffle {
namespace core {
namespace avm2 {

// Forward declarations
template<typename GC>
class Op;

/// A peephole optimizer to run before type-aware optimizations.
/// This should be called once on the entire code slice.
///
/// \param ops The bytecode operations to optimize (modified in place)
template<typename GC>
void preprocess_peephole(std::vector<std::cell::RefCell<Op<GC>>>& ops);

/// A peephole optimizer to run after type-aware optimizations.
/// This should be called once on the entire code slice.
///
/// \param ops The bytecode operations to optimize (modified in place)
/// \param jump_targets Set of indices that are jump targets
/// \param has_exceptions Whether the method has exceptions
template<typename GC>
void postprocess_peephole(
    std::vector<std::cell::RefCell<Op<GC>>>& ops,
    const std::unordered_set<size_t>& jump_targets,
    bool has_exceptions
);

/// Checks if the method fits the simple scope structure pattern.
///
/// Pattern:
/// [Debug/DebugFile/DebugLine] zero or more times
/// GetLocal { index: 0 }
/// [Debug/DebugFile/DebugLine] zero or more times
/// PushScope
/// ...
///
/// Conditions:
/// - No jumps to that initial PushScope opcode, or anything before it
/// - No additional scope-related opcodes (PushScope, PushWith, PopScope)
///
/// \param ops The bytecode operations
/// \param jump_targets Set of indices that are jump targets
/// \return Pair of (getlocal0_pos, pushscope_pos) if pattern matches, nullopt otherwise
template<typename GC>
std::optional<std::pair<size_t, size_t>> simple_scope_structure(
    const std::vector<std::cell::RefCell<Op<GC>>>& ops,
    const std::unordered_set<size_t>& jump_targets
);

// Template implementations

template<typename GC>
void preprocess_peephole(std::vector<std::cell::RefCell<Op<GC>>>& ops) {
    for (size_t i = 0; i < ops.size(); ++i) {
        auto& op_cell = ops[i];
        auto op = op_cell.get();

        if (op.is_jump()) {
            size_t offset = op.get_offset();
            if (offset == i + 1) {
                op_cell.set(Op<GC>::nop());
            }
        } else if (op.is_if_true() || op.is_if_false()) {
            size_t offset = op.get_offset();
            if (offset == i + 1) {
                op_cell.set(Op<GC>::pop());
            }
        }
    }
}

template<typename GC>
void postprocess_peephole(
    std::vector<std::cell::RefCell<Op<GC>>>& ops,
    const std::unordered_set<size_t>& jump_targets,
    bool has_exceptions
) {
    // Gather some information...
    bool sets_local_0 = false;

    for (const auto& op_cell : ops) {
        auto op = op_cell.get();

        if (op.is_set_local() || op.is_kill() || op.is_dec_local() ||
            op.is_dec_local_i() || op.is_inc_local() || op.is_inc_local_i()) {
            if (op.get_index() == 0) {
                sets_local_0 = true;
                break;
            }
        } else if (op.is_has_next_2()) {
            if (op.get_object_register() == 0 || op.get_index_register() == 0) {
                sets_local_0 = true;
                break;
            }
        }
    }

    auto simple_scope_ops = simple_scope_structure(ops, jump_targets);
    bool has_simple_scope = simple_scope_ops.has_value() && !has_exceptions;

    // Now actually run the peephole optimizer.
    std::optional<size_t> last_op_idx;

    for (size_t i = 0; i < ops.size(); ++i) {
        if (jump_targets.count(i) > 0) {
            // If this op was a jump target, we don't know what the last op was
            last_op_idx = std::nullopt;
        }

        auto& current_op_cell = ops[i];
        auto current_op = current_op_cell.get();

        if (last_op_idx.has_value()) {
            auto& last_op_cell = ops[last_op_idx.value()];
            auto last_op = last_op_cell.get();

            // Eliminate PushXXX+Pop and GetLocal+Pop
            if (last_op.is_pure_push() && current_op.is_pop()) {
                last_op_cell.set(Op<GC>::nop());
                current_op_cell.set(Op<GC>::nop());
                last_op_idx = std::nullopt;
                continue;
            }

            // PushXXX+PopJump becomes Nop+Jump
            if (last_op.is_pure_push() && current_op.is_pop_jump()) {
                last_op_cell.set(Op<GC>::nop());
                current_op_cell.set(Op<GC>::jump(current_op.get_offset()));
                last_op_idx = std::nullopt;
                continue;
            }

            // Remove CoerceB before IfTrue, IfFalse, and Not
            if (last_op.is_coerce_b() && (current_op.is_if_true() || current_op.is_if_false() || current_op.is_not())) {
                last_op_cell.set(Op<GC>::nop());
            }

            // Dup+SetLocal becomes Nop+StoreLocal
            if (last_op.is_dup() && current_op.is_set_local()) {
                last_op_cell.set(Op<GC>::nop());
                current_op_cell.set(Op<GC>::store_local(current_op.get_index()));
                last_op_idx = std::nullopt;
                continue;
            }

            // SetLocal+GetLocal becomes Nop+StoreLocal
            if (last_op.is_set_local() && current_op.is_get_local() &&
                last_op.get_index() == current_op.get_index()) {
                last_op_cell.set(Op<GC>::nop());
                current_op_cell.set(Op<GC>::store_local(last_op.get_index()));
                last_op_idx = std::nullopt;
                continue;
            }
        }

        // Replace `getscopeobject 0` with `getlocal 0` if possible
        if (current_op.is_get_scope_object() && current_op.get_index() == 0) {
            if (has_simple_scope && !sets_local_0) {
                current_op_cell.set(Op<GC>::get_local(0));
            }
        }

        // Don't set last_op to the current_op if the current op does nothing.
        // This allows us to peephole-optimize sequences such as
        // `getlocal0`-`nop`-`pop`, as when the `pop` op is being processed,
        // `last_op` will still be set to the `getlocal0`.
        if (!current_op.is_nop()) {
            last_op_idx = i;
        }
    }

    // Gather some more information...
    bool uses_scope_ops = false;

    for (const auto& op_cell : ops) {
        auto op = op_cell.get();

        if (op.is_get_scope_object() || op.is_set_global_slot() ||
            op.is_find_property() || op.is_find_prop_strict() ||
            op.is_new_function() || op.is_new_class()) {
            uses_scope_ops = true;
            break;
        }
    }

    // Eliminate the `getlocal0` and `pushscope` ops at the beginning of the
    // method, if possible.
    if (has_simple_scope && !uses_scope_ops) {
        auto [getlocal0_pos, pushscope_pos] = simple_scope_ops.value();
        ops[getlocal0_pos].set(Op<GC>::nop());
        ops[pushscope_pos].set(Op<GC>::nop());
    }
}

template<typename GC>
std::optional<std::pair<size_t, size_t>> simple_scope_structure(
    const std::vector<std::cell::RefCell<Op<GC>>>& ops,
    const std::unordered_set<size_t>& jump_targets
) {
    std::optional<size_t> getlocal0_pos;

    for (size_t i = 0; i < ops.size(); ++i) {
        auto op = ops[i].get();

        // Ignore any initial debug opcodes
        if (op.is_debug() || op.is_debug_file() || op.is_debug_line()) {
            continue;
        }

        // Look for an initial getlocal0
        if (op.is_get_local() && op.get_index() == 0) {
            getlocal0_pos = i;
            break;
        }

        // Anything else doesn't fit the pattern, so give up
        return std::nullopt;
    }

    // Give up if we didn't find it
    if (!getlocal0_pos.has_value()) {
        return std::nullopt;
    }

    std::optional<size_t> pushscope_pos;
    for (size_t i = getlocal0_pos.value() + 1; i < ops.size(); ++i) {
        auto op = ops[i].get();

        // Ignore any debug opcodes
        if (op.is_debug() || op.is_debug_file() || op.is_debug_line()) {
            continue;
        }

        // Look for a pushscope
        if (op.is_push_scope()) {
            pushscope_pos = i;
            break;
        }

        // Anything else doesn't fit the pattern, so give up
        return std::nullopt;
    }

    // Give up if we didn't find it
    if (!pushscope_pos.has_value()) {
        return std::nullopt;
    }

    // Check for jumps to the initial scope ops
    for (size_t i = 0; i <= pushscope_pos.value(); ++i) {
        if (jump_targets.count(i) > 0) {
            return std::nullopt;
        }
    }

    // Check for additional scope-related opcodes
    for (size_t i = pushscope_pos.value() + 1; i < ops.size(); ++i) {
        auto op = ops[i].get();
        if (op.is_push_scope() || op.is_push_with() || op.is_pop_scope()) {
            return std::nullopt;
        }
    }

    return std::make_pair(getlocal0_pos.value(), pushscope_pos.value());
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OPTIMIZER_PEEPHOLE_H
