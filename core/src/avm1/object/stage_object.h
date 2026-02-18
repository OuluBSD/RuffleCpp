// C++ translation of core/src/avm1/object/stage_object.rs
//! DisplayObject-specific AVM1 operations.

#ifndef RUFFLE_CORE_AVM1_OBJECT_STAGE_OBJECT_H
#define RUFFLE_CORE_AVM1_OBJECT_STAGE_OBJECT_H

#include <cstdint>
#include <optional>
#include <vector>
#include <string>
#include <functional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC> class AvmString;
    template<typename GC> class WStr;
    template<typename GC> class WString;
    template<typename GC> class Mutation;
    template<typename GC> class UpdateContext;
    template<typename GC> class StringContext;

    namespace display_object {
        template<typename GC> class DisplayObject;
        template<typename GC> class MovieClip;
        template<typename GC> class TDisplayObject;
        template<typename GC> class TDisplayObjectContainer;
        template<typename GC> class TInteractiveObject;
    }

    namespace avm1 {
        template<typename GC> class Value;
        template<typename GC> class Activation;
        template<typename GC> class Error;
        template<typename GC> class PropertyMap;
    }

    namespace types {
        template<typename GC> class Percent;
    }
}
}

namespace swf {
    template<typename GC>
    struct Twips;
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace stage_object {

/// Get a property from a display object.
/// 
/// Property search order for DisplayObjects:
/// 1) Path properties such as _root, _parent, _levelN (obeys case sensitivity)
/// 2) Child display objects with the given instance name
/// 3) Display object properties such as _x, _y (never case sensitive)
template<typename GC>
std::optional<Value<GC>> get_property(
    display_object::DisplayObject<GC> dobj,
    AvmString<GC> name,
    Activation<GC>* activation
);

/// Notify any bound text fields of a property change.
///
/// This should be called every time a property is set on a AVM1 object.
template<typename GC>
Result<void, Error<GC>> notify_property_change(
    display_object::DisplayObject<GC> dobj,
    AvmString<GC> property_name,
    Value<GC> value,
    Activation<GC>* activation
);

/// Check if a display object has a property.
template<typename GC>
bool has_display_object_property(
    display_object::DisplayObject<GC> dobj,
    Activation<GC>* activation,
    AvmString<GC> name
);

/// Enumerate keys of a display object.
///
/// Keys from the underlying object are listed first, followed by
/// child display objects in order from highest depth to lowest depth.
template<typename GC>
void enumerate_keys(
    display_object::DisplayObject<GC> dobj,
    std::vector<AvmString<GC>>& keys
);

/// Properties shared by display objects in AVM1, such as _x and _y.
/// These are only accessible for movie clips, buttons, text fields, and videos.
/// These exist outside the global or prototype machinery. Instead, they are
/// "special" properties stored in a separate map that display objects look at in addition
/// to normal property lookup.
template<typename GC>
class DisplayProperty {
private:
    using DisplayGetter = std::function<Value<GC>(Activation<GC>*, display_object::DisplayObject<GC>)>;
    using DisplaySetter = std::function<Result<void, Error<GC>>(Activation<GC>*, display_object::DisplayObject<GC>, Value<GC>)>;

    DisplayGetter get_;
    std::optional<DisplaySetter> set_;

public:
    DisplayProperty(DisplayGetter get, std::optional<DisplaySetter> set)
        : get_(std::move(get)), set_(std::move(set)) {}

    Value<GC> get(Activation<GC>* activation, display_object::DisplayObject<GC> this_) const {
        return get_(activation, this_);
    }

    Result<void, Error<GC>> set(Activation<GC>* activation, display_object::DisplayObject<GC> this_, Value<GC> value) const {
        if (set_.has_value()) {
            return set_.value()(activation, this_, value);
        }
        return {};
    }

    bool is_read_only() const {
        return !set_.has_value();
    }
};

/// The map from key/index to function pointers for special display object properties.
template<typename GC>
class DisplayPropertyMap {
private:
    PropertyMap<GC, DisplayProperty<GC>> property_map_;

public:
    /// Creates the display property map.
    static DisplayPropertyMap<GC> new_(StringContext<GC>* context);

    /// Gets a property slot by name.
    /// Used by GetMember, GetVariable, SetMember, and SetVariable.
    std::optional<DisplayProperty<GC>> get_by_name(AvmString<GC> name) const {
        // Display object properties are case insensitive, regardless of SWF version!?
        return property_map_.get(name, false);
    }

    /// Gets a property slot by SWF4 index.
    /// The order is defined by the SWF specs.
    /// Used by GetProperty/SetProperty.
    /// SWF19 pp. 85-86
    std::optional<DisplayProperty<GC>> get_by_index(size_t index) const {
        return property_map_.get_index(index);
    }

private:
    void add_property(AvmString<GC> name, DisplayProperty<GC> prop) {
        property_map_.insert(name, prop, false);
    }
};

// Property getter/setter function types
template<typename GC>
using DisplayGetter = Value<GC> (*)(Activation<GC>*, display_object::DisplayObject<GC>);

template<typename GC>
using DisplaySetter = Result<void, Error<GC>> (*)(Activation<GC>*, display_object::DisplayObject<GC>, Value<GC>);

// Property getter implementations
template<typename GC>
Value<GC> x_getter(Activation<GC>* activation, display_object::DisplayObject<GC> this_) {
    return Value<GC>::number(this_->x().to_pixels());
}

template<typename GC>
Result<void, Error<GC>> x_setter(Activation<GC>* activation, display_object::DisplayObject<GC> this_, Value<GC> val) {
    if (auto x = property_coerce_to_number(activation, val)) {
        this_->set_x(Twips<GC>::from_pixels(x.value()));
    }
    return {};
}

template<typename GC>
Value<GC> y_getter(Activation<GC>* activation, display_object::DisplayObject<GC> this_) {
    return Value<GC>::number(this_->y().to_pixels());
}

template<typename GC>
Result<void, Error<GC>> y_setter(Activation<GC>* activation, display_object::DisplayObject<GC> this_, Value<GC> val) {
    if (auto y = property_coerce_to_number(activation, val)) {
        this_->set_y(Twips<GC>::from_pixels(y.value()));
    }
    return {};
}

template<typename GC>
Value<GC> x_scale_getter(Activation<GC>* activation, display_object::DisplayObject<GC> this_) {
    return Value<GC>::number(this_->scale_x().percent());
}

template<typename GC>
Result<void, Error<GC>> x_scale_setter(Activation<GC>* activation, display_object::DisplayObject<GC> this_, Value<GC> val) {
    if (auto val_num = property_coerce_to_number(activation, val)) {
        this_->set_scale_x(Percent<GC>::from(val_num.value()));
    }
    return {};
}

template<typename GC>
Value<GC> y_scale_getter(Activation<GC>* activation, display_object::DisplayObject<GC> this_) {
    return Value<GC>::number(this_->scale_y().percent());
}

template<typename GC>
Result<void, Error<GC>> y_scale_setter(Activation<GC>* activation, display_object::DisplayObject<GC> this_, Value<GC> val) {
    if (auto val_num = property_coerce_to_number(activation, val)) {
        this_->set_scale_y(Percent<GC>::from(val_num.value()));
    }
    return {};
}

template<typename GC>
Value<GC> current_frame_getter(Activation<GC>* activation, display_object::DisplayObject<GC> this_) {
    if (auto mc = this_->as_movie_clip()) {
        return Value<GC>::integer(mc->current_frame());
    }
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> total_frames_getter(Activation<GC>* activation, display_object::DisplayObject<GC> this_) {
    if (auto mc = this_->as_movie_clip()) {
        return Value<GC>::integer(mc->header_frames());
    }
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> alpha_getter(Activation<GC>* activation, display_object::DisplayObject<GC> this_) {
    return Value<GC>::number(this_->alpha() * 100.0);
}

template<typename GC>
Result<void, Error<GC>> alpha_setter(Activation<GC>* activation, display_object::DisplayObject<GC> this_, Value<GC> val) {
    if (auto val_num = property_coerce_to_number(activation, val)) {
        this_->set_alpha(val_num.value() / 100.0);
    }
    return {};
}

template<typename GC>
Value<GC> visible_getter(Activation<GC>* activation, display_object::DisplayObject<GC> this_) {
    return Value<GC>::boolean(this_->visible());
}

template<typename GC>
Result<void, Error<GC>> visible_setter(Activation<GC>* activation, display_object::DisplayObject<GC> this_, Value<GC> val) {
    // Because this property dates to the era of Flash 4, this is actually coerced to an integer.
    // _visible = "false"; coerces to NaN and has no effect.
    if (auto n = property_coerce_to_number(activation, val)) {
        this_->set_visible(activation->context(), n.value() != 0.0);
    }
    return {};
}

template<typename GC>
Value<GC> width_getter(Activation<GC>* activation, display_object::DisplayObject<GC> this_) {
    return Value<GC>::number(this_->width());
}

template<typename GC>
Result<void, Error<GC>> width_setter(Activation<GC>* activation, display_object::DisplayObject<GC> this_, Value<GC> val) {
    if (auto val_num = property_coerce_to_number(activation, val)) {
        this_->set_width(activation->context(), val_num.value());
    }
    return {};
}

template<typename GC>
Value<GC> height_getter(Activation<GC>* activation, display_object::DisplayObject<GC> this_) {
    return Value<GC>::number(this_->height());
}

template<typename GC>
Result<void, Error<GC>> height_setter(Activation<GC>* activation, display_object::DisplayObject<GC> this_, Value<GC> val) {
    if (auto val_num = property_coerce_to_number(activation, val)) {
        this_->set_height(activation->context(), val_num.value());
    }
    return {};
}

template<typename GC>
Value<GC> rotation_getter(Activation<GC>* activation, display_object::DisplayObject<GC> this_) {
    double degrees = static_cast<double>(this_->rotation());
    return Value<GC>::number(degrees);
}

template<typename GC>
Result<void, Error<GC>> rotation_setter(Activation<GC>* activation, display_object::DisplayObject<GC> this_, Value<GC> degrees) {
    if (auto deg = property_coerce_to_number(activation, degrees)) {
        double d = deg.value();
        // Normalize into the range of [-180, 180].
        d = std::fmod(d, 360.0);
        if (d < -180.0) {
            d += 360.0;
        } else if (d > 180.0) {
            d -= 360.0;
        }
        this_->set_rotation(d);
    }
    return {};
}

template<typename GC>
Value<GC> target_getter(Activation<GC>* activation, display_object::DisplayObject<GC> this_) {
    return Value<GC>::string(AvmString<GC>::new_(activation->gc(), this_->slash_path()));
}

template<typename GC>
Value<GC> frames_loaded_getter(Activation<GC>* activation, display_object::DisplayObject<GC> this_) {
    if (auto mc = this_->as_movie_clip()) {
        return Value<GC>::integer(std::min(mc->frames_loaded(), static_cast<int>(mc->header_frames())));
    }
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> name_getter(Activation<GC>* activation, display_object::DisplayObject<GC> this_) {
    return Value<GC>::string(this_->name().value_or(AvmString<GC>()));
}

template<typename GC>
Result<void, Error<GC>> name_setter(Activation<GC>* activation, display_object::DisplayObject<GC> this_, Value<GC> val) {
    auto name = val.coerce_to_string(activation);
    this_->set_name(activation->gc(), name);
    return {};
}

template<typename GC>
Value<GC> drop_target_getter(Activation<GC>* activation, display_object::DisplayObject<GC> this_) {
    if (auto mc = this_->as_movie_clip()) {
        if (auto target = mc->drop_target()) {
            return Value<GC>::string(AvmString<GC>::new_(activation->gc(), target->slash_path()));
        }
    }
    if (activation->swf_version() < 6) {
        return Value<GC>::undefined();
    }
    return Value<GC>::string(AvmString<GC>());
}

template<typename GC>
Value<GC> url_getter(Activation<GC>* activation, display_object::DisplayObject<GC> this_) {
    if (auto mc = this_->as_movie_clip()) {
        return Value<GC>::string(AvmString<GC>::new_utf8(activation->gc(), mc->movie()->url()));
    }
    return Value<GC>::string(AvmString<GC>());
}

// Helper function to coerce property values to numbers
template<typename GC>
std::optional<double> property_coerce_to_number(Activation<GC>* activation, Value<GC> value) {
    if (value != Value<GC>::undefined() && value != Value<GC>::null()) {
        double n = value.coerce_to_f64(activation);
        if (std::isfinite(n)) {
            return n;
        }
    }
    // Invalid value; do not set.
    return std::nullopt;
}

/// Coerces a value according to the property index.
/// Used by SetProperty.
template<typename GC>
Value<GC> action_property_coerce(Activation<GC>* activation, size_t index, Value<GC> value) {
    switch (index) {
        // Coerce to a number. This affects the following properties (including some which have no setter):
        // _x, _y, _xscale, _yscale, _currentframe, _totalframes, _alpha, _visible, _width, _height, _rotation, _framesloaded.
        case 0 ... 10:
        case 12: {
            if (auto val = property_coerce_to_number(activation, value)) {
                return Value<GC>::number(val.value());
            }
            return value;
        }
        // Coerce to a f64. This affects the following properties (including some which have no setter):
        // _highquality, _soundbuftime, _xmouse, _ymouse.
        case 16:
        case 18:
        case 20 ... 21:
            return Value<GC>::number(value.coerce_to_f64(activation));
        // Coerce to a string. This affects the following properties:
        // _name, _quality.
        case 13:
        case 19:
            return Value<GC>::string(value.coerce_to_string(activation));
        // No coercion. This affects the following properties:
        // _target, _droptarget, _url, _focusrect.
        default:
            return value;
    }
}

// Implementation of resolve_path_property helper
template<typename GC>
std::optional<Value<GC>> resolve_path_property(
    display_object::DisplayObject<GC> dobj,
    AvmString<GC> name,
    Activation<GC>* activation
) {
    bool case_sensitive = activation->is_case_sensitive();
    
    if (name->eq_with_case(b"_root", case_sensitive)) {
        return dobj->avm1_root().object1_or_undef();
    } else if (name->eq_with_case(b"_parent", case_sensitive)) {
        if (auto parent = dobj->avm1_parent()) {
            return Value<GC>::object(parent->object1_or_bare(activation->gc()));
        }
        return Value<GC>::undefined();
    } else if (activation->swf_version() > 5 && name->eq_with_case(b"_global", case_sensitive)) {
        // _global is available only in SWF6+
        return activation->global_object().into();
    }

    // Resolve level names _levelN.
    if (auto prefix = name->slice(0, 6)) {
        // _flash is a synonym of _level, a relic from the earliest Flash versions.
        if (prefix->eq_with_case(b"_level", case_sensitive) ||
            prefix->eq_with_case(b"_flash", case_sensitive)) {
            auto level_id = parse_level_id(name->slice(6).value_or(WStr<GC>::from_units(b"")));
            auto level = activation->get_level(level_id)
                .map([](auto o) { return o.object1_or_undef(); })
                .value_or(Value<GC>::undefined());
            return level;
        }
    }

    return std::nullopt;
}

// Helper to parse level ID from string
template<typename GC>
int32_t parse_level_id(const WStr<GC>& digits) {
    // TODO: Use split_first?
    bool is_negative = false;
    WStr<GC> d = digits;
    
    if (auto first = digits.get(0)) {
        if (first == 45) { // '-'
            is_negative = true;
            d = digits.slice(1).value_or(WStr<GC>());
        }
    }
    
    int32_t level_id = 0;
    for (auto c : d.iter()) {
        if (auto digit = static_cast<char32_t>(c.into()).to_digit(10)) {
            level_id = level_id * 10 + static_cast<int32_t>(digit.value());
        }
    }
    
    if (is_negative) {
        level_id = -level_id;
    }
    
    return level_id;
}

} // namespace stage_object
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_OBJECT_STAGE_OBJECT_H
