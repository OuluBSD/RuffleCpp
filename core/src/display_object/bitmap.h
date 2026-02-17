// C++ translation of core/src/display_object/bitmap.rs
// Bitmap display object

#ifndef RUFFLE_CORE_DISPLAY_OBJECT_BITMAP_H
#define RUFFLE_CORE_DISPLAY_OBJECT_BITMAP_H

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
}
}

namespace render {
namespace bitmap {
    class Bitmap;
    enum class BitmapFormat;
    enum class PixelSnapping;
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
    template<typename T> class FunctionArgs;
    template<typename T> class StageObject;
    namespace globals {
        namespace flash {
            namespace display {
                template<typename T> class BitmapDataObject;
            }
        }
    }
}

namespace bitmap {
namespace bitmap_data {
    template<typename T> class BitmapData;
    class Color;
}
}

namespace display_object {
    enum class BoundsMode;
    struct Rectangle;
    struct Twips;
    class DisplayObjectBase;
    template<typename T> class DisplayObjectPtr;
    template<typename T> class DisplayObjectWeak;
    template<typename T> class TDisplayObject;

    template<typename T> class BitmapWeak;

    /// The AVM2 class for the Bitmap associated with this object.
    ///
    /// Bitmaps may be associated with either a `Bitmap` or a `BitmapData`
    /// subclass. Its superclass determines how the Bitmap will be constructed.
    template<typename GC>
    enum class BitmapClass {
        /// This Bitmap uses the stock Flash Player classes for itself.
        NoSubclass,

        /// This Bitmap overrides its `Bitmap` class and holds a stock `BitmapData`
        /// with its pixel data.
        ///
        /// This is the normal symbol class association for Flex image embeds.
        /// Adobe Animate does not support compiling Bitmaps with `Bitmap`
        /// subclasses (as of version 2022).
        Bitmap,

        /// This Bitmap uses the stock `Bitmap` class with a custom `BitmapData`
        /// subclass to hold its pixel data.
        ///
        /// This is the normal symbol class association for Adobe Animate image
        /// embeds.
        BitmapData,
    };

    /// Bitmap display object data
    /// Uses HasPrefixField pattern - base must be first member
    template<typename GC>
    class BitmapGraphicData {
    public:
        BitmapGraphicData() = default;

        // Base display object - must be first for HasPrefixField pattern
        DisplayObjectBase<GC> base;

        std::shared_ptr<common::tag_utils::SwfMovie> movie;

        /// The AVM2 side of this object.
        ///
        /// AVM1 code cannot directly reference `Bitmap`s, so this does not support
        /// storing an AVM1 object.
        std::optional<avm2::StageObject<GC>> avm2_object;

        /// The class associated with this Bitmap.
        BitmapClass<GC> avm2_bitmap_class;

        /// The current bitmap data object.
        bitmap::bitmap_data::BitmapData<GC> bitmap_data;

        /// The width and height values are cached from the BitmapData
        /// when this Bitmap instance is first created,
        /// and continue to be reported even if the BitmapData is disposed.
        uint32_t width;
        uint32_t height;

        swf::CharacterId id;

        /// Whether or not bitmap smoothing is enabled.
        bool smoothing;

        /// How to snap this bitmap to the pixel grid
        render::bitmap::PixelSnapping pixel_snapping;

        // Friend classes for accessing private members
        friend class Bitmap<GC>;
        friend class BitmapWeak<GC>;
    };

    /// A Bitmap display object is a raw bitmap on the stage.
    /// This can only be instantiated on the display list in SWFv9 AVM2 files.
    /// In AVM1, this is only a library symbol that is referenced by `Graphic`.
    /// Normally bitmaps are drawn in Flash as part of a Shape tag (`Graphic`),
    /// but starting in AVM2, a raw `Bitmap` display object can be created
    /// with the `PlaceObject3` tag.
    /// It can also be created in ActionScript using the `Bitmap` class.
    template<typename GC>
    class Bitmap {
    public:
        using DataType = BitmapGraphicData<GC>;
        using WeakType = BitmapWeak<GC>;

