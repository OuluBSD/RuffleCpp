// C++ translation of core/src/avm2/events.rs
//! Core event structure

#ifndef RUFFLE_CORE_AVM2_EVENTS_H
#define RUFFLE_CORE_AVM2_EVENTS_H

#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Avm2;
    template<typename T> class Activation;
    template<typename T> class FunctionObject;
    template<typename T> class Object;
    template<typename T> class EventObject;
    template<typename T> class AvmString;
    template<typename T> class Value;
    template<typename T> class Error;
    
    namespace globals {
        namespace slots {
            namespace flash_events_event_dispatcher {
                enum Slot {
                    DISPATCH_LIST,
                    TARGET
                };
            }
        }
    }
}
namespace display_object {
    template<typename T> class DisplayObject;
    template<typename T> class TDisplayObject;
    template<typename T> class MovieClip;
}
template<typename T> class UpdateContext;
}
}

namespace fnv {
    template<typename K, typename V>
    class FnvHashMap : public std::unordered_map<K, V> {};
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Which phase of event dispatch is currently occurring
enum class EventPhase {
    /// The event has yet to be fired on the target and is descending the
    /// ancestors of the event target
    Capturing = 1,

    /// The event is currently firing on the target
    AtTarget = 2,

    /// The event has already fired on the target and is ascending the
    /// ancestors of the event target
    Bubbling = 3
};

/// How this event is allowed to propagate
enum class PropagationMode {
    /// Propagate events normally
    Allow,

    /// Stop capturing or bubbling events
    Stop,

    /// Stop running event handlers altogether
    StopImmediate
};

/// Represents data fields of an event that can be fired on an object that
/// implements IEventDispatcher
template<typename GC>
class Event {
private:
    /// Whether the event "bubbles" - fires on its parents after it
    /// fires on the child
    bool bubbles;

    /// Whether the event has a default response that an event handler
    /// can request to not occur
    bool cancelable;

    /// Whether the event's default response has been cancelled
    bool cancelled;

    /// Whether event propagation has stopped
    PropagationMode propagation;

    /// The object currently having its event handlers invoked
    Object<GC>* current_target;

    /// The current event phase
    EventPhase event_phase;

    /// The object this event was dispatched on
    Object<GC>* target;

    /// The name of the event being triggered
    AvmString<GC> event_type;

public:
    /// Construct a new event of a given type
    explicit Event(AvmString<GC> event_type)
        : bubbles(false)
        , cancelable(false)
        , cancelled(false)
        , propagation(PropagationMode::Allow)
        , current_target(nullptr)
        , event_phase(EventPhase::AtTarget)
        , target(nullptr)
        , event_type(event_type)
    {}

    AvmString<GC> event_type() const { return event_type; }
    void set_event_type(AvmString<GC> type) { event_type = type; }

    bool is_bubbling() const { return bubbles; }
    void set_bubbles(bool bubbling) { bubbles = bubbling; }

    bool is_cancelable() const { return cancelable; }
    void set_cancelable(bool cancelable) { this->cancelable = cancelable; }

    bool is_cancelled() const { return cancelled; }
    void cancel() {
        if (cancelable) {
            cancelled = true;
        }
    }

    bool is_propagation_stopped() const {
        return propagation != PropagationMode::Allow;
    }

    void stop_propagation() {
        if (propagation != PropagationMode::StopImmediate) {
            propagation = PropagationMode::Stop;
        }
    }

    bool is_propagation_stopped_immediately() const {
        return propagation == PropagationMode::StopImmediate;
    }

    void stop_immediate_propagation() {
        propagation = PropagationMode::StopImmediate;
    }

    EventPhase phase() const { return event_phase; }
    void set_phase(EventPhase phase) { event_phase = phase; }

    Object<GC>* target() const { return target; }
    void set_target(Object<GC>* t) { target = t; }

    Object<GC>* current_target() const { return current_target; }
    void set_current_target(Object<GC>* t) { current_target = t; }
};

/// A single instance of an event handler
template<typename GC>
class EventHandler {
private:
    /// The event handler to call
    FunctionObject<GC>* handler;

    /// Indicates if this handler should only be called for capturing events
    /// (when true), or if it should only be called for bubbling and
    /// at-target events (when false)
    bool use_capture;

public:
    EventHandler(FunctionObject<GC>* handler, bool use_capture)
        : handler(handler), use_capture(use_capture) {}

