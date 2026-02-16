// C++ translation of core/src/avm1/globals/text_renderer.rs
// TextRenderer object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_TEXT_RENDERER_H
#define RUFFLE_CORE_AVM1_GLOBALS_TEXT_RENDERER_H

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
namespace text_renderer {

/// Property declarations for TextRenderer prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the TextRenderer class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created TextRenderer class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// TextRenderer constructor
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace text_renderer
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_TEXT_RENDERER_H