        Bitmap() = default;
        explicit Bitmap(std::shared_ptr<DataType> data) : data_(std::move(data)) {}

        /// Create a `Bitmap` with dynamic bitmap data.
        ///
        /// If `bitmap_data` is provided, the associated `bitmap_handle` must match
        /// the same handle that the data has provided. If it does not match, then
        /// this `Bitmap` will render the wrong data when added to the display
        /// list. If no data is provided then you are free to add whatever handle
        /// you like.
        static Bitmap create_with_bitmap_data(
            swf::CharacterId id,
            bitmap::bitmap_data::BitmapData<GC> bitmap_data,
            bool smoothing,
            std::shared_ptr<common::tag_utils::SwfMovie> movie
        );

        /// Create a `Bitmap` with static bitmap data only.
        static Bitmap create(
            swf::CharacterId id,
            render::bitmap::Bitmap bitmap,
            std::shared_ptr<common::tag_utils::SwfMovie> movie
        );

        /// Get the bitmap width (cached value)
        uint16_t bitmap_width() const {
            return static_cast<uint16_t>(data_->width);
        }

        /// Get the bitmap height (cached value)
        uint16_t bitmap_height() const {
            return static_cast<uint16_t>(data_->height);
        }

        /// Get the pixel snapping mode
        render::bitmap::PixelSnapping pixel_snapping() const {
            return data_->pixel_snapping;
        }

        /// Set the pixel snapping mode
        void set_pixel_snapping(render::bitmap::PixelSnapping value) {
            data_->pixel_snapping = value;
        }

        /// Get the bitmap data
        bitmap::bitmap_data::BitmapData<GC> bitmap_data() const {
            return data_->bitmap_data;
        }

        /// Associate this `Bitmap` with new `BitmapData`.
        ///
        /// Once associated with the new data, the reported width, height, and
        /// bitmap handle of this display object will change to match the given
        /// bitmap data.
        ///
        /// This also forces the `BitmapData` to be sent to the rendering backend,
        /// if that has not already been done.
        void set_bitmap_data(bitmap::bitmap_data::BitmapData<GC> bitmap_data);

        /// Get the AVM2 BitmapData class if this Bitmap uses a custom BitmapData subclass
        std::optional<avm2::ClassObject<GC>> avm2_bitmapdata_class() const;

        /// Get the AVM2 Bitmap class if this Bitmap uses a custom Bitmap subclass
        std::optional<avm2::ClassObject<GC>> avm2_bitmap_class() const;

        /// Set the AVM2 BitmapData class
        void set_avm2_bitmapdata_class(BitmapClass<GC> bitmap_class);

        /// Get whether smoothing is enabled
        bool smoothing() const {
            return data_->smoothing;
        }

        /// Set whether smoothing is enabled
        void set_smoothing(bool smoothing) {
            data_->smoothing = smoothing;
        }

        /// Create a weak reference to this Bitmap
        BitmapWeak<GC> downgrade() const;

        /// Get the base display object data (for TDisplayObject trait)
        const DisplayObjectBase<GC>& base() const {
            return data_->base;
        }

        /// Get the character ID
        swf::CharacterId id() const {
            return data_->id;
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
        void set_object2(avm2::StageObject<GC> object) {
            data_->avm2_object = std::make_optional(std::move(object));
        }

    private:
        std::shared_ptr<DataType> data_;
    };

    /// Weak reference to Bitmap
    template<typename GC>
    class BitmapWeak {
    public:
        BitmapWeak() = default;
        explicit BitmapWeak(std::weak_ptr<BitmapGraphicData<GC>> data)
            : data_(std::move(data)) {}

        /// Try to upgrade the weak reference to a strong reference
        std::optional<Bitmap<GC>> upgrade() const {
            if (auto locked = data_.lock()) {
                return std::make_optional(Bitmap<GC>(locked));
            }
            return std::nullopt;
        }

    private:
        std::weak_ptr<BitmapGraphicData<GC>> data_;
    };

} // namespace display_object
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DISPLAY_OBJECT_BITMAP_H
