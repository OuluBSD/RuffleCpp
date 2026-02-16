// C++ translation of core/src/debug_ui/display_object/search.rs
//! Display Object Search Window for Debug UI

#ifndef RUFFLE_CORE_DEBUG_UI_DISPLAY_OBJECT_SEARCH_H
#define RUFFLE_CORE_DEBUG_UI_DISPLAY_OBJECT_SEARCH_H

#include <vector>
#include <unordered_map>
#include <optional>

// Forward declarations
namespace swf {
    struct Color;
    template<typename T> struct Point;
    class Twips;
}

namespace egui {
    class Context;
    class Ui;
    class Vec2;
}

namespace ruffle {
namespace core {
    template<typename GC> class UpdateContext;
    namespace display_object {
        template<typename GC> class DisplayObject;
        template<typename GC> class TDisplayObject;
        template<typename GC> class TDisplayObjectContainer;
        template<typename GC> class TInteractiveObject;
        class DisplayObjectPtr;
    }
    namespace debug_ui {
        struct Message;
    }
}
}

namespace ruffle {
namespace core {
namespace debug_ui {
namespace display_object {

// Forward declaration
class DisplayObjectHandle;

/// A tree node in the display object search results
struct DisplayObjectTree {
    /// Handle to the display object
    DisplayObjectHandle handle;

    /// Child objects in the tree
    std::vector<DisplayObjectTree> children;

    /// Color for highlighting (RGB values in [0, 1])
    float color[3];
};

/// Display Object Search Window
///
/// Allows users to click on the stage to find display objects
/// at that location and inspect them in a tree view.
class DisplayObjectSearchWindow {
private:
    /// Whether we are currently searching (waiting for click)
    bool finding;

    /// Search results as a tree of display objects
    std::vector<DisplayObjectTree> results;

    /// Unique results mapped by handle for color assignment
    std::unordered_map<DisplayObjectHandle, swf::Color> unique_results;

    /// Currently hovered debug rect
    std::optional<DisplayObjectHandle> hovered_debug_rect;

    /// Whether to include hidden objects in search
    bool include_hidden;

    /// Whether to only include mouse-enabled objects
    bool only_mouse_enabled;

public:
    /// Default constructor
    DisplayObjectSearchWindow();

    /// Get iterator over hovered debug rects
    ///
    /// Returns either the single hovered rect or all unique results
    /// depending on hover state.
    [[nodiscard]] std::vector<std::pair<const DisplayObjectHandle*, const swf::Color*>> hovered_debug_rects() const;

    /// Show the search window
    ///
    /// \param egui_ctx The egui context
    /// \param context The update context
    /// \param messages Vector to collect debug messages
    /// \param movie_offset Offset for movie position
    /// \return true if window should remain open
    bool show(
        egui::Context* egui_ctx,
        UpdateContext</* GC */ void>* context,
        std::vector<Message>& messages,
        double movie_offset
    );

    /// Generate search results based on pointer position
    ///
    /// \param egui_ctx The egui context
    /// \param context The update context
    /// \param movie_offset Offset for movie position
    void generate_results(
        egui::Context* egui_ctx,
        UpdateContext</* GC */ void>* context,
        double movie_offset
    );

private:
    /// Check if an object matches the search criteria
    ///
    /// \param object The display object to check
    /// \param cursor The cursor position in twips
    /// \return true if object matches
    bool object_matches(
        const display_object::DisplayObjectPtr* object,
        const swf::Point<swf::Twips>& cursor
    ) const;

    /// Create a result tree for an object and its children
    ///
    /// \param context The update context
    /// \param cursor The cursor position
    /// \param object The display object
    /// \param add_to Vector to add the tree to
    void create_result_tree(
        UpdateContext</* GC */ void>* context,
        const swf::Point<swf::Twips>& cursor,
        const display_object::DisplayObjectPtr* object,
        std::vector<DisplayObjectTree>& add_to
    );
};

/// Show an object tree in the UI
///
/// \param ui The egui UI
/// \param context The update context
/// \param tree The tree to display
/// \param messages Vector to collect debug messages
/// \param hovered_debug_rect Reference to update hovered rect
void show_object_tree(
    egui::Ui* ui,
    UpdateContext</* GC */ void>* context,
    const DisplayObjectTree& tree,
    std::vector<Message>& messages,
    std::optional<DisplayObjectHandle>& hovered_debug_rect
);

/// Show a single item in the object tree
///
/// \param ui The egui UI
/// \param context The update context
/// \param tree The tree node
/// \param messages Vector to collect debug messages
/// \param hovered_debug_rect Reference to update hovered rect
void show_item(
    egui::Ui* ui,
    UpdateContext</* GC */ void>* context,
    const DisplayObjectTree& tree,
    std::vector<Message>& messages,
    std::optional<DisplayObjectHandle>& hovered_debug_rect
);

} // namespace display_object
} // namespace debug_ui
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DEBUG_UI_DISPLAY_OBJECT_SEARCH_H
