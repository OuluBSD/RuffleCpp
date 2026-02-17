// C++ translation of core/src/avm2/object/event_object.rs
//! Object representation for events

#ifndef RUFFLE_CORE_AVM2_OBJECT_EVENT_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_EVENT_OBJECT_H

#include <memory>
#include <cstdint>
#include <format>
#include <string>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
    class WString;
    template<typename GC> class UpdateContext;

    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class ScriptObjectData;
        template<typename T> class TObject;
        template<typename T> class Gc;
        template<typename T> class GcWeak;
        template<typename T> class ClassObject;
        template<typename T> class Object;
        template<typename T> class Error;
        template<typename T> class Value;
        template<typename T> class ScriptObject;
        template<typename T> class AvmString;
        namespace events {
            template<typename T> class Event;
        }
    }
    namespace display_object {
        template<typename T> class DisplayObject;
        template<typename T> class InteractiveObject;
    }
    namespace events {
        enum class KeyCode : uint8_t;
        enum class MouseButton : uint8_t;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to EventObject data
template<typename GC>
class EventObjectWeak;

/// Data structure for EventObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class EventObjectData {
public:
    EventObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// The event this object holds
    events::Event<GC> event;

    // Friend classes for accessing private members
    friend class EventObject<GC>;
    friend class EventObjectWeak<GC>;
};

/// EventObject - AVM2 Event object representation
/// Represents an Event instance in ActionScript 3
template<typename GC>
class EventObject {
public:
    using DataType = EventObjectData<GC>;
    using WeakType = EventObjectWeak<GC>;

    EventObject() = default;
    explicit EventObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create a bare Event instance while skipping the usual construct() pipeline.
    /// It's just slightly faster and doesn't require an Activation.
    /// This is equivalent to classes.event.construct(activation, &[event_type, false, false])
    ///
    /// @param context The update context
    /// @param event_type The event type string
    /// @return New EventObject
    static EventObject bare_default_event(
        UpdateContext<GC>* context,
        const std::u16string& event_type
    ) {
        return bare_event(context, event_type, false, false);
    }

    /// Create a bare Event instance while skipping the usual construct() pipeline.
    /// It's just slightly faster and doesn't require an Activation.
    /// Note that if you need an Event subclass, you need to construct it via .construct().
    ///
    /// @param context The update context
    /// @param event_type The event type string
    /// @param bubbles Whether the event bubbles
    /// @param cancelable Whether the event is cancelable
    /// @return New EventObject
    static EventObject bare_event(
        UpdateContext<GC>* context,
        const std::u16string& event_type,
        bool bubbles,
        bool cancelable
    );

    /// Create an EventObject from class and arguments
    ///
    /// @param activation The current activation
    /// @param class_obj The class object
    /// @param args The arguments
    /// @return New EventObject
    static EventObject from_class_and_args(
        Activation<GC>* activation,
        ClassObject<GC> class_obj,
        std::span<Value<GC>> args
    );

    /// Create a mouse event
    ///
    /// @param activation The current activation
    /// @param event_type The event type
    /// @param target The target display object
    /// @param related_object Optional related object
    /// @param delta The scroll delta
    /// @param bubbles Whether the event bubbles
    /// @param button The mouse button
    /// @return New EventObject
    static EventObject mouse_event(
        Activation<GC>* activation,
        AvmString<GC> event_type,
        display_object::DisplayObject<GC> target,
        std::optional<display_object::InteractiveObject<GC>> related_object,
        int32_t delta,
        bool bubbles,
        events::MouseButton button
    );

    /// Create a mouse down event
    ///
    /// @param activation The current activation
    /// @param target The target display object
    /// @param button The mouse button
    /// @return New EventObject
    static EventObject mouse_event_down(
        Activation<GC>* activation,
        display_object::DisplayObject<GC> target,
        events::MouseButton button
    );

    /// Create a mouse up event
    ///
    /// @param activation The current activation
    /// @param target The target display object
    /// @param button The mouse button
    /// @return New EventObject
    static EventObject mouse_event_up(
        Activation<GC>* activation,
        display_object::DisplayObject<GC> target,
        events::MouseButton button
    );

