/*
 * C++ header for display object functionality
 * This replaces the functionality of core/src/display_object.rs
 */

#ifndef DISPLAY_OBJECT_H
#define DISPLAY_OBJECT_H

#include "avm1.h"
#include "avm2.h"
#include "context.h"
#include "drawing.h"
#include "string.h"
#include "tag_utils.h"
#include "types.h"
#include "vminterface.h"
#include "transform.h"
#include "perspective_projection.h"
#include "pixel_bender.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <optional>
#include <variant>

namespace ruffle {

// Forward declarations
class DisplayObject;
class DisplayObjectContainer;
class InteractiveObject;
class MovieClip;
class EditText;
class Bitmap;
class Graphic;
class MorphShape;
class Stage;
class Avm1Button;
class Avm2Button;
class Text;
class Video;
class LoaderDisplay;

// Enum for different display object types
enum class DisplayObjectType {
    STAGE,
    MOVIE_CLIP,
    BUTTON,
    TEXT,
    EDIT_TEXT,
    BITMAP,
    GRAPHIC,
    MORPH_SHAPE,
    VIDEO,
    LOADER_DISPLAY
};

// Base class for all display objects
class DisplayObject {
protected:
    std::weak_ptr<DisplayObject> self_ref_;
    std::shared_ptr<DisplayObject> parent_;
    std::shared_ptr<DisplayObject> root_;
    std::string name_;
    int depth_;
    bool visible_;
    float x_, y_;
    float rotation_;
    float scale_x_, scale_y_;
    float width_, height_;
    float alpha_;
    std::shared_ptr<ColorTransform> color_transform_;
    std::shared_ptr<Transform> transform_;
    std::shared_ptr<PerspectiveProjection> perspective_projection_;
    std::shared_ptr<PixelBenderShaderHandle> pixel_bender_shader_;
    std::shared_ptr<Avm1Object> avm1_object_;
    std::shared_ptr<Avm2Object> avm2_object_;
    std::shared_ptr<Drawing> drawing_;
    bool is_on_stage_;
    bool is_removed_;
    std::shared_ptr<SwfMovie> movie_;
    uint16_t id_;
    DisplayObjectType type_;

public:
    DisplayObject(DisplayObjectType type, uint16_t id)
        : type_(type), id_(id), depth_(0), visible_(true), x_(0.0f), y_(0.0f),
          rotation_(0.0f), scale_x_(1.0f), scale_y_(1.0f), width_(0.0f), height_(0.0f),
          alpha_(1.0f), is_on_stage_(false), is_removed_(false) {}

    virtual ~DisplayObject() = default;

    // Getters
    std::shared_ptr<DisplayObject> parent() const { return parent_; }
    std::shared_ptr<DisplayObject> root() const { return root_; }
    const std::string& name() const { return name_; }
    int depth() const { return depth_; }
    bool visible() const { return visible_; }
    float x() const { return x_; }
    float y() const { return y_; }
    float rotation() const { return rotation_; }
    float scale_x() const { return scale_x_; }
    float scale_y() const { return scale_y_; }
    float width() const { return width_; }
    float height() const { return height_; }
    float alpha() const { return alpha_; }
    std::shared_ptr<ColorTransform> color_transform() const { return color_transform_; }
    std::shared_ptr<Transform> transform() const { return transform_; }
    std::shared_ptr<PerspectiveProjection> perspective_projection() const { return perspective_projection_; }
    std::shared_ptr<PixelBenderShaderHandle> pixel_bender_shader() const { return pixel_bender_shader_; }
    std::shared_ptr<Avm1Object> avm1_object() const { return avm1_object_; }
    std::shared_ptr<Avm2Object> avm2_object() const { return avm2_object_; }
    std::shared_ptr<Drawing> drawing() const { return drawing_; }
    bool on_stage() const { return is_on_stage_; }
    bool avm1_removed() const { return is_removed_; }
    std::shared_ptr<SwfMovie> movie() const { return movie_; }
    uint16_t id() const { return id_; }
    DisplayObjectType type() const { return type_; }

