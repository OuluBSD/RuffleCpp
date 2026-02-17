// C++ translation of core/src/avm2/globals/vector.rs
//! Vector builtin/prototype functions

#ifndef RUFFLE_CORE_AVM2_GLOBALS_VECTOR_H
#define RUFFLE_CORE_AVM2_GLOBALS_VECTOR_H

#include <cstdint>
#include <vector>
#include <optional>
#include <algorithm>
#include <functional>
#include <compare>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class AvmString;
    template<typename GC>
    class Value;
    template<typename GC>
    class Object;
    template<typename GC>
    class Class;
    template<typename GC>
    class ClassObject;
    template<typename GC>
    class Activation;
    template<typename GC>
    class Error;
    template<typename GC>
    class Multiname;
    template<typename GC>
    class QName;
    template<typename GC>
    class VectorObject;
    template<typename GC>
    class VectorStorage;
    template<typename GC>
    class FunctionObject;
    class UpdateContext;

    namespace avm2 {
        namespace function {
            template<typename GC>
            class FunctionArgs;
        }
        namespace globals {
            namespace array {
                enum class SortOptions;
                template<typename GC>
                class ArrayIter;
            }
        }
    }

    namespace gc_arena {
        template<typename T>
        class Gc;
        class Mutation;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace vector {

using gc_arena::Gc;
using gc_arena::Mutation;
using function::FunctionArgs;
using globals::array::SortOptions;
using globals::array::ArrayIter;

/// Allocator for generic Vector, not specialized Vector
template<typename GC>
Result<Object<GC>, Error<GC>> vector_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
);

/// Implements Vector's instance constructor
template<typename GC>
Result<Value<GC>, Error<GC>> instance_init(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector's call handler (constructor called as function)
template<typename GC>
Result<Value<GC>, Error<GC>> call_handler(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Vector.length getter
template<typename GC>
Result<Value<GC>, Error<GC>> get_length(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Vector.length setter
template<typename GC>
Result<Value<GC>, Error<GC>> set_length(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Vector.fixed getter
template<typename GC>
Result<Value<GC>, Error<GC>> get_fixed(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Vector.fixed setter
template<typename GC>
Result<Value<GC>, Error<GC>> set_fixed(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Helper function for Vector.concat impl
template<typename GC>
Result<Value<GC>, Error<GC>> concat_helper(
    Activation<GC>* activation,
    Class<GC> my_base_vector_class,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.join
template<typename GC>
Result<Value<GC>, Error<GC>> join(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.every (delegated to array)
template<typename GC>
Result<Value<GC>, Error<GC>> every(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.forEach (delegated to array)
template<typename GC>
Result<Value<GC>, Error<GC>> for_each(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.some
template<typename GC>
Result<Value<GC>, Error<GC>> some(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.filter
template<typename GC>
Result<Value<GC>, Error<GC>> filter(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.indexOf
template<typename GC>
Result<Value<GC>, Error<GC>> index_of(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.lastIndexOf
template<typename GC>
Result<Value<GC>, Error<GC>> last_index_of(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.map
template<typename GC>
Result<Value<GC>, Error<GC>> map(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.pop
template<typename GC>
Result<Value<GC>, Error<GC>> pop(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.push
template<typename GC>
Result<Value<GC>, Error<GC>> push(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.shift
template<typename GC>
Result<Value<GC>, Error<GC>> shift(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.unshift
template<typename GC>
Result<Value<GC>, Error<GC>> unshift(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.insertAt
template<typename GC>
Result<Value<GC>, Error<GC>> insert_at(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.removeAt
template<typename GC>
Result<Value<GC>, Error<GC>> remove_at(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.reverse
template<typename GC>
Result<Value<GC>, Error<GC>> reverse(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.slice
template<typename GC>
Result<Value<GC>, Error<GC>> slice(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.sort
template<typename GC>
Result<Value<GC>, Error<GC>> sort(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Vector.splice
template<typename GC>
Result<Value<GC>, Error<GC>> splice(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set up a builtin vector's Class.
///
/// This will change its name, mark it as a specialization of Vector,
/// and set its class parameter to the passed param_class.
/// This function returns the vector Class.
template<typename GC>
Class<GC> setup_vector_class(
    Activation<GC>* activation,
    const char* old_name,
    const char* new_name,
    std::optional<Class<GC>> param_class
);

/// Initialize vector class definitions
template<typename GC>
void init_vector_class_defs(Activation<GC>* activation);

/// Set up a builtin vector's ClassObject.
///
/// This marks it as a specialization of Vector.
/// This function returns the vector ClassObject.
template<typename GC>
ClassObject<GC> setup_vector_class_object(
    Activation<GC>* activation,
    const char* vector_name,
    std::optional<Class<GC>> param_class
);

/// Initialize vector class objects
template<typename GC>
void init_vector_class_objects(Activation<GC>* activation);

// ============================================================================
// Inline implementations for delegated array methods
// ============================================================================

/// Implements Vector.concat
template<typename GC>
Result<Value<GC>, Error<GC>> concat(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
) {
    auto this_class = this_value.as_object()->instance_class();
    return concat_helper(activation, this_class, this_value, args);
}

// Delegate to array module for some methods
// These would be implemented by calling the corresponding array functions

} // namespace vector
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_VECTOR_H
