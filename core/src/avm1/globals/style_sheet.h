// C++ translation of core/src/avm1/globals/style_sheet.rs
// AVM1 StyleSheet object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_STYLE_SHEET_H
#define RUFFLE_CORE_AVM1_GLOBALS_STYLE_SHEET_H

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {

namespace avm1 {
    template<typename GC>
    class Activation;
    template<typename GC>
    class Error;
    template<typename GC>
    class Object;
    template<typename GC>
    class Value;
    enum class NativeObject;
    template<typename GC>
    class ArrayBuilder;
    template<typename GC>
    class ExecutionReason;
    namespace property_decl {
        template<typename GC>
        class DeclContext;
        template<typename GC>
        class StaticDeclarations;
        template<typename GC>
        struct SystemClass;
    }
}

namespace html {
    template<typename GC>
    class StyleSheet;
    class TextFormat;
    class CssStream;
}

namespace string {
    class AvmString;
}

namespace wstr {
    class WStr;
    class WString;
}

namespace backend {
namespace navigator {
    template<typename GC>
    class Request;
}
}

} // namespace core
} // namespace ruffle

namespace gc_arena {
    template<typename GC>
    class Mutation;
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace style_sheet {

/// A StyleSheet object that is tied to a style sheet.
template<typename GC>
class StyleSheetObject {
private:
    std::shared_ptr<html::StyleSheet<GC>> style_sheet_;

public:
    StyleSheetObject() = default;

    /// Create a new StyleSheet object
    explicit StyleSheetObject(std::shared_ptr<html::StyleSheet<GC>> style_sheet)
        : style_sheet_(std::move(style_sheet)) {}

    /// Create a new empty style sheet
    static StyleSheetObject create(gc_arena::Mutation<GC>* mc);

    /// Set style for a selector
    void set_style(WString selector, html::TextFormat format);

    /// Get the underlying style sheet
    std::shared_ptr<html::StyleSheet<GC>> style_sheet() const {
        return style_sheet_;
    }
};

/// Property declarations for StyleSheet prototype
extern const avm1::property_decl::StaticDeclarations PROTO_DECLS;

/// Create the StyleSheet class
template<typename GC>
avm1::property_decl::SystemClass<GC> create_class(
    avm1::property_decl::DeclContext<GC>* context,
    avm1::Object<GC> super_proto
);

/// Constructor function
template<typename GC>
avm1::Value<GC> constructor(
    avm1::Activation<GC>* activation,
    avm1::Object<GC> this_obj,
    const std::vector<avm1::Value<GC>>& args
);

/// Shallow copy an object
template<typename GC>
avm1::Value<GC> shallow_copy(
    avm1::Activation<GC>* activation,
    avm1::Value<GC> value
);

/// setStyle method
template<typename GC>
avm1::Value<GC> set_style(
    avm1::Activation<GC>* activation,
    avm1::Object<GC> this_obj,
    const std::vector<avm1::Value<GC>>& args
);

/// getStyle method
template<typename GC>
avm1::Value<GC> get_style(
    avm1::Activation<GC>* activation,
    avm1::Object<GC> this_obj,
    const std::vector<avm1::Value<GC>>& args
);

/// getStyleNames method
template<typename GC>
avm1::Value<GC> get_style_names(
    avm1::Activation<GC>* activation,
    avm1::Object<GC> this_obj,
    const std::vector<avm1::Value<GC>>& args
);

/// load method
template<typename GC>
avm1::Value<GC> load(
    avm1::Activation<GC>* activation,
    avm1::Object<GC> this_obj,
    const std::vector<avm1::Value<GC>>& args
);

/// transform method
template<typename GC>
avm1::Value<GC> transform(
    avm1::Activation<GC>* activation,
    avm1::Object<GC> this_obj,
    const std::vector<avm1::Value<GC>>& args
);

/// parseCSS/parse method
template<typename GC>
avm1::Value<GC> parse_css(
    avm1::Activation<GC>* activation,
    avm1::Object<GC> this_obj,
    const std::vector<avm1::Value<GC>>& args
);

/// clear method
template<typename GC>
avm1::Value<GC> clear(
    avm1::Activation<GC>* activation,
    avm1::Object<GC> this_obj,
    const std::vector<avm1::Value<GC>>& args
);

/// Parse color from string (e.g., "#FF0000")
std::optional<swf::Color> parse_color(const string::AvmString& input);

/// Parse suffixed number as i32
template<typename GC>
std::result::Result<int32_t, avm1::Error<GC>> parse_suffixed_number_i32(
    avm1::Activation<GC>* activation,
    const avm1::Value<GC>* input
);

/// Parse suffixed number as f64
template<typename GC>
std::result::Result<double, avm1::Error<GC>> parse_suffixed_number_f64(
    avm1::Activation<GC>* activation,
    const avm1::Value<GC>* input
);

} // namespace style_sheet
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_STYLE_SHEET_H