    FunctionObject<GC>* get_handler() const { return handler; }
    bool is_capture() const { return use_capture; }

    bool operator==(const EventHandler<GC>& other) const {
        return use_capture == other.use_capture && handler == other.handler;
    }
};

/// A set of handlers organized by event type, priority, and order added
template<typename GC>
class DispatchList {
private:
    // Map: event_type -> (priority -> list of handlers)
    fnv::FnvHashMap<AvmString<GC>, std::map<int32_t, std::vector<EventHandler<GC>>>> handlers;

public:
    DispatchList() = default;

    /// Get all of the event handlers for a given event type
    const std::map<int32_t, std::vector<EventHandler<GC>>>* get_event(AvmString<GC> event) const {
        auto it = handlers.find(event);
        if (it != handlers.end()) {
            return &it->second;
        }
        return nullptr;
    }

    /// Get all of the event handlers for a given event type, for mutation
    std::map<int32_t, std::vector<EventHandler<GC>>>& get_event_mut(AvmString<GC> event) {
        return handlers[event];
    }

    /// Get a single priority level of event handlers for a given event type
    std::vector<EventHandler<GC>>& get_event_priority_mut(AvmString<GC> event, int32_t priority) {
        return handlers[event][priority];
    }

    /// Add an event handler to this dispatch list
    void add_event_listener(
        AvmString<GC> event,
        int32_t priority,
        FunctionObject<GC>* handler,
        bool use_capture
    ) {
        EventHandler<GC> new_handler(handler, use_capture);

        // Check if handler already exists at any priority
        auto it = handlers.find(event);
        if (it != handlers.end()) {
            for (const auto& [prio, handler_list] : it->second) {
                for (const auto& h : handler_list) {
                    if (h == new_handler) {
                        return; // Already exists
                    }
                }
            }
        }

        get_event_priority_mut(event, priority).push_back(new_handler);
    }

    /// Remove an event handler from this dispatch list
    void remove_event_listener(
        AvmString<GC> event,
        FunctionObject<GC>* handler,
        bool use_capture
    ) {
        EventHandler<GC> old_handler(handler, use_capture);

        auto it = handlers.find(event);
        if (it != handlers.end()) {
            for (auto& [prio, handler_list] : it->second) {
                auto pos = std::find(handler_list.begin(), handler_list.end(), old_handler);
                if (pos != handler_list.end()) {
                    handler_list.erase(pos);
                }
            }
        }
    }

    /// Determine if there are any event listeners in this dispatch list
    bool has_event_listener(AvmString<GC> event) const {
        auto it = handlers.find(event);
        if (it != handlers.end()) {
            for (const auto& [prio, handler_list] : it->second) {
                if (!handler_list.empty()) {
                    return true;
                }
            }
        }
        return false;
    }

    /// Yield the event handlers on this dispatch list for a given event
    /// Event handlers will be yielded in the order they are intended to be executed
    std::vector<FunctionObject<GC>*> iter_event_handlers(AvmString<GC> event, bool use_capture) {
        std::vector<FunctionObject<GC>*> result;
        auto& event_map = get_event_mut(event);
        
        // Iterate in reverse priority order (highest priority first)
        for (auto it = event_map.rbegin(); it != event_map.rend(); ++it) {
            for (const auto& eh : it->second) {
                if (eh.is_capture() == use_capture) {
                    result.push_back(eh.get_handler());
                }
            }
        }
        
        return result;
    }
};

/// Retrieve the parent of a given EventDispatcher
/// EventDispatcher does not provide a generic way for its subclasses to
/// indicate ancestry. Instead, only specific event targets provide a hierarchy
/// to traverse.
template<typename GC>
Object<GC>* parent_of(Object<GC>* target);

/// Call all of the event handlers on a given target
/// The target is the current target of the event
template<typename GC>
void dispatch_event_to_target(
    Activation<GC>* activation,
    Object<GC>* dispatcher,
    Object<GC>* real_target,
    Object<GC>* current_target,
    EventObject<GC>* event,
    bool simulate_dispatch
);

/// Dispatch an event to a target and its ancestors
/// Returns true if the event was handled
template<typename GC>
bool dispatch_event(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    EventObject<GC>* event,
    bool simulate_dispatch
);

/// Like dispatch_event, but does not run the Capturing and Bubbling phases
/// and dispatches the event regardless of whether propagation has been stopped
template<typename GC>
void broadcast_event(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    EventObject<GC>* event
);

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_EVENTS_H