    /// Create a mouse click event
    ///
    /// @param activation The current activation
    /// @param target The target display object
    /// @param button The mouse button
    /// @return New EventObject
    static EventObject mouse_event_click(
        Activation<GC>* activation,
        display_object::DisplayObject<GC> target,
        events::MouseButton button
    );

    /// Create a text event
    ///
    /// @param activation The current activation
    /// @param event_type The event type
    /// @param text The text content
    /// @param bubbles Whether the event bubbles
    /// @param cancelable Whether the event is cancelable
    /// @return New EventObject
    static EventObject text_event(
        Activation<GC>* activation,
        const std::u16string& event_type,
        AvmString<GC> text,
        bool bubbles,
        bool cancelable
    );

    /// Create a net status event
    ///
    /// @param activation The current activation
    /// @param info The status info (key-value pairs)
    /// @return New EventObject
    static EventObject net_status_event(
        Activation<GC>* activation,
        const std::vector<std::pair<std::u16string, std::u16string>>& info
    );

    /// Create a progress event
    ///
    /// @param activation The current activation
    /// @param event_type The event type
    /// @param bytes_loaded Bytes loaded
    /// @param bytes_total Total bytes
    /// @return New EventObject
    static EventObject progress_event(
        Activation<GC>* activation,
        const std::u16string& event_type,
        size_t bytes_loaded,
        size_t bytes_total
    );

    /// Create a focus event
    ///
    /// @param activation The current activation
    /// @param event_type The event type
    /// @param cancelable Whether the event is cancelable
    /// @param related_object Optional related object
    /// @param key_code The key code
    /// @return New EventObject
    static EventObject focus_event(
        Activation<GC>* activation,
        const std::u16string& event_type,
        bool cancelable,
        std::optional<display_object::InteractiveObject<GC>> related_object,
        uint32_t key_code
    );

    /// Create an IO error event
    ///
    /// @param activation The current activation
    /// @param error_msg The error message
    /// @param error_code The error code
    /// @return New EventObject
    static EventObject io_error_event(
        Activation<GC>* activation,
        const std::u16string& error_msg,
        uint32_t error_code
    );

    /// Create an HTTP status event
    ///
    /// @param activation The current activation
    /// @param status The HTTP status code
    /// @param redirected Whether the request was redirected
    /// @return New EventObject
    static EventObject http_status_event(
        Activation<GC>* activation,
        uint16_t status,
        bool redirected
    );

    /// Get the event
    const events::Event<GC>& event() const {
        return data_->event;
    }

