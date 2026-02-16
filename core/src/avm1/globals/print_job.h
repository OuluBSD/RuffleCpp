// C++ translation of core/src/avm1/globals/print_job.rs
// PrintJob object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_PRINT_JOB_H
#define RUFFLE_CORE_AVM1_GLOBALS_PRINT_JOB_H

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
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace print_job {

/// Property declarations for PrintJob prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the PrintJob class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created PrintJob class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// PrintJob constructor
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements PrintJob.start method
template<typename GC>
Value<GC> start(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements PrintJob.addPage method
template<typename GC>
Value<GC> add_page(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements PrintJob.send method
template<typename GC>
Value<GC> send(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements PrintJob.paperHeight property getter
template<typename GC>
Value<GC> paper_height(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements PrintJob.paperWidth property getter
template<typename GC>
Value<GC> paper_width(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements PrintJob.pageHeight property getter
template<typename GC>
Value<GC> page_height(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements PrintJob.pageWidth property getter
template<typename GC>
Value<GC> page_width(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements PrintJob.orientation property getter
template<typename GC>
Value<GC> orientation(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace print_job
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_PRINT_JOB_H
