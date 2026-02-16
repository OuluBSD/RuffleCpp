// C++ translation of core/common/src/avm_string.rs
// AVM string module - provides string implementations for AVM1 and AVM2

#ifndef RUFFLE_CORE_COMMON_AVM_STRING_H
#define RUFFLE_CORE_AVM_STRING_H

// Forward declarations for AVM string components
namespace ruffle {
namespace core {
namespace avm_string {

// Submodules - these would be defined in their respective headers
namespace avm_string_sub { }
namespace common { }
namespace context { }
namespace interner { }
namespace repr { }

// Re-exports from submodules
// These types would be defined in the respective submodule headers
// pub use avm_string::AvmString;
// pub use common::CommonStrings;
// pub use context::{HasStringContext, StringContext};
// pub use interner::{AvmAtom, AvmStringInterner};

// Forward declarations for the main types
template<typename GC>
class AvmString;

class CommonStrings;

template<typename GC>
class HasStringContext;

template<typename GC>
class StringContext;

template<typename GC>
class AvmAtom;

template<typename GC>
class AvmStringInterner;

} // namespace avm_string
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_COMMON_AVM_STRING_H
