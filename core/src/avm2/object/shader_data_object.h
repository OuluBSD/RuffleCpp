// C++ translation of core/src/avm2/object/shader_data_object.rs
//! Object representation for `ShaderData`

#ifndef RUFFLE_CORE_AVM2_OBJECT_SHADER_DATA_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_SHADER_DATA_OBJECT_H

#include <memory>
#include <cstdint>
#include <optional>
#include <format>

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
namespace render {
namespace pixel_bender {
    class PixelBenderShaderHandle;
}
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to ShaderDataObject data
template<typename GC>
class ShaderDataObjectWeak;

/// Data structure for ShaderDataObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class ShaderDataObjectData {
public:
    ShaderDataObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// PixelBender shader handle (optional, cell-like semantics)
    mutable std::optional<render::pixel_bender::PixelBenderShaderHandle> shader;

    // Friend classes for accessing private members
    friend class ShaderDataObject<GC>;
    friend class ShaderDataObjectWeak<GC>;
};

/// ShaderDataObject - AVM2 ShaderData object representation
/// Represents a ShaderData instance in ActionScript
template<typename GC>
class ShaderDataObject {
public:
    using DataType = ShaderDataObjectData<GC>;
    using WeakType = ShaderDataObjectWeak<GC>;

    ShaderDataObject() = default;
    explicit ShaderDataObject(Gc<GC, DataType> data) : data_(data) {}

    /// Get the PixelBender shader
    /// Takes ownership of the shader (cell-like semantics)
    /// @return The shader handle if present, nullopt otherwise
    std::optional<render::pixel_bender::PixelBenderShaderHandle> pixel_bender_shader() const {
        auto shader = data_->shader;
        data_->shader = std::nullopt;
        return shader;
    }

    /// Set the PixelBender shader
    /// @param shader The new shader handle
    void set_pixel_bender_shader(render::pixel_bender::PixelBenderShaderHandle shader) {
        data_->shader = shader;
    }

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to ShaderDataObject
template<typename GC>
class ShaderDataObjectWeak {
public:
    ShaderDataObjectWeak() = default;
    explicit ShaderDataObjectWeak(GcWeak<GC, ShaderDataObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, ShaderDataObjectData<GC>> data_;
};

/// ShaderData object allocator function
///
/// Creates a new ShaderData object with no shader attached
///
/// @param class_obj The class object for ShaderData
/// @param activation The current activation context
/// @return Result containing the new ShaderDataObject or error
template<typename GC>
Object<GC> shader_data_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation);

// Template method implementations

template<typename GC>
Object<GC> shader_data_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation) {
    auto base = ScriptObjectData<GC>::create(class_obj);
    auto data = Gc<GC, DataType>::create(
        activation->gc(),
        DataType{base, std::nullopt}
    );
    return Object<GC>(ShaderDataObject<GC>(data));
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::ShaderDataObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::ShaderDataObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "ShaderDataObject(ptr={})",
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_SHADER_DATA_OBJECT_H
