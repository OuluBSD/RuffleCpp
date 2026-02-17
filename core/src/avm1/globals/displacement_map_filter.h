// C++ translation of core/src/avm1/globals/displacement_map_filter.rs
//! flash.filters.DisplacementMapFilter object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_DISPLACEMENT_MAP_FILTER_H
#define RUFFLE_CORE_AVM1_GLOBALS_DISPLACEMENT_MAP_FILTER_H

#include <memory>
#include <cstdint>
#include <optional>
#include <cell>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC> class UpdateContext;
    template<typename GC> class Mutation;
    template<typename GC> class Gc;

    namespace avm1 {
        template<typename T> class Activation;
        template<typename T> class Object;
        template<typename T> class Error;
        template<typename T> class Value;
        template<typename T> class NativeObject;

        namespace property_decl {
            template<typename T> class DeclContext;
            struct StaticDeclarations;
            template<typename T> class SystemClass;
        }
    }

    namespace bitmap {
        namespace bitmap_data {
            template<typename T> class BitmapData;
        }
    }

    namespace ruffle_render {
        namespace filters {
            enum class DisplacementMapFilterMode {
                Wrap,
                Clamp,
                Ignore,
                Color
            };

            struct DisplacementMapFilter;
        }
    }

    namespace swf {
        struct Color;
        template<typename T> struct Point;
    }
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {

/// Data structure for DisplacementMapFilter
template<typename GC>
class DisplacementMapFilterData {
public:
    DisplacementMapFilterData() = default;

    std::optional<bitmap::bitmap_data::BitmapData<GC>> map_bitmap;
    swf::Point<int32_t> map_point{0, 0};
    int32_t component_x = 0;
    int32_t component_y = 0;
    float scale_x = 0.0f;
    float scale_y = 0.0f;
    ruffle_render::filters::DisplacementMapFilterMode mode = ruffle_render::filters::DisplacementMapFilterMode::Wrap;
    swf::Color color;

    friend class DisplacementMapFilter<GC>;
};

/// DisplacementMapFilter - flash.filters.DisplacementMapFilter object
template<typename GC>
class DisplacementMapFilter {
public:
    DisplacementMapFilter() = default;
    explicit DisplacementMapFilter(Gc<GC, DisplacementMapFilterData<GC>> data) : data_(data) {}

    /// Create a new DisplacementMapFilter from arguments
    ///
    /// @param activation The current activation
    /// @param args Constructor arguments
    /// @return New DisplacementMapFilter or error
    static Result<DisplacementMapFilter<GC>, Error<GC>> new_(
        Activation<GC>* activation,
        const std::span<Value<GC>>& args
    );

    /// Create from a render filter
    ///
    /// @param gc_context The mutation context
    /// @param filter The render filter
    /// @return New DisplacementMapFilter
    static DisplacementMapFilter from_filter(
        Mutation<GC>* gc_context,
        const ruffle_render::filters::DisplacementMapFilter& filter
    );

    /// Duplicate this filter
    ///
    /// @param gc_context The mutation context
    /// @return Duplicated filter
    DisplacementMapFilter duplicate(Mutation<GC>* gc_context) const;

    /// Get the map bitmap
    ///
    /// @param activation The current activation
    /// @return Optional map bitmap object
    std::optional<Object<GC>> map_bitmap(Activation<GC>* activation) const;

    /// Set the map bitmap
    ///
    /// @param activation The current activation
    /// @param value Optional value to set
    void set_map_bitmap(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get the map point
    ///
    /// @param activation The current activation
    /// @return Map point as Value
    Result<Value<GC>, Error<GC>> map_point(Activation<GC>* activation) const;

    /// Set the map point
    ///
    /// @param activation The current activation
    /// @param value Optional value to set
    void set_map_point(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get component X
    int32_t component_x() const { return data_->component_x; }

    /// Set component X
    void set_component_x(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get component Y
    int32_t component_y() const { return data_->component_y; }

    /// Set component Y
    void set_component_y(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get scale X
    float scale_x() const { return data_->scale_x; }

    /// Set scale X
    void set_scale_x(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get scale Y
    float scale_y() const { return data_->scale_y; }

    /// Set scale Y
    void set_scale_y(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get the mode
    ruffle_render::filters::DisplacementMapFilterMode mode() const { return data_->mode; }

    /// Set the mode
    void set_mode(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get the color
    swf::Color color() const { return data_->color; }

    /// Set the color
    void set_color(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Set the alpha
    void set_alpha(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Convert to render filter
    ///
    /// @param context The update context
    /// @return Render filter
    ruffle_render::filters::DisplacementMapFilter filter(UpdateContext<GC>* context) const;

private:
    Gc<GC, DisplacementMapFilterData<GC>> data_;
};

// Method indices for property dispatch
namespace displacement_map_filter_method {
    constexpr uint16_t CONSTRUCTOR = 0;
    constexpr uint16_t GET_MAP_BITMAP = 1;
    constexpr uint16_t SET_MAP_BITMAP = 2;
    constexpr uint16_t GET_MAP_POINT = 3;
    constexpr uint16_t SET_MAP_POINT = 4;
    constexpr uint16_t GET_COMPONENT_X = 5;
    constexpr uint16_t SET_COMPONENT_X = 6;
    constexpr uint16_t GET_COMPONENT_Y = 7;
    constexpr uint16_t SET_COMPONENT_Y = 8;
    constexpr uint16_t GET_SCALE_X = 9;
    constexpr uint16_t SET_SCALE_X = 10;
    constexpr uint16_t GET_SCALE_Y = 11;
    constexpr uint16_t SET_SCALE_Y = 12;
    constexpr uint16_t GET_MODE = 13;
    constexpr uint16_t SET_MODE = 14;
    constexpr uint16_t GET_COLOR = 15;
    constexpr uint16_t SET_COLOR = 16;
    constexpr uint16_t GET_ALPHA = 17;
    constexpr uint16_t SET_ALPHA = 18;
}

/// Create the DisplacementMapFilter class
///
/// @param context The declaration context
/// @param super_proto The superclass prototype
/// @return SystemClass for DisplacementMapFilter
template<typename GC>
property_decl::SystemClass<GC> create_displacement_map_filter_class(
    property_decl::DeclContext<GC>* context,
    Object<GC> super_proto
);

/// Method handler for DisplacementMapFilter
///
/// @param activation The current activation
/// @param this_obj The this object
/// @param args Method arguments
/// @param index The method index
/// @return Result value or error
template<typename GC>
Result<Value<GC>, Error<GC>> displacement_map_filter_method(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::span<Value<GC>>& args,
    uint16_t index
);

// Template method implementations

template<typename GC>
Result<DisplacementMapFilter<GC>, Error<GC>> DisplacementMapFilter<GC>::new_(
    Activation<GC>* activation,
    const std::span<Value<GC>>& args
) {
    auto filter = DisplacementMapFilter<GC>(
        Gc<GC, DisplacementMapFilterData<GC>>::create(activation->gc())
    );

    filter.set_map_bitmap(activation, args.size() > 0 ? std::optional(args[0]) : std::nullopt);
    filter.set_map_point(activation, args.size() > 1 ? std::optional(args[1]) : std::nullopt);
    filter.set_component_x(activation, args.size() > 2 ? std::optional(args[2]) : std::nullopt);
    filter.set_component_y(activation, args.size() > 3 ? std::optional(args[3]) : std::nullopt);
    filter.set_scale_x(activation, args.size() > 4 ? std::optional(args[4]) : std::nullopt);
    filter.set_scale_y(activation, args.size() > 5 ? std::optional(args[5]) : std::nullopt);
    filter.set_mode(activation, args.size() > 6 ? std::optional(args[6]) : std::nullopt);
    filter.set_color(activation, args.size() > 7 ? std::optional(args[7]) : std::nullopt);
    filter.set_alpha(activation, args.size() > 8 ? std::optional(args[8]) : std::nullopt);

    return Ok(filter);
}

template<typename GC>
DisplacementMapFilter<GC> DisplacementMapFilter<GC>::from_filter(
    Mutation<GC>* gc_context,
    const ruffle_render::filters::DisplacementMapFilter& filter
) {
    auto data = std::make_unique<DisplacementMapFilterData<GC>>();
    data->map_point = swf::Point<int32_t>{filter.map_point.first, filter.map_point.second};
    data->component_x = static_cast<int32_t>(filter.component_x);
    data->component_y = static_cast<int32_t>(filter.component_y);
    data->scale_x = filter.scale_x;
    data->scale_y = filter.scale_y;
    data->mode = filter.mode;
    data->color = filter.color;

    return DisplacementMapFilter<GC>(
        Gc<GC, DisplacementMapFilterData<GC>>::create(gc_context, std::move(data))
    );
}

template<typename GC>
DisplacementMapFilter<GC> DisplacementMapFilter<GC>::duplicate(Mutation<GC>* gc_context) const {
    auto data = std::make_unique<DisplacementMapFilterData<GC>>(*data_);
    return DisplacementMapFilter<GC>(
        Gc<GC, DisplacementMapFilterData<GC>>::create(gc_context, std::move(data))
    );
}

template<typename GC>
std::optional<Object<GC>> DisplacementMapFilter<GC>::map_bitmap(Activation<GC>* activation) const {
    if (data_->map_bitmap.has_value()) {
        auto proto = activation->prototypes()->bitmap_data;
        auto result = Object<GC>::new_object(activation->strings());
        result.set_native(activation->gc(), NativeObject<GC>::bitmap_data(data_->map_bitmap.value()));
        return result;
    }
    return std::nullopt;
}

template<typename GC>
void DisplacementMapFilter<GC>::set_map_bitmap(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value() && value->is_object()) {
        auto obj = value->as_object();
        if (obj.has_value()) {
            auto native = obj->native();
            if (native.is_bitmap_data()) {
                data_->map_bitmap = native.bitmap_data();
            }
        }
    }
}

template<typename GC>
Result<Value<GC>, Error<GC>> DisplacementMapFilter<GC>::map_point(Activation<GC>* activation) const {
    auto& point = data_->map_point;
    std::array<Value<GC>, 2> args = {
        Value<GC>::from_i32(point.x),
        Value<GC>::from_i32(point.y)
    };

    auto constructor = activation->prototypes()->point_constructor;
    return constructor.construct(activation, args);
}

template<typename GC>
void DisplacementMapFilter<GC>::set_map_point(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (!value.has_value()) {
        data_->map_point = swf::Point<int32_t>{0, 0};
        return;
    }

    if (value->is_object()) {
        auto obj = value->as_object();
        if (obj.has_value()) {
            auto x_result = obj->get_local_stored(u"x", activation);
            auto y_result = obj->get_local_stored(u"y", activation);

            swf::Point<int32_t> point{0, 0};
            if (x_result.has_value() && y_result.has_value()) {
                auto x_f64 = x_result->coerce_to_f64(activation);
                auto y_f64 = y_result->coerce_to_f64(activation);
                if (x_f64.is_ok() && y_f64.is_ok()) {
                    point.x = static_cast<int32_t>(x_f64.unwrap());
                    point.y = static_cast<int32_t>(y_f64.unwrap());
                }
            }
            data_->map_point = point;
        }
    } else {
        data_->map_point = swf::Point<int32_t>{0, 0};
    }
}

template<typename GC>
void DisplacementMapFilter<GC>::set_component_x(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        auto result = value->coerce_to_i32(activation);
        if (result.is_ok()) {
            data_->component_x = result.unwrap();
        }
    }
}

template<typename GC>
void DisplacementMapFilter<GC>::set_component_y(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        auto result = value->coerce_to_i32(activation);
        if (result.is_ok()) {
            data_->component_y = result.unwrap();
        }
    }
}

template<typename GC>
void DisplacementMapFilter<GC>::set_scale_x(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        constexpr double MAX = static_cast<double>(UINT16_MAX);
        constexpr double MIN = -MAX;
        auto result = value->coerce_to_f64(activation);
        if (result.is_ok()) {
            double scale_x = result.unwrap();
            if (scale_x < MIN) scale_x = MIN;
            if (scale_x > MAX) scale_x = MAX;
            data_->scale_x = static_cast<float>(scale_x);
        }
    }
}

template<typename GC>
void DisplacementMapFilter<GC>::set_scale_y(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        constexpr double MAX = static_cast<double>(UINT16_MAX);
        constexpr double MIN = -MAX;
        auto result = value->coerce_to_f64(activation);
        if (result.is_ok()) {
            double scale_y = result.unwrap();
            if (scale_y < MIN) scale_y = MIN;
            if (scale_y > MAX) scale_y = MAX;
            data_->scale_y = static_cast<float>(scale_y);
        }
    }
}

template<typename GC>
void DisplacementMapFilter<GC>::set_mode(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        auto mode_str = value->coerce_to_string(activation);
        if (mode_str.is_ok()) {
            auto& mode = mode_str.unwrap();
            if (mode == u"clamp") {
                data_->mode = ruffle_render::filters::DisplacementMapFilterMode::Clamp;
            } else if (mode == u"ignore") {
                data_->mode = ruffle_render::filters::DisplacementMapFilterMode::Ignore;
            } else if (mode == u"color") {
                data_->mode = ruffle_render::filters::DisplacementMapFilterMode::Color;
            } else {
                data_->mode = ruffle_render::filters::DisplacementMapFilterMode::Wrap;
            }
        }
    }
}

template<typename GC>
void DisplacementMapFilter<GC>::set_color(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        auto result = value->coerce_to_u32(activation);
        if (result.is_ok()) {
            auto color = data_->color;
            color.rgb = result.unwrap();
            data_->color = color;
        }
    }
}

template<typename GC>
void DisplacementMapFilter<GC>::set_alpha(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        auto result = value->coerce_to_f64(activation);
        if (result.is_ok()) {
            double alpha = result.unwrap();
            if (alpha < 0.0) alpha = 0.0;
            if (alpha > 1.0) alpha = 1.0;
            data_->color.a = static_cast<uint8_t>(alpha * 255.0);
        }
    }
}

template<typename GC>
ruffle_render::filters::DisplacementMapFilter DisplacementMapFilter<GC>::filter(
    UpdateContext<GC>* context
) const {
    ruffle_render::filters::DisplacementMapFilter result;
    result.color = data_->color;
    result.component_x = static_cast<uint8_t>(data_->component_x);
    result.component_y = static_cast<uint8_t>(data_->component_y);
    result.map_point = {data_->map_point.x, data_->map_point.y};
    result.mode = data_->mode;
    result.scale_x = data_->scale_x;
    result.scale_y = data_->scale_y;
    result.viewscale_x = 1.0;
    result.viewscale_y = 1.0;

    if (data_->map_bitmap.has_value()) {
        result.map_bitmap = data_->map_bitmap->bitmap_handle(context->gc(), context->renderer());
    }

    return result;
}

} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_DISPLACEMENT_MAP_FILTER_H
