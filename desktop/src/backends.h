// C++ translation of desktop/src/backends.rs
// Desktop backends module

#ifndef RUFFLE_DESKTOP_BACKENDS_H
#define RUFFLE_DESKTOP_BACKENDS_H

// Desktop backend submodules
namespace ruffle {
namespace desktop {
namespace backends {

// ExternalInterface module
namespace external_interface {
    class DesktopExternalInterfaceProvider;
}

// FSCommand module
namespace fscommand {
    class DesktopFSCommandProvider;
}

// Navigator module
namespace navigator {
    class DesktopNavigatorInterface;
    class PathAllowList;
}

// UI module
namespace ui {
    class DesktopUiBackend;
}

// Re-exports
using DesktopExternalInterfaceProvider = external_interface::DesktopExternalInterfaceProvider;
using DesktopFSCommandProvider = fscommand::DesktopFSCommandProvider;
using DesktopNavigatorInterface = navigator::DesktopNavigatorInterface;
using PathAllowList = navigator::PathAllowList;
using DesktopUiBackend = ui::DesktopUiBackend;

} // namespace backends
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_BACKENDS_H
