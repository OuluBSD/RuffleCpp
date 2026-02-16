// C++ translation of core/src/avm2/globals/flash/utils/dictionary.rs
// flash.utils.Dictionary

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_UTILS_DICTIONARY_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_UTILS_DICTIONARY_H

// Re-export dictionary_allocator from the object module
#include "../../../object/dictionary_allocator.h"

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace utils {

// Dictionary uses the dictionary_allocator from the object module
using dictionary_allocator = ruffle::core::avm2::object::dictionary_allocator;

} // namespace utils
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_UTILS_DICTIONARY_H
