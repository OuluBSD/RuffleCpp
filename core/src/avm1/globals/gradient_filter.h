// C++ translation of core/src/avm1/globals/gradient_filter.rs
//! flash.filters.GradientBevelFilter and flash.filters.GradientGlowFilter objects

#ifndef RUFFLE_CORE_AVM1_GLOBALS_GRADIENT_FILTER_H
#define RUFFLE_CORE_AVM1_GLOBALS_GRADIENT_FILTER_H

#include <vector>
#include <optional>
#include <memory>
#include <cstdint>
#include <array>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename GC>
    class Activation;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    template<typename GC>
    class Object;
    enum class NativeObject;

    namespace globals {
        namespace bevel_filter {
            enum class BevelFilterType;
        }
    }
}

namespace gc_arena {
    template<typename T>
    class Gc;
    class Mutation;
}
}
}

namespace swf {
struct Color;
struct Fixed8;
struct Fixed16;
enum class GradientFilterFlags;
struct GradientRecord;
struct GradientFilter;
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace gradient_filter {

using gc_arena::Gc;
using gc_arena::Mutation;

constexpr size_t MAX_COLORS = 16;

/// Internal data structure for gradient filter state
struct GradientFilterData {
    double distance;
    double angle;
    std::array<swf::GradientRecord, MAX_COLORS> colors;
    size_t num_colors;
    double blur_x;
    double blur_y;
    uint16_t strength;  // Stored as fixed-point (multiplied by 256)
    int32_t quality;
    bevel_filter::BevelFilterType type;
    bool knockout;

    GradientFilterData();

    double get_strength() const {
        return static_cast<double>(strength) / 256.0;
    }

    void set_strength(double strength_val) {
        strength = static_cast<uint16_t>(std::clamp(strength_val * 256.0, 0.0, 65280.0));
    }
};

/// GradientFilter template class for AVM1 gradient filters
template<typename GC>
class GradientFilter {
public:
    using GcContext = gc_arena::Mutation;

private:
    Gc<GC, GradientFilterData> data_;

public:
    /// Create a new gradient filter from ActionScript arguments
    static GradientFilter<GC> new_(
        Activation<GC>* activation,
        const std::vector<Value<GC>>& args
    );

    /// Create from SWF filter
    static GradientFilter<GC> from_filter(
        GcContext* gc_context,
        const swf::GradientFilter& filter
    );

    /// Duplicate the filter
    GradientFilter<GC> duplicate(GcContext* gc_context) const;

    // Property getters
    double distance() const { return data_->distance; }
    double angle() const { return data_->angle * 180.0 / 3.14159265358979323846; }  // radians to degrees
    double blur_x() const { return data_->blur_x; }
    double blur_y() const { return data_->blur_y; }
    double strength() const { return data_->get_strength(); }
    int32_t quality() const { return data_->quality; }
    bevel_filter::BevelFilterType type() const { return data_->type; }
    bool knockout() const { return data_->knockout; }

    // Property setters
    void set_distance(Activation<GC>* activation, std::optional<Value<GC>> value);
    void set_angle(Activation<GC>* activation, std::optional<Value<GC>> value);
    void set_blur_x(Activation<GC>* activation, std::optional<Value<GC>> value);
    void set_blur_y(Activation<GC>* activation, std::optional<Value<GC>> value);
    void set_strength(Activation<GC>* activation, std::optional<Value<GC>> value);
    void set_quality(Activation<GC>* activation, std::optional<Value<GC>> value);
    void set_type(Activation<GC>* activation, std::optional<Value<GC>> value);
    void set_knockout(Activation<GC>* activation, std::optional<Value<GC>> value);

    // Array property getters
    Object<GC> colors(Activation<GC>* activation) const;
    Object<GC> alphas(Activation<GC>* activation) const;
    Object<GC> ratios(Activation<GC>* activation) const;

    // Array property setters
    void set_colors(Activation<GC>* activation, std::optional<Value<GC>> value);
    void set_alphas(Activation<GC>* activation, std::optional<Value<GC>> value);
    void set_ratios(Activation<GC>* activation, std::optional<Value<GC>> value);

    /// Convert to SWF filter
    swf::GradientFilter filter() const;

    /// Get the internal data
    const Gc<GC, GradientFilterData>& gc_data() const { return data_; }
};

// Method indices for property dispatch
namespace method {
    constexpr uint16_t GLOW_CONSTRUCTOR = 0;
    constexpr uint16_t GET_DISTANCE = 1;
    constexpr uint16_t SET_DISTANCE = 2;
    constexpr uint16_t GET_ANGLE = 3;
    constexpr uint16_t SET_ANGLE = 4;
    constexpr uint16_t GET_COLORS = 5;
    constexpr uint16_t SET_COLORS = 6;
    constexpr uint16_t GET_ALPHAS = 7;
    constexpr uint16_t SET_ALPHAS = 8;
    constexpr uint16_t GET_RATIOS = 9;
    constexpr uint16_t SET_RATIOS = 10;
    constexpr uint16_t GET_BLUR_X = 11;
    constexpr uint16_t SET_BLUR_X = 12;
    constexpr uint16_t GET_BLUR_Y = 13;
    constexpr uint16_t SET_BLUR_Y = 14;
    constexpr uint16_t GET_QUALITY = 15;
    constexpr uint16_t SET_QUALITY = 16;
    constexpr uint16_t GET_STRENGTH = 17;
    constexpr uint16_t SET_STRENGTH = 18;
    constexpr uint16_t GET_KNOCKOUT = 19;
    constexpr uint16_t SET_KNOCKOUT = 20;
    constexpr uint16_t GET_TYPE = 21;
    constexpr uint16_t SET_TYPE = 22;
    constexpr uint16_t BEVEL_CONSTRUCTOR = 1000;
}

/// Create the GradientBevelFilter class
template<typename GC>
struct SystemClass {
    Object<GC> proto;
    Object<GC> class_object;
};

template<typename GC>
SystemClass<GC> create_bevel_class(
    Object<GC> super_proto
);

template<typename GC>
SystemClass<GC> create_glow_class(
    Object<GC> super_proto
);

/// Method dispatcher for gradient filter properties
template<typename GC>
Value<GC> method(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args,
    uint16_t index
);

// ============================================================================
// Template Implementations
// ============================================================================

template<typename GC>
GradientFilter<GC> GradientFilter<GC>::new_(
    Activation<GC>* activation,
    const std::vector<Value<GC>>& args
) {
    auto filter = GradientFilter<GC>(Gc<GC, GradientFilterData>::new_gc(
        activation->gc(),
        GradientFilterData()
    ));

    filter.set_distance(activation, args.size() > 0 ? std::optional(args[0]) : std::nullopt);
    filter.set_angle(activation, args.size() > 1 ? std::optional(args[1]) : std::nullopt);
    filter.set_colors(activation, args.size() > 2 ? std::optional(args[2]) : std::nullopt);
    filter.set_alphas(activation, args.size() > 3 ? std::optional(args[3]) : std::nullopt);
    filter.set_ratios(activation, args.size() > 4 ? std::optional(args[4]) : std::nullopt);
    filter.set_blur_x(activation, args.size() > 5 ? std::optional(args[5]) : std::nullopt);
    filter.set_blur_y(activation, args.size() > 6 ? std::optional(args[6]) : std::nullopt);
    filter.set_strength(activation, args.size() > 7 ? std::optional(args[7]) : std::nullopt);
    filter.set_quality(activation, args.size() > 8 ? std::optional(args[8]) : std::nullopt);
    filter.set_type(activation, args.size() > 9 ? std::optional(args[9]) : std::nullopt);
    filter.set_knockout(activation, args.size() > 10 ? std::optional(args[10]) : std::nullopt);

    return filter;
}

template<typename GC>
GradientFilter<GC> GradientFilter<GC>::from_filter(
    GcContext* gc_context,
    const swf::GradientFilter& filter
) {
    GradientFilterData data;
    data.distance = filter.distance.to_pixels();
    data.angle = filter.angle.to_radians();
    data.blur_x = filter.blur_x.to_pixels();
    data.blur_y = filter.blur_y.to_pixels();
    data.strength = static_cast<uint16_t>(filter.strength.to_f64() * 256.0);
    data.quality = static_cast<int32_t>(filter.num_passes());
    data.knockout = filter.is_knockout();

    // Convert flags to type
    // TODO: Implement flag conversion

    data.num_colors = std::min(filter.colors.size(), MAX_COLORS);
    for (size_t i = 0; i < data.num_colors; ++i) {
        data.colors[i] = filter.colors[i];
    }

    return GradientFilter<GC>(Gc<GC, GradientFilterData>::new_gc(gc_context, data));
}

template<typename GC>
GradientFilter<GC> GradientFilter<GC>::duplicate(GcContext* gc_context) const {
    return GradientFilter<GC>(Gc<GC, GradientFilterData>::new_gc(gc_context, *data_));
}

template<typename GC>
void GradientFilter<GC>::set_distance(Activation<GC>* activation, std::optional<Value<GC>> value) {
    if (value.has_value()) {
        data_->distance = value->coerce_to_f64(activation);
    }
}

template<typename GC>
void GradientFilter<GC>::set_angle(Activation<GC>* activation, std::optional<Value<GC>> value) {
    if (value.has_value()) {
        double angle = std::fmod(value->coerce_to_f64(activation), 360.0);
        data_->angle = angle * 3.14159265358979323846 / 180.0;  // degrees to radians
    }
}

template<typename GC>
void GradientFilter<GC>::set_blur_x(Activation<GC>* activation, std::optional<Value<GC>> value) {
    if (value.has_value()) {
        data_->blur_x = std::clamp(value->coerce_to_f64(activation), 0.0, 255.0);
    }
}

template<typename GC>
void GradientFilter<GC>::set_blur_y(Activation<GC>* activation, std::optional<Value<GC>> value) {
    if (value.has_value()) {
        data_->blur_y = std::clamp(value->coerce_to_f64(activation), 0.0, 255.0);
    }
}

template<typename GC>
void GradientFilter<GC>::set_strength(Activation<GC>* activation, std::optional<Value<GC>> value) {
    if (value.has_value()) {
        data_->set_strength(value->coerce_to_f64(activation));
    }
}

template<typename GC>
void GradientFilter<GC>::set_quality(Activation<GC>* activation, std::optional<Value<GC>> value) {
    if (value.has_value()) {
        data_->quality = std::clamp(value->coerce_to_i32(activation), 0, 15);
    }
}

template<typename GC>
void GradientFilter<GC>::set_type(Activation<GC>* activation, std::optional<Value<GC>> value) {
    if (value.has_value()) {
        auto type_str = value->coerce_to_string(activation);
        if (type_str == "inner") {
            data_->type = bevel_filter::BevelFilterType::Inner;
        } else if (type_str == "outer") {
            data_->type = bevel_filter::BevelFilterType::Outer;
        } else {
            data_->type = bevel_filter::BevelFilterType::Full;
        }
    }
}

template<typename GC>
void GradientFilter<GC>::set_knockout(Activation<GC>* activation, std::optional<Value<GC>> value) {
    if (value.has_value()) {
        data_->knockout = value->as_bool(activation->swf_version());
    }
}

template<typename GC>
Object<GC> GradientFilter<GC>::colors(Activation<GC>* activation) const {
    // TODO: Implement array creation for colors
    return Object<GC>::null();
}

template<typename GC>
Object<GC> GradientFilter<GC>::alphas(Activation<GC>* activation) const {
    // TODO: Implement array creation for alphas
    return Object<GC>::null();
}

template<typename GC>
Object<GC> GradientFilter<GC>::ratios(Activation<GC>* activation) const {
    // TODO: Implement array creation for ratios
    return Object<GC>::null();
}

template<typename GC>
void GradientFilter<GC>::set_colors(Activation<GC>* activation, std::optional<Value<GC>> value) {
    if (!value.has_value()) return;

    // TODO: Implement color array setting
}

template<typename GC>
void GradientFilter<GC>::set_alphas(Activation<GC>* activation, std::optional<Value<GC>> value) {
    if (!value.has_value()) return;

    // TODO: Implement alpha array setting
}

template<typename GC>
void GradientFilter<GC>::set_ratios(Activation<GC>* activation, std::optional<Value<GC>> value) {
    if (!value.has_value()) return;

    // TODO: Implement ratio array setting
}

template<typename GC>
swf::GradientFilter GradientFilter<GC>::filter() const {
    // TODO: Implement conversion to SWF filter
    return swf::GradientFilter{};
}

template<typename GC>
SystemClass<GC> create_bevel_class(Object<GC> super_proto) {
    // TODO: Implement class creation
    return SystemClass<GC>{Object<GC>::null(), Object<GC>::null()};
}

template<typename GC>
SystemClass<GC> create_glow_class(Object<GC> super_proto) {
    // TODO: Implement class creation
    return SystemClass<GC>{Object<GC>::null(), Object<GC>::null()};
}

template<typename GC>
Value<GC> method(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args,
    uint16_t index
) {
    using namespace method;

    // Get the gradient filter from the object
    auto native = this_obj.native();
    GradientFilter<GC> filter;

    // TODO: Extract filter from native object
    // For now, return undefined
    return Value<GC>::undefined();
}

} // namespace gradient_filter
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_GRADIENT_FILTER_H
