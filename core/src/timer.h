// C++ translation of core/src/timer.rs
//! Timer handling for setInterval/setTimeout/Timer AVM timers.
//!
//! We tick the timers during our normal frame loop for deterministic operation.
//! The timers are stored in a priority queue, where we check if the nearest timer
//! is ready to tick each frame.

#ifndef RUFFLE_CORE_TIMER_H
#define RUFFLE_CORE_TIMER_H

#include <cstdint>
#include <vector>
#include <queue>
#include <optional>
#include <variant>
#include <functional>

namespace ruffle {
namespace core {

// Forward declarations
namespace avm1 {
    template<typename GC>
    class Object;
    template<typename GC>
    class Value;
    template<typename GC>
    class Activation;
}

namespace avm2 {
    template<typename GC>
    class FunctionObject;
    template<typename GC>
    class Value;
    template<typename GC>
    class Activation;
    template<typename GC>
    class Error;
}

namespace string {
    template<typename GC>
    class AvmString;
}

class DisplayObject;
class UpdateContext;

/// A callback fired by a setInterval/setTimeout timer.
///
/// A callback is either a function object, or a parent object with a method name.
template<typename GC>
struct TimerCallback {
    /// AVM1 function callback
    struct Avm1Function {
        avm1::Object<GC> func;
        std::vector<avm1::Value<GC>> params;
    };

    /// AVM1 method callback (object.method)
    struct Avm1Method {
        avm1::Object<GC> this_obj;
        string::AvmString<GC> method_name;
        std::vector<avm1::Value<GC>> params;
    };

    /// AVM2 callback
    struct Avm2Callback {
        std::optional<avm2::FunctionObject<GC>> closure;
        std::vector<avm2::Value<GC>> params;
    };

    std::variant<Avm1Function, Avm1Method, Avm2Callback> callback;
};

/// A timer created via setInterval/setTimeout.
/// Runs a callback when it ticks.
template<typename GC>
struct Timer {
    /// The ID of the timer.
    int32_t id;

    /// The callback that this timer runs when it fires.
    TimerCallback<GC> callback;

    /// The time when this timer should fire.
    uint64_t tick_time;

    /// The interval between timer ticks, in microseconds.
    uint64_t interval;

    /// This timer only fires once if is_timeout is true.
    bool is_timeout;
};

/// Comparator for Timer min-heap (earliest tick_time first)
template<typename GC>
struct TimerComparator {
    bool operator()(const Timer<GC>& a, const Timer<GC>& b) const {
        return a.tick_time > b.tick_time;  // Reverse for min-heap
    }
};

/// Manages the collection of timers.
template<typename GC>
class Timers {
public:
    /// The minimum interval we allow for timers.
    static constexpr int32_t MIN_INTERVAL = 10;

    /// The maximum timer ticks per call to update_timers, for sanity.
    static constexpr int32_t MAX_TICKS = 10;

    /// The scale of the timers (microseconds).
    static constexpr double TIMER_SCALE = 1000.0;

    /// Creates a new Timers collection.
    Timers() : timer_counter_(0), cur_time_(0) {}

    /// The number of timers currently active.
    size_t num_timers() const {
        return timers_.size();
    }

    /// Whether there are no timers active.
    bool is_empty() const {
        return timers_.empty();
    }

    /// Whether a timer with the given ID exists.
    bool timer_exists(int32_t id) const {
        for (const auto& timer : timers_) {
            if (timer.id == id) {
                return true;
            }
        }
        return false;
    }

    /// Registers a new timer and returns the timer ID.
    ///
    /// \param callback The callback to execute when the timer fires
    /// \param interval The interval in milliseconds
    /// \param is_timeout Whether this is a one-shot timeout
    /// \return The timer ID
    int32_t add_timer(
        TimerCallback<GC> callback,
        int32_t interval,
        bool is_timeout
    );

    /// Removes a timer by ID.
    ///
    /// \param id The timer ID to remove
    /// \return True if the timer was found and removed
    bool remove(int32_t id);

    /// Removes all timers.
    void remove_all() {
        timers_.clear();
    }

    /// Changes the delay of a timer.
    ///
    /// \param id The timer ID to modify
    /// \param interval The new interval in milliseconds
    void set_delay(int32_t id, int32_t interval);

    /// Ticks all timers and runs necessary callbacks.
    ///
    /// \param context The update context
    /// \param dt Delta time in milliseconds
    /// \return Estimated time until next timer tick, or nullopt if no timers
    static std::optional<double> update_timers(UpdateContext* context, double dt);

private:
    /// The collection of active timers (priority queue, min-heap).
    std::priority_queue<Timer<GC>, std::vector<Timer<GC>>, TimerComparator<GC>> timers_;

    /// An increasing ID used for created timers.
    int32_t timer_counter_;

    /// The current global time.
    uint64_t cur_time_;

    /// Get the next timer to fire (peek).
    const Timer<GC>* peek() const;

    /// Get the next timer to fire (mutable peek).
    Timer<GC>* peek_mut();

    /// Remove and return the next timer to fire.
    std::optional<Timer<GC>> pop();
};

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_TIMER_H
