// C++ translation of core/src/debug_ui/avm2.rs
// Debug UI for AVM2 object inspection

#ifndef RUFFLE_CORE_DEBUG_UI_AVM2_H
#define RUFFLE_CORE_DEBUG_UI_AVM2_H

#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <cstdint>
#include <unordered_map>

// Forward declarations
namespace ruffle {
namespace core {

namespace avm2 {
    template<typename GC>
    class Activation;
    template<typename GC>
    class Error;
    template<typename GC>
    class Object;
    template<typename GC>
    class Value;
    template<typename GC>
    class ClassObject;
    template<typename GC>
    class Namespace;
    template<typename GC>
    class ArrayStorage;
    template<typename GC>
    class StyleSheetObject;
    enum class Property;
}

namespace debug_ui {
    enum class Message;
    struct DisplayObjectHandle;
    struct AVM2ObjectHandle;
    struct ItemToSave;
}

namespace display_object {
    template<typename GC>
    class DisplayObject;
}

namespace tag_utils {
    template<typename GC>
    class SwfMovie;
}

namespace bitmap {
    template<typename GC>
    class BitmapData;
}

} // namespace core
} // namespace ruffle

namespace gc_arena {
    template<typename GC>
    class Mutation;
}

namespace ruffle {
namespace core {
namespace debug_ui {
namespace avm2 {

/// Panel types for AVM2 object window
enum class Panel {
    Information,
    Properties,
    Elements,
    Class,
    StyleSheet
};

/// Widget for displaying a value result
template<typename GC>
class ValueResultWidget {
public:
    /// Create from result
    static ValueResultWidget create(
        avm2::Activation<GC>* activation,
        std::result::Result<avm2::Value<GC>, avm2::Error<GC>> value
    );

    /// Show the widget
    void show(void* ui, std::vector<Message>* messages) const;
};

/// Window for inspecting AVM2 objects
class Avm2ObjectWindow {
private:
    std::optional<DisplayObjectHandle> hovered_debug_rect_;
    bool show_private_items_ = false;
    bool call_getters_ = false;
    std::unordered_map<std::pair<std::string, std::string>, std::optional<ValueResultWidget>> getter_values_;
    std::string search_;
    Panel open_panel_ = Panel::Properties;

public:
    Avm2ObjectWindow() = default;

    /// Get the hovered debug rect
    std::optional<DisplayObjectHandle> hovered_debug_rect() const {
        return hovered_debug_rect_;
    }

    /// Show the AVM2 object window
    /// Returns true if window should stay open
    template<typename GC>
    bool show(
        void* egui_ctx,
        UpdateContext<GC>* context,
        avm2::Object<GC> object,
        std::vector<Message>* messages
    );

private:
    /// Show information panel
    template<typename GC>
    void show_information(
        avm2::Object<GC> object,
        std::vector<Message>* messages,
        avm2::Activation<GC>* activation,
        void* ui
    );

    /// Show elements panel for arrays
    template<typename GC>
    void show_elements(
        std::shared_ptr<avm2::ArrayStorage<GC>> array,
        std::vector<Message>* messages,
        UpdateContext<GC>* context,
        void* ui
    );

    /// Show class panel
    template<typename GC>
    void show_class(
        avm2::ClassObject<GC> class_obj,
        std::vector<Message>* messages,
        avm2::Activation<GC>* activation,
        void* ui
    );

    /// Show style sheet panel
    template<typename GC>
    void show_style_sheet(
        avm2::StyleSheetObject<GC> style_sheet,
        void* ui
    );

    /// Show properties panel
    template<typename GC>
    void show_properties(
        avm2::Object<GC> object,
        std::vector<Message>* messages,
        avm2::Activation<GC>* activation,
        void* ui
    );

    /// Show a single property
    template<typename GC>
    void show_property(
        avm2::Object<GC> object,
        std::vector<Message>* messages,
        avm2::Activation<GC>* activation,
        void* body,
        const std::string& name,
        avm2::Namespace<GC> ns,
        avm2::Property prop
    );
};

/// Show an AVM2 value widget
template<typename GC>
void show_avm2_value(
    void* ui,
    UpdateContext<GC>* context,
    avm2::Value<GC> value,
    std::vector<Message>* messages
);

/// Get a display name for an AVM2 object
template<typename GC>
std::string object_name(gc_arena::Mutation<GC>* mc, avm2::Object<GC> object);

} // namespace avm2
} // namespace debug_ui
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DEBUG_UI_AVM2_H
