// C++ translation of core/src/avm2/object/textformat_object.rs
//! Object representation for TextFormat

#ifndef RUFFLE_CORE_AVM2_OBJECT_TEXTFORMAT_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_TEXTFORMAT_OBJECT_H

#include <memory>
#include <format>
#include <cell>

// Forward declarations
namespace ruffle {
namespace core {
    class TextFormat;

    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class ScriptObjectData;
        template<typename T> class TObject;
        template<typename T> class Gc;
        template<typename T> class GcWeak;
        template<typename T> class ClassObject;
        template<typename T> class Object;
        template<typename T> class Error;
        template<typename T> class Mutation;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to TextFormatObject data
template<typename GC>
class TextFormatObjectWeak;

/// Data structure for TextFormatObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class TextFormatObjectData {
public:
    TextFormatObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// The TextFormat this object wraps
    mutable std::cell::RefCell<TextFormat> text_format;

    // Friend classes for accessing private members
    friend class TextFormatObject<GC>;
    friend class TextFormatObjectWeak<GC>;
};

/// TextFormatObject - AVM2 TextFormat object representation
/// Represents a TextFormat instance in ActionScript 3
template<typename GC>
class TextFormatObject {
public:
    using DataType = TextFormatObjectData<GC>;
    using WeakType = TextFormatObjectWeak<GC>;

    TextFormatObject() = default;
    explicit TextFormatObject(Gc<GC, DataType> data) : data_(data) {}

    /// A class instance allocator that allocates TextFormat objects.
    ///
    /// @param class_object The class object for TextFormat
    /// @param activation The current activation
    /// @return New TextFormatObject
    static Object<GC> textformat_allocator(
        ClassObject<GC> class_object,
        Activation<GC>* activation
    );

    /// Create a TextFormatObject from a TextFormat
    ///
    /// @param activation The current activation
    /// @param text_format The TextFormat to wrap
    /// @return New TextFormatObject
    static Object<GC> from_text_format(
        Activation<GC>* activation,
        TextFormat text_format
    );

    /// Get the TextFormat (immutable)
    /// @return Ref to TextFormat
    std::cell::Ref<TextFormat> text_format() const {
        return data_->text_format.borrow();
    }

    /// Get the TextFormat (mutable)
    /// @return RefMut to TextFormat
    std::cell::RefMut<TextFormat> text_format_mut() const {
        return data_->text_format.borrow_mut();
    }

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to TextFormatObject
template<typename GC>
class TextFormatObjectWeak {
public:
    TextFormatObjectWeak() = default;
    explicit TextFormatObjectWeak(GcWeak<GC, TextFormatObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, TextFormatObjectData<GC>> data_;
};

// Template method implementations

template<typename GC>
Object<GC> TextFormatObject<GC>::textformat_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
) {
    auto base = ScriptObjectData<GC>::new_instance(class_object);
    auto text_format = TextFormat::default_();
    text_format.display = TextDisplay::Block;

    auto data = Gc<GC, DataType>::create(
        activation->gc(),
        DataType{
            base,
            std::cell::RefCell<TextFormat>::new(text_format)
        }
    );

    return Object<GC>::from_textformat_object(TextFormatObject<GC>(data));
}

template<typename GC>
Object<GC> TextFormatObject<GC>::from_text_format(
    Activation<GC>* activation,
    TextFormat text_format
) {
    auto class_object = activation->avm2()->classes()->textformat;

    auto base = ScriptObjectData<GC>::new_instance(class_object);

    auto data = Gc<GC, DataType>::create(
        activation->gc(),
        DataType{
            base,
            std::cell::RefCell<TextFormat>::new(text_format)
        }
    );

    return Object<GC>::from_textformat_object(TextFormatObject<GC>(data));
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::TextFormatObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::TextFormatObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "TextFormatObject(ptr={:p})",
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_TEXTFORMAT_OBJECT_H