    // Setters
    void set_parent(std::shared_ptr<DisplayObject> parent) { parent_ = std::move(parent); }
    void set_name(const std::string& name) { name_ = name; }
    void set_depth(int depth) { depth_ = depth; }
    void set_visible(bool visible) { visible_ = visible; }
    void set_x(float x) { x_ = x; }
    void set_y(float y) { y_ = y; }
    void set_rotation(float rotation) { rotation_ = rotation; }
    void set_scale_x(float scale_x) { scale_x_ = scale_x; }
    void set_scale_y(float scale_y) { scale_y_ = scale_y; }
    void set_width(float width) { width_ = width; }
    void set_height(float height) { height_ = height; }
    void set_alpha(float alpha) { alpha_ = alpha; }
    void set_color_transform(std::shared_ptr<ColorTransform> ct) { color_transform_ = std::move(ct); }
    void set_transform(std::shared_ptr<Transform> t) { transform_ = std::move(t); }
    void set_perspective_projection(std::shared_ptr<PerspectiveProjection> pp) { perspective_projection_ = std::move(pp); }
    void set_pixel_bender_shader(std::shared_ptr<PixelBenderShaderHandle> shader) { pixel_bender_shader_ = std::move(shader); }
    void set_avm1_object(std::shared_ptr<Avm1Object> obj) { avm1_object_ = std::move(obj); }
    void set_avm2_object(std::shared_ptr<Avm2Object> obj) { avm2_object_ = std::move(obj); }
    void set_drawing(std::shared_ptr<Drawing> drawing) { drawing_ = std::move(drawing); }
    void set_on_stage(bool on_stage) { is_on_stage_ = on_stage; }
    void set_removed(bool removed) { is_removed_ = removed; }
    void set_movie(std::shared_ptr<SwfMovie> movie) { movie_ = std::move(movie); }

    // Virtual methods to be implemented by subclasses
    virtual void render(std::shared_ptr<RenderContext> context) = 0;
    virtual void update(std::shared_ptr<UpdateContext> context) = 0;
    virtual bool is_on_stage() const { return is_on_stage_; }
    virtual bool is_root() const { return parent_ == nullptr; }
    virtual bool is_container() const { return false; }
    virtual bool is_interactive() const { return false; }
    virtual std::shared_ptr<DisplayObjectContainer> as_container() { return nullptr; }
    virtual std::shared_ptr<InteractiveObject> as_interactive() { return nullptr; }
    virtual std::shared_ptr<MovieClip> as_movie_clip() { return nullptr; }
    virtual std::shared_ptr<EditText> as_edit_text() { return nullptr; }
    virtual std::shared_ptr<Bitmap> as_bitmap() { return nullptr; }
    virtual std::shared_ptr<Graphic> as_graphic() { return nullptr; }
    virtual std::shared_ptr<MorphShape> as_morph_shape() { return nullptr; }
    virtual std::shared_ptr<Stage> as_stage() { return nullptr; }
    virtual std::shared_ptr<Avm1Button> as_avm1_button() { return nullptr; }
    virtual std::shared_ptr<Avm2Button> as_avm2_button() { return nullptr; }
    virtual std::shared_ptr<Text> as_text() { return nullptr; }
    virtual std::shared_ptr<Video> as_video() { return nullptr; }
    virtual std::shared_ptr<LoaderDisplay> as_loader_display() { return nullptr; }

    // Positioning methods
    void set_position(float x, float y) {
        set_x(x);
        set_y(y);
    }

    void set_scale(float scale_x, float scale_y) {
        set_scale_x(scale_x);
        set_scale_y(scale_y);
    }

    // Get the full transformation matrix including parent transformations
    Matrix get_full_transform_matrix() const {
        Matrix matrix = get_local_transform_matrix();
        if (parent_) {
            matrix = parent_->get_full_transform_matrix() * matrix;
        }
        return matrix;
    }

