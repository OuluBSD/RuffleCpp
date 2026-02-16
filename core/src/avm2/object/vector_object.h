// C++ translation of core/src/avm2/object/vector_object.rs
// Vector storage object for AVM2

#ifndef RUFFLE_CORE_AVM2_OBJECT_VECTOR_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_VECTOR_OBJECT_H

#include <cstdint>
#include <optional>
#include <memory>
#include <mutex>
#include <fmt/format.h>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class Activation;
    template<typename GC>
    class ScriptObjectData;
    template<typename GC>
    class Gc;
    template<typename GC>
    class GcWeak;
    template<typename GC>
    class Mutation;
    template<typename GC>
    class AvmString;
    namespace avm2 {
        template<typename GC>
        class Object;
        template<typename GC>
        class TObject;
        template<typename GC>
        class Value;
        template<typename GC>
        class Class;
        template<typename GC>
        class Multiname;
        template<typename GC>
        class Error;
        template<typename GC>
        class VectorStorage;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace object {

/// Weak reference to VectorObjectData
template<typename GC>
struct VectorObjectWeak {
    GcWeak<GC, VectorObjectData<GC>> weak;

    explicit VectorObjectWeak(GcWeak<GC, VectorObjectData<GC>> w)
        : weak(w) {}
};

/// Data structure for Vector objects
template<typename GC>
struct VectorObjectData {
    /// Base script object
    ScriptObjectData<GC> base;

    /// Vector-structured properties (thread-safe with mutex)
    mutable std::mutex vector_mutex;
    VectorStorage<GC> vector;
};

/// An Object which stores typed properties in vector storage
template<typename GC>
class VectorObject {
private:
    Gc<GC, VectorObjectData<GC>> data_;

    /// Parse a string as a vector index
    /// @param local_name The name to parse
    /// @return Optional index value, or nullopt if not a valid number
    static std::optional<double> as_vector_index(const AvmString<GC>& local_name) {
        // TODO: match avmplus's parsing more closely
        try {
            return std::stod(local_name.to_string());
        } catch (...) {
            return std::nullopt;
        }
    }

    /// Generate a read error when indexing fails
    /// @param activation Current activation
    /// @param name The multiname being accessed
    /// @param index The numeric index
    /// @return Optional error (nullopt means fall back to prototype chain)
    std::optional<Error<GC>> fail_read_error(
        Activation<GC>* activation,
        const Multiname<GC>& name,
        double index
    ) {
        // TODO the error thrown sometimes depends on JIT behavior

        if (activation->caller_movie_or_root()->version() >= 11) {
            // When in >=SWFv11, a RangeError is always thrown.
            size_t storage_len = data_->vector.length();
            return make_error_1125(activation, index, storage_len);
        } else if (index > 0.0) {
            // Non-negative values throw a ReferenceError on SWFv10
            return make_reference_error(
                activation,
                ReferenceErrorCode::InvalidRead,
                name,
                instance_class()
            );
        } else {
            // Negative values fall back to the prototype chain on SWFv10
            return std::nullopt;
        }
    }

    /// Generate a write error when indexing fails
    /// @param activation Current activation
    /// @param name The multiname being accessed
    /// @param index The numeric index
    /// @return Error to throw
    Error<GC> fail_write_error(
        Activation<GC>* activation,
        const Multiname<GC>& name,
        double index
    ) {
        // TODO the error thrown sometimes depends on JIT behavior

        if (activation->caller_movie_or_root()->version() >= 11) {
            // When in >=SWFv11, a RangeError is always thrown.
            size_t storage_len = data_->vector.length();
            return make_error_1125(activation, index, storage_len);
        } else {
            return make_reference_error(
                activation,
                ReferenceErrorCode::InvalidWrite,
                name,
                instance_class()
            );
        }
    }

    /// Set an element in the vector
    /// @param activation Current activation
    /// @param index Element index
    /// @param value Value to set
    void set_element(
        Activation<GC>* activation,
        size_t index,
        Value<GC> value
    ) {
        auto type_of = data_->vector.value_type_for_coercion(activation);
        Value<GC> coerced_value = value.coerce_to_type(activation, type_of);

        // Handle undefined/null by using default value
        if (coerced_value.is_undefined() || coerced_value.is_null()) {
            coerced_value = data_->vector.default_value();
        }

        std::lock_guard<std::mutex> lock(data_->vector_mutex);
        data_->vector.set(index, coerced_value, activation);
    }

public:
    /// Wrap an existing vector in an object
    ///
    /// @param vector The vector storage to wrap
    /// @param activation Current activation
    /// @return VectorObject wrapping the storage
    static VectorObject from_vector(
        VectorStorage<GC> vector,
        Activation<GC>* activation
    ) {
        auto value_type = vector.value_type();
        auto vector_class = activation->avm2()->classes().generic_vector;

        auto applied_class = vector_class.parametrize(activation, value_type);

        VectorObjectData<GC> data {
            .base = ScriptObjectData<GC>::new_instance(applied_class),
            .vector = std::move(vector)
        };

        return VectorObject(Gc::new(activation->gc(), std::move(data)));
    }

    /// Get immutable reference to vector storage
    VectorStorage<GC> storage() const {
        std::lock_guard<std::mutex> lock(data_->vector_mutex);
        return data_->vector;
    }

    /// Get mutable reference to vector storage
    VectorStorage<GC>& storage_mut(Mutation<GC>* mc) {
        (void)mc; // GC barrier handling would go here
        return data_->vector;
    }

    /// Get the underlying GC data
    Gc<GC, VectorObjectData<GC>> gc_data() const {
        return data_;
    }

    /// Get weak reference
    VectorObjectWeak<GC> weak() const {
        return VectorObjectWeak<GC>(data_.weak());
    }
};

/// Debug formatting for VectorObject
template<typename GC>
struct DebugFormatter<VectorObject<GC>> {
    static std::string format(const VectorObject<GC>& obj) {
        return fmt::format("VectorObject(ptr={})", fmt::ptr(obj.gc_data().get()));
    }
};

} // namespace object
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_VECTOR_OBJECT_H
