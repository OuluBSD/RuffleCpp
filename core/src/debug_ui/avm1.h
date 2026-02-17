// C++ translation of core/src/debug_ui/avm1.rs
// Debug UI for AVM1 object inspection

#ifndef RUFFLE_CORE_DEBUG_UI_AVM1_H
#define RUFFLE_CORE_DEBUG_UI_AVM1_H

#include <string>
#include <vector>
#include <optional>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {

namespace avm1 {
    template<typename GC>
    class Activation;
    template<typename GC>
    class Error;
    template<typename GC>
    class Object;
    template<typename GC>
    class Value;
    enum class NativeObject;
}

namespace string {
    class AvmString;
}

namespace wstr {
    class WStr;
    class WString;
}

namespace debug_ui {
    enum class Message;
    struct DisplayObjectHandle;
}

namespace display_object {
    template<typename GC>
    class DisplayObject;
}

} // namespace core
} // namespace ruffle

namespace ruffle {
namespace core {
namespace debug_ui {
namespace avm1 {

/// Panel types for AVM1 object window
enum class Panel {
    Object,
    NativeObject
};

/// Window for inspecting AVM1 objects
class Avm1ObjectWindow {
private:
    std::optional<DisplayObjectHandle> hovered_debug_rect_;
    std::string key_filter_string_;
    std::optional<WString> edited_key_;
    std::string value_edit_buf_;
    bool focus_text_edit_ = false;
    Panel open_panel_ = Panel::Object;

public:
    Avm1ObjectWindow() = default;

    /// Get the hovered debug rect
    std::optional<DisplayObjectHandle> hovered_debug_rect() const {
        return hovered_debug_rect_;
    }

    /// Show the AVM1 object window
    /// Returns true if window should stay open
    template<typename GC>
    bool show(
        void* egui_ctx,
        UpdateContext<GC>* context,
        avm1::Object<GC> object,
        std::vector<Message>* messages
    );

private:
    /// Show object panel
    template<typename GC>
    void show_object_panel(
        void* ui,
        avm1::Object<GC> object,
        avm1::Activation<GC>* activation,
        std::vector<Message>* messages
    );

    /// Show AVM1 value widget
    /// Optionally returns the updated value if user edited it
    template<typename GC>
    std::optional<avm1::Value<GC>> show_avm1_value(
        void* ui,
        avm1::Activation<GC>* activation,
        string::AvmString key,
        std::result::Result<avm1::Value<GC>, avm1::Error<GC>> value,
        std::vector<Message>* messages
    );

    /// Show number edit UI
    template<typename GC>
    std::optional<double> num_edit_ui(
        void* ui,
        string::AvmString key,
        double num
    );

    /// Show string edit UI
    template<typename GC>
    std::optional<std::string> string_edit_ui(
        void* ui,
        string::AvmString key,
        string::AvmString string
    );

    /// Show style sheet panel
    template<typename GC>
    void show_style_sheet_panel(
        void* ui,
        avm1::NativeObject::StyleSheetObject<GC> object
    );
};

/// Dropdown menu indicating the type of the value
template<typename GC>
std::optional<avm1::Value<GC>> show_value_type_combo_box(
    void* ui,
    const wstr::WStr* key,
    const avm1::Value<GC>* value,
    void* mutation
);

/// Get label for value type
const char* value_label(const avm1::Value<GC>& value);

/// UI extension trait for edit/set/cancel buttons
class UiExt {
public:
    /// Edit button with pencil icon
    static void* edit_button(void* ui);

    /// Set button with checkmark icon
    static void* set_button(void* ui);

    /// Cancel button with cancel icon
    static void* cancel_button(void* ui);
};

/// Get a display name for an AVM1 object
template<typename GC>
std::string object_name(avm1::Object<GC> object);

// Icon constants
constexpr const char* PENCIL_ICON = "\xE2\x9C\x8F";      // ✏
constexpr const char* CHECKMARK_ICON = "\xE2\x9C\x94";   // ✔
constexpr const char* CANCEL_ICON = "\xF0\x9F\x97\x99";  // 🗙

} // namespace avm1
} // namespace debug_ui
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DEBUG_UI_AVM1_H
