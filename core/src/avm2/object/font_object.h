// C++ translation of core/src/avm2/object/font_object.rs
//! Object representation for Font objects

#ifndef RUFFLE_CORE_AVM2_OBJECT_FONT_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_FONT_OBJECT_H

#include <memory>
#include <optional>
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
    class Mutation;
}
namespace character {
    class Character;
}
namespace font {
    template<typename T> class Font;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to FontObject data
template<typename GC>
class FontObjectWeak;

/// Data structure for FontObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class FontObjectData {
public:
    FontObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// The font character
    std::optional<font::Font<GC>> font;

    // Friend classes for accessing private members
    friend class FontObject<GC>;
    friend class FontObjectWeak<GC>;
};

/// FontObject - AVM2 Font object representation
/// Represents a Font instance in ActionScript
template<typename GC>
class FontObject {
public:
    using DataType = FontObjectData<GC>;
    using WeakType = FontObjectWeak<GC>;

    FontObject() = default;
    explicit FontObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create a FontObject for a specific font
    /// @param mc The mutation context
    /// @param class_obj The class object for Font
    /// @param font The font character
    /// @return New FontObject
    static FontObject for_font(
        Mutation* mc,
        ClassObject<GC> class_obj,
        font::Font<GC> font);

    /// Get the font character
    std::optional<font::Font<GC>> font() const {
        return data_->font;
    }

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to FontObject
template<typename GC>
class FontObjectWeak {
public:
    FontObjectWeak() = default;
    explicit FontObjectWeak(GcWeak<GC, FontObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, FontObjectData<GC>> data_;
};

/// Font object allocator function
/// @param class_obj The class object for Font
/// @param activation The current activation context
/// @return Result containing the new FontObject or error
template<typename GC>
Object<GC> font_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation);

// Template method implementations
template<typename GC>
FontObject<GC> FontObject<GC>::for_font(
    Mutation* mc,
    ClassObject<GC> class_obj,
    font::Font<GC> font) {
    auto base = ScriptObjectData<GC>::create(class_obj);
    auto data = Gc<GC, DataType>::create(mc, DataType{base, font});
    return FontObject<GC>(data);
}

template<typename GC>
Object<GC> font_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation) {
    auto base = ScriptObjectData<GC>::create(class_obj);

    // Try to get the font from the library
    std::optional<font::Font<GC>> font;
    
    // Note: This would need access to activation.context.library
    // Simplified implementation for now - font will be None
    
    auto data = Gc<GC, DataType>::create(activation->gc(), DataType{base, font});
    return Object<GC>(FontObject<GC>(data));
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_FONT_OBJECT_H
