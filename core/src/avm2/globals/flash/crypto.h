// C++ translation of core/src/avm2/globals/flash/crypto.rs
// flash.crypto namespace

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_CRYPTO_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_CRYPTO_H

#include <vector>
#include <cstdint>
#include <random>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace crypto {

/// Implements flash.crypto.generateRandomBytes
/// 
/// Generates cryptographically secure random bytes.
/// 
/// \param activation The current activation context
/// \param this_value The this value (unused)
/// \param args Function arguments (length: number of bytes to generate)
/// \return A ByteArray containing the random bytes
/// \throws Error 2004 if length is not in range [1, 1024]
template<typename GC>
Value<GC> generate_random_bytes(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace crypto
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_CRYPTO_H
