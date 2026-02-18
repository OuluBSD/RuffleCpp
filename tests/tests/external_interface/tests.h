// C++ translation of tests/tests/external_interface/tests.rs
// External interface tests

#ifndef RUFFLE_TESTS_EXTERNAL_INTERFACE_TESTS_H
#define RUFFLE_TESTS_EXTERNAL_INTERFACE_TESTS_H

#include <memory>
#include <string>
#include <chrono>
#include <thread>

// Forward declarations
namespace ruffle {
namespace core {
namespace external {
    class Value;
}
class ExternalInterfaceProvider;
}
namespace test_framework {
namespace environment {
    class Environment;
}
namespace options {
    struct TestOptions;
}
namespace runner {
    enum class TestStatus;
    class TestRunner;
}
namespace test {
    class Test;
}
namespace vfs {
    class VfsPath;
    class PhysicalFS;
}
}
}

// External interface test provider
namespace ruffle {
namespace tests {
namespace external_interface {

class ExternalInterfaceTestProvider;

} // namespace external_interface
} // namespace tests
} // namespace ruffle

namespace ruffle {
namespace tests {
namespace external_interface {

/// Test function for AVM1 external interface
/// Tests the external interface functionality with AVM1
bool external_interface_avm1(
    const ruffle::test_framework::environment::Environment* environment
);

/// Test function for AVM2 external interface
/// Tests the external interface functionality with AVM2
bool external_interface_avm2(
    const ruffle::test_framework::environment::Environment* environment
);

} // namespace external_interface
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_EXTERNAL_INTERFACE_TESTS_H
