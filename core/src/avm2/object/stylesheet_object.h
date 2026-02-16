// C++ translation of core/src/avm2/object/stylesheet_object.rs
//! StyleSheetObject - AVM2 StyleSheet object representation

#ifndef RUFFLE_CORE_AVM2_OBJECT_STYLESHEET_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_STYLESHEET_OBJECT_H

#include <memory>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class ScriptObjectData;
    template<typename T> class TObject;
    template<typename T> class Gc;
    template<typename T> class GcWeak;
    template<typename T> class ClassObject;
    template<typename T> class Object;
    template<typename T> class Error;
}
namespace html {
    template<typename T> class StyleSheet;
    class TextFormat;
}
namespace string {
    template<typename T> class WString;
    class WStr;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to StyleSheetObject data
template<typename GC>
class StyleSheetObjectWeak;

/// Data structure for StyleSheetObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class StyleSheetObjectData {
public:
    StyleSheetObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// The style sheet
    html::StyleSheet<GC> style_sheet;

    // Friend classes for accessing private members
    friend class StyleSheetObject<GC>;
    friend class StyleSheetObjectWeak<GC>;
};

/// StyleSheetObject - AVM2 StyleSheet object representation
/// Represents a StyleSheet instance in ActionScript
template<typename GC>
class StyleSheetObject {
public:
    using DataType = StyleSheetObjectData<GC>;
    using WeakType = StyleSheetObjectWeak<GC>;

    StyleSheetObject() = default;
    explicit StyleSheetObject(Gc<GC, DataType> data) : data_(data) {}

    /// Set style for a selector
    /// @param selector The selector string
    /// @param format The text format to apply
    void set_style(string::WString selector, html::TextFormat format);

    /// Remove style for a selector
    /// @param selector The selector string
    void remove_style(const string::WStr* selector);

    /// Clear all styles
    void clear();

    /// Get the style sheet
    html::StyleSheet<GC> style_sheet() const {
        return data_->style_sheet;
    }

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to StyleSheetObject
template<typename GC>
class StyleSheetObjectWeak {
public:
    StyleSheetObjectWeak() = default;
    explicit StyleSheetObjectWeak(GcWeak<GC, StyleSheetObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, StyleSheetObjectData<GC>> data_;
};

/// StyleSheet object allocator function
/// @param class_obj The class object for StyleSheet
/// @param activation The current activation context
/// @return Result containing the new StyleSheetObject or error
template<typename GC>
Object<GC> style_sheet_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation);

// Template method implementations
template<typename GC>
void StyleSheetObject<GC>::set_style(string::WString selector, html::TextFormat format) {
    data_->style_sheet.set_style(selector, format);
}

template<typename GC>
void StyleSheetObject<GC>::remove_style(const string::WStr* selector) {
    data_->style_sheet.remove_style(selector);
}

template<typename GC>
void StyleSheetObject<GC>::clear() {
    data_->style_sheet.clear();
}

template<typename GC>
Object<GC> style_sheet_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation) {
    auto base = ScriptObjectData<GC>::create(class_obj);
    auto data = Gc<GC, DataType>::create(activation->gc(), DataType{base});
    data->style_sheet = html::StyleSheet<GC>::create(activation->gc());
    return Object<GC>(StyleSheetObject<GC>(data));
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_STYLESHEET_OBJECT_H
