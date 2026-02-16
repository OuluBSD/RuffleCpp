// C++ translation of core/src/avm1/globals/bitmap_filter.rs
// flash.filters.BitmapFilter object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_BITMAP_FILTER_H
#define RUFFLE_CORE_AVM1_GLOBALS_BITMAP_FILTER_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
    enum class NativeObject;
}
}
namespace swf {
    // Forward declaration for Filter enum
    enum class Filter;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace bitmap_filter {

/// Property declarations for BitmapFilter prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the BitmapFilter class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created BitmapFilter class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// Implements BitmapFilter.clone
///
/// Creates a copy of the filter.
///
/// \param activation The current activation context
/// \param this_obj The BitmapFilter object
/// \param args Arguments (should be empty)
/// \return A new filter object of the same type
template<typename GC>
Value<GC> clone(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Converts an AVM1 filter object to a SWF Filter
///
/// \param object The AVM1 filter object
/// \param context The update context
/// \return The corresponding SWF Filter, or None if invalid
template<typename GC>
swf::Filter avm1_to_filter(
    Object<GC>* object,
    void* context
);

/// Converts a SWF Filter to an AVM1 filter object
///
/// \param activation The current activation context
/// \param filter The SWF filter to convert
/// \return The corresponding AVM1 filter object
template<typename GC>
Value<GC> filter_to_avm1(
    Activation<GC>* activation,
    swf::Filter filter
);

} // namespace bitmap_filter
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_BITMAP_FILTER_H
