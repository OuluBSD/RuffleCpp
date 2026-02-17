// C++ translation of core/src/avm2/globals/flash/display/sprite.rs
//! `flash.display.Sprite` builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SPRITE_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SPRITE_H

#include <vector>
#include <optional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class Activation;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    template<typename GC>
    class ClassObject;
    template<typename GC>
    class Object;
    template<typename GC>
    class DisplayObject;
    template<typename GC>
    class MovieClip;
    template<typename GC>
    class UpdateContext;
    template<typename GC>
    class Mutation;

    namespace display_object {
        struct SoundTransform;
        template<typename GC>
        class TDisplayObject;
        template<typename GC>
        class TDisplayObjectContainer;
    }

    namespace gc_arena {
        template<typename T>
        class Gc;
        class Mutation;
    }
}
}

namespace swf {
struct Rectangle;
struct Twips;
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace display {
namespace sprite {

using gc_arena::Gc;
using gc_arena::Mutation;

/// A class instance allocator that allocates Sprite objects.
/// @param class_object The class object for Sprite
/// @param activation The current activation
/// @return New Sprite object (as DisplayObject)
template<typename GC>
Object<GC> sprite_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
);

/// Construct children of this Sprite
template<typename GC>
Value<GC> construct_children(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `dropTarget`'s getter
template<typename GC>
Value<GC> get_drop_target(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `graphics` getter
template<typename GC>
Value<GC> get_graphics(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `soundTransform`'s getter
template<typename GC>
Value<GC> get_sound_transform(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `soundTransform`'s setter
template<typename GC>
Value<GC> set_sound_transform(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `buttonMode`'s getter
template<typename GC>
Value<GC> get_button_mode(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `buttonMode`'s setter
template<typename GC>
Value<GC> set_button_mode(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Starts dragging this display object, making it follow the cursor.
template<typename GC>
Value<GC> start_drag(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Stops dragging the current display object
template<typename GC>
Value<GC> stop_drag(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `useHandCursor`'s getter
template<typename GC>
Value<GC> get_use_hand_cursor(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `useHandCursor`'s setter
template<typename GC>
Value<GC> set_use_hand_cursor(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `hitArea`'s getter
template<typename GC>
Value<GC> get_hit_area(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `hitArea`'s setter
template<typename GC>
Value<GC> set_hit_area(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

// Template method implementations

template<typename GC>
Object<GC> sprite_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
) {
    auto sprite_cls = activation->avm2().classes().sprite.inner_class_definition();

    auto class_def = std::optional(class_object.inner_class_definition());
    auto orig_class = class_object;
    
    while (class_def.has_value()) {
        if (class_def.value() == sprite_cls) {
            auto movie = activation->caller_movie_or_root();
            auto display_object = MovieClip<GC>::new_movie_clip(movie, activation->gc()).into();
            return initialize_for_allocator(activation->context, display_object, orig_class).into();
        }

        if (auto symbol_info = activation->context->library->avm2_class_registry()->class_symbol(class_def.value())) {
            auto [movie, symbol] = symbol_info.value();
            
            auto child = activation->context->library
                ->library_for_movie_mut(movie)
                ->instantiate_by_id(symbol, activation->context->gc_context);

            if (child.has_value()) {
                return initialize_for_allocator(activation->context, child.value(), orig_class).into();
            } else {
                // Return error 2136
                return Object<GC>::null(); // Placeholder
            }
        }
        
        auto super_class = class_def.value().super_class();
        class_def = super_class;
    }
    
    // Should not reach here - A Sprite subclass should have Sprite in superclass chain
    // unreachable!()
    return Object<GC>::null();
}

template<typename GC>
Value<GC> construct_children(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();
    auto this_display = this_obj->as_display_object().value();
    auto clip = this_display->as_movie_clip().value();

    // Construct children of this Sprite
    clip->set_constructing_frame(true);
    for (auto child : clip->iter_render_list()) {
        child->construct_frame(activation->context);
    }
    clip->set_constructing_frame(false);

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_drop_target(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto display = this_obj->as_display_object()) {
        if (auto mc = display->as_movie_clip()) {
            if (auto drop_target = mc->drop_target()) {
                return drop_target->object2_or_null();
            }
        }
    }

    return Value<GC>::null();
}

template<typename GC>
Value<GC> get_graphics(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto dobj = this_obj->as_display_object()) {
        // Lazily initialize the Graphics object in a hidden property.
        constexpr uint32_t GRAPHICS_SLOT = 0; // sprite_slots::_GRAPHICS placeholder
        
        auto graphics = dobj->get_slot(GRAPHICS_SLOT);
        if (graphics.is_undefined() || graphics.is_null()) {
            graphics = Value<GC>::from_object(StageObject<GC>::graphics(activation, dobj.value()));
            dobj->set_slot(GRAPHICS_SLOT, graphics, activation);
        }
        return graphics;
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_sound_transform(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto dobj = this_obj->as_display_object()) {
        auto dobj_st = dobj->base().sound_transform();
        return Value<GC>::from_object(dobj_st.into_avm2_object(activation).value());
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> set_sound_transform(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto dobj = this_obj->as_display_object()) {
        auto as3_st = args.get_object(activation, 0, "soundTransform").value();
        auto dobj_st = display_object::SoundTransform::from_avm2_object(as3_st);

        dobj->set_sound_transform(activation->context, dobj_st);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_button_mode(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        return Value<GC>::from_bool(mc->forced_button_mode());
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> set_button_mode(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        auto forced_button_mode = args.get_bool(0);
        mc->set_forced_button_mode(forced_button_mode);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> start_drag(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto display_object = this_obj->as_display_object()) {
        auto lock_center = args.get_bool(0);

        std::optional<swf::Rectangle> constraint;
        
        if (auto rectangle = args.try_get_object(1)) {
            constexpr uint32_t X_SLOT = 0;      // rectangle_slots::X placeholder
            constexpr uint32_t Y_SLOT = 1;      // rectangle_slots::Y placeholder
            constexpr uint32_t WIDTH_SLOT = 2;  // rectangle_slots::WIDTH placeholder
            constexpr uint32_t HEIGHT_SLOT = 3; // rectangle_slots::HEIGHT placeholder

            auto x = rectangle->get_slot(X_SLOT).coerce_to_number(activation).value();
            auto y = rectangle->get_slot(Y_SLOT).coerce_to_number(activation).value();
            auto width = rectangle->get_slot(WIDTH_SLOT).coerce_to_number(activation).value();
            auto height = rectangle->get_slot(HEIGHT_SLOT).coerce_to_number(activation).value();

            // Normalize the bounds.
            auto x_min = swf::Twips::from_pixels(x);
            auto y_min = swf::Twips::from_pixels(y);
            auto x_max = swf::Twips::from_pixels(x + width);
            auto y_max = swf::Twips::from_pixels(y + height);
            
            if (x_max.get() < x_min.get()) {
                std::swap(x_min, x_max);
            }
            if (y_max.get() < y_min.get()) {
                std::swap(y_min, y_max);
            }

            constraint = swf::Rectangle{x_min, y_min, x_max, y_max};
        } else {
            // No constraints.
            constraint = swf::Rectangle::default();
        }

        auto drag_object = player::DragObject<GC>{
            display_object.value(),
            *activation->context->mouse_position,
            lock_center,
            constraint.value_or(swf::Rectangle::default())
        };
        *activation->context->drag_object = std::optional(drag_object);
    }
    
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> stop_drag(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // It doesn't matter which clip we call this on; it simply stops any active drag.

    // We might not have had an opportunity to call update_drag
    // if AS did startDrag(mc); stopDrag(); in one go,
    // so let's do it here.
    player::Player<GC>::update_drag(activation->context);

    *activation->context->drag_object = std::nullopt;
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_use_hand_cursor(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        return Value<GC>::from_bool(mc->avm2_use_hand_cursor());
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> set_use_hand_cursor(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        mc->set_avm2_use_hand_cursor(args.get_bool(0));
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_hit_area(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        if (auto hit_area = mc->hit_area()) {
            return hit_area->object2_or_null();
        }
    }

    return Value<GC>::null();
}

template<typename GC>
Value<GC> set_hit_area(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        auto object = args.try_get_object(0)
            .and_then([](auto obj) { return obj->as_display_object(); });
        mc->set_hit_area(activation->gc(), object);
    }

    return Value<GC>::undefined();
}

} // namespace sprite
} // namespace display
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SPRITE_H
