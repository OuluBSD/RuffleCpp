// C++ translation of core/src/display_object/loader_display.rs
// LoaderDisplay display object

#ifndef RUFFLE_CORE_DISPLAY_OBJECT_LOADER_DISPLAY_H
#define RUFFLE_CORE_DISPLAY_OBJECT_LOADER_DISPLAY_H

#include <memory>
#include <vector>
#include <cstdint>
#include <optional>
#include <atomic>

// Forward declarations
namespace ruffle {
namespace swf {
    using CharacterId = uint16_t;
}

namespace common {
namespace tag_utils {
    class SwfMovie;
    class SwfSlice;
}
namespace utils {
    template<typename Self, typename Inner>
    struct HasPrefixField;
}
}

namespace core {
namespace avm1 {
    template<typename T> class Object;
}

namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class StageObject;
}

namespace context {
    template<typename T> class RenderContext;
    template<typename T> class UpdateContext;
}

namespace display_object {
    enum class BoundsMode;
    struct Rectangle;
    struct Twips;
    struct Point;
    class DisplayObjectBase;
    template<typename T> class DisplayObjectPtr;
    template<typename T> class DisplayObjectWeak;
    template<typename T> class TDisplayObject;
    template<typename T> class TInteractiveObject;
    template<typename T> class TDisplayObjectContainer;
    template<typename T> class InteractiveObjectBase;
    template<typename T> class ChildContainer;
    namespace interactive {
        template<typename GC>
        struct Avm2MousePick;
    }

    namespace events {
        template<typename T> class ClipEvent;
        enum class ClipEventResult;
    }

    namespace vminterface {
        enum class Instantiator;
    }

    /// LoaderDisplay display object data
    /// Uses HasPrefixField pattern - base must be first member
    template<typename GC>
    class LoaderDisplayData {
    public:
        LoaderDisplayData() = default;

        // Base display object - must be first for HasPrefixField pattern
        InteractiveObjectBase<GC> base;

        /// Child container for display list management
        std::shared_ptr<ChildContainer<GC>> container;

        /// The AVM2 side of this object.
        std::optional<avm2::StageObject<GC>> avm2_object;

        /// The movie this loader display belongs to
        std::shared_ptr<common::tag_utils::SwfMovie> movie;

        // Friend classes for accessing private members
        friend class LoaderDisplay<GC>;
        friend class LoaderDisplayWeak<GC>;
    };

    /// LoaderDisplay display object
    /// A display object that can load and display content dynamically
    template<typename GC>
    class LoaderDisplay {
    public:
        using DataType = LoaderDisplayData<GC>;
        using WeakType = LoaderDisplayWeak<GC>;

        LoaderDisplay() = default;
        explicit LoaderDisplay(std::shared_ptr<DataType> data) : data_(std::move(data)) {}

        /// Create an empty LoaderDisplay
        static LoaderDisplay create_empty(
            avm2::Activation<GC>* activation,
            std::shared_ptr<common::tag_utils::SwfMovie> movie
        );

        /// Create a weak reference to this LoaderDisplay
        LoaderDisplayWeak<GC> downgrade() const;

        /// Get the base display object (for TDisplayObject trait)
        const DisplayObjectBase<GC>& base() const {
            return data_->base;
        }

        /// Get the character ID
        swf::CharacterId id() const {
            return swf::CharacterId(UINT16_MAX);
        }

        /// Get the movie
        std::shared_ptr<common::tag_utils::SwfMovie> movie() const {
            return data_->movie;
        }

        /// Get the AVM2 object
        std::optional<avm2::StageObject<GC>> object2() const {
            return data_->avm2_object;
        }

        /// Set the AVM2 object
        void set_object2(avm2::StageObject<GC> object);

        /// Render this display object
        void render_self(context::RenderContext<GC>* context);

        /// Get self bounds
        Rectangle<Twips> self_bounds(BoundsMode mode) const;

        /// Get AVM1 object (not supported for LoaderDisplay)
        std::optional<avm1::Object<GC>> object1() const {
            return std::nullopt;
        }

        /// Enter frame handler
        void enter_frame(context::UpdateContext<GC>* context);

        /// Construct frame handler
        void construct_frame(context::UpdateContext<GC>* context);

        /// Post instantiation handler
        void post_instantiation(
            context::UpdateContext<GC>* context,
            std::optional<avm1::Object<GC>> init_object,
            vminterface::Instantiator instantiated_by,
            bool run_frame
        );

        /// Handler for when parent is removed
        void on_parent_removed(context::UpdateContext<GC>* context);

        /// Filter clip event
        events::ClipEventResult filter_clip_event(
            context::UpdateContext<GC>* context,
            events::ClipEvent<GC> event
        );

        /// Event dispatch handler
        events::ClipEventResult event_dispatch(
            context::UpdateContext<GC>* context,
            events::ClipEvent<GC> event
        );

        /// Mouse pick for AVM2
        interactive::Avm2MousePick<GC> mouse_pick_avm2(
            context::UpdateContext<GC>* context,
            Point<Twips> point,
            bool require_button_mode
        );

    private:
        std::shared_ptr<DataType> data_;
    };

    /// Weak reference to LoaderDisplay
    template<typename GC>
    class LoaderDisplayWeak {
    public:
        LoaderDisplayWeak() = default;
        explicit LoaderDisplayWeak(std::weak_ptr<LoaderDisplayData<GC>> data)
            : data_(std::move(data)) {}

        /// Try to upgrade the weak reference to a strong reference
        std::optional<LoaderDisplay<GC>> upgrade() const {
            if (auto locked = data_.lock()) {
                return std::make_optional(LoaderDisplay<GC>(locked));
            }
            return std::nullopt;
        }

        /// Get the raw pointer
        const DisplayObjectPtr* as_ptr() const {
            return reinterpret_cast<const DisplayObjectPtr*>(data_.lock().get());
        }

    private:
        std::weak_ptr<LoaderDisplayData<GC>> data_;
    };

} // namespace display_object
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DISPLAY_OBJECT_LOADER_DISPLAY_H
