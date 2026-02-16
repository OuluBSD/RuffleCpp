// C++ translation of core/src/debug_ui/common.rs
// Debug UI common utilities

#ifndef RUFFLE_CORE_DEBUG_UI_COMMON_H
#define RUFFLE_CORE_DEBUG_UI_COMMON_H

// Note: Original Rust file uses egui for UI components.
// In C++ version, this would need to be adapted to use wxWidgets.
// For now, this is a stub placeholder.

// Forward declarations
namespace ruffle {
namespace core {
namespace html {
    class StyleSheet;
    class TextFormat;
}
}
}

namespace ruffle {
namespace core {
namespace debug_ui {
namespace common {

// Stub function declarations - would need wxWidgets implementation
// Original functions:
// - show_text_format(ui: &mut Ui, tf: &TextFormat, skip_none: bool)
// - show_style_sheet(ui: &mut Ui, style_sheet: StyleSheet<'_>)

// These functions display TextFormat and StyleSheet properties in a UI grid.
// In C++ version, would use wxWidgets controls like wxGrid or wxFlexGridSizer.

} // namespace common
} // namespace debug_ui
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DEBUG_UI_COMMON_H
