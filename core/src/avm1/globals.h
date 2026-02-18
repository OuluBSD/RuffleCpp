// C++ translation of core/src/avm1/globals.rs
//! AVM1 global functions and builtins

#ifndef RUFFLE_CORE_AVM1_GLOBALS_H
#define RUFFLE_CORE_AVM1_GLOBALS_H

#include <cstdint>
#include <vector>
#include <string>
#include <optional>
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
        template<typename GC> class TDisplayObject;
        template<typename GC> class TDisplayObjectContainer;
    }

    namespace avm1 {
        template<typename GC> class Value;
        template<typename GC> class Object;
        template<typename GC> class Activation;
        template<typename GC> class Error;

        namespace property_decl {
            template<typename GC> class DeclContext;
        }

        namespace as_broadcaster {
            template<typename GC>
            struct BroadcasterFunctions;
        }
    }
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {

/// Depths used/returned by ActionScript are offset by this amount from depths used inside the SWF/by the VM.
/// The depth of objects placed on timeline in the Flash IDE start from 0 in the SWF,
/// but are negative when queried from MovieClip.getDepth().
/// Add this to convert from AS -> SWF depth.
inline constexpr int32_t AVM_DEPTH_BIAS = 16384;

/// The maximum depth that the AVM will allow you to swap or attach clips to.
inline constexpr int32_t AVM_MAX_DEPTH = 2'130'706'428;

/// The maximum depth that the AVM will allow you to remove clips from.
inline constexpr int32_t AVM_MAX_REMOVE_DEPTH = 2'130'706'416;

/// This structure represents all system builtins that are used regardless of
/// whatever the hell happens to _global. These are, of course,
/// user-modifiable.
template<typename GC>
struct SystemPrototypes {
    Object<GC> button;
    Object<GC> object;
    Object<GC> object_constructor;
    Object<GC> function;
    Object<GC> movie_clip;
    Object<GC> text_field;
    Object<GC> text_format;
    Object<GC> array;
    Object<GC> array_constructor;
    Object<GC> xml_node_constructor;
    Object<GC> xml_constructor;
    Object<GC> matrix_constructor;
    Object<GC> point_constructor;
    Object<GC> rectangle;
    Object<GC> rectangle_constructor;
    Object<GC> transform_constructor;
    Object<GC> shared_object_constructor;
    Object<GC> color_transform_constructor;
    Object<GC> context_menu_constructor;
    Object<GC> context_menu_item_constructor;
    Object<GC> date_constructor;
    Object<GC> bitmap_data;
    Object<GC> video;
    Object<GC> blur_filter;
    Object<GC> bevel_filter;
    Object<GC> glow_filter;
    Object<GC> drop_shadow_filter;
    Object<GC> color_matrix_filter;
    Object<GC> displacement_map_filter;
    Object<GC> convolution_filter;
    Object<GC> gradient_bevel_filter;
    Object<GC> gradient_glow_filter;
};

/// Global function implementations

/// Escape a string for URL encoding
template<typename GC>
Value<GC> escape(Activation<GC>* activation, const std::vector<Value<GC>>& args);

/// Unescape a URL-encoded string
template<typename GC>
Value<GC> unescape(Activation<GC>* activation, const std::vector<Value<GC>>& args);

/// Parse an integer from a string
template<typename GC>
Value<GC> parse_int(Activation<GC>* activation, const std::vector<Value<GC>>& args);

/// Parse a float from a string
template<typename GC>
Value<GC> parse_float(Activation<GC>* activation, const std::vector<Value<GC>>& args);

/// Trace output to console
template<typename GC>
Value<GC> trace(Activation<GC>* activation, const std::vector<Value<GC>>& args);

/// Check if a value is finite
template<typename GC>
Value<GC> is_finite(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Check if a value is NaN
template<typename GC>
Value<GC> is_nan(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Get Infinity value
template<typename GC>
Value<GC> get_infinity(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Get NaN value
template<typename GC>
Value<GC> get_nan(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Set interval timer
template<typename GC>
Value<GC> set_interval(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Set timeout timer
template<typename GC>
Value<GC> set_timeout(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Clear interval/timeout timer
template<typename GC>
Value<GC> clear_interval(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Request render after event
template<typename GC>
Value<GC> update_after_event(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Get depth of display object
template<typename GC>
Value<GC> get_depth(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Remove display object from parent
template<typename GC>
void remove_display_object(
    display_object::DisplayObject<GC> this_,
    Activation<GC>* activation
);

/// Internal helper for parse_int
template<typename GC>
Value<GC> parse_int_internal(
    Activation<GC>* activation,
    const Value<GC>* string,
    std::optional<const Value<GC>*> radix
);

/// Create timer helper for setInterval/setTimeout
template<typename GC>
Value<GC> create_timer(
    Activation<GC>* activation,
    Object<GC> this_,
    const std::vector<Value<GC>>& args,
    bool is_timeout
);

/// Initialize default global scope and builtins for an AVM1 instance.
template<typename GC>
std::tuple<SystemPrototypes<GC>, Object<GC>, as_broadcaster::BroadcasterFunctions<GC>>
create_globals(StringContext<GC>* context);

// Inline implementations

template<typename GC>
Value<GC> trace(Activation<GC>* activation, const std::vector<Value<GC>>& args) {
    // Unlike Action::Trace, _global.trace always coerces
    // undefined to "" in SWF6 and below.
    auto out = args.empty() 
        ? AvmString<GC>()
        : args[0].coerce_to_string(activation);
    
    activation->context().avm_trace(out.to_utf8_lossy());
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> is_finite(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    if (!args.empty()) {
        return Value<GC>::boolean(std::isfinite(args[0].coerce_to_f64(activation)));
    }
    return Value<GC>::boolean(false);
}

template<typename GC>
Value<GC> is_nan(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    if (!args.empty()) {
        return Value<GC>::boolean(std::isnan(args[0].coerce_to_f64(activation)));
    }
    return Value<GC>::boolean(true);
}

template<typename GC>
Value<GC> get_infinity(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    if (activation->swf_version() > 4) {
        return Value<GC>::number(std::numeric_limits<double>::infinity());
    }
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_nan(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    if (activation->swf_version() > 4) {
        return Value<GC>::number(std::numeric_limits<double>::quiet_NaN());
    }
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> parse_int(Activation<GC>* activation, const std::vector<Value<GC>>& args) {
    // ECMA-262 violation: parseInt() == undefined // not NaN
    if (args.empty()) {
        return Value<GC>::undefined();
    }
    
    std::optional<const Value<GC>*> radix = args.size() > 1 ? &args[1] : std::nullopt;
    return parse_int_internal(activation, &args[0], radix);
}

template<typename GC>
Value<GC> parse_float(Activation<GC>* activation, const std::vector<Value<GC>>& args) {
    if (!args.empty()) {
        auto string = args[0].coerce_to_string(activation);
        return Value<GC>::number(ruffle::core::avm1::value::parse_float_impl(&string, false));
    }
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> escape(Activation<GC>* activation, const std::vector<Value<GC>>& args) {
    if (args.empty()) {
        return Value<GC>::undefined();
    }
    
    auto s = args[0].coerce_to_string(activation);
    std::vector<uint8_t> buffer;
    
    // TODO: unpaired surrogates will be lost; this is incorrect
    for (char c : s.to_utf8_lossy()) {
        if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            buffer.push_back(static_cast<uint8_t>(c));
        } else {
            // ECMA-262 violation: Avm1 does not support unicode escapes
            static const uint8_t DIGITS[] = "0123456789ABCDEF";
            buffer.push_back('%');
            buffer.push_back(DIGITS[(c / 16) & 0xF]);
            buffer.push_back(DIGITS[c % 16]);
        }
    }
    
    return Value<GC>::string(AvmString<GC>::new_(activation->gc(), WString<GC>::from_buf(buffer)));
}

template<typename GC>
Value<GC> unescape(Activation<GC>* activation, const std::vector<Value<GC>>& args) {
    if (args.empty()) {
        return Value<GC>::undefined();
    }
    
    auto s = args[0].coerce_to_string(activation);
    auto s_utf8 = s.to_utf8_lossy();
    std::string out_bytes;
    out_bytes.reserve(s_utf8.size());

    int remain = 0;
    std::string hex_chars;
    hex_chars.reserve(2);

    // TODO: unpaired surrogates will be lost; this is incorrect
    for (char c : s_utf8) {
        if (c == '%') {
            remain = 2;
        } else if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
            if (remain > 0) {
                remain--;
                hex_chars.push_back(c);
                
                if (remain == 0) {
                    uint8_t b = static_cast<uint8_t>(std::stoul(hex_chars, nullptr, 16));
                    out_bytes.push_back(b);
                    hex_chars.clear();
                }
            } else {
                out_bytes.push_back(c);
            }
        } else if (remain > 0) {
            remain = 0;
            hex_chars.clear();
            out_bytes.push_back(c);
        } else if (c == '+') {
            out_bytes.push_back(' ');
        } else {
            out_bytes.push_back(c);
        }
    }
    
    return Value<GC>::string(AvmString<GC>::new_utf8(activation->gc(), out_bytes));
}

template<typename GC>
Value<GC> set_interval(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    return create_timer(activation, this_, args, false);
}

template<typename GC>
Value<GC> set_timeout(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    return create_timer(activation, this_, args, true);
}

template<typename GC>
Value<GC> clear_interval(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    int32_t id = args.empty() ? 0 : args[0].coerce_to_i32(activation);
    
    if (!activation->context().timers.remove(id)) {
        // Timer does not exist
    }
    
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> update_after_event(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    *activation->context().needs_render = true;
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_depth(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    if (auto display_object = this_->as_display_object()) {
        int32_t depth = display_object.depth() - AVM_DEPTH_BIAS;
        return Value<GC>::number(depth);
    }
    return Value<GC>::undefined();
}

template<typename GC>
void remove_display_object(display_object::DisplayObject<GC> this_, Activation<GC>* activation) {
    int32_t depth = this_.depth() - 0;
    
    // Can only remove positive depths (when offset by the AVM depth bias).
    if (depth >= AVM_DEPTH_BIAS && depth < AVM_MAX_REMOVE_DEPTH && !this_.avm1_removed()) {
        // Need a parent to remove from.
        if (auto parent = this_.avm1_parent().and_then([](auto o) { return o.as_movie_clip(); })) {
            parent->remove_child(activation->context(), this_);
        }
    }
}

// Method indices for global functions
namespace method {
    inline constexpr uint16_t ESCAPE = 0;
    inline constexpr uint16_t UNESCAPE = 1;
    inline constexpr uint16_t PARSE_INT = 2;
    inline constexpr uint16_t PARSE_FLOAT = 3;
    inline constexpr uint16_t TRACE = 4;
}

/// Dispatch method by index
template<typename GC>
Value<GC> method(
    Activation<GC>* activation,
    Object<GC> this_,
    const std::vector<Value<GC>>& args,
    uint16_t index
) {
    switch (index) {
        case method::ESCAPE:
            return escape(activation, args);
        case method::UNESCAPE:
            return unescape(activation, args);
        case method::PARSE_INT:
            return parse_int(activation, args);
        case method::PARSE_FLOAT:
            return parse_float(activation, args);
        case method::TRACE:
            return trace(activation, args);
        default:
            return Value<GC>::undefined();
    }
}

} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_H
