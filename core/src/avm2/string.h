// C++ translation of core/src/avm2/string.rs
// AVM2 String representation

#ifndef RUFFLE_CORE_AVM2_STRING_H
#define RUFFLE_CORE_AVM2_STRING_H

// Re-export AvmString from the string module
#include "../string.h"

namespace ruffle {
namespace core {
namespace avm2 {

// AVM2 String is the same as the base AvmString
using AvmString = ruffle::core::AvmString;

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_STRING_H
