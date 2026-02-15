/*
 * C++ header for graphic display object functionality
 * This replaces the functionality of core/src/display_object/graphic.rs
 */

#ifndef GRAPHIC_DISPLAY_OBJECT_H
#define GRAPHIC_DISPLAY_OBJECT_H

#include "display_object.h"
#include "display_object/interactive.h"
#include "avm1.h"
#include "avm2.h"
#include "context.h"
#include "drawing.h"
#include "render_handle.h"
#include "swf_types.h"
#include <memory>
#include <optional>
#include <mutex>

namespace ruffle {

// Forward declarations
class UpdateContext;
class RenderContext;
class Drawing;
class ShapeHandle;

// Data shared between all instances of a Graphic
struct GraphicShared {
    uint16_t id;
    swf::Shape shape;
    std::optional<ShapeHandle> render_handle;
    Rectangle<Twips> bounds;
    std::shared_ptr<SwfMovie> movie;

    GraphicShared(uint16_t id, 
                  swf::Shape shape_data,
                  std::optional<ShapeHandle> handle,
                  Rectangle<Twips> bounds,
                  std::shared_ptr<SwfMovie> movie_data)
        : id(id), shape(std::move(shape_data)), render_handle(std::move(handle)),
          bounds(bounds), movie(std::move(movie_data)) {}
};

// Graphic display object class
class Graphic : public DisplayObject {
private:
    std::shared_ptr<GraphicShared> shared_data_;
    std::shared_ptr<Avm2ClassObject> avm2_class_;
    std::shared_ptr<Avm2StageObject> avm2_object_;
    std::shared_ptr<Drawing> drawing_;

public:
    Graphic(std::shared_ptr<GraphicShared> shared_data)
        : DisplayObject(DisplayObjectType::GRAPHIC, shared_data->id),
          shared_data_(std::move(shared_data)) {}

    // Create a Graphic from an SWF shape tag
    static std::shared_ptr<Graphic> from_swf_tag(
        std::shared_ptr<UpdateContext> context,
        swf::Shape swf_shape,
        std::shared_ptr<SwfMovie> movie) {
        
        auto library = context->library->library_for_movie(movie);
        
        auto render_handle = context->renderer->register_shape(swf_shape, library);
        
        auto shared_data = std::make_shared<GraphicShared>(
            swf_shape.id,
            std::move(swf_shape),
            std::make_optional(std::move(render_handle)),
            swf_shape.shape_bounds,
            std::move(movie)
        );

        auto graphic = std::make_shared<Graphic>(std::move(shared_data));
        return graphic;
    }

    // Create an empty Graphic
    static std::shared_ptr<Graphic> empty(std::shared_ptr<UpdateContext> context) {
        swf::Shape empty_shape;
        empty_shape.version = 32;
        empty_shape.id = 0;
        empty_shape.shape_bounds = Rectangle<Twips>();
        empty_shape.edge_bounds = Rectangle<Twips>();
        empty_shape.flags = 0; // Assuming a simple flag representation
        empty_shape.styles.fill_styles = std::vector<swf::FillStyle>();
        empty_shape.styles.line_styles = std::vector<swf::LineStyle>();
        empty_shape.shape = std::vector<swf::ShapeRecord>();

        auto shared_data = std::make_shared<GraphicShared>(
            0,
            std::move(empty_shape),
            std::nullopt,
            Rectangle<Twips>(),
            context->root_swf
        );

        return std::make_shared<Graphic>(std::move(shared_data));
    }

    // Get mutable reference to drawing (for dynamic graphics)
    std::shared_ptr<Drawing> drawing_mut() {
        if (!drawing_) {
            drawing_ = std::make_shared<Drawing>();
        }
        return drawing_;
    }

    // Set the AVM2 class for this graphic
    void set_avm2_class(std::shared_ptr<Avm2ClassObject> cls) {
        avm2_class_ = std::move(cls);
    }

    // Getters
    const std::shared_ptr<GraphicShared>& shared_data() const { return shared_data_; }
    std::shared_ptr<Avm2ClassObject> avm2_class() const { return avm2_class_; }
    std::shared_ptr<Avm2StageObject> avm2_object() const { return avm2_object_; }
    std::shared_ptr<Drawing> drawing() const { return drawing_; }
    std::shared_ptr<SwfMovie> movie() const override { return shared_data_->movie; }

