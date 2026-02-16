// C++ translation of core/src/avm2/globals/void.rs
// AVM2 void type implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_VOID_H
#define RUFFLE_CORE_AVM2_GLOBALS_VOID_H

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Class;
    template<typename T> class QName;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {

/// Create the void class
/// The `void` class has no interfaces and is marked as FINAL | SEALED
template<typename GC>
Class<GC> create_class(Activation<GC>* activation);

} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_VOID_H