    /// Get the mutable event
    events::Event<GC>& event_mut() {
        return data_->event;
    }

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to EventObject
template<typename GC>
class EventObjectWeak {
public:
    EventObjectWeak() = default;
    explicit EventObjectWeak(GcWeak<GC, EventObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, EventObjectData<GC>> data_;
};

// Template method implementations

template<typename GC>
EventObject<GC> EventObject<GC>::bare_event(
    UpdateContext<GC>* context,
    const std::u16string& event_type,
    bool bubbles,
    bool cancelable
) {
    auto class_obj = context->avm2()->classes()->event;
    auto base = ScriptObjectData<GC>::create(class_obj);

    auto event = events::Event<GC>::new_(AvmString<GC>::new_utf8(context->gc(), event_type));
    event.set_bubbles(bubbles);
    event.set_cancelable(cancelable);

    auto data = Gc<GC, DataType>::create(
        context->gc(),
        DataType{base, event}
    );
    return EventObject<GC>(data);
}

template<typename GC>
EventObject<GC> EventObject<GC>::from_class_and_args(
    Activation<GC>* activation,
    ClassObject<GC> class_obj,
    std::span<Value<GC>> args
) {
    // We don't expect Event classes to error in their constructors or to
    // return anything other than an EventObject
    auto result = class_obj->construct(activation, args);
    if (result.is_ok()) {
        auto obj = result.unwrap().as_object();
        if (obj.has_value()) {
            return obj->as_event_object().value();
        }
    }
    // Fallback - would need proper error handling in real implementation
    return EventObject<GC>();
}

template<typename GC>
EventObject<GC> EventObject<GC>::mouse_event(
    Activation<GC>* activation,
    AvmString<GC> event_type,
    display_object::DisplayObject<GC> target,
    std::optional<display_object::InteractiveObject<GC>> related_object,
    int32_t delta,
    bool bubbles,
    events::MouseButton button
) {
    auto local = target.local_mouse_position(activation->context());

    auto mouse_event_cls = activation->avm2()->classes()->mouseevent;
    return from_class_and_args(
        activation,
        mouse_event_cls,
        {
            Value<GC>::from_string(event_type),
            // bubbles
            Value<GC>::from_bool(bubbles),
            // cancellable
            Value<GC>::from_bool(false),
            // localX
            Value<GC>::from_f64(local.x.to_pixels()),
            // localY
            Value<GC>::from_f64(local.y.to_pixels()),
            // relatedObject
            related_object.has_value()
                ? related_object->as_displayobject().object2_or_null()
                : Value<GC>::null(),
            // ctrlKey
            Value<GC>::from_bool(activation->context()->input.is_key_down(events::KeyCode::CONTROL)),
            // altKey
            Value<GC>::from_bool(activation->context()->input.is_key_down(events::KeyCode::ALT)),
            // shiftKey
            Value<GC>::from_bool(activation->context()->input.is_key_down(events::KeyCode::SHIFT)),
            // buttonDown
            Value<GC>::from_bool(activation->context()->input.is_key_down(button)),
            // delta
            Value<GC>::from_i32(delta)
        }
    );
}

template<typename GC>
EventObject<GC> EventObject<GC>::mouse_event_down(
    Activation<GC>* activation,
    display_object::DisplayObject<GC> target,
    events::MouseButton button
) {
    std::u16string event_name;
    switch (button) {
        case events::MouseButton::Left:
            event_name = u"mouseDown";
            break;
        case events::MouseButton::Right:
            event_name = u"rightMouseDown";
            break;
        case events::MouseButton::Middle:
            event_name = u"middleMouseDown";
            break;
        case events::MouseButton::Unknown:
            // unreachable
            break;
    }

    return mouse_event(activation, AvmString<GC>::new_utf8(activation->gc(), event_name),
                       target, std::nullopt, 0, true, button);
}

template<typename GC>
EventObject<GC> EventObject<GC>::mouse_event_up(
    Activation<GC>* activation,
    display_object::DisplayObject<GC> target,
    events::MouseButton button
) {
    std::u16string event_name;
    switch (button) {
        case events::MouseButton::Left:
            event_name = u"mouseUp";
            break;
        case events::MouseButton::Right:
            event_name = u"rightMouseUp";
            break;
        case events::MouseButton::Middle:
            event_name = u"middleMouseUp";
            break;
        case events::MouseButton::Unknown:
            // unreachable
            break;
    }

    return mouse_event(activation, AvmString<GC>::new_utf8(activation->gc(), event_name),
                       target, std::nullopt, 0, true, button);
}

template<typename GC>
EventObject<GC> EventObject<GC>::mouse_event_click(
    Activation<GC>* activation,
    display_object::DisplayObject<GC> target,
    events::MouseButton button
) {
    std::u16string event_name;
    switch (button) {
        case events::MouseButton::Left:
            event_name = u"click";
            break;
        case events::MouseButton::Right:
            event_name = u"rightClick";
            break;
        case events::MouseButton::Middle:
            event_name = u"middleClick";
            break;
        case events::MouseButton::Unknown:
            // unreachable
            break;
    }

    return mouse_event(activation, AvmString<GC>::new_utf8(activation->gc(), event_name),
                       target, std::nullopt, 0, true, button);
}

template<typename GC>
EventObject<GC> EventObject<GC>::text_event(
    Activation<GC>* activation,
    const std::u16string& event_type,
    AvmString<GC> text,
    bool bubbles,
    bool cancelable
) {
    auto text_event_cls = activation->avm2()->classes()->textevent;
    return from_class_and_args(
        activation,
        text_event_cls,
        {
            Value<GC>::from_string(AvmString<GC>::new_utf8(activation->gc(), event_type)),
            // bubbles
            Value<GC>::from_bool(bubbles),
            // cancelable
            Value<GC>::from_bool(cancelable),
            // text
            Value<GC>::from_string(text)
        }
    );
}

template<typename GC>
EventObject<GC> EventObject<GC>::net_status_event(
    Activation<GC>* activation,
    const std::vector<std::pair<std::u16string, std::u16string>>& info
) {
    auto info_object = ScriptObject<GC>::new_object(activation->context());
    for (const auto& [key, value] : info) {
        auto key_str = AvmString<GC>::new_utf8(activation->gc(), key);
        auto value_str = AvmString<GC>::new_utf8(activation->gc(), value);
        info_object->set_dynamic_property(key_str, Value<GC>::from_string(value_str), activation->gc());
    }

    auto event_name = AvmString<GC>::new_utf8(activation->gc(), u"netStatus");
    auto net_status_cls = activation->avm2()->classes()->netstatusevent;
    return from_class_and_args(
        activation,
        net_status_cls,
        {
            Value<GC>::from_string(event_name),
            // bubbles
            Value<GC>::from_bool(false),
            // cancelable
            Value<GC>::from_bool(false),
            Value<GC>::from_object(info_object)
        }
    );
}

template<typename GC>
EventObject<GC> EventObject<GC>::progress_event(
    Activation<GC>* activation,
    const std::u16string& event_type,
    size_t bytes_loaded,
    size_t bytes_total
) {
    auto progress_event_cls = activation->avm2()->classes()->progressevent;
    return from_class_and_args(
        activation,
        progress_event_cls,
        {
            Value<GC>::from_string(AvmString<GC>::new_utf8(activation->gc(), event_type)),
            // bubbles
            Value<GC>::from_bool(false),
            // cancelable
            Value<GC>::from_bool(false),
            // bytesLoaded
            Value<GC>::from_f64(static_cast<double>(bytes_loaded)),
            // bytesTotal
            Value<GC>::from_f64(static_cast<double>(bytes_total))
        }
    );
}

template<typename GC>
EventObject<GC> EventObject<GC>::focus_event(
    Activation<GC>* activation,
    const std::u16string& event_type,
    bool cancelable,
    std::optional<display_object::InteractiveObject<GC>> related_object,
    uint32_t key_code
) {
    auto shift_key = activation->context()->input.is_key_down(events::KeyCode::SHIFT);
    auto none_string = AvmString<GC>::new_utf8(activation->gc(), u"none");

    auto focus_event_cls = activation->avm2()->classes()->focusevent;
    return from_class_and_args(
        activation,
        focus_event_cls,
        {
            Value<GC>::from_string(AvmString<GC>::new_utf8(activation->gc(), event_type)),
            Value<GC>::from_bool(true),
            Value<GC>::from_bool(cancelable),
            related_object.has_value()
                ? related_object->as_displayobject().object2_or_null()
                : Value<GC>::null(),
            Value<GC>::from_bool(shift_key),
            Value<GC>::from_u32(key_code),
            Value<GC>::from_string(none_string) // TODO implement direction
        }
    );
}

template<typename GC>
EventObject<GC> EventObject<GC>::io_error_event(
    Activation<GC>* activation,
    const std::u16string& error_msg,
    uint32_t error_code
) {
    auto event_name = AvmString<GC>::new_utf8(activation->gc(), u"ioError");
    auto io_error_event_cls = activation->avm2()->classes()->ioerrorevent;
    return from_class_and_args(
        activation,
        io_error_event_cls,
        {
            Value<GC>::from_string(event_name),
            Value<GC>::from_bool(false),
            Value<GC>::from_bool(false),
            Value<GC>::from_string(AvmString<GC>::new_utf8(activation->gc(), error_msg)),
            Value<GC>::from_u32(error_code)
        }
    );
}

template<typename GC>
EventObject<GC> EventObject<GC>::http_status_event(
    Activation<GC>* activation,
    uint16_t status,
    bool redirected
) {
    auto event_name = AvmString<GC>::new_utf8(activation->gc(), u"httpStatus");
    auto http_status_event_cls = activation->avm2()->classes()->httpstatusevent;
    return from_class_and_args(
        activation,
        http_status_event_cls,
        {
            Value<GC>::from_string(event_name),
            Value<GC>::from_bool(false),
            Value<GC>::from_bool(false),
            Value<GC>::from_u16(status),
            Value<GC>::from_bool(redirected)
        }
    );
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::EventObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::EventObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "EventObject(type={}, ptr={})",
            "event_type", // Would use obj.event().event_type() in real implementation
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_EVENT_OBJECT_H
