// C++ translation of tests/framework/src/lib.rs
// Test framework main module

#ifndef RUFFLE_TESTS_FRAMEWORK_LIB_H
#define RUFFLE_TESTS_FRAMEWORK_LIB_H

// Test framework submodules
namespace ruffle {
namespace tests {
namespace framework {

// Submodule declarations
namespace environment {
    class Environment;
    class RenderInterface;
}

namespace fs_commands {
    // File system commands
}

namespace image_trigger {
    // Image trigger functionality
}

namespace options {
    class PlayerOptions;
    class KnownFailure;
}

namespace runner {
    class TestRunner;
}

namespace test {
    // Test functionality
}

namespace backends {
    // Backend implementations
    namespace audio {
        class TestAudioBackend;
    }
    namespace log {
        class TestLogBackend;
    }
    namespace navigator {
        class TestNavigatorBackend;
    }
    namespace ui {
        class TestUiBackend;
    }
}

namespace util {
    // Utility functions
}

// Re-exports
using Environment = environment::Environment;
using RenderInterface = environment::RenderInterface;
using PlayerOptions = options::PlayerOptions;
using KnownFailure = options::KnownFailure;
using TestRunner = runner::TestRunner;
using TestAudioBackend = backends::audio::TestAudioBackend;
using TestLogBackend = backends::log::TestLogBackend;
using TestNavigatorBackend = backends::navigator::TestNavigatorBackend;
using TestUiBackend = backends::ui::TestUiBackend;

} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_LIB_H
