// C++ translation of tests/framework/src/fs_commands.rs
//! Test FS command provider for testing FS commands

#ifndef RUFFLE_TESTS_FRAMEWORK_FS_COMMANDS_H
#define RUFFLE_TESTS_FRAMEWORK_FS_COMMANDS_H

#include <string>
#include <optional>
#include <memory>
#include <functional>

// Forward declarations
namespace ruffle {
namespace core {
namespace external {
    class FsCommandProvider;
}
}
}

namespace ruffle {
namespace tests {
namespace framework {

/// FS command types for testing
class FsCommand {
public:
    /// Command types
    enum class Type {
        Quit,
        CaptureImage
    };

private:
    Type type_;
    std::optional<std::string> argument_;

public:
    /// Create a Quit command
    static FsCommand quit() {
        return FsCommand(Type::Quit, std::nullopt);
    }

    /// Create a CaptureImage command
    /// @param path The path to capture the image to
    static FsCommand capture_image(const std::string& path) {
        return FsCommand(Type::CaptureImage, path);
    }

    /// Get the command type
    Type type() const { return type_; }

    /// Get the command argument (if any)
    const std::optional<std::string>& argument() const { return argument_; }

    /// Create an FsCommand from a command string and arguments
    /// @param command The command name (e.g., "quit", "captureImage")
    /// @param args The command arguments
    /// @return Optional FsCommand if the command is recognized
    static std::optional<FsCommand> from_command(const std::string& command, const std::string& args) {
        if (command == "quit") {
            return quit();
        } else if (command == "captureImage") {
            return capture_image(args);
        }
        return std::nullopt;
    }

private:
    FsCommand(Type type, std::optional<std::string> argument)
        : type_(type), argument_(std::move(argument)) {}
};

/// Callback type for receiving FS commands
using FsCommandCallback = std::function<void(const FsCommand&)>;

/// Test FS command provider
///
/// Provides a test implementation of FsCommandProvider that sends
/// commands to a callback for verification in tests.
class TestFsCommandProvider : public core::external::FsCommandProvider {
private:
    FsCommandCallback callback_;

public:
    /// Create a new test FS command provider
    /// @param callback The callback to invoke when a command is received
    explicit TestFsCommandProvider(FsCommandCallback callback)
        : callback_(std::move(callback)) {}

    /// Handle an FSCommand
    ///
    /// Sends the command to the callback if it's recognized.
    ///
    /// @param command The command name
    /// @param args The command arguments
    /// @return true if the command was handled, false otherwise
    bool on_fs_command(const std::string& command, const std::string& args) override {
        if (auto fs_command = FsCommand::from_command(command, args)) {
            callback_(*fs_command);
            return true;
        }
        return false;
    }
};

} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_FS_COMMANDS_H
