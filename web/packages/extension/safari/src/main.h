// C++ translation of web/packages/extension/safari/src/main.rs
//! Safari Web Extension entry point
//! macOS-specific extension handling using Objective-C runtime

#ifndef RUFFLE_WEB_PACKAGES_EXTENSION_SAFARI_MAIN_H
#define RUFFLE_WEB_PACKAGES_EXTENSION_SAFARI_MAIN_H

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

namespace ruffle {
namespace web {
namespace extension {
namespace safari {

#ifdef __APPLE__
#if TARGET_OS_MAC

/// macOS-specific extension implementation
namespace macos {

/// Register the RuffleWebExtension class with the Objective-C runtime
/// This class implements the NSExtensionRequestHandling protocol
void extension_class();

/// Main entry point for the Safari extension on macOS
/// Calls NSExtensionMain after registering our classes
/// 
/// Note: NSExtensionMain is a private Foundation function used as the
/// entry point for all app extensions. In Obj-C/Swift apps, a linker
/// flag sets this as the entry point, but we need a main function to
/// register our Obj-C classes first.
void main();

} // namespace macos

#endif // TARGET_OS_MAC
#endif // __APPLE__

/// Main entry point for Safari extension
/// On macOS: registers Obj-C classes and calls NSExtensionMain
/// On other platforms: panics as Safari extension is macOS-only
inline void main() {
#ifdef __APPLE__
#if TARGET_OS_MAC
    macos::main();
#else
    // Safari extension not available on non-macOS Apple platforms
    throw std::runtime_error("Safari stub binary not available outside of macOS");
#endif
#else
    // Safari extension only available on macOS
    throw std::runtime_error("Safari stub binary not available outside of macOS");
#endif
}

} // namespace safari
} // namespace extension
} // namespace web
} // namespace ruffle

#endif // RUFFLE_WEB_PACKAGES_EXTENSION_SAFARI_MAIN_H
