// C++ translation of core/src/avm2/object/texture_object.rs
//! Object representation for Texture3D objects

#ifndef RUFFLE_CORE_AVM2_OBJECT_TEXTURE_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_TEXTURE_OBJECT_H

#include <memory>
#include <cstdint>
#include <optional>
#include <format>

// Forward declarations
namespace ruffle {
namespace render {
namespace backend {
    enum class Context3DTextureFormat;
    class Texture;
}
}

namespace core {

namespace gc_arena {
    template<typename T>
    class Gc;
    template<typename T>
    class GcWeak;
    class Mutation;
}

namespace avm2 {

template<typename GC>
class Activation;
template<typename GC>
class Object;
template<typename GC>
class ClassObject;
template<typename GC>
class ScriptObjectData;
template<typename GC>
class TObject;

namespace script_object {
    template<typename GC>
    class ScriptObjectData;
}

} // namespace avm2
} // namespace core
} // namespace ruffle

namespace ruffle {
namespace core {
namespace avm2 {

using gc_arena::Gc;
using gc_arena::GcWeak;
using gc_arena::Mutation;

// Forward declare Context3DObject
template<typename GC>
class Context3DObject;

/// Weak reference to TextureObject
template<typename GC>
class TextureObjectWeak;

/// Object representation for Texture3D objects
template<typename GC>
class TextureObject {
private:
    Gc<GC, TextureObjectData<GC>> data_;

    friend class TextureObjectWeak<GC>;

public:
    explicit TextureObject(Gc<GC, TextureObjectData<GC>> data)
        : data_(data)
    {}

    /// Create a TextureObject from a texture handle
    /// @param activation The current activation
    /// @param context3d The Context3D object
    /// @param handle The texture handle
    /// @param original_format The original texture format
    /// @param class The class object for this texture
    /// @return Object wrapping the TextureObject
    static Object<GC> from_handle(
        Activation<GC>* activation,
        Context3DObject<GC> context3d,
        std::shared_ptr<render::backend::Texture> handle,
        render::backend::Context3DTextureFormat original_format,
        ClassObject<GC> class_object
    ) {
        auto data = Gc<GC, TextureObjectData<GC>>::new_(
            activation->gc(),
            TextureObjectData<GC>{
                script_object::ScriptObjectData<GC>::new_instance(class_object),
                context3d,
                original_format,
                handle
            }
        );
        return Object<GC>::from_texture(TextureObject<GC>(data));
    }

    /// Get the original texture format
    render::backend::Context3DTextureFormat original_format() const {
        return data_->original_format;
    }

    /// Get the texture handle
    std::shared_ptr<render::backend::Texture> handle() const {
        return data_->handle;
    }

    /// Get the Context3D object
    Context3DObject<GC> context3d() const {
        return data_->context3d;
    }

    /// Get the base script object data
    Gc<GC, script_object::ScriptObjectData<GC>> gc_base() const {
        return data_->as_prefix_gc();
    }
};

/// Weak reference to TextureObject
template<typename GC>
class TextureObjectWeak {
private:
    GcWeak<GC, TextureObjectData<GC>> data_;

public:
    explicit TextureObjectWeak(GcWeak<GC, TextureObjectData<GC>> data)
        : data_(data)
    {}

    /// Try to upgrade the weak reference to a strong reference
    std::optional<TextureObject<GC>> upgrade(Mutation<GC>* mc) const {
        auto strong = data_.upgrade(mc);
        if (strong.has_value()) {
            return TextureObject<GC>(*strong);
        }
        return std::nullopt;
    }
};

/// Internal data for TextureObject
/// Uses HasPrefixField pattern for base class access
template<typename GC>
class TextureObjectData {
private:
    /// Base script object data
    script_object::ScriptObjectData<GC> base_;

    /// The Context3D object this texture belongs to
    Context3DObject<GC> context3d_;

    /// The original texture format (static, not collected)
    render::backend::Context3DTextureFormat original_format_;

    /// The texture handle (static, not collected)
    std::shared_ptr<render::backend::Texture> handle_;

public:
    TextureObjectData(
        script_object::ScriptObjectData<GC> base,
        Context3DObject<GC> context3d,
        render::backend::Context3DTextureFormat original_format,
        std::shared_ptr<render::backend::Texture> handle
    )
        : base_(std::move(base))
        , context3d_(context3d)
        , original_format_(original_format)
        , handle_(std::move(handle))
    {}

    /// Get the base script object data using HasPrefixField pattern
    script_object::ScriptObjectData<GC>& as_prefix_gc() {
        return base_;
    }

    const script_object::ScriptObjectData<GC>& as_prefix_gc() const {
        return base_;
    }
};

// Debug formatter for TextureObject
template<typename GC>
struct std::formatter<TextureObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const TextureObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "Texture3D");
    }
};

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_TEXTURE_OBJECT_H
