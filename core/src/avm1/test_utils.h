// C++ translation of core/src/avm1/test_utils.rs
// AVM1 test utilities

#ifndef RUFFLE_CORE_AVM1_TEST_UTILS_H
#define RUFFLE_CORE_AVM1_TEST_UTILS_H

#include <cstdint>
#include <functional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Object;
    template<typename T> class Error;

    namespace activation {
        template<typename T> class Activation;
    }
}
namespace display_object {
    template<typename T> class TDisplayObject;
}
namespace tag_utils {
    class SwfMovie;
}
namespace player {
    class PlayerBuilder;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace test_utils {

/// Run a test function with AVM1 activation context
/// 
/// \param swf_version The SWF version to use for the test
/// \param test The test function to run
template<typename GC, typename F>
void with_avm(uint8_t swf_version, F test)
where
    F: for<'a, 'gc> FnOnce(&mut Activation<'_, 'gc>, Object<'gc>) -> Result<(), Error<'gc>>
{
    // Note: This is a stub implementation
    // The full implementation would require:
    // - Creating an empty SwfMovie with the given version
    // - Building a player with that movie
    // - Getting the root clip and creating an activation
    // - Running the test function
    // - Handling any errors by panicking
}

} // namespace test_utils
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_TEST_UTILS_H
