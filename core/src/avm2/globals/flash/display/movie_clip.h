// C++ translation of core/src/avm2/globals/flash/display/movie_clip.rs
//! `flash.display.MovieClip` builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_MOVIE_CLIP_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_MOVIE_CLIP_H

#include <vector>
#include <optional>
#include <string>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename GC>
    class Activation;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    template<typename GC>
    class Object;
    template<typename GC>
    class ArrayObject;
    template<typename GC>
    class AvmString;

    namespace globals {
        namespace flash {
            namespace display {
                template<typename GC>
                class Scene;
            }
        }
    }
}

namespace display_object {
    template<typename GC>
    class MovieClip;
    struct Scene;
}

namespace gc_arena {
    class Mutation;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace display {
namespace movie_clip {

using gc_arena::Mutation;

/// Implements `addFrameScript`, an undocumented method of `MovieClip` used to
/// specify what methods of a clip's class run on which frames.
template<typename GC>
Value<GC> add_frame_script(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `currentFrame`.
template<typename GC>
Value<GC> get_current_frame(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `currentFrameLabel`.
template<typename GC>
Value<GC> get_current_frame_label(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `currentLabel`.
template<typename GC>
Value<GC> get_current_label(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `currentLabels`.
template<typename GC>
Value<GC> get_current_labels(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `currentScene`.
template<typename GC>
Value<GC> get_current_scene(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `enabled` getter.
template<typename GC>
Value<GC> get_enabled(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `enabled` setter.
template<typename GC>
Value<GC> set_enabled(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `scenes`.
template<typename GC>
Value<GC> get_scenes(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `framesLoaded`.
template<typename GC>
Value<GC> get_frames_loaded(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `isPlaying`.
template<typename GC>
Value<GC> get_is_playing(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `totalFrames`.
template<typename GC>
Value<GC> get_total_frames(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `gotoAndPlay`.
template<typename GC>
Value<GC> goto_and_play(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `gotoAndStop`.
template<typename GC>
Value<GC> goto_and_stop(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Internal helper for gotoAndPlay/gotoAndStop
template<typename GC>
void goto_frame(
    Activation<GC>* activation,
    display_object::MovieClip<GC> mc,
    Value<GC> frame_or_label,
    std::optional<AvmString<GC>> scene_str,
    bool stop
);

/// Implements `stop`.
template<typename GC>
Value<GC> stop(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `play`.
template<typename GC>
Value<GC> play(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `prevFrame`.
template<typename GC>
Value<GC> prev_frame(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `nextFrame`.
template<typename GC>
Value<GC> next_frame(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `prevScene`.
template<typename GC>
Value<GC> prev_scene(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `nextScene`.
template<typename GC>
Value<GC> next_scene(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// Template Implementations
// ============================================================================

template<typename GC>
Value<GC> add_frame_script(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        // Process args in chunks of 2 (frame_id, callable)
        for (size_t i = 0; i + 1 < args.size(); i += 2) {
            auto frame_id = static_cast<uint16_t>(args[i].coerce_to_u32(activation)) + 1;
            auto callable = args[i + 1].as_object();

            mc->register_frame_script(frame_id, callable, activation->context);
        }
    } else {
        // tracing::error!("Attempted to add frame scripts to non-MovieClip this!");
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_current_frame(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        if (auto scene = mc->current_scene()) {
            // Return frame relative to scene start
            return Value<GC>::from_int(static_cast<int32_t>((mc->current_frame() + 1) - scene->start));
        } else {
            return Value<GC>::from_int(static_cast<int32_t>(mc->current_frame()));
        }
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_current_frame_label(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        if (auto label = mc->current_label()) {
            auto [label_str, start_frame] = *label;
            if (start_frame < mc->current_frame()) {
                return Value<GC>::null();
            }
            return Value<GC>::from_string(AvmString<GC>::new_string(activation->gc(), label_str));
        }
        return Value<GC>::null();
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_current_label(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        if (auto label = mc->current_label()) {
            auto [label_str, _start_frame] = *label;
            return Value<GC>::from_string(AvmString<GC>::new_string(activation->gc(), label_str));
        }
        return Value<GC>::null();
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_current_labels(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        auto scene = mc->current_scene().value_or(display_object::Scene{
            "",
            1,
            mc->header_frames()
        });

        auto labels_array = labels_for_scene(activation, mc, &scene);
        return Value<GC>::from_object(labels_array);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_current_scene(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        auto scene = mc->current_scene().value_or(display_object::Scene{
            "",
            1,
            mc->header_frames()
        });

        auto [scene_name, scene_length, scene_labels] = labels_for_scene(activation, mc, &scene);
        auto scene_class = activation->avm2().classes().scene;

        std::vector<Value<GC>> scene_args = {
            Value<GC>::from_string(AvmString<GC>::new_utf8(activation->gc(), scene_name)),
            Value<GC>::from_object(scene_labels),
            Value<GC>::from_int(static_cast<int32_t>(scene_length))
        };

        auto scene_obj = scene_class.construct(activation, scene_args);
        return Value<GC>::from_object(scene_obj);
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

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        return Value<GC>::from_bool(mc->avm2_enabled());
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

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        mc->set_avm2_enabled(args[0].as_bool());
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_scenes(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        auto mc_scenes = mc->scenes();
        if (mc_scenes.empty()) {
            mc_scenes.push_back(display_object::Scene{
                "",
                1,
                mc->header_frames()
            });
        }

        std::vector<Value<GC>> scene_objects;
        for (const auto& scene : mc_scenes) {
            auto [scene_name, scene_length, scene_labels] = labels_for_scene(activation, mc, &scene);
            auto scene_class = activation->avm2().classes().scene;

            std::vector<Value<GC>> scene_args = {
                Value<GC>::from_string(AvmString<GC>::new_utf8(activation->gc(), scene_name)),
                Value<GC>::from_object(scene_labels),
                Value<GC>::from_int(static_cast<int32_t>(scene_length))
            };

            scene_objects.push_back(Value<GC>::from_object(scene_class.construct(activation, scene_args)));
        }

        return Value<GC>::from_object(ArrayObject<GC>::from_array(activation->context, scene_objects));
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_frames_loaded(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        return Value<GC>::from_int(std::min(mc->frames_loaded(), static_cast<int32_t>(mc->header_frames())));
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_is_playing(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        return Value<GC>::from_bool(mc->programmatically_played() && mc->playing());
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_total_frames(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        return Value<GC>::from_int(static_cast<int32_t>(mc->header_frames()));
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> goto_and_play(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        mc->set_programmatically_played();

        auto frame_or_label = args.size() > 0 ? args[0] : Value<GC>::undefined();
        std::optional<AvmString<GC>> scene = args.size() > 1 ? std::optional(args[1].coerce_to_string(activation)) : std::nullopt;

        goto_frame(activation, mc, frame_or_label, scene, false);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> goto_and_stop(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        auto frame_or_label = args.size() > 0 ? args[0] : Value<GC>::undefined();
        std::optional<AvmString<GC>> scene = args.size() > 1 ? std::optional(args[1].coerce_to_string(activation)) : std::nullopt;

        goto_frame(activation, mc, frame_or_label, scene, true);
    }

    return Value<GC>::undefined();
}

template<typename GC>
void goto_frame(
    Activation<GC>* activation,
    display_object::MovieClip<GC> mc,
    Value<GC> frame_or_label,
    std::optional<AvmString<GC>> scene_str,
    bool stop
) {
    // Determine scene offset
    int32_t scene = 0;
    if (scene_str.has_value()) {
        if (auto scene_num = mc->scene_label_to_number(scene_str.value())) {
            scene = static_cast<int32_t>(*scene_num) - 1;
        }
    } else if (auto current_scene = mc->current_scene()) {
        if (auto scene_num = mc->scene_label_to_number(current_scene->name)) {
            scene = static_cast<int32_t>(*scene_num) - 1;
        }
    }

    // Determine frame
    int32_t frame;
    if (frame_or_label.is_integer()) {
        frame = frame_or_label.as_integer() + scene;
    } else {
        auto frame_str = frame_or_label.coerce_to_string(activation);

        // Try to parse as number
        // TODO: Implement string_to_int
        bool is_number = false;
        int32_t parsed_frame = 0;
        // parsed_frame = string_to_int(frame_str, 10, true);

        if (!is_number) {
            if (scene_str.has_value()) {
                // Validate frame exists within scene
                if (!mc->frame_exists_within_scene(frame_str, scene_str.value(), activation->context)) {
                    // Return error 2109
                    // TODO: Implement error
                    return;
                }
            }

            if (auto frame_num = mc->frame_label_to_number(frame_str, activation->context)) {
                if (activation->caller_movie_or_root()->version() >= 11) {
                    frame = static_cast<int32_t>(*frame_num);
                } else {
                    frame = static_cast<int32_t>(*frame_num);
                }
            } else {
                frame = 0;  // Old swf versions silently jump to frame 1 for invalid labels
            }
        } else {
            frame = parsed_frame - 1 + scene + 1;
        }
    }

    mc->goto_frame(activation->context, std::max(1, frame), stop);
}

template<typename GC>
Value<GC> stop(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        mc->stop(activation->context);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> play(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        mc->set_programmatically_played();
        mc->play();
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> prev_frame(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        mc->prev_frame(activation->context);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> next_frame(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        mc->next_frame(activation->context);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> prev_scene(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        if (auto scene = mc->previous_scene()) {
            mc->goto_frame(activation->context, scene->start, false);
        }
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> next_scene(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();

    if (auto mc = this_obj->as_display_object()->as_movie_clip()) {
        if (auto scene = mc->next_scene()) {
            mc->goto_frame(activation->context, scene->start, false);
        }
    }

    return Value<GC>::undefined();
}

/// Helper function to produce scene labels
template<typename GC>
ArrayObject<GC> labels_for_scene(
    Activation<GC>* activation,
    display_object::MovieClip<GC> mc,
    const display_object::Scene* scene
) {
    auto frame_label_class = activation->avm2().classes().framelabel;

    std::vector<Value<GC>> frame_labels;
    auto labels = mc->labels_in_range(scene->start, scene->start + scene->length);

    for (const auto& [name, frame] : labels) {
        auto name_val = Value<GC>::from_string(AvmString<GC>::new_string(activation->gc(), name));
        auto local_frame = static_cast<int32_t>(frame - scene->start + 1);

        std::vector<Value<GC>> label_args = {name_val, Value<GC>::from_int(local_frame)};
        frame_labels.push_back(Value<GC>::from_object(frame_label_class.construct(activation, label_args)));
    }

    return ArrayObject<GC>::from_array(activation->context, frame_labels);
}

} // namespace movie_clip
} // namespace display
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_MOVIE_CLIP_H
