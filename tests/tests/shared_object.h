// C++ translation of tests/tests/shared_object/mod.rs
// SharedObject persistence tests

#ifndef RUFFLE_TESTS_TESTS_SHARED_OBJECT_H
#define RUFFLE_TESTS_TESTS_SHARED_OBJECT_H

#include <string>
#include <memory>
#include <chrono>

// Forward declarations
namespace ruffle {
namespace core {
namespace backend {
namespace storage {
    class StorageBackend;
    class MemoryStorageBackend;
}
}
}
namespace test_framework {
namespace environment {
    class Environment;
}
namespace options {
    class TestOptions;
}
namespace runner {
    enum class TestStatus {
        Continue,
        Sleep,
        Finished,
    };
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

namespace libtest_mimic {
    class Failed;
}

namespace ruffle {
namespace tests {
namespace shared_object {

/// Test SharedObject persistence for AVM1
/// Run an SWF that saves data to a shared object twice and verify that the data is saved.
/// @param environment The test environment
/// @return Result of the test
libtest_mimic::Failed shared_object_avm1(const ruffle::test_framework::environment::Environment* environment);

/// Test SharedObject self-reference for AVM1
/// Run an SWF that saves data with self-references to a shared object and verify persistence.
/// @param environment The test environment
/// @return Result of the test
libtest_mimic::Failed shared_object_self_ref_avm1(const ruffle::test_framework::environment::Environment* environment);

/// Test SharedObject persistence for AVM2
/// Run an SWF that saves data to a shared object twice and verify that the data is saved.
/// @param environment The test environment
/// @return Result of the test
libtest_mimic::Failed shared_object_avm2(const ruffle::test_framework::environment::Environment* environment);

} // namespace shared_object
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_TESTS_SHARED_OBJECT_H
