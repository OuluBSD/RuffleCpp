// C++ translation of core/src/avm1/globals/bevel_filter.rs
//! flash.filters.BevelFilter object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_BEVEL_FILTER_H
#define RUFFLE_CORE_AVM1_GLOBALS_BEVEL_FILTER_H

#include <memory>
#include <cstdint>
#include <optional>
#include <cmath>

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

    namespace swf {
        struct Color;
        struct Fixed8;
        struct Fixed16;

        struct BevelFilterFlags {
            static constexpr uint8_t INNER_SHADOW = 0x01;
            static constexpr uint8_t ON_TOP = 0x02;
            static constexpr uint8_t COMPOSITE_SOURCE = 0x04;
            static constexpr uint8_t KNOCKOUT = 0x08;

            uint8_t flags = 0;

            bool contains(uint8_t flag) const { return (flags & flag) != 0; }
            void set(uint8_t flag, bool value) {
                if (value) flags |= flag;
                else flags &= ~flag;
            }

            static BevelFilterFlags from_passes(uint8_t passes) {
                BevelFilterFlags result;
                result.flags = passes << 4;
                return result;
            }

            uint8_t num_passes() const { return (flags >> 4) & 0x0F; }
            bool is_knockout() const { return contains(KNOCKOUT); }
        };

        struct GradientFilterFlags {
            static constexpr uint8_t INNER_SHADOW = 0x01;
            static constexpr uint8_t ON_TOP = 0x02;

            uint8_t flags = 0;
            bool contains(uint8_t flag) const { return (flags & flag) != 0; }
        };

        struct BevelFilter;
    }
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {

/// Bevel filter type enumeration
enum class BevelFilterType {
    Inner,
    Outer,
    Full
};

impl BevelFilterType {
    /// Convert to BevelFilterFlags
    swf::BevelFilterFlags as_bevel_flags() const {
        switch (type_) {
            case BevelFilterType::Inner:
                return swf::BevelFilterFlags{swf::BevelFilterFlags::INNER_SHADOW};
            case BevelFilterType::Outer:
                return swf::BevelFilterFlags{};
            case BevelFilterType::Full:
                return swf::BevelFilterFlags{swf::BevelFilterFlags::ON_TOP};
        }
    }

    /// Convert to GradientFilterFlags
    swf::GradientFilterFlags as_gradient_flags() const {
        switch (type_) {
            case BevelFilterType::Inner:
                return swf::GradientFilterFlags{swf::GradientFilterFlags::INNER_SHADOW};
            case BevelFilterType::Outer:
                return swf::GradientFilterFlags{};
            case BevelFilterType::Full:
                return swf::GradientFilterFlags{swf::GradientFilterFlags::ON_TOP};
        }
    }
};

/// Convert from BevelFilterFlags
inline BevelFilterType bevel_type_from_flags(const swf::BevelFilterFlags& flags) {
    if (flags.contains(swf::BevelFilterFlags::ON_TOP)) {
        return BevelFilterType::Full;
    } else if (flags.contains(swf::BevelFilterFlags::INNER_SHADOW)) {
        return BevelFilterType::Inner;
    } else {
        return BevelFilterType::Outer;
    }
}

/// Convert from GradientFilterFlags
inline BevelFilterType bevel_type_from_gradient_flags(const swf::GradientFilterFlags& flags) {
    if (flags.contains(swf::GradientFilterFlags::ON_TOP)) {
        return BevelFilterType::Full;
    } else if (flags.contains(swf::GradientFilterFlags::INNER_SHADOW)) {
        return BevelFilterType::Inner;
    } else {
        return BevelFilterType::Outer;
    }
}

/// Data structure for BevelFilter
template<typename GC>
class BevelFilterData {
public:
    BevelFilterData() = default;

    double distance = 4.0;
    double angle = 0.785398163; // ~45 degrees in radians
    swf::Color highlight{255, 255, 255, 255}; // White
    swf::Color shadow{0, 0, 0, 255}; // Black
    int32_t quality = 1;
    uint16_t strength = 256; // 1.0 * 256
    bool knockout = false;
    double blur_x = 4.0;
    double blur_y = 4.0;
    BevelFilterType type_ = BevelFilterType::Inner;

    /// Get strength as double (0.0 - 255.0)
    double get_strength() const {
        return static_cast<double>(strength) / 256.0;
    }

    /// Set strength from double
    void set_strength(double value) {
        auto s = static_cast<uint16_t>(value * 256.0);
        strength = std::clamp(s, static_cast<uint16_t>(0), static_cast<uint16_t>(0xFF00));
    }

    friend class BevelFilter<GC>;
};

/// BevelFilter - flash.filters.BevelFilter object
template<typename GC>
class BevelFilter {
public:
    BevelFilter() = default;
    explicit BevelFilter(Gc<GC, BevelFilterData<GC>> data) : data_(data) {}

    /// Create a new BevelFilter from arguments
    ///
    /// @param activation The current activation
    /// @param args Constructor arguments
    /// @return New BevelFilter or error
    static Result<BevelFilter<GC>, Error<GC>> new_(
        Activation<GC>* activation,
        const std::span<Value<GC>>& args
    );

    /// Create from a SWF filter
    ///
    /// @param gc_context The mutation context
    /// @param filter The SWF filter
    /// @return New BevelFilter
    static BevelFilter from_filter(
        Mutation<GC>* gc_context,
        const swf::BevelFilter& filter
    );

    /// Duplicate this filter
    ///
    /// @param gc_context The mutation context
    /// @return Duplicated filter
    BevelFilter duplicate(Mutation<GC>* gc_context) const;

    /// Get distance
    double distance() const { return data_->distance; }

    /// Set distance
    void set_distance(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get angle in degrees
    double angle() const { return data_->angle * 180.0 / M_PI; }

    /// Set angle in degrees
    void set_angle(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get highlight color (RGB)
    int32_t highlight_color() const {
        return static_cast<int32_t>(data_->highlight.to_rgb());
    }

    /// Set highlight color
    void set_highlight_color(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get highlight alpha (0.0 - 1.0)
    double highlight_alpha() const {
        return static_cast<double>(data_->highlight.a) / 255.0;
    }

    /// Set highlight alpha
    void set_highlight_alpha(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get shadow color (RGB)
    int32_t shadow_color() const {
        return static_cast<int32_t>(data_->shadow.to_rgb());
    }

    /// Set shadow color
    void set_shadow_color(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get shadow alpha (0.0 - 1.0)
    double shadow_alpha() const {
        return static_cast<double>(data_->shadow.a) / 255.0;
    }

    /// Set shadow alpha
    void set_shadow_alpha(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get quality
    int32_t quality() const { return data_->quality; }

    /// Set quality
    void set_quality(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get strength
    double strength() const { return data_->get_strength(); }

    /// Set strength
    void set_strength(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get knockout
    bool knockout() const { return data_->knockout; }

    /// Set knockout
    void set_knockout(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get blur X
    double blur_x() const { return data_->blur_x; }

    /// Set blur X
    void set_blur_x(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get blur Y
    double blur_y() const { return data_->blur_y; }

    /// Set blur Y
    void set_blur_y(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Get type
    BevelFilterType type() const { return data_->type_; }

    /// Set type
    void set_type(Activation<GC>* activation, const std::optional<Value<GC>>& value);

    /// Convert to SWF filter
    swf::BevelFilter filter() const;

private:
    Gc<GC, BevelFilterData<GC>> data_;
};

// Method indices for property dispatch
namespace bevel_filter_method {
    constexpr uint16_t CONSTRUCTOR = 0;
    constexpr uint16_t GET_DISTANCE = 1;
    constexpr uint16_t SET_DISTANCE = 2;
    constexpr uint16_t GET_ANGLE = 3;
    constexpr uint16_t SET_ANGLE = 4;
    constexpr uint16_t GET_HIGHLIGHT_COLOR = 5;
    constexpr uint16_t SET_HIGHLIGHT_COLOR = 6;
    constexpr uint16_t GET_HIGHLIGHT_ALPHA = 7;
    constexpr uint16_t SET_HIGHLIGHT_ALPHA = 8;
    constexpr uint16_t GET_SHADOW_COLOR = 9;
    constexpr uint16_t SET_SHADOW_COLOR = 10;
    constexpr uint16_t GET_SHADOW_ALPHA = 11;
    constexpr uint16_t SET_SHADOW_ALPHA = 12;
    constexpr uint16_t GET_QUALITY = 13;
    constexpr uint16_t SET_QUALITY = 14;
    constexpr uint16_t GET_STRENGTH = 15;
    constexpr uint16_t SET_STRENGTH = 16;
    constexpr uint16_t GET_KNOCKOUT = 17;
    constexpr uint16_t SET_KNOCKOUT = 18;
    constexpr uint16_t GET_BLUR_X = 19;
    constexpr uint16_t SET_BLUR_X = 20;
    constexpr uint16_t GET_BLUR_Y = 21;
    constexpr uint16_t SET_BLUR_Y = 22;
    constexpr uint16_t GET_TYPE = 23;
    constexpr uint16_t SET_TYPE = 24;
}

/// Create the BevelFilter class
///
/// @param context The declaration context
/// @param super_proto The superclass prototype
/// @return SystemClass for BevelFilter
template<typename GC>
property_decl::SystemClass<GC> create_bevel_filter_class(
    property_decl::DeclContext<GC>* context,
    Object<GC> super_proto
);

/// Method handler for BevelFilter
///
/// @param activation The current activation
/// @param this_obj The this object
/// @param args Method arguments
/// @param index The method index
/// @return Result value or error
template<typename GC>
Result<Value<GC>, Error<GC>> bevel_filter_method(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::span<Value<GC>>& args,
    uint16_t index
);

// Template method implementations

template<typename GC>
Result<BevelFilter<GC>, Error<GC>> BevelFilter<GC>::new_(
    Activation<GC>* activation,
    const std::span<Value<GC>>& args
) {
    auto filter = BevelFilter<GC>(
        Gc<GC, BevelFilterData<GC>>::create(activation->gc())
    );

    filter.set_distance(activation, args.size() > 0 ? std::optional(args[0]) : std::nullopt);
    filter.set_angle(activation, args.size() > 1 ? std::optional(args[1]) : std::nullopt);
    filter.set_highlight_color(activation, args.size() > 2 ? std::optional(args[2]) : std::nullopt);
    filter.set_highlight_alpha(activation, args.size() > 3 ? std::optional(args[3]) : std::nullopt);
    filter.set_shadow_color(activation, args.size() > 4 ? std::optional(args[4]) : std::nullopt);
    filter.set_shadow_alpha(activation, args.size() > 5 ? std::optional(args[5]) : std::nullopt);
    filter.set_blur_x(activation, args.size() > 6 ? std::optional(args[6]) : std::nullopt);
    filter.set_blur_y(activation, args.size() > 7 ? std::optional(args[7]) : std::nullopt);
    filter.set_strength(activation, args.size() > 8 ? std::optional(args[8]) : std::nullopt);
    filter.set_quality(activation, args.size() > 9 ? std::optional(args[9]) : std::nullopt);
    filter.set_type(activation, args.size() > 10 ? std::optional(args[10]) : std::nullopt);
    filter.set_knockout(activation, args.size() > 11 ? std::optional(args[11]) : std::nullopt);

    return Ok(filter);
}

template<typename GC>
BevelFilter<GC> BevelFilter<GC>::from_filter(
    Mutation<GC>* gc_context,
    const swf::BevelFilter& filter
) {
    auto data = std::make_unique<BevelFilterData<GC>>();
    data->distance = filter.distance.to_f64();
    data->angle = filter.angle.to_f64();
    data->highlight = filter.highlight_color;
    data->shadow = filter.shadow_color;
    data->quality = static_cast<int32_t>(filter.num_passes());
    data->strength = static_cast<uint16_t>(filter.strength.to_f64() * 256.0);
    data->knockout = filter.is_knockout();
    data->blur_x = filter.blur_x.to_f64();
    data->blur_y = filter.blur_y.to_f64();
    data->type_ = bevel_type_from_flags(filter.flags);

    return BevelFilter<GC>(
        Gc<GC, BevelFilterData<GC>>::create(gc_context, std::move(data))
    );
}

template<typename GC>
BevelFilter<GC> BevelFilter<GC>::duplicate(Mutation<GC>* gc_context) const {
    auto data = std::make_unique<BevelFilterData<GC>>(*data_);
    return BevelFilter<GC>(
        Gc<GC, BevelFilterData<GC>>::create(gc_context, std::move(data))
    );
}

template<typename GC>
void BevelFilter<GC>::set_distance(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        auto result = value->coerce_to_f64(activation);
        if (result.is_ok()) {
            data_->distance = result.unwrap();
        }
    }
}

template<typename GC>
void BevelFilter<GC>::set_angle(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        auto result = value->coerce_to_f64(activation);
        if (result.is_ok()) {
            double angle = std::fmod(result.unwrap(), 360.0);
            data_->angle = angle * M_PI / 180.0;
        }
    }
}

template<typename GC>
void BevelFilter<GC>::set_highlight_color(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        auto result = value->coerce_to_u32(activation);
        if (result.is_ok()) {
            auto color = data_->highlight;
            color.rgb = result.unwrap();
            data_->highlight = color;
        }
    }
}

template<typename GC>
void BevelFilter<GC>::set_highlight_alpha(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        auto result = value->coerce_to_f64(activation);
        if (result.is_ok()) {
            auto color = data_->highlight;
            color.a = static_cast<uint8_t>(result.unwrap() * 255.0);
            data_->highlight = color;
        }
    }
}

template<typename GC>
void BevelFilter<GC>::set_shadow_color(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        auto result = value->coerce_to_u32(activation);
        if (result.is_ok()) {
            auto color = data_->shadow;
            color.rgb = result.unwrap();
            data_->shadow = color;
        }
    }
}

template<typename GC>
void BevelFilter<GC>::set_shadow_alpha(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        auto result = value->coerce_to_f64(activation);
        if (result.is_ok()) {
            auto color = data_->shadow;
            color.a = static_cast<uint8_t>(result.unwrap() * 255.0);
            data_->shadow = color;
        }
    }
}

template<typename GC>
void BevelFilter<GC>::set_quality(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        auto result = value->coerce_to_i32(activation);
        if (result.is_ok()) {
            data_->quality = std::clamp(result.unwrap(), 0, 15);
        }
    }
}

template<typename GC>
void BevelFilter<GC>::set_strength(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        auto result = value->coerce_to_f64(activation);
        if (result.is_ok()) {
            data_->set_strength(result.unwrap());
        }
    }
}

template<typename GC>
void BevelFilter<GC>::set_knockout(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        data_->knockout = value->as_bool(activation->swf_version());
    }
}

template<typename GC>
void BevelFilter<GC>::set_blur_x(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        auto result = value->coerce_to_f64(activation);
        if (result.is_ok()) {
            data_->blur_x = std::clamp(result.unwrap(), 0.0, 255.0);
        }
    }
}

template<typename GC>
void BevelFilter<GC>::set_blur_y(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        auto result = value->coerce_to_f64(activation);
        if (result.is_ok()) {
            data_->blur_y = std::clamp(result.unwrap(), 0.0, 255.0);
        }
    }
}

template<typename GC>
void BevelFilter<GC>::set_type(
    Activation<GC>* activation,
    const std::optional<Value<GC>>& value
) {
    if (value.has_value()) {
        auto type_str = value->coerce_to_string(activation);
        if (type_str.is_ok()) {
            auto& type = type_str.unwrap();
            if (type == u"inner") {
                data_->type_ = BevelFilterType::Inner;
            } else if (type == u"outer") {
                data_->type_ = BevelFilterType::Outer;
            } else {
                data_->type_ = BevelFilterType::Full;
            }
        }
    }
}

template<typename GC>
swf::BevelFilter BevelFilter<GC>::filter() const {
    swf::BevelFilter result;
    result.shadow_color = data_->shadow;
    result.highlight_color = data_->highlight;
    result.blur_x = swf::Fixed16::from_f64(data_->blur_x);
    result.blur_y = swf::Fixed16::from_f64(data_->blur_y);
    result.angle = swf::Fixed16::from_f64(data_->angle);
    result.distance = swf::Fixed16::from_f64(data_->distance);
    result.strength = swf::Fixed8::from_f64(data_->get_strength());

    result.flags = swf::BevelFilterFlags{};
    result.flags.flags |= swf::BevelFilterFlags::COMPOSITE_SOURCE;
    result.flags.flags |= (static_cast<uint8_t>(data_->quality) & 0x0F) << 4;
    auto type_flags = data_->type_.as_bevel_flags();
    result.flags.flags |= type_flags.flags;
    result.flags.set(swf::BevelFilterFlags::KNOCKOUT, data_->knockout);

    return result;
}

} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_BEVEL_FILTER_H
