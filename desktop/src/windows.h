// C++ translation of desktop/src/windows.rs
//! Windows-specific console attachment for desktop Ruffle

#ifndef RUFFLE_DESKTOP_WINDOWS_H
#define RUFFLE_DESKTOP_WINDOWS_H

#ifdef _WIN32

#include <cstdint>

namespace ruffle {
namespace desktop {
namespace windows {

/// RAII guard for the attached parent console.
/// Frees the console on drop if it was successfully attached.
///
/// When linked with the windows subsystem, Windows won't automatically attach
/// to the console of the parent process, so we do it explicitly. This fails
/// silently if the parent has no console.
///
/// However, if stdout/stderr are already redirected (e.g., `ruffle.exe > file.txt`),
/// we should NOT attach to the console as that would bypass the redirection.
class Console {
private:
    bool attached;

public:
    /// Default constructor
    Console() : attached(false) {}

    /// Attach to the parent console
    ///
    /// Checks if stdout is already redirected to a file or pipe.
    /// If so, don't attach to console as that would bypass the redirection.
    /// Otherwise, attach to parent console for interactive use.
    static Console attach();

    /// Destructor - frees the console if it was attached
    ~Console();

    // Disable copy
    Console(const Console&) = delete;
    Console& operator=(const Console&) = delete;

    // Enable move
    Console(Console&& other) noexcept : attached(other.attached) {
        other.attached = false;
    }

    Console& operator=(Console&& other) noexcept {
        if (this != &other) {
            if (attached) {
                free_console();
            }
            attached = other.attached;
            other.attached = false;
        }
        return *this;
    }

private:
    /// Free the console
    void free_console();
};

} // namespace windows
} // namespace desktop
} // namespace ruffle

#endif // _WIN32

#endif // RUFFLE_DESKTOP_WINDOWS_H
