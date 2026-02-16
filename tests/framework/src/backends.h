// C++ translation of tests/framework/src/backends.rs
// Test framework backends module

#ifndef RUFFLE_TESTS_FRAMEWORK_BACKENDS_H
#define RUFFLE_TESTS_FRAMEWORK_BACKENDS_H

// Tests framework backends submodules
namespace ruffle {
namespace tests {
namespace framework {
namespace backends {

// Audio backend
namespace audio {
    class TestAudioBackend;
}

// Log backend
namespace log {
    class TestLogBackend;
}

// Navigator backend
namespace navigator {
    class TestNavigatorBackend;
}

// UI backend
namespace ui {
    class TestUiBackend;
}

// Re-exports
using TestAudioBackend = audio::TestAudioBackend;
using TestLogBackend = log::TestLogBackend;
using TestNavigatorBackend = navigator::TestNavigatorBackend;
using TestUiBackend = ui::TestUiBackend;

} // namespace backends
} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_BACKENDS_H
