// C++ translation of tests/framework/src/runner.rs
// Test runner module

#ifndef RUFFLE_TESTS_FRAMEWORK_RUNNER_H
#define RUFFLE_TESTS_FRAMEWORK_RUNNER_H

#include <memory>
#include <mutex>
#include <unordered_map>
#include <string>
#include <chrono>
#include <optional>
#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
    class Player;
    namespace tag_utils {
        class SwfMovie;
    }
    namespace limits {
        class ExecutionLimit;
    }
    namespace backend {
        namespace navigator {
            class NullExecutor;
        }
    }
}
namespace input_format {
    class InputInjector;
}
namespace render {
    namespace backend {
        class RenderBackend;
        struct ViewportDimensions;
    }
}
namespace socket_format {
    struct SocketEvent;
}
namespace vfs {
    class VfsPath;
}
}

namespace ruffle {
namespace frontend {
namespace utils {
namespace options {
    class TestOptions;
    namespace image_comparison {
        class ImageComparison;
    }
    namespace known_failure {
        class KnownFailure;
    }
}
}
}
}

namespace ruffle {
namespace tests {
namespace framework {

// Forward declarations from runner submodules
namespace automation {
    void perform_automated_event(/* event type */, ruffle::core::Player* player);
}
namespace image_test {
    void capture_and_compare_image(
        const ruffle::vfs::VfsPath& root_path,
        const std::shared_ptr<std::mutex>& player,
        const std::string& name,
        const ruffle::frontend::utils::options::image_comparison::ImageComparison& comparison,
        void* render_interface);
}
namespace trace {
    void compare_trace_output(
        void* log,
        const ruffle::vfs::VfsPath& output_path,
        const void* approximations,
        const ruffle::frontend::utils::options::known_failure::KnownFailure& known_failure);
}

// Test status enum
enum class TestStatus {
    Continue,
    Sleep,
    Finished
};

/// Test runner for executing SWF tests
class TestRunner {
private:
    ruffle::vfs::VfsPath root_path;
    ruffle::vfs::VfsPath output_path;
    ruffle::frontend::utils::options::TestOptions options;
    std::shared_ptr<std::mutex> player;
    ruffle::input_format::InputInjector injector;
    ruffle::core::backend::navigator::NullExecutor executor;
    double frame_time;
    std::chrono::milliseconds frame_time_duration;
    void* log;  // TestLogBackend
    std::unordered_map<std::string, ruffle::frontend::utils::options::image_comparison::ImageComparison> images;
    uint32_t remaining_iterations;
    uint32_t current_iteration;
    bool preloaded;

public:
    /// Create a new test runner
    /// @param test The test configuration
    /// @param movie The SWF movie to test
    /// @param injector Input injector for automated events
    /// @param socket_events Optional socket events
    /// @param renderer Optional renderer (interface, backend) pair
    /// @param viewport_dimensions Viewport dimensions
    static std::optional<TestRunner> create(
        const void* test,
        const std::shared_ptr<ruffle::core::tag_utils::SwfMovie>& movie,
        const ruffle::input_format::InputInjector& injector,
        const std::optional<std::vector<ruffle::socket_format::SocketEvent>>& socket_events,
        const std::pair<void*, std::shared_ptr<ruffle::render::backend::RenderBackend>>& renderer,
        const ruffle::render::backend::ViewportDimensions& viewport_dimensions);

    /// Get the player instance
    const std::shared_ptr<std::mutex>& get_player() const {
        return player;
    }

    /// Get the test options
    const ruffle::frontend::utils::options::TestOptions& get_options() const {
        return options;
    }

    /// Check if next tick may be the last
    bool next_tick_may_be_last() const {
        return remaining_iterations == 1;
    }

    /// Check if the movie is preloaded
    bool is_preloaded() const {
        return preloaded;
    }

    /// Tick the test forward
    /// Runs actionscript, progresses timeline, executes FsCommands, performs tests
    /// @return Test status indicating what to do next
    std::optional<TestStatus> tick();

    /// Get the sleep duration if status is Sleep
    std::chrono::milliseconds get_sleep_duration() const {
        return frame_time_duration;
    }

private:
    /// Internal tick implementation
    void do_tick();

    /// Run tests after a tick
    void test();

    /// Final test checks on last iteration
    void last_test();

    /// Take image comparison by trigger
    std::optional<std::pair<std::string, ruffle::frontend::utils::options::image_comparison::ImageComparison>>
    take_image_comparison_by_trigger(int trigger);
};

} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_RUNNER_H
