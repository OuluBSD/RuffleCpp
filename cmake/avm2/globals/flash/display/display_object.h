/*
 * C++ header for AVM2 flash.display.DisplayObject functionality
 * This replaces the functionality of core/src/avm2/globals/flash/display/display_object.rs
 */

#ifndef AVM2_FLASH_DISPLAY_DISPLAY_OBJECT_H
#define AVM2_FLASH_DISPLAY_DISPLAY_OBJECT_H

#include "avm2/object.h"
#include "avm2/class.h"
#include "avm2/activation.h"
#include "display_object.h"
#include "matrix.h"
#include "color_transform.h"
#include "rectangle.h"
#include "point.h"
#include <memory>
#include <string>
#include <vector>

namespace ruffle::avm2::globals::flash::display {

// Forward declarations
class DisplayObject;
class Avm2Activation;
class Avm2Object;
class Avm2ClassObject;

// DisplayObject class representing the base class for all display objects in AVM2
class DisplayObjectAvm2 : public ruffle::Object {
private:
    std::shared_ptr<ruffle::DisplayObject> display_object_;

public:
    explicit DisplayObjectAvm2(std::shared_ptr<ruffle::DisplayObject> display_obj)
        : display_object_(std::move(display_obj)) {}

    // Static factory method
    static std::shared_ptr<DisplayObjectAvm2> create(std::shared_ptr<ruffle::DisplayObject> display_obj) {
        return std::make_shared<DisplayObjectAvm2>(std::move(display_obj));
    }

    // Get the underlying display object
    std::shared_ptr<ruffle::DisplayObject> display_object() const { return display_object_; }

    // Get the x position
    double get_x(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->x().to_pixels();
    }

    // Set the x position
    void set_x(std::shared_ptr<Avm2Activation> activation, double value) {
        display_object_->set_x(activation->context(), Twips::from_pixels(value));
    }

    // Get the y position
    double get_y(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->y().to_pixels();
    }

    // Set the y position
    void set_y(std::shared_ptr<Avm2Activation> activation, double value) {
        display_object_->set_y(activation->context(), Twips::from_pixels(value));
    }

    // Get the width
    double get_width(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->width().to_pixels();
    }

    // Set the width
    void set_width(std::shared_ptr<Avm2Activation> activation, double value) {
        display_object_->set_width(activation->context(), Twips::from_pixels(value));
    }

    // Get the height
    double get_height(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->height().to_pixels();
    }

    // Set the height
    void set_height(std::shared_ptr<Avm2Activation> activation, double value) {
        display_object_->set_height(activation->context(), Twips::from_pixels(value));
    }

    // Get the rotation
    double get_rotation(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->rotation().to_degrees();
    }

    // Set the rotation
    void set_rotation(std::shared_ptr<Avm2Activation> activation, double value) {
        display_object_->set_rotation(activation->context(), Degrees::from_radians(value * M_PI / 180.0));
    }

    // Get the scale X
    double get_scale_x(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->scale_x();
    }

    // Set the scale X
    void set_scale_x(std::shared_ptr<Avm2Activation> activation, double value) {
        display_object_->set_scale_x(activation->context(), value);
    }

    // Get the scale Y
    double get_scale_y(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->scale_y();
    }

    // Set the scale Y
    void set_scale_y(std::shared_ptr<Avm2Activation> activation, double value) {
        display_object_->set_scale_y(activation->context(), value);
    }

    // Get the alpha
    double get_alpha(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->alpha();
    }

    // Set the alpha
    void set_alpha(std::shared_ptr<Avm2Activation> activation, double value) {
        display_object_->set_alpha(activation->context(), std::clamp(value, 0.0, 1.0));
    }

    // Get the visible property
    bool get_visible(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->visible();
    }

    // Set the visible property
    void set_visible(std::shared_ptr<Avm2Activation> activation, bool value) {
        display_object_->set_visible(activation->context(), value);
    }

    // Get the name property
    std::string get_name(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->name();
    }

    // Set the name property
    void set_name(std::shared_ptr<Avm2Activation> activation, const std::string& value) {
        display_object_->set_name(activation->context(), value);
    }

    // Get the parent
    std::shared_ptr<DisplayObjectAvm2> get_parent(std::shared_ptr<Avm2Activation> activation) const {
        if (auto parent = display_object_->parent()) {
            return std::make_shared<DisplayObjectAvm2>(parent);
        }
        return nullptr;
    }

    // Get the root
    std::shared_ptr<DisplayObjectAvm2> get_root(std::shared_ptr<Avm2Activation> activation) const {
        if (auto root = display_object_->root()) {
            return std::make_shared<DisplayObjectAvm2>(root);
        }
        return nullptr;
    }

    // Get the stage
    std::shared_ptr<DisplayObjectAvm2> get_stage(std::shared_ptr<Avm2Activation> activation) const {
        if (auto stage = display_object_->stage()) {
            return std::make_shared<DisplayObjectAvm2>(stage);
        }
        return nullptr;
    }

    // Get the mouseX
    double get_mouse_x(std::shared_ptr<Avm2Activation> activation) const {
        auto mouse_pos = activation->context()->mouse_pos();
        if (auto local_matrix = display_object_->global_to_local_matrix()) {
            auto local_pos = local_matrix->transform_point(mouse_pos);
            return local_pos.x.to_pixels();
        }
        return 0.0;
    }

    // Get the mouseY
    double get_mouse_y(std::shared_ptr<Avm2Activation> activation) const {
        auto mouse_pos = activation->context()->mouse_pos();
        if (auto local_matrix = display_object_->global_to_local_matrix()) {
            auto local_pos = local_matrix->transform_point(mouse_pos);
            return local_pos.y.to_pixels();
        }
        return 0.0;
    }

