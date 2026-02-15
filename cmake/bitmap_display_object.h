/*
 * C++ header for bitmap display object functionality
 * This replaces the functionality of core/src/display_object/bitmap.rs
 */

#ifndef BITMAP_DISPLAY_OBJECT_H
#define BITMAP_DISPLAY_OBJECT_H

#include "display_object.h"
#include "avm2.h"
#include "context.h"
#include "tag_utils.h"
#include "bitmap/bitmap_data.h"
#include "render/bitmap.h"
#include <memory>
#include <optional>

namespace ruffle {

// Forward declarations
class BitmapData;
class Avm2StageObject;
class Avm2ClassObject;
class UpdateContext;
class RenderContext;

// Enum for bitmap class types
enum class BitmapClassType {
    NO_SUBCLASS,
    BITMAP_CLASS,
    BITMAP_DATA_CLASS
};

// Structure to hold bitmap class information
struct BitmapClass {
    BitmapClassType type;
    std::shared_ptr<Avm2ClassObject> class_object;

    BitmapClass() : type(BitmapClassType::NO_SUBCLASS) {}
    BitmapClass(BitmapClassType t, std::shared_ptr<Avm2ClassObject> obj) 
        : type(t), class_object(std::move(obj)) {}

    static BitmapClass no_subclass() {
        return BitmapClass(BitmapClassType::NO_SUBCLASS, nullptr);
    }

    static BitmapClass bitmap_class(std::shared_ptr<Avm2ClassObject> obj) {
        return BitmapClass(BitmapClassType::BITMAP_CLASS, std::move(obj));
    }

    static BitmapClass bitmap_data_class(std::shared_ptr<Avm2ClassObject> obj) {
        return BitmapClass(BitmapClassType::BITMAP_DATA_CLASS, std::move(obj));
    }

    std::shared_ptr<Avm2ClassObject> get_bitmap_class() const {
        return type == BitmapClassType::BITMAP_CLASS ? class_object : nullptr;
    }

    std::shared_ptr<Avm2ClassObject> get_bitmap_data_class() const {
        return type == BitmapClassType::BITMAP_DATA_CLASS ? class_object : nullptr;
    }
};

// Weak reference to a bitmap object
class BitmapWeak {
private:
    std::weak_ptr<struct BitmapData> bitmap_data_;

public:
    explicit BitmapWeak(std::shared_ptr<struct BitmapData> bitmap) : bitmap_data_(bitmap) {}

    std::shared_ptr<struct Bitmap> upgrade() const;
    
    void* as_ptr() const {
        auto locked = bitmap_data_.lock();
        return locked ? locked.get() : nullptr;
    }
};

// A Bitmap display object is a raw bitmap on the stage.
// This can only be instantiated on the display list in SWFv9 AVM2 files.
// In AVM1, this is only a library symbol that is referenced by `Graphic`.
// Normally bitmaps are drawn in Flash as part of a Shape tag (`Graphic`),
// but starting in AVM2, a raw `Bitmap` display object can be created
// with the `PlaceObject3` tag.
// It can also be created in ActionScript using the `Bitmap` class.
class Bitmap : public DisplayObject {
private:
    std::shared_ptr<SwfMovie> movie_;
    std::shared_ptr<Avm2StageObject> avm2_object_;
    BitmapClass avm2_bitmap_class_;
    std::shared_ptr<BitmapData> bitmap_data_;
    uint32_t width_;
    uint32_t height_;
    uint16_t id_;
    bool smoothing_;
    PixelSnapping pixel_snapping_;

public:
    Bitmap(std::shared_ptr<SwfMovie> movie, 
           std::shared_ptr<BitmapData> bitmap_data,
           uint16_t id, 
           bool smoothing = true)
        : DisplayObject(DisplayObjectType::BITMAP, id),
          movie_(std::move(movie)), 
          bitmap_data_(std::move(bitmap_data)),
          id_(id), smoothing_(smoothing), 
          pixel_snapping_(PixelSnapping::AUTO) {
        
        width_ = bitmap_data_ ? bitmap_data_->width() : 0;
        height_ = bitmap_data_ ? bitmap_data_->height() : 0;
    }

