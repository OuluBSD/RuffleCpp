// C++ translation of desktop/src/main.rs
//! Ruffle Desktop Application Entry Point
//!
//! This is the main entry point for the Ruffle desktop application.
//! It handles:
//! - Command-line argument parsing
//! - Preferences loading
//! - Logging setup
//! - Panic handling
//! - Application initialization

#ifndef RUFFLE_DESKTOP_MAIN_H
#define RUFFLE_DESKTOP_MAIN_H

#include <string>
#include <memory>
#include <optional>
#include <functional>

// Forward declarations
namespace ruffle {
namespace desktop {

class App;
class GlobalPreferences;
class Opt;

/// Static callstack information for crash reporting
class StaticCallstack;

/// Panic hook information
struct PanicHookInfo;

}
}

namespace ruffle {
namespace desktop {
namespace main {

/// Version string for Ruffle
extern const char* RUFFLE_VERSION;

/// Thread-local storage for callstack information
class CallstackStorage {
public:
    CallstackStorage() = default;

    void set_callstack(std::optional<StaticCallstack> callstack);
    std::optional<StaticCallstack> get_callstack() const;

private:
    std::optional<StaticCallstack> callstack_;
};

/// Thread-local storage for render information
class RenderInfoStorage {
public:
    RenderInfoStorage() = default;

    void set_info(std::optional<std::string> info);
    std::optional<std::string> get_info() const;

private:
    std::optional<std::string> info_;
};

/// Thread-local storage for SWF information
class SwfInfoStorage {
public:
    SwfInfoStorage() = default;

    void set_info(std::optional<std::string> info);
    std::optional<std::string> get_info() const;

private:
    std::optional<std::string> info_;
};

/// Panic hook handler for crash reporting
///
/// Displays an error dialog to the user and offers to open a GitHub issue.
/// @param info Information about the panic
void panic_hook(const PanicHookInfo& info);

/// Main entry point for the desktop application
///
/// @param argc Number of command-line arguments
/// @param argv Command-line arguments
/// @return 0 on success, non-zero on error
int main(int argc, char* argv[]);

/// Move logs from config directory into proper log directory.
///
/// This exists because in older versions Ruffle created log files in the config directory.
/// TODO Remove this after some time.
///
/// @param preferences Global preferences
/// @param log_path Path to the log directory
/// @return true on success, false on error
bool migrate_logs(const GlobalPreferences* preferences, const std::string& log_path);

/// Application entry point after initialization
///
/// @param preferences Loaded preferences
/// @return 0 on success, non-zero on error
int run_app(GlobalPreferences* preferences);

} // namespace main
} // namespace desktop
} // namespace ruffle

// Platform-specific declarations
#ifdef _WIN32
namespace ruffle {
namespace desktop {
namespace windows {

/// Windows console attachment helper
class Console {
public:
    Console() = default;
    ~Console();

    /// Attach to console (Windows only)
    static Console attach();
};

} // namespace windows
} // namespace desktop
} // namespace ruffle
#endif

// Tracy profiler support (optional)
#ifdef RUFFLE_TRACY
namespace ruffle {
namespace desktop {
namespace tracy {

/// Tracy profiler configuration
struct RuffleTracyConfig {
    // Tracy configuration options
};

} // namespace tracy
} // namespace desktop
} // namespace ruffle
#endif

#endif // RUFFLE_DESKTOP_MAIN_H
