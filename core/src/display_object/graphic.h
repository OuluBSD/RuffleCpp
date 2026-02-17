// C++ translation of core/src/display_object/graphic.rs
// Graphic display object

#ifndef RUFFLE_CORE_DISPLAY_OBJECT_GRAPHIC_H
#define RUFFLE_CORE_DISPLAY_OBJECT_GRAPHIC_H

#include <memory>
#include <vector>
#include <cstdint>
#include <optional>
#include <atomic>
#include <cell>
#include <mutex>

// Forward declarations
namespace ruffle {
namespace swf {
    using CharacterId = uint16_t;
    class Shape;
    class ShapeStyles;
    class Rectangle;
    template<typename T> class Point;
    enum class ShapeFlag;
}

namespace common {
namespace tag_utils {
    class SwfMovie;
}
namespace utils {
    template<typename Self, typename Inner>
    struct HasPrefixField;
}
}

namespace render {
namespace backend {
    using ShapeHandle = uint32_t;
}
namespace commands {
    template<typename T> class CommandHandler;
}
}

namespace core {
namespace avm1 {
    template<typename T> class Object;
}

namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Avm2;
    template<typename T> class ClassObject;
    template<typename T> class StageObject;
}

namespace context {
    template<typename T> class RenderContext;
    template<typename T> class UpdateContext;
}

namespace drawing {
    class Drawing;
}

namespace library {
    class MovieLibrarySource;
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

    template<typename GC> class GraphicWeak;

    /// Graphic display object data
    /// Uses HasPrefixField pattern - base must be first member
    template<typename GC>
    class GraphicData {
    public:
        GraphicData() = default;

        // Base display object - must be first for HasPrefixField pattern
        DisplayObjectBase<GC> base;

        /// Shared data between all instances of a Graphic
        std::shared_ptr<GraphicShared<GC>> shared;

        /// The AVM2 class for this graphic
        std::optional<avm2::ClassObject<GC>> avm2_class;

        /// The AVM2 side of this object
        std::optional<avm2::StageObject<GC>> avm2_object;

        /// Drawing data - lazily allocated on demand
        std::optional<std::shared_ptr<Drawing>> drawing;

        // Friend classes for accessing private members
        friend class Graphic<GC>;
        friend class GraphicWeak<GC>;
    };

    /// Data shared between all instances of a Graphic
    template<typename GC>
    struct GraphicShared {
        swf::CharacterId id;
        swf::Shape shape;
        std::optional<render::backend::ShapeHandle> render_handle;
        swf::Rectangle<swf::Twips> bounds;
        std::shared_ptr<common::tag_utils::SwfMovie> movie;
    };

    /// A Graphic display object represents a shape from a SWF file.
    /// Graphics are static shapes that can be instantiated multiple times.
    template<typename GC>
    class Graphic {
    public:
        using DataType = GraphicData<GC>;
        using WeakType = GraphicWeak<GC>;

        Graphic() = default;
        explicit Graphic(std::shared_ptr<DataType> data) : data_(std::move(data)) {}

        /// Construct a Graphic from its associated Shape tag
        static Graphic from_swf_tag(
            context::UpdateContext<GC>* context,
            const swf::Shape& swf_shape,
            std::shared_ptr<common::tag_utils::SwfMovie> movie
        );

        /// Construct an empty Graphic
        static Graphic empty(context::UpdateContext<GC>* context);

        /// Get mutable reference to drawing
        Drawing& drawing_mut();

        /// Set the AVM2 class
        void set_avm2_class(avm2::ClassObject<GC> avm2_class);

        /// Get the base display object (for TDisplayObject trait)
        const DisplayObjectBase<GC>& base() const {
            return data_->base;
        }

        /// Get the character ID
        swf::CharacterId id() const {
            return data_->shared->id;
        }

        /// Get self bounds
        Rectangle<Twips> self_bounds(BoundsMode mode) const;

        /// Construct frame handler
        void construct_frame(context::UpdateContext<GC>* context);

        /// Replace with another graphic by ID
        void replace_with(context::UpdateContext<GC>* context, swf::CharacterId id);

        /// Render this display object
        void render_self(context::RenderContext<GC>* context);

        /// Hit test against a shape
        bool hit_test_shape(
            context::UpdateContext<GC>* context,
            Point<Twips> point,
            HitTestOptions options
        );

        /// Post instantiation handler
        void post_instantiation(
            context::UpdateContext<GC>* context,
            std::optional<avm1::Object<GC>> init_object,
            vminterface::Instantiator instantiated_by,
            bool run_frame
        );

        /// Get the movie
        std::shared_ptr<common::tag_utils::SwfMovie> movie() const {
            return data_->shared->movie;
        }

        /// Get AVM1 object (not supported for Graphic)
        std::optional<avm1::Object<GC>> object1() const {
            return std::nullopt;
        }

        /// Get AVM2 object
        std::optional<avm2::StageObject<GC>> object2() const {
            return data_->avm2_object;
        }

        /// Set AVM2 object
        void set_object2(context::UpdateContext<GC>* context, avm2::StageObject<GC> to);

        /// Get as drawing (if available)
        std::optional<Drawing&> as_drawing();

        /// Create a weak reference to this Graphic
        GraphicWeak<GC> downgrade() const;

    private:
        std::shared_ptr<DataType> data_;

        /// Set shared data
        void set_shared(std::shared_ptr<GraphicShared<GC>> shared);
    };

    /// Weak reference to Graphic
    template<typename GC>
    class GraphicWeak {
    public:
        GraphicWeak() = default;
        explicit GraphicWeak(std::weak_ptr<GraphicData<GC>> data)
            : data_(std::move(data)) {}

        /// Try to upgrade the weak reference to a strong reference
        std::optional<Graphic<GC>> upgrade() const {
            if (auto locked = data_.lock()) {
                return std::make_optional(Graphic<GC>(locked));
            }
            return std::nullopt;
        }

    private:
        std::weak_ptr<GraphicData<GC>> data_;
    };

} // namespace display_object
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DISPLAY_OBJECT_GRAPHIC_H
