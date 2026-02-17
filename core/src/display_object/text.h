// C++ translation of core/src/display_object/text.rs
// Text (StaticText) display object

#ifndef RUFFLE_CORE_DISPLAY_OBJECT_TEXT_H
#define RUFFLE_CORE_DISPLAY_OBJECT_TEXT_H

#include <memory>
#include <vector>
#include <cstdint>
#include <optional>
#include <string>
#include <atomic>

// Forward declarations
namespace ruffle {
namespace swf {
    using CharacterId = uint16_t;
    class Text;
    class TextRecord;
    class Color;
    class Rectangle;
    class Matrix;
    template<typename T> class Point;
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
namespace transform {
    class Transform;
}
}

namespace wstr {
    class WString;
    class WStr;
}

namespace core {
namespace avm1 {
    template<typename T> class Object;
}

namespace avm2 {
    template<typename T> class StageObject;
}

namespace context {
    template<typename T> class RenderContext;
    template<typename T> class UpdateContext;
}

namespace font {
    class TextRenderSettings;
    template<typename T> class FontLike;
}

namespace display_object {
    enum class BoundsMode;
    struct Rectangle;
    struct Twips;
    struct Point;
    struct Matrix;
    class DisplayObjectBase;
    template<typename T> class DisplayObjectPtr;
    template<typename T> class DisplayObjectWeak;
    template<typename T> class TDisplayObject;
    template<typename T> class MovieClip;

    template<typename GC> class TextWeak;
    template<typename GC> class TextSnapshot;

    /// Text (StaticText) display object data
    /// Uses HasPrefixField pattern - base must be first member
    template<typename GC>
    class TextData {
    public:
        TextData() = default;

        // Base display object - must be first for HasPrefixField pattern
        DisplayObjectBase<GC> base;

        /// Shared data between all instances of a Text
        std::shared_ptr<TextShared> shared;

        /// Text render settings
        font::TextRenderSettings render_settings;

        /// The AVM2 side of this object
        std::optional<avm2::StageObject<GC>> avm2_object;

        // Friend classes for accessing private members
        friend class Text<GC>;
        friend class TextWeak<GC>;
    };

    /// Data shared between all instances of a text object
    struct TextShared {
        std::shared_ptr<common::tag_utils::SwfMovie> swf;
        swf::CharacterId id;
        swf::Rectangle<swf::Twips> bounds;
        swf::Matrix text_transform;
        std::vector<swf::TextRecord> text_blocks;
    };

    /// A Text (StaticText) display object represents static text from a SWF file.
    template<typename GC>
    class Text {
    public:
        using DataType = TextData<GC>;
        using WeakType = TextWeak<GC>;

        Text() = default;
        explicit Text(std::shared_ptr<DataType> data) : data_(std::move(data)) {}

        /// Construct a Text from SWF tag
        static Text from_swf_tag(
            context::UpdateContext<GC>* context,
            std::shared_ptr<common::tag_utils::SwfMovie> swf,
            const swf::Text& tag
        );

        /// Set render settings
        void set_render_settings(const font::TextRenderSettings& settings);

        /// Get the text content
        std::optional<wstr::WString> text(context::UpdateContext<GC>* context);

        /// Get the base display object (for TDisplayObject trait)
        const DisplayObjectBase<GC>& base() const {
            return data_->base;
        }

        /// Get the character ID
        swf::CharacterId id() const {
            return data_->shared->id;
        }

        /// Get the movie
        std::shared_ptr<common::tag_utils::SwfMovie> movie() const {
            return data_->shared->swf;
        }

        /// Replace with another text by ID
        void replace_with(context::UpdateContext<GC>* context, swf::CharacterId id);

        /// Render this display object
        void render_self(context::RenderContext<GC>* context);

        /// Get self bounds
        Rectangle<Twips> self_bounds(BoundsMode mode) const {
            return data_->shared->bounds;
        }

        /// Hit test against a shape
        bool hit_test_shape(
            context::UpdateContext<GC>* context,
            Point<Twips> point,
            HitTestOptions options
        );

        /// Construct frame handler
        void construct_frame(context::UpdateContext<GC>* context);

        /// Post instantiation handler
        void post_instantiation(
            context::UpdateContext<GC>* context,
            std::optional<avm1::Object<GC>> init_object,
            vminterface::Instantiator instantiated_by,
            bool run_frame
        );

        /// Get AVM1 object (not supported for Text)
        std::optional<avm1::Object<GC>> object1() const {
            return std::nullopt;
        }

        /// Get AVM2 object
        std::optional<avm2::StageObject<GC>> object2() const {
            return data_->avm2_object;
        }

        /// Set AVM2 object
        void set_object2(context::UpdateContext<GC>* context, avm2::StageObject<GC> to);

        /// Create a weak reference to this Text
        TextWeak<GC> downgrade() const;

    private:
        std::shared_ptr<DataType> data_;

        /// Set shared data
        void set_shared(context::UpdateContext<GC>* context, std::shared_ptr<TextShared> to);
    };

    /// Weak reference to Text
    template<typename GC>
    class TextWeak {
    public:
        TextWeak() = default;
        explicit TextWeak(std::weak_ptr<TextData<GC>> data)
            : data_(std::move(data)) {}

        /// Try to upgrade the weak reference to a strong reference
        std::optional<Text<GC>> upgrade() const {
            if (auto locked = data_.lock()) {
                return std::make_optional(Text<GC>(locked));
            }
            return std::nullopt;
        }

    private:
        std::weak_ptr<TextData<GC>> data_;
    };

    /// Text snapshot for text selection
    template<typename GC>
    class TextSnapshot {
    public:
        TextSnapshot() = default;
        explicit TextSnapshot(std::shared_ptr<TextSnapshotData<GC>> data)
            : data_(std::move(data)) {}

        /// Create a text snapshot from a movie clip
        static TextSnapshot create(
            context::UpdateContext<GC>* context,
            MovieClip<GC> target
        );

        /// Get the total character count
        size_t count() const;

        /// Get text in a range
        wstr::WString get_text(int32_t from, int32_t to, bool include_newlines);

        /// Find text in the snapshot
        int32_t find_text(int32_t from, const wstr::WStr& text, bool case_sensitive);

    private:
        std::shared_ptr<TextSnapshotData<GC>> data_;
    };

    /// Text snapshot data
    template<typename GC>
    class TextSnapshotData {
    public:
        std::vector<TextSnapshotChunk<GC>> chunks;
    };

    /// Text snapshot chunk
    template<typename GC>
    class TextSnapshotChunk {
    public:
        Text<GC> object;
        wstr::WString text;
        size_t global_index;

        /// Get substring from this chunk
        const wstr::WStr& sub_string(size_t global_index_start, size_t global_index_end) const;
    };

} // namespace display_object
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DISPLAY_OBJECT_TEXT_H
