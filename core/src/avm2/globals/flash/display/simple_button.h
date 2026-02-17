// C++ translation of core/src/avm2/globals/flash/display/simple_button.rs
//! `flash.display.SimpleButton` builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SIMPLE_BUTTON_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SIMPLE_BUTTON_H

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
    class Avm2Button;
    template<typename GC>
    class StageObject;
    template<typename GC>
    class UpdateContext;
    template<typename GC>
    class Mutation;

    namespace vminterface {
        enum class Instantiator;
    }

    namespace gc_arena {
        template<typename T>
        class Gc;
        class Mutation;
    }
}
}

namespace swf {
enum class ButtonState;
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace display {
namespace simple_button {

using gc_arena::Gc;
using gc_arena::Mutation;

// Re-export from sound_mixer (as mentioned in the Rust file)
using ruffle::core::avm2::globals::flash::media::sound_mixer::get_sound_transform;
using ruffle::core::avm2::globals::flash::media::sound_mixer::set_sound_transform;

/// A class instance allocator that allocates SimpleButton objects.
/// @param class_object The class object for SimpleButton
/// @param activation The current activation
/// @return New SimpleButton object (as DisplayObject)
template<typename GC>
Object<GC> simple_button_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
);

/// Implements `flash.display.SimpleButton`'s 'init' method, which is called from the constructor
template<typename GC>
Value<GC> init(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `downState`'s getter
template<typename GC>
Value<GC> get_down_state(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `downState`'s setter
template<typename GC>
Value<GC> set_down_state(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `overState`'s getter
template<typename GC>
Value<GC> get_over_state(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `overState`'s setter
template<typename GC>
Value<GC> set_over_state(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `hitTestState`'s getter
template<typename GC>
Value<GC> get_hit_test_state(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `hitTestState`'s setter
template<typename GC>
Value<GC> set_hit_test_state(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `upState`'s getter
template<typename GC>
Value<GC> get_up_state(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `upState`'s setter
template<typename GC>
Value<GC> set_up_state(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `trackAsMenu`'s getter
template<typename GC>
Value<GC> get_track_as_menu(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `trackAsMenu`'s setter
template<typename GC>
Value<GC> set_track_as_menu(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `enabled`'s getter
template<typename GC>
Value<GC> get_enabled(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `enabled`'s setter
template<typename GC>
Value<GC> set_enabled(
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

// Template method implementations

template<typename GC>
Object<GC> simple_button_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
) {
    auto simplebutton_cls = activation->avm2()
        .classes()
        .simplebutton
        .inner_class_definition();

    auto class_def = std::optional(class_object.inner_class_definition());
    auto orig_class = class_object;
    
    while (class_def.has_value()) {
        if (class_def.value() == simplebutton_cls) {
            auto button = Avm2Button<GC>::empty_button(activation->context);
            // Buttons specifically need to be post-instantiated
            button->post_instantiation(
                activation->context, 
                std::nullopt, 
                vminterface::Instantiator::Avm2, 
                false
            );
            auto display_object = button.into();
            auto obj = StageObject<GC>::for_display_object(
                activation->gc(), 
                display_object, 
                orig_class
            );
            display_object->set_object2(activation->context, obj);
            return obj.into();
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
    
    // Should not reach here - A SimpleButton subclass should have SimpleButton in superclass chain
    // unreachable!()
    return Object<GC>::null();
}

template<typename GC>
Value<GC> init(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto new_do = this_obj->as_display_object()->as_avm2_button()) {
        auto up_state = args.try_get_object(0)
            .and_then([](auto obj) { return obj->as_display_object(); });
        if (up_state.has_value()) {
            new_do->set_state_child(activation->context, swf::ButtonState::UP, up_state);
        }

        auto over_state = args.try_get_object(1)
            .and_then([](auto obj) { return obj->as_display_object(); });
        if (over_state.has_value()) {
            new_do->set_state_child(activation->context, swf::ButtonState::OVER, over_state);
        }

        auto down_state = args.try_get_object(2)
            .and_then([](auto obj) { return obj->as_display_object(); });
        if (down_state.has_value()) {
            new_do->set_state_child(activation->context, swf::ButtonState::DOWN, down_state);
        }

        auto hit_state = args.try_get_object(3)
            .and_then([](auto obj) { return obj->as_display_object(); });
        if (hit_state.has_value()) {
            new_do->set_state_child(activation->context, swf::ButtonState::HIT_TEST, hit_state);
        }

        // This performs the child state construction.
        new_do->construct_frame(activation->context);
    } else {
        // Should not happen
        // unreachable!()
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_down_state(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto btn = this_obj->as_display_object()->as_avm2_button()) {
        if (auto state = btn->get_state_child(swf::ButtonState::DOWN)) {
            return state->object2_or_null();
        }
        return Value<GC>::null();
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> set_down_state(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto btn = this_obj->as_display_object()->as_avm2_button()) {
        auto new_state = args.try_get_object(0)
            .and_then([](auto obj) { return obj->as_display_object(); });
        btn->set_state_child(activation->context, swf::ButtonState::DOWN, new_state);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_over_state(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto btn = this_obj->as_display_object()->as_avm2_button()) {
        if (auto state = btn->get_state_child(swf::ButtonState::OVER)) {
            return state->object2_or_null();
        }
        return Value<GC>::null();
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> set_over_state(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto btn = this_obj->as_display_object()->as_avm2_button()) {
        auto new_state = args.try_get_object(0)
            .and_then([](auto obj) { return obj->as_display_object(); });
        btn->set_state_child(activation->context, swf::ButtonState::OVER, new_state);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_hit_test_state(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto btn = this_obj->as_display_object()->as_avm2_button()) {
        if (auto state = btn->get_state_child(swf::ButtonState::HIT_TEST)) {
            return state->object2_or_null();
        }
        return Value<GC>::null();
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> set_hit_test_state(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto btn = this_obj->as_display_object()->as_avm2_button()) {
        auto new_state = args.try_get_object(0)
            .and_then([](auto obj) { return obj->as_display_object(); });
        btn->set_state_child(activation->context, swf::ButtonState::HIT_TEST, new_state);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_up_state(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto btn = this_obj->as_display_object()->as_avm2_button()) {
        if (auto state = btn->get_state_child(swf::ButtonState::UP)) {
            return state->object2_or_null();
        }
        return Value<GC>::null();
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> set_up_state(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto btn = this_obj->as_display_object()->as_avm2_button()) {
        auto new_state = args.try_get_object(0)
            .and_then([](auto obj) { return obj->as_display_object(); });
        btn->set_state_child(activation->context, swf::ButtonState::UP, new_state);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_track_as_menu(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto btn = this_obj->as_display_object()->as_avm2_button()) {
        return Value<GC>::from_bool(btn->button_tracking() == ButtonTracking::Menu);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> set_track_as_menu(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto btn = this_obj->as_display_object()->as_avm2_button()) {
        if (args.get_bool(0)) {
            btn->set_button_tracking(ButtonTracking::Menu);
        } else {
            btn->set_button_tracking(ButtonTracking::Push);
        }
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_enabled(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto btn = this_obj->as_display_object()->as_avm2_button()) {
        return Value<GC>::from_bool(btn->enabled());
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> set_enabled(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto btn = this_obj->as_display_object()->as_avm2_button()) {
        btn->set_enabled(activation->context, args.get_bool(0));
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_use_hand_cursor(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto btn = this_obj->as_display_object()->as_avm2_button()) {
        return Value<GC>::from_bool(btn->use_hand_cursor());
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

    if (auto btn = this_obj->as_display_object()->as_avm2_button()) {
        btn->set_use_hand_cursor(args.get_bool(0));
    }

    return Value<GC>::undefined();
}

} // namespace simple_button
} // namespace display
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SIMPLE_BUTTON_H