    // Display object interface implementation
    Rectangle<Twips> self_bounds(BoundsMode mode) const override {
        if (drawing_) {
            return drawing_->self_bounds();
        } else {
            return shared_data_->bounds;
        }
    }

    void render_self(std::shared_ptr<RenderContext> context) override {
        // Check if object is off-screen and can be culled
        if (!context->is_offscreen && 
            !world_bounds(BoundsMode::ENGINE).intersects(context->stage->view_bounds())) {
            // Off-screen; culled
            return;
        }

        if (drawing_) {
            drawing_->render(context);
        } else if (shared_data_->render_handle.has_value()) {
            context->commands->render_shape(
                shared_data_->render_handle.value(),
                context->transform_stack->transform()
            );
        }
    }

    bool hit_test_shape(std::shared_ptr<UpdateContext> context,
                       Point<Twips> point,
                       HitTestOptions options) const override {
        // Transform point to local coordinates and test
        if ((!options.contains(HitTestOptions::SKIP_INVISIBLE) || visible()) &&
            world_bounds(BoundsMode::ENGINE).contains(point)) {
            
            auto local_matrix = global_to_local_matrix();
            if (local_matrix.has_value()) {
                auto local_point = local_matrix.value() * point;
                
                if (drawing_) {
                    return drawing_->hit_test(local_point, &local_matrix.value());
                } else {
                    // Use shape hit test utility
                    return shape_hit_test(&shared_data_->shape, local_point, &local_matrix.value());
                }
            }
        }
        
        return false;
    }

    void post_instantiation(std::shared_ptr<UpdateContext> context,
                           std::shared_ptr<Object> init_object,
                           Instantiator instantiated_by,
                           bool run_frame) override {
        DisplayObject::post_instantiation(context, init_object, instantiated_by, run_frame);
        
        if (shared_data_->movie->is_action_script_3()) {
            set_default_instance_name(context);
        }
    }

    void construct_frame(std::shared_ptr<UpdateContext> context) override {
        if (shared_data_->movie->is_action_script_3() && !object2()) {
            auto class_object = avm2_class_ ? avm2_class_ : context->avm2->classes().shape;

            try {
                auto object = Avm2StageObject::for_display_object_childless(
                    context, shared_from_this(), class_object);

                set_object2(context, object);
                on_construction_complete(context);
            } catch (const std::exception& e) {
                // Handle AVM2 construction error
                printf("Error running AVM2 construction for graphic: %s\n", e.what());
            }
        }
    }

    void replace_with(std::shared_ptr<UpdateContext> context, uint16_t id) override {
        // Static assets like Graphics can replace themselves via a PlaceObject tag with PlaceObjectAction::Replace.
        // This does not create a new instance, but instead swaps out the underlying static data to point to the new art.
        if (auto new_graphic = context->library->library_for_movie_mut(shared_data_->movie)->get_graphic(id)) {
            shared_data_ = new_graphic->shared_data();
        } else {
            printf("PlaceObject: expected Graphic at character ID %d\n", id);
        }
        invalidate_cached_bitmap();
    }

    std::shared_ptr<Object> object1() const override {
        // Graphics don't have AVM1 objects
        return nullptr;
    }

    std::shared_ptr<Avm2StageObject> object2() const override {
        return avm2_object_;
    }

    void set_object2(std::shared_ptr<UpdateContext> context, 
                    std::shared_ptr<Avm2StageObject> obj) override {
        avm2_object_ = std::move(obj);
    }

    // Cast to specific types
    std::shared_ptr<Graphic> as_graphic() override { 
        return std::static_pointer_cast<Graphic>(shared_from_this()); 
    }
    
    bool is_graphic() const override { return true; }

    // Check if this object allows being used as a mask
    bool allow_as_mask() const override {
        return true; // Graphics can be used as masks
    }

    // Check if this object is empty
    bool is_empty() const override {
        if (drawing_) {
            return drawing_->is_empty();
        }
        // For static graphics, check if the shape has any content
        return shared_data_->shape.shape.empty();
    }

    // Get drawing interface (for dynamic graphics)
    std::shared_ptr<Drawing> as_drawing() const override {
        return drawing_;
    }
};

} // namespace ruffle

#endif // GRAPHIC_DISPLAY_OBJECT_H