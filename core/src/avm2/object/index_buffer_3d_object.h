// C++ translation of core/src/avm2/object/index_buffer_3d_object.rs
//! Object representation for IndexBuffer3D objects

#ifndef RUFFLE_CORE_AVM2_OBJECT_INDEX_BUFFER_3D_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_INDEX_BUFFER_3D_OBJECT_H

#include <memory>
#include <cstdint>
#include <format>
#include <cell>

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
    }
    namespace render {
        namespace backend {
            class IndexBuffer;
        }
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to IndexBuffer3DObject data
template<typename GC>
class IndexBuffer3DObjectWeak;

/// Data structure for IndexBuffer3DObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class IndexBuffer3DObjectData {
public:
    IndexBuffer3DObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// Index buffer handle
    std::unique_ptr<render::backend::IndexBuffer> handle;

    /// Buffer count
    size_t count = 0;

    /// Context3D object
    class Context3DObject<GC> context3d;

    // Friend classes for accessing private members
    friend class IndexBuffer3DObject<GC>;
    friend class IndexBuffer3DObjectWeak<GC>;
};

/// IndexBuffer3DObject - AVM2 IndexBuffer3D object representation
/// Represents an IndexBuffer3D instance in ActionScript 3
template<typename GC>
class IndexBuffer3DObject {
public:
    using DataType = IndexBuffer3DObjectData<GC>;
    using WeakType = IndexBuffer3DObjectWeak<GC>;

    IndexBuffer3DObject() = default;
    explicit IndexBuffer3DObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create an IndexBuffer3DObject from a handle
    ///
    /// @param activation The current activation context
    /// @param context3d The Context3D object
    /// @param handle The index buffer handle
    /// @return New IndexBuffer3DObject as Object
    static Object<GC> from_handle(
        Activation<GC>* activation,
        Context3DObject<GC> context3d,
        std::unique_ptr<render::backend::IndexBuffer> handle);

    /// Get the buffer count
    size_t count() const {
        return data_->count;
    }

    /// Set the buffer count
    void set_count(size_t val) {
        data_->count = val;
    }

    /// Get the index buffer handle
    render::backend::IndexBuffer& handle() {
        return *data_->handle;
    }

    /// Get the Context3D object
    Context3DObject<GC> context3d() const {
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

/// Weak reference to IndexBuffer3DObject
template<typename GC>
class IndexBuffer3DObjectWeak {
public:
    IndexBuffer3DObjectWeak() = default;
    explicit IndexBuffer3DObjectWeak(GcWeak<GC, IndexBuffer3DObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, IndexBuffer3DObjectData<GC>> data_;
};

// Template method implementations

template<typename GC>
Object<GC> IndexBuffer3DObject<GC>::from_handle(
    Activation<GC>* activation,
    Context3DObject<GC> context3d,
    std::unique_ptr<render::backend::IndexBuffer> handle) {
    auto class_obj = activation->avm2()->classes()->indexbuffer3d;
    auto base = ScriptObjectData<GC>::create(class_obj);
    auto data = Gc<GC, DataType>::create(
        activation->gc(),
        DataType{base, std::move(handle), 0, context3d}
    );
    return Object<GC>(IndexBuffer3DObject<GC>(data));
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::IndexBuffer3DObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::IndexBuffer3DObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "IndexBuffer3DObject(ptr={})",
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_INDEX_BUFFER_3D_OBJECT_H