    // Get the width and height as a rectangle
    Rectangle<Twips> get_bounds(std::shared_ptr<Avm2Activation> activation,
                               std::shared_ptr<DisplayObjectAvm2> target_coordinate_space) const {
        auto target_obj = target_coordinate_space ? 
                         target_coordinate_space->display_object() : 
                         display_object_;
        return display_object_->bounds(target_obj);
    }

    // Get the local bounds
    Rectangle<Twips> get_local_bounds(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->local_bounds();
    }

    // Get the transform
    std::shared_ptr<Transform> get_transform(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->transform();
    }

    // Set the transform
    void set_transform(std::shared_ptr<Avm2Activation> activation, 
                      std::shared_ptr<Transform> value) {
        display_object_->set_transform(activation->context(), value);
    }

    // Get the blend mode
    std::string get_blend_mode(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->blend_mode();
    }

    // Set the blend mode
    void set_blend_mode(std::shared_ptr<Avm2Activation> activation, 
                       const std::string& value) {
        display_object_->set_blend_mode(activation->context(), value);
    }

    // Get the cache as bitmap
    bool get_cache_as_bitmap(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->cache_as_bitmap();
    }

    // Set the cache as bitmap
    void set_cache_as_bitmap(std::shared_ptr<Avm2Activation> activation, bool value) {
        display_object_->set_cache_as_bitmap(activation->context(), value);
    }

    // Get the opaque background
    std::optional<Color> get_opaque_background(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->opaque_background();
    }

    // Set the opaque background
    void set_opaque_background(std::shared_ptr<Avm2Activation> activation, 
                              std::optional<Color> value) {
        display_object_->set_opaque_background(activation->context(), value);
    }

    // Get the scroll rect
    std::optional<Rectangle<Twips>> get_scroll_rect(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->scroll_rect();
    }

    // Set the scroll rect
    void set_scroll_rect(std::shared_ptr<Avm2Activation> activation,
                        std::optional<Rectangle<Twips>> value) {
        display_object_->set_scroll_rect(activation->context(), value);
    }

    // Get the filters
    std::vector<std::shared_ptr<Filter>> get_filters(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->filters();
    }

    // Set the filters
    void set_filters(std::shared_ptr<Avm2Activation> activation,
                    const std::vector<std::shared_ptr<Filter>>& value) {
        display_object_->set_filters(activation->context(), value);
    }

    // Get the mask
    std::shared_ptr<DisplayObjectAvm2> get_mask(std::shared_ptr<Avm2Activation> activation) const {
        if (auto mask = display_object_->mask()) {
            return std::make_shared<DisplayObjectAvm2>(mask);
        }
        return nullptr;
    }

    // Set the mask
    void set_mask(std::shared_ptr<Avm2Activation> activation,
                 std::shared_ptr<DisplayObjectAvm2> value) {
        auto mask_obj = value ? value->display_object() : nullptr;
        display_object_->set_mask(activation->context(), mask_obj);
    }

    // Get the z property
    double get_z(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->z();
    }

    // Set the z property
    void set_z(std::shared_ptr<Avm2Activation> activation, double value) {
        display_object_->set_z(activation->context(), value);
    }

    // Get the scale9Grid
    std::optional<Rectangle<Twips>> get_scale9_grid(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->scale9_grid();
    }

    // Set the scale9Grid
    void set_scale9_grid(std::shared_ptr<Avm2Activation> activation,
                        std::optional<Rectangle<Twips>> value) {
        display_object_->set_scale9_grid(activation->context(), value);
    }

    // Get the loaderInfo
    std::shared_ptr<LoaderInfoObject> get_loader_info(std::shared_ptr<Avm2Activation> activation) const {
        return display_object_->loader_info();
    }

    // Hit test point
    bool hit_test_point(std::shared_ptr<Avm2Activation> activation,
                       double x, double y, bool shape_flag = false) const {
        Point<Twips> point = Point<Twips>(Twips::from_pixels(x), Twips::from_pixels(y));
        return display_object_->hit_test_point(activation->context(), point, shape_flag);
    }

    // Hit test object
    bool hit_test_object(std::shared_ptr<Avm2Activation> activation,
                        std::shared_ptr<DisplayObjectAvm2> other) const {
        if (!other) return false;
        return display_object_->hit_test_object(activation->context(), other->display_object());
    }

    // Local to global
    Point<double> local_to_global(std::shared_ptr<Avm2Activation> activation,
                                 Point<double> point) const {
        auto twips_point = Point<Twips>(Twips::from_pixels(point.x), Twips::from_pixels(point.y));
        auto global_twips = display_object_->local_to_global(activation->context(), twips_point);
        return Point<double>(global_twips.x.to_pixels(), global_twips.y.to_pixels());
    }

    // Global to local
    Point<double> global_to_local(std::shared_ptr<Avm2Activation> activation,
                                 Point<double> point) const {
        auto twips_point = Point<Twips>(Twips::from_pixels(point.x), Twips::from_pixels(point.y));
        auto local_twips = display_object_->global_to_local(activation->context(), twips_point);
        return Point<double>(local_twips.x.to_pixels(), local_twips.y.to_pixels());
    }

    // Get the AVM2 class for DisplayObject
    static std::shared_ptr<Avm2ClassObject> create_class(std::shared_ptr<Avm2Activation> activation) {
        // In a real implementation, this would create the AVM2 DisplayObject class
        // with all its properties and methods
        return nullptr; // Placeholder
    }
};

} // namespace ruffle::avm2::globals::flash::display

#endif // AVM2_FLASH_DISPLAY_DISPLAY_OBJECT_H