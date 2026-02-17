// C++ translation of core/src/debug_ui/domain.rs
// Debug UI Domain List Window

#ifndef RUFFLE_CORE_DEBUG_UI_DOMAIN_H
#define RUFFLE_CORE_DEBUG_UI_DOMAIN_H

#include <string>
#include <vector>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class UpdateContext;

    namespace avm2 {
        template<typename GC>
        class Domain;
    }

    namespace debug_ui {
        namespace handle {
            template<typename GC>
            class AVM2ObjectHandle;
            template<typename GC>
            class DomainHandle;
        }

        enum class Message;
    }
}
}

// Egui forward declarations (would be replaced with wxWidgets in actual implementation)
namespace egui {
    class Context;
    class Ui;
    class TextEdit;
    class Window;
    class ScrollArea;
    class CollapsingState;
    class CollapsingHeader;
}

namespace ruffle {
namespace core {
namespace debug_ui {

using egui::Context;
using egui::Ui;

/// Domain List Window for debugging AVM2 domains
template<typename GC>
class DomainListWindow {
private:
    std::string search_;

public:
    DomainListWindow() : search_("") {}

    /// Show the domain list window
    /// Returns true if window should remain open
    bool show(
        Context* egui_ctx,
        UpdateContext<GC>* context,
        std::vector<Message>* messages
    );

    /// Show a domain and its children recursively
    static void show_domain(
        Ui* ui,
        UpdateContext<GC>* context,
        avm2::Domain<GC> domain,
        std::vector<Message>* messages,
        const std::string& search,
        size_t depth
    );
};

/// Open domain button helper function
template<typename GC>
void open_domain_button(
    Ui* ui,
    UpdateContext<GC>* context,
    std::vector<Message>* messages,
    avm2::Domain<GC> domain
);

} // namespace debug_ui
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DEBUG_UI_DOMAIN_H
