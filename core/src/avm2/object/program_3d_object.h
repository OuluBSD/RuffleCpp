// C++ translation of core/src/avm2/object/program_3d_object.rs
//! Object representation for VertexBuffer3D objects

#ifndef RUFFLE_CORE_AVM2_OBJECT_PROGRAM_3D_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_PROGRAM_3D_OBJECT_H

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
    template<typename T> class Object;
    namespace globals {
        namespace flash {
            namespace display3D {
                template<typename T> class Context3DObject;
            }
        }
    }
}
namespace render {
namespace backend {
    class ShaderModule;
}
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to Program3DObject data
template<typename GC>
class Program3DObjectWeak;

/// Data structure for Program3DObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class Program3DObjectData {
public:
    Program3DObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// Context3D object
    globals::flash::display3D::Context3DObject<GC> context3d;

    /// Shader module handle
    std::optional<std::shared_ptr<render::backend::ShaderModule>> shader_module_handle;

    // Friend classes for accessing private members
    friend class Program3DObject<GC>;
    friend class Program3DObjectWeak<GC>;
};

/// Program3DObject - AVM2 Program3D object representation
/// Represents a Program3D instance in ActionScript (Vertex/Fragment shader program)
template<typename GC>
class Program3DObject {
public:
    using DataType = Program3DObjectData<GC>;
    using WeakType = Program3DObjectWeak<GC>;

    Program3DObject() = default;
    explicit Program3DObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create a Program3D object from a Context3D
    /// @param activation The current activation context
    /// @param context3d The Context3D object
    /// @return New Program3DObject
    static Program3DObject from_context(
        Activation<GC>* activation,
        globals::flash::display3D::Context3DObject<GC> context3d);

    /// Get the shader module handle
    const std::optional<std::shared_ptr<render::backend::ShaderModule>>& shader_module_handle() const {
        return data_->shader_module_handle;
    }

    /// Get the shader module handle (mutable)
    std::optional<std::shared_ptr<render::backend::ShaderModule>>& shader_module_handle_mut() {
        return data_->shader_module_handle;
    }

    /// Get the Context3D object
    globals::flash::display3D::Context3DObject<GC> context3d() const {
        return data_->context3d;
    }

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to Program3DObject
template<typename GC>
class Program3DObjectWeak {
public:
    Program3DObjectWeak() = default;
    explicit Program3DObjectWeak(GcWeak<GC, Program3DObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, Program3DObjectData<GC>> data_;
};

// Template method implementations
template<typename GC>
Program3DObject<GC> Program3DObject<GC>::from_context(
    Activation<GC>* activation,
    globals::flash::display3D::Context3DObject<GC> context3d) {
    auto class_obj = activation->avm2()->classes()->program3d;
    auto base = ScriptObjectData<GC>::create(class_obj);
    auto data = Gc<GC, DataType>::create(activation->gc(), DataType{base, context3d});
    return Program3DObject<GC>(data);
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_PROGRAM_3D_OBJECT_H
