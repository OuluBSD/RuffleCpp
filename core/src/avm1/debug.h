// C++ translation of core/src/avm1/debug.rs
// AVM1 variable debugging utilities

#ifndef RUFFLE_CORE_AVM1_DEBUG_H
#define RUFFLE_CORE_AVM1_DEBUG_H

#include <cstdint>
#include <string>
#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {

template<typename GC>
class Activation;

template<typename GC>
class Value;

template<typename GC>
class Object;

namespace value {
    using ObjectPtr = void*; // Placeholder for actual object pointer type
}
}
}
}

namespace ruffle {
namespace core {
namespace string {
    template<typename GC>
    class AvmString;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace debug {

/// Variable dumper for debugging AVM1 values and objects
template<typename GC>
class VariableDumper {
private:
    std::vector<void*> objects;  // Track printed object pointers
    uint32_t depth;
    std::string output;
    const char* indent;

    /// Get or create object ID
    /// @param object The object to get ID for
    /// @return Pair of (id, is_new)
    std::pair<size_t, bool> object_id(const Object<GC>& object);

    /// Add indentation to output
    void indent_output();

public:
    /// Create a new variable dumper
    /// @param indent The indentation string to use
    explicit VariableDumper(const char* indent);

    /// Dump a value to string
    /// @param value The value to dump
    /// @param indent The indentation string
    /// @param activation The activation context
    /// @return String representation of the value
    static std::string dump(
        const Value<GC>* value,
        const char* indent,
        Activation<GC>* activation
    );

    /// Get the current output
    const std::string& output() const {
        return output;
    }

    /// Print a string value with escaping
    /// @param string The string to print
    void print_string(const ruffle::core::string::AvmString<GC>& string);

    /// Print an object
    /// @param object The object to print
    /// @param activation The activation context
    void print_object(Object<GC> object, Activation<GC>* activation);

    /// Print a property value
    /// @param object The object containing the property
    /// @param key The property name
    /// @param activation The activation context
    void print_property(
        Object<GC> object,
        const ruffle::core::string::AvmString<GC>& key,
        Activation<GC>* activation
    );

    /// Print all properties of an object
    /// @param object The object to print properties for
    /// @param activation The activation context
    void print_properties(Object<GC> object, Activation<GC>* activation);

    /// Print a value
    /// @param value The value to print
    /// @param activation The activation context
    void print_value(const Value<GC>* value, Activation<GC>* activation);

    /// Print variables from an object
    /// @param header Section header
    /// @param name Object name prefix
    /// @param object The object to print variables from
    /// @param activation The activation context
    void print_variables(
        const char* header,
        const char* name,
        Object<GC> object,
        Activation<GC>* activation
    );
};

} // namespace debug
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_DEBUG_H