    // Create a Bitmap with dynamic bitmap data
    static std::shared_ptr<Bitmap> create_with_bitmap_data(
        std::shared_ptr<MutationContext> mc,
        uint16_t id,
        std::shared_ptr<BitmapData> bitmap_data,
        bool smoothing,
        std::shared_ptr<SwfMovie> movie) {
        
        auto bitmap = std::make_shared<Bitmap>(std::move(movie), std::move(bitmap_data), id, smoothing);
        
        if (bitmap_data) {
            // Add this display object to the bitmap data's display object list
            bitmap_data->add_display_object(bitmap->weak_ref());
        }
        
        return bitmap;
    }

    // Create a Bitmap with static bitmap data
    static std::shared_ptr<Bitmap> create(
        std::shared_ptr<MutationContext> mc,
        uint16_t id,
        ruffle_render::Bitmap bitmap,
        std::shared_ptr<SwfMovie> movie) {
        
        uint32_t width = bitmap.width();
        uint32_t height = bitmap.height();
        
        // Determine if the bitmap has transparency based on format
        bool transparency = (bitmap.format() == BitmapFormat::RGBA);
        
        // Convert pixel data
        std::vector<Color> pixels;
        auto colors = bitmap.as_colors();
        for (auto color : colors) {
            pixels.push_back(Color::from(color));
        }
        
        auto bitmap_data = BitmapData::create_with_pixels(mc, width, height, transparency, pixels);
        bool smoothing = true;
        
        return create_with_bitmap_data(mc, id, bitmap_data, smoothing, movie);
    }

    // Getters
    uint16_t bitmap_width() const { return static_cast<uint16_t>(width_); }
    uint16_t bitmap_height() const { return static_cast<uint16_t>(height_); }
    PixelSnapping pixel_snapping() const { return pixel_snapping_; }
    std::shared_ptr<BitmapData> bitmap_data() const { return bitmap_data_; }
    std::shared_ptr<Avm2ClassObject> avm2_bitmapdata_class() const {
        return avm2_bitmap_class_.get_bitmap_data_class();
    }
    std::shared_ptr<Avm2ClassObject> avm2_bitmap_class() const {
        return avm2_bitmap_class_.get_bitmap_class();
    }
    bool smoothing() const { return smoothing_; }
    std::shared_ptr<SwfMovie> movie() const override { return movie_; }

    // Setters
    void set_pixel_snapping(PixelSnapping value) { pixel_snapping_ = value; }
    
    // Associate this Bitmap with new BitmapData
    void set_bitmap_data(std::shared_ptr<UpdateContext> context,
                        std::shared_ptr<BitmapData> bitmap_data) {
        // Remove this display object from the old bitmap data's list
        if (bitmap_data_) {
            bitmap_data_->remove_display_object(weak_ref());
        }

        // Update cached values
        width_ = bitmap_data ? bitmap_data->width() : 0;
        height_ = bitmap_data ? bitmap_data->height() : 0;
        
        bitmap_data_ = std::move(bitmap_data);

        // Add this display object to the new bitmap data's list
        if (bitmap_data_) {
            bitmap_data_->add_display_object(weak_ref());
        }
    }

    void set_avm2_bitmapdata_class(std::shared_ptr<Avm2ClassObject> cls) {
        avm2_bitmap_class_ = BitmapClass::bitmap_data_class(cls);
    }

    void set_smoothing(bool smoothing) { smoothing_ = smoothing; }

    // Create a weak reference to this bitmap
    BitmapWeak weak_ref() const {
        // In a real implementation, this would return a proper weak reference
        // For now, we'll return a placeholder
        return BitmapWeak(nullptr);
    }

    // Display object interface implementation
    Rectangle<Twips> self_bounds(BoundsMode mode) const override {
        return Rectangle<Twips>(
            Twips(0),
            Twips(0),
            Twips::from_pixels(bitmap_width()),
            Twips::from_pixels(bitmap_height())
        );
    }

