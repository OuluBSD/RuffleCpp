// C++ translation of core/src/avm2/object/vertex_buffer_3d_object.rs
// Object representation for VertexBuffer3D objects

#ifndef RUFFLE_CORE_AVM2_OBJECT_VERTEX_BUFFER_3D_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_VERTEX_BUFFER_3D_OBJECT_H

#include <cstdint>
#include <memory>
#include <fmt/format.h>

// Forward declarations
namespace ruffle {
namespace render {
    class VertexBuffer;
}
namespace core {
    template<typename GC>
    class Activation;
    template<typename GC>
    class ScriptObjectData;
    template<typename GC>
    class Gc;
    template<typename GC>
    class GcWeak;
    namespace avm2 {
        template<typename GC>
        class Object;
        template<typename GC>
        class TObject;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace object {

// Forward declaration
template<typename GC>
class Context3DObject;

/// Weak reference to VertexBuffer3DObjectData
template<typename GC>
struct VertexBuffer3DObjectWeak {
    GcWeak<GC, VertexBuffer3DObjectData<GC>> weak;

    explicit VertexBuffer3DObjectWeak(GcWeak<GC, VertexBuffer3DObjectData<GC>> w)
        : weak(w) {}
};

/// Data structure for VertexBuffer3D objects
template<typename GC>
struct VertexBuffer3DObjectData {
    /// Base script object
    ScriptObjectData<GC> base;

    /// Context3D object this buffer belongs to
    Context3DObject<GC> context3d;

    /// The vertex buffer handle
    std::shared_ptr<ruffle::render::VertexBuffer> handle;

    /// The 'data32PerVertex' value that this object was created with.
    /// This is the number of 32-bit values associated with each vertex,
    /// and is at most 64
    uint8_t data32_per_vertex;
};

/// Object representation for VertexBuffer3D objects
template<typename GC>
class VertexBuffer3DObject {
private:
    Gc<GC, VertexBuffer3DObjectData<GC>> data_;

public:
    /// Create a VertexBuffer3DObject from a handle
    ///
    /// @param activation The current activation
    /// @param context3d The Context3D object this buffer belongs to
    /// @param handle The vertex buffer handle
    /// @param data32_per_vertex Number of 32-bit values per vertex (max 64)
    /// @return Object representation
    static Object<GC> from_handle(
        Activation<GC>* activation,
        Context3DObject<GC> context3d,
        std::shared_ptr<ruffle::render::VertexBuffer> handle,
        uint8_t data32_per_vertex
    );

    /// Get the vertex buffer handle
    std::shared_ptr<ruffle::render::VertexBuffer> handle() const {
        return data_->handle;
    }

    /// Get the Context3D object
    Context3DObject<GC> context3d() const {
        return data_->context3d;
    }

    /// Get the data32PerVertex value
    uint8_t data32_per_vertex() const {
        return data_->data32_per_vertex;
    }

    /// Get the underlying GC data
    Gc<GC, VertexBuffer3DObjectData<GC>> gc_data() const {
        return data_;
    }

    /// Get weak reference
    VertexBuffer3DObjectWeak<GC> weak() const {
        return VertexBuffer3DObjectWeak<GC>(data_.weak());
    }
};

/// Debug formatting for VertexBuffer3DObject
template<typename GC>
struct DebugFormatter<VertexBuffer3DObject<GC>> {
    static std::string format(const VertexBuffer3DObject<GC>& obj) {
        return "VertexBuffer3D";
    }
};

} // namespace object
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_VERTEX_BUFFER_3D_OBJECT_H
