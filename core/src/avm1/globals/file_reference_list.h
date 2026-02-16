// C++ translation of core/src/avm1/globals/file_reference_list.rs
// flash.net.FileReferenceList object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_FILE_REFERENCE_LIST_H
#define RUFFLE_CORE_AVM1_GLOBALS_FILE_REFERENCE_LIST_H

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
namespace file_reference_list {

// Property declarations for FileReferenceList prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the FileReferenceList class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created FileReferenceList class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// FileReferenceList constructor
///
/// \param activation The current activation context
/// \param this_obj The object being constructed
/// \param args Arguments passed to the constructor
/// \return Value::Undefined as the constructor doesn't return anything meaningful
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace file_reference_list
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_FILE_REFERENCE_LIST_H
