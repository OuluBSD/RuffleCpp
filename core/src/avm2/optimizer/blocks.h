// C++ translation of core/src/avm2/optimizer/blocks.rs
// AVM2 Optimizer Blocks

#ifndef RUFFLE_CORE_AVM2_OPTIMIZER_BLOCKS_H
#define RUFFLE_CORE_AVM2_OPTIMIZER_BLOCKS_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <cstddef>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
namespace op {
    class Op;
}
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace optimizer {

/// Represents non-overlapping slices of ops with
/// a single entry point and many exit points.
/// (as opposed to basic blocks, which have 1 entry and exit point)
template<typename GC>
struct Block {
    /// The ops making up this block.
    const std::shared_ptr<op::Op>* ops;

    /// The index of the first op making up this Block.
    size_t start_index;

    /// The number of ops in this block.
    size_t size;

    Block() : ops(nullptr), start_index(0), size(0) {}

    Block(const std::shared_ptr<op::Op>* ops_ptr, size_t start_idx, size_t sz)
        : ops(ops_ptr), start_index(start_idx), size(sz) {}
};

/// Assemble blocks from code and jump targets.
/// 
/// Parameters:
/// - code: The array of operations
/// - jump_targets: Set of indices that are jump targets
/// 
/// Returns:
/// - A pair containing:
///   - Vector of blocks
///   - Map from op indices to block indices
template<typename GC>
std::pair<std::vector<Block<GC>>, std::unordered_map<size_t, size_t>>
assemble_blocks(
    const std::vector<std::shared_ptr<op::Op>>& code,
    const std::unordered_set<size_t>& jump_targets
) {
    std::vector<Block<GC>> block_list;
    block_list.reserve(2);
    size_t current_block_start = 0;

    for (size_t i = 0; i < code.size(); ++i) {
        const auto& op = code[i];
        
        // Check if this op ends a block
        bool ends_block = false;
        
        // Check for jump/return/throw/switch ops
        // (In real implementation, would check op type)
        if (op->is_jump() || op->is_return() || op->is_throw() || op->is_lookup_switch()) {
            ends_block = true;
        }
        
        // Check if the next op is a jump target
        if (jump_targets.count(i + 1) > 0) {
            ends_block = true;
        }

        if (ends_block) {
            Block<GC> block(
                code.data() + current_block_start,
                current_block_start,
                i + 1 - current_block_start
            );
            block_list.push_back(block);
            current_block_start = i + 1;
        }
    }

    // Create a table mapping op indices to block indices.
    std::unordered_map<size_t, size_t> op_index_to_block_index_table;
    for (size_t i = 0; i < block_list.size(); ++i) {
        op_index_to_block_index_table[block_list[i].start_index] = i;
    }

    return {block_list, op_index_to_block_index_table};
}

} // namespace optimizer
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OPTIMIZER_BLOCKS_H