    // Get the local transformation matrix
    virtual Matrix get_local_transform_matrix() const {
        Matrix m;
        m.translate(x_, y_);
        m.rotate(rotation_);
        m.scale(scale_x_, scale_y_);
        return m;
    }

    // Hit testing
    virtual bool hit_test_point(float x, float y, bool shape_flag = false) const {
        // Default implementation - check if point is within bounds
        float local_x = x - this->x();
        float local_y = y - this->y();
        return (local_x >= 0 && local_x <= width_ && local_y >= 0 && local_y <= height_);
    }

    // Event handling
    virtual void handle_event(std::shared_ptr<Event> event) {
        // Default implementation does nothing
    }

    // Get the display object as a value for AVM
    virtual std::shared_ptr<Value> as_avm_value() const {
        if (avm1_object_) {
            return std::make_shared<Value>(avm1_object_);
        } else if (avm2_object_) {
            return std::make_shared<Value>(avm2_object_);
        }
        return std::make_shared<Value>(Value::UNDEFINED);
    }

    // Set the self reference (used for weak pointers)
    void set_self_reference(std::shared_ptr<DisplayObject> self) {
        self_ref_ = self;
    }

    // Get a weak reference to self
    std::weak_ptr<DisplayObject> weak_self() const {
        return self_ref_;
    }

    // Get the object as a display object (for trait implementations)
    std::shared_ptr<DisplayObject> as_display_object() const {
        return std::const_pointer_cast<DisplayObject>(shared_from_this());
    }

    // Check if this object is equal to another
    static bool equals(std::shared_ptr<DisplayObject> a, std::shared_ptr<DisplayObject> b) {
        if (!a && !b) return true;
        if (!a || !b) return false;
        return a.get() == b.get();  // Compare addresses
    }
};

// Container interface for display objects that can have children
class DisplayObjectContainer : public DisplayObject {
protected:
    std::vector<std::shared_ptr<DisplayObject>> children_;
    std::unordered_map<std::string, std::shared_ptr<DisplayObject>> child_names_;

public:
    DisplayObjectContainer() : DisplayObject(DisplayObjectType::MOVIE_CLIP, 0) {}

    bool is_container() const override { return true; }
    std::shared_ptr<DisplayObjectContainer> as_container() override { return std::static_pointer_cast<DisplayObjectContainer>(shared_from_this()); }

    // Add a child to this container
    void add_child(std::shared_ptr<DisplayObject> child, int depth) {
        if (!child) return;

        // Remove from current parent if it has one
        if (child->parent()) {
            child->parent()->remove_child(child);
        }

        // Set parent relationship
        child->set_parent(shared_from_this());
        child->set_depth(depth);

        // Add to children vector maintaining depth order
        auto insert_pos = std::upper_bound(children_.begin(), children_.end(), depth,
            [](int depth, const std::shared_ptr<DisplayObject>& obj) {
                return depth < obj->depth();
            });
        children_.insert(insert_pos, child);

        // Add to name map if named
        if (!child->name().empty()) {
            child_names_[child->name()] = child;
        }

        // Dispatch events
        dispatch_added_event(child);
    }

    // Remove a child from this container
    void remove_child(std::shared_ptr<DisplayObject> child) {
        if (!child || child->parent() != shared_from_this()) return;

        // Remove from children vector
        auto it = std::find(children_.begin(), children_.end(), child);
        if (it != children_.end()) {
            children_.erase(it);
        }

        // Remove from name map
        if (!child->name().empty()) {
            child_names_.erase(child->name());
        }

        // Clear parent relationship
        child->set_parent(nullptr);
        child->set_depth(0);

        // Dispatch events
        dispatch_removed_event(child);
    }

    // Get child by name
    std::shared_ptr<DisplayObject> get_child_by_name(const std::string& name, bool case_sensitive = true) const {
        if (case_sensitive) {
            auto it = child_names_.find(name);
            if (it != child_names_.end()) {
                return it->second;
            }
        } else {
            // Case-insensitive search
            for (const auto& [child_name, child] : child_names_) {
                if (std::equal(child_name.begin(), child_name.end(), name.begin(), name.end(),
                              [](char a, char b) { return std::tolower(a) == std::tolower(b); })) {
                    return child;
                }
            }
        }
        return nullptr;
    }

