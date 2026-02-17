// C++ translation of core/src/display_object/morph_shape.rs
// MorphShape display object for shape tweens

#ifndef RUFFLE_CORE_DISPLAY_OBJECT_MORPH_SHAPE_H
#define RUFFLE_CORE_DISPLAY_OBJECT_MORPH_SHAPE_H

#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include <unordered_map>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {

namespace avm1 {
    template<typename GC>
    class Object;
}

namespace avm2 {
    template<typename GC>
    class StageObject;
}

namespace tag_utils {
    template<typename GC>
    class SwfMovie;
}

namespace library {
    template<typename GC>
    class Library;
    class MovieLibrarySource;
}

namespace render {
namespace backend {
    class ShapeHandle;
}
namespace commands {
    template<typename GC>
    class CommandHandler;
}
}

namespace display_object {
    template<typename GC>
    class DisplayObjectBase;
    template<typename GC>
    class DisplayObject;
    enum class BoundsMode;
    template<typename GC>
    class TDisplayObject;
}

} // namespace core
} // namespace ruffle

namespace swf {
    struct DefineMorphShape;
    struct MorphShape;
    struct Shape;
    struct Color;
    struct FillStyle;
    struct LineStyle;
    struct ShapeRecord;
    struct ShapeStyles;
    struct Gradient;
    struct Matrix;
    class Fixed8;
    class Fixed16;
}

namespace ruffle_render {
namespace shape_utils {
    template<typename T>
    struct Rectangle;
    template<typename T>
    struct Point;
}
}

namespace gc_arena {
    template<typename GC>
    class Gc;
    template<typename GC>
    class Mutation;
    namespace lock {
        template<typename T>
        class Lock;
    }
}

namespace ruffle {
namespace core {
namespace display_object {

using swf::CharacterId;
using ruffle_render::shape_utils::Rectangle;
using ruffle_render::shape_utils::Point;
using Twips = int32_t;  // Simplified Twips type

/// A precalculated intermediate frame for a morph shape.
struct Frame {
    std::optional<render::backend::ShapeHandle> shape_handle;
    swf::Shape shape;
    Rectangle<Twips> bounds;
};

/// Data shared between all instances of a morph shape.
class MorphShapeShared {
private:
    CharacterId id_;
    swf::MorphShape start_;
    swf::MorphShape end_;
    std::unordered_map<uint16_t, Frame> frames_;
    std::shared_ptr<tag_utils::SwfMovie> movie_;

public:
    MorphShapeShared(
        CharacterId id,
        swf::MorphShape start,
        swf::MorphShape end,
        std::shared_ptr<tag_utils::SwfMovie> movie
    )
        : id_(id)
        , start_(std::move(start))
        , end_(std::move(end))
        , movie_(std::move(movie))
    {}

    /// Create from SWF tag
    static MorphShapeShared from_swf_tag(
        const swf::DefineMorphShape& swf_tag,
        std::shared_ptr<tag_utils::SwfMovie> movie
    );

    /// Get character ID
    CharacterId id() const { return id_; }

    /// Get the Frame for the given ratio (lazily initialized)
    const Frame& get_frame(uint16_t ratio);

    /// Get the ShapeHandle for the given ratio (lazily tessellated)
    template<typename GC>
    render::backend::ShapeHandle get_shape(
        RenderContext<GC>* context,
        const Library<GC>* library,
        uint16_t ratio
    );

private:
    /// Build a morph frame at the given ratio
    Frame build_morph_frame(uint16_t ratio);

    /// Update position based on shape record
    static void update_pos(Twips* x, Twips* y, const swf::ShapeRecord& record);
};

/// MorphShape display object data
template<typename GC>
class MorphShapeData {
private:
    DisplayObjectBase<GC> base_;
    gc_arena::lock::Lock<std::shared_ptr<MorphShapeShared>> shared_;
    gc_arena::lock::Lock<std::optional<avm2::StageObject<GC>>> object_;

public:
    MorphShapeData() = default;