    void render_self(std::shared_ptr<RenderContext> context) override {
        // Check if the bitmap is off-screen and can be culled
        if (!context->is_offscreen && 
            !world_bounds(BoundsMode::ENGINE).intersects(context->stage->view_bounds())) {
            // Off-screen; culled
            return;
        }

        if (bitmap_data_) {
            bitmap_data_->render(smoothing_, context, pixel_snapping_);
        }
    }

    std::shared_ptr<Object> object1() const override {
        // AVM1 objects are not supported for Bitmap display objects
        return nullptr;
    }

    std::shared_ptr<Avm2StageObject> object2() const override {
        return avm2_object_;
    }

    void set_object2(std::shared_ptr<UpdateContext> context, 
                    std::shared_ptr<Avm2StageObject> obj) override {
        avm2_object_ = std::move(obj);
    }

    void post_instantiation(std::shared_ptr<UpdateContext> context,
                           std::shared_ptr<Object> init_object,
                           Instantiator instantiated_by,
                           bool run_frame) override {
        if (movie_->is_action_script_3()) {
            set_default_instance_name(context);

            if (!instantiated_by.is_avm()) {
                auto bitmap_cls = avm2_bitmap_class_.get_bitmap_class();
                if (!bitmap_cls) {
                    bitmap_cls = context->avm2->classes().bitmap;
                }
                
                auto bitmapdata_cls = avm2_bitmapdata_class();
                if (!bitmapdata_cls) {
                    bitmapdata_cls = context->avm2->classes().bitmapdata;
                }

                // Create AVM2 object for this bitmap
                auto bitmap_obj = Avm2StageObject::for_display_object(context->gc(), 
                                                                     shared_from_this(), 
                                                                     bitmap_cls);

                // Call the constructor
                try {
                    bitmap_cls->call_constructor(bitmap_obj, {}, context);
                } catch (const std::exception& e) {
                    // Handle error in AVM2 construction
                    printf("Error running AVM2 construction for bitmap: %s\n", e.what());
                }

                avm2_object_ = bitmap_obj;

                // Create bitmap data object
                auto dummy_bitmap_data = BitmapData::dummy(context->gc());
                auto bitmap_data_obj = Avm2BitmapDataObject::from_bitmap_data_and_class(
                    context->gc(), dummy_bitmap_data, bitmapdata_cls);

                // Call the bitmap data constructor with dummy args
                try {
                    std::vector<std::shared_ptr<Value>> args = {
                        std::make_shared<Value>(1.0),  // width
                        std::make_shared<Value>(1.0)   // height
                    };
                    bitmapdata_cls->call_constructor(bitmap_data_obj, args, context);
                } catch (const std::exception& e) {
                    // Handle error in AVM2 construction for bitmap data
                    printf("Error running AVM2 construction for bitmap data: %s\n", e.what());
                }

                // Set the actual bitmap data
                set_bitmap_data(context, bitmap_data_obj->get_bitmap_data());
            }

            on_construction_complete(context);
        }
    }

    // Check if this object allows being used as a mask
    bool allow_as_mask() const override {
        return true; // Bitmaps can be used as masks
    }

    // Check if this object is empty
    bool is_empty() const override {
        return !bitmap_data_ || (width_ == 0 && height_ == 0);
    }

    // Cast to specific types
    std::shared_ptr<Bitmap> as_bitmap() override { 
        return std::static_pointer_cast<Bitmap>(shared_from_this()); 
    }
    
    bool is_bitmap() const override { return true; }
};

// Helper function to upgrade a weak reference
inline std::shared_ptr<Bitmap> BitmapWeak::upgrade() const {
    if (auto locked = bitmap_data_.lock()) {
        // In a real implementation, we would need to get the actual Bitmap object
        // from the BitmapData. This is a simplified version.
        return nullptr; // Placeholder
    }
    return nullptr;
}

} // namespace ruffle

#endif // BITMAP_DISPLAY_OBJECT_H