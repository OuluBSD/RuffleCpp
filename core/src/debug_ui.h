// C++ translation of core/src/debug_ui.rs
// Debug UI for Ruffle

#ifndef RUFFLE_CORE_DEBUG_UI_H
#define RUFFLE_CORE_DEBUG_UI_H

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class UpdateContext;
    template<typename GC>
    class RenderContext;

    namespace display_object {
        template<typename GC>
        class DisplayObject;
    }

    namespace tag_utils {
        template<typename GC>
        class SwfMovie;
    }

    namespace debug_ui {
        namespace avm1 {
            template<typename GC>
            class Avm1ObjectWindow;
        }

        namespace avm2 {
            template<typename GC>
            class Avm2ObjectWindow;
        }

        namespace display_object {
            template<typename GC>
            class DisplayObjectWindow;
            template<typename GC>
            class DisplayObjectSearchWindow;
        }

        namespace handle {
            template<typename GC>
            class DisplayObjectHandle;
            template<typename GC>
            class AVM1ObjectHandle;
            template<typename GC>
            class AVM2ObjectHandle;
            template<typename GC>
            class DomainHandle;
        }

        namespace movie {
            template<typename GC>
            class MovieWindow;
            class MovieListWindow;
        }

        namespace domain {
            template<typename GC>
            class DomainListWindow;
        }

        enum class Message;
    }
}
}

// External library forward declarations
namespace gc_arena {
    template<typename T>
    class DynamicRootSet;
    class Mutation;
}

namespace hashbrown {
    template<typename K, typename V>
    class HashMap;
}

namespace weak_table {
    template<typename K, typename V>
    class PtrWeakKeyHashMap;
}

namespace swf {
    struct Color;
    template<typename T>
    struct Rectangle;
    class Twips;
}

namespace egui {
    class Context;
}

namespace ruffle {
namespace core {

using gc_arena::DynamicRootSet;
using hashbrown::HashMap;
using weak_table::PtrWeakKeyHashMap;

namespace debug_ui {

using display_object::DisplayObjectWindow;
using display_object::DisplayObjectSearchWindow;
using avm1::Avm1ObjectWindow;
using avm2::Avm2ObjectWindow;
using handle::DisplayObjectHandle;
using handle::AVM1ObjectHandle;
using handle::AVM2ObjectHandle;
using handle::DomainHandle;
using movie::MovieWindow;
using movie::MovieListWindow;
using domain::DomainListWindow;

/// Item to save from debug UI
struct ItemToSave {
    std::string suggested_name;
    std::vector<uint8_t> data;
};

/// Debug UI state for Ruffle
template<typename GC>
class DebugUi {
private:
    HashMap<DisplayObjectHandle<GC>, DisplayObjectWindow<GC>> display_objects_;
    PtrWeakKeyHashMap<std::weak_ptr<tag_utils::SwfMovie<GC>>, MovieWindow<GC>> movies_;
    HashMap<AVM1ObjectHandle<GC>, Avm1ObjectWindow<GC>> avm1_objects_;
    HashMap<AVM2ObjectHandle<GC>, Avm2ObjectWindow<GC>> avm2_objects_;
    HashMap<DomainHandle<GC>, DomainListWindow<GC>> domains_;
    std::vector<Message> queued_messages_;
    std::vector<ItemToSave> items_to_save_;
    std::optional<MovieListWindow<GC>> movie_list_;
    std::optional<DomainListWindow<GC>> domain_list_;
    std::optional<DisplayObjectSearchWindow<GC>> display_object_search_;

public:
    DebugUi() = default;

    /// Show the debug UI
    void show(
        egui::Context* egui_ctx,
        UpdateContext<GC>* context,
        double movie_offset
    );

    /// Check if player should be suspended (for search dialog)
    bool should_suspend_player() const {
        return display_object_search_.has_value();
    }

    /// Get items to save
    std::vector<ItemToSave> items_to_save() {
        std::vector<ItemToSave> result = std::move(items_to_save_);
        return result;
    }

    /// Queue a message for processing
    void queue_message(Message message) {
        queued_messages_.push_back(std::move(message));
    }

    /// Track a display object
    void track_display_object(DisplayObjectHandle<GC> handle) {
        display_objects_.insert(handle, DisplayObjectWindow<GC>());
    }

    /// Draw debug rects for tracked objects
    void draw_debug_rects(
        RenderContext<GC>* context,
        DynamicRootSet<GC> dynamic_root_set
    );

private:
    void track_display_object_internal(DisplayObjectHandle<GC> handle);
};

/// Draw a debug rectangle
void draw_debug_rect(
    RenderContext<GC>* context,
    const swf::Color& color,
    const swf::Rectangle<swf::Twips>& bounds,
    float thickness
);

} // namespace debug_ui
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DEBUG_UI_H