    DisplayObjectBase<GC>& base() { return base_; }
    const DisplayObjectBase<GC>& base() const { return base_; }

    std::shared_ptr<MorphShapeShared> shared() const { return shared_.get(); }
    void set_shared(std::shared_ptr<MorphShapeShared> shared) { shared_.set(std::move(shared)); }

    std::optional<avm2::StageObject<GC>> object() const { return object_.get(); }
    void set_object(std::optional<avm2::StageObject<GC>> obj) { object_.set(std::move(obj)); }
};

/// MorphShape display object for shape tweens
template<typename GC>
class MorphShape {
private:
    gc_arena::Gc<GC, MorphShapeData<GC>> data_;

public:
    MorphShape() = default;
    explicit MorphShape(gc_arena::Gc<GC, MorphShapeData<GC>> data) : data_(data) {}

    /// Create from SWF tag
    static MorphShape from_swf_tag(
        gc_arena::Mutation<GC>* gc_context,
        const swf::DefineMorphShape& tag,
        std::shared_ptr<tag_utils::SwfMovie> movie
    );

    /// Get base display object
    DisplayObjectBase<GC>& base() { return data_->base(); }
    const DisplayObjectBase<GC>& base() const { return data_->base(); }

    /// Instantiate a new copy of this display object
    DisplayObject<GC> instantiate(gc_arena::Mutation<GC>* gc_context);

    /// Get character ID
    CharacterId id() const;

    /// Replace with another morph shape
    template<typename Context>
    void replace_with(Context* context, CharacterId id);

    /// Get AVM1 object (always None for MorphShape)
    std::optional<avm1::Object<GC>> object1() const { return std::nullopt; }

    /// Get AVM2 object
    std::optional<avm2::StageObject<GC>> object2() const { return data_->object().get(); }

    /// Set AVM2 object
    template<typename Context>
    void set_object2(Context* context, avm2::StageObject<GC> obj);

    /// Construct frame (initialize AVM2 object if needed)
    template<typename Context>
    void construct_frame(Context* context);

    /// Render the morph shape
    template<typename Context>
    void render_self(Context* context);

    /// Get bounds
    Rectangle<Twips> self_bounds(BoundsMode mode) const;

    /// Hit test against shape
    template<typename Context>
    bool hit_test_shape(
        Context* context,
        Point<Twips> point,
        HitTestOptions options
    );

    /// Post-instantiation callback
    template<typename Context>
    void post_instantiation(
        Context* context,
        std::optional<avm1::Object<GC>> init_object,
        Instantiator instantiated_by,
        bool run_frame
    );

    /// Get the movie
    std::shared_ptr<tag_utils::SwfMovie> movie() const;

    /// Get the current ratio
    uint16_t ratio() const;

    /// Invalidate cached bitmap
    void invalidate_cached_bitmap();
};

// Interpolation functions

/// Interpolate between two colors
swf::Color lerp_color(const swf::Color& start, const swf::Color& end, float a, float b);

/// Interpolate between two Twips values
Twips lerp_twips(Twips start, Twips end, float a, float b);

/// Interpolate between two points
Point<Twips> lerp_point_twips(const Point<Twips>& start, const Point<Twips>& end, float a, float b);

/// Interpolate between two fill styles
swf::FillStyle lerp_fill(const swf::FillStyle* start, const swf::FillStyle* end, float a, float b);

/// Interpolate between two shape edges
swf::ShapeRecord lerp_edges(
    const Point<Twips>& start_pen,
    const Point<Twips>& end_pen,
    const swf::ShapeRecord* start,
    const swf::ShapeRecord* end,
    float a,
    float b
);

/// Interpolate between two matrices
swf::Matrix lerp_matrix(const swf::Matrix* start, const swf::Matrix* end, float a, float b);

/// Interpolate between two gradients
swf::Gradient lerp_gradient(const swf::Gradient* start, const swf::Gradient* end, float a, float b);

} // namespace display_object
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DISPLAY_OBJECT_MORPH_SHAPE_H