    // Get child by depth
    std::shared_ptr<DisplayObject> get_child_by_depth(int depth) const {
        for (const auto& child : children_) {
            if (child->depth() == depth) {
                return child;
            }
        }
        return nullptr;
    }

    // Get child by index
    std::shared_ptr<DisplayObject> get_child_at_index(size_t index) const {
        if (index < children_.size()) {
            return children_[index];
        }
        return nullptr;
    }

    // Get number of children
    size_t num_children() const { return children_.size(); }

    // Get all children
    const std::vector<std::shared_ptr<DisplayObject>>& children() const { return children_; }

    // Swap child depths
    void swap_child_depths(int depth1, int depth2) {
        std::shared_ptr<DisplayObject> child1 = get_child_by_depth(depth1);
        std::shared_ptr<DisplayObject> child2 = get_child_by_depth(depth2);

        if (child1 && child2) {
            child1->set_depth(depth2);
            child2->set_depth(depth1);

            // Re-sort children vector
            std::sort(children_.begin(), children_.end(),
                [](const std::shared_ptr<DisplayObject>& a, const std::shared_ptr<DisplayObject>& b) {
                    return a->depth() < b->depth();
                });
        }
    }

    // Dispatch added event
    void dispatch_added_event(std::shared_ptr<DisplayObject> child) {
        // In a real implementation, this would dispatch an event to the child
        // For now, we'll just update the on-stage status recursively
        update_on_stage_status(child, is_on_stage());
    }

    // Dispatch removed event
    void dispatch_removed_event(std::shared_ptr<DisplayObject> child) {
        // In a real implementation, this would dispatch an event to the child
        update_on_stage_status(child, false);
    }

    // Update on-stage status recursively
    void update_on_stage_status(std::shared_ptr<DisplayObject> obj, bool on_stage) {
        obj->set_on_stage(on_stage);
        if (auto container = obj->as_container()) {
            for (const auto& child : container->children()) {
                update_on_stage_status(child, on_stage);
            }
        }
    }
};

// Interactive object interface
class InteractiveObject : public DisplayObject {
protected:
    bool mouse_enabled_;
    bool tab_enabled_;
    int tab_index_;

public:
    InteractiveObject(DisplayObjectType type, uint16_t id)
        : DisplayObject(type, id), mouse_enabled_(true), tab_enabled_(false), tab_index_(-1) {}

    bool is_interactive() const override { return true; }
    std::shared_ptr<InteractiveObject> as_interactive() override { return std::static_pointer_cast<InteractiveObject>(shared_from_this()); }

    // Getters
    bool mouse_enabled() const { return mouse_enabled_; }
    bool tab_enabled() const { return tab_enabled_; }
    int tab_index() const { return tab_index_; }

    // Setters
    void set_mouse_enabled(bool enabled) { mouse_enabled_ = enabled; }
    void set_tab_enabled(bool enabled) { tab_enabled_ = enabled; }
    void set_tab_index(int index) { tab_index_ = index; }
};

// Helper functions for comparing interactive objects
static bool interactive_object_ptr_eq(std::shared_ptr<InteractiveObject> a, 
                                     std::shared_ptr<InteractiveObject> b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
    return a.get() == b.get();
}

// Helper functions for comparing display objects
static bool display_object_ptr_eq(std::shared_ptr<DisplayObject> a, 
                                 std::shared_ptr<DisplayObject> b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
    return a.get() == b.get();
}

} // namespace ruffle

// Include specific display object types
#include "display_object/stage.h"
#include "display_object/movie_clip.h"
#include "display_object/button.h"
#include "display_object/text.h"
#include "display_object/edit_text.h"
#include "display_object/bitmap.h"
#include "display_object/graphic.h"
#include "display_object/morph_shape.h"
#include "display_object/video.h"
#include "display_object/loader_display.h"

#endif // DISPLAY_OBJECT_H