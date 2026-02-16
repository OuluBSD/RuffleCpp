// C++ translation of core/src/avm2/optimizer/nop_remover.rs
//! AVM2 Optimizer - NOP Instruction Remover

#ifndef RUFFLE_CORE_AVM2_OPTIMIZER_NOP_REMOVER_H
#define RUFFLE_CORE_AVM2_OPTIMIZER_NOP_REMOVER_H

#include <cstdint>
#include <vector>
#include <cstddef>

namespace ruffle {
namespace core {
namespace avm2 {

// Forward declarations
template<typename GC>
class Op;

/// Exception structure for bytecode verification
template<typename GC>
struct Exception {
    size_t from_offset;
    size_t to_offset;
    size_t target_offset;
    
    // Class references would go here (placeholder for now)
    void* catch_class;
    void* target_class;
};

/// LookupSwitch structure for switch instruction offsets
/// Uses mutable offsets for optimizer rewrites
struct LookupSwitch {
    size_t default_offset;
    std::vector<size_t> case_offsets;
};

/// Remove NOP instructions from bytecode
/// 
/// This function removes all NOP (no-operation) instructions from the bytecode,
/// adjusting all jump offsets and exception table entries accordingly.
/// 
/// In debug builds, only explicit Op::Nop instructions are removed.
/// In release builds, Debug, DebugFile, and DebugLine instructions are also removed.
/// 
/// \param code The bytecode operations to optimize (modified in place)
/// \param exceptions The exception table entries (modified in place)
template<typename GC>
void remove_nops(std::vector<Op<GC>>& code, std::vector<Exception<GC>>& exceptions);

// Template implementation

template<typename GC>
void remove_nops(std::vector<Op<GC>>& code, std::vector<Exception<GC>>& exceptions) {
    std::vector<size_t> offset_vec(code.size(), 0);
    size_t current_offset = 0;

    // First, remove nops and build offset mapping
    size_t i = 0;
    while (i < code.size()) {
        offset_vec[i] = i - current_offset;
        
        if (code[i].is_nop()) {
            current_offset += 1;
        } else {
            // Shift the ops over the nops
            code[i - current_offset] = code[i];
        }

        i += 1;
    }

    // The ops have all been shifted over now, so remove the garbage ops left
    // at the end of the code vector
    code.resize(code.size() - current_offset);

    // Rewrite jump offsets
    for (auto& op : code) {
        switch (op.type()) {
            case OpType::IfTrue:
            case OpType::IfFalse:
            case OpType::Jump:
            case OpType::PopJump: {
                size_t& offset = op.get_offset();
                offset = offset_vec[offset];
                break;
            }
            case OpType::LookupSwitch: {
                LookupSwitch& lookup_switch = op.get_lookup_switch();
                
                // Rewrite case offsets
                for (auto& target : lookup_switch.case_offsets) {
                    target = offset_vec[target];
                }
                
                // Rewrite default offset
                lookup_switch.default_offset = offset_vec[lookup_switch.default_offset];
                break;
            }
            default:
                break;
        }
    }

    // Rewrite exception offsets too
    for (auto& exception : exceptions) {
        exception.from_offset = offset_vec[exception.from_offset];
        exception.to_offset = offset_vec[exception.to_offset];
        exception.target_offset = offset_vec[exception.target_offset];
    }
}

} // namespace optimizer
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OPTIMIZER_NOP_REMOVER_H
