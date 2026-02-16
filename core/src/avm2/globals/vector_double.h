// C++ translation of core/src/avm2/globals/vector_double.rs
// Vector.<double> implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_VECTOR_DOUBLE_H
#define RUFFLE_CORE_AVM2_GLOBALS_VECTOR_DOUBLE_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;

    namespace object {
        template<typename GC>
        using vector_allocator = /* would come from object module */ void;
    }

    namespace globals {
        namespace vector {
            // Vector helper functions
            template<typename GC>
            Value<GC> call_handler(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            void instance_init(Activation<GC>* activation, Value<GC> this_value);

            template<typename GC>
            Value<GC> get_fixed(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> set_fixed(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> get_length(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> set_length(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> every(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> filter(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> for_each(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> index_of(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> insert_at(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> join(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> last_index_of(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> map(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> pop(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> push(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> remove_at(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> reverse(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> shift(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> slice(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> _some(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> sort(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> splice(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> unshift(Activation<GC>* activation, Value<GC> this_value, const std::vector<Value<GC>>& args);

            template<typename GC>
            Value<GC> concat_helper(Activation<GC>* activation, void* vector_class, Value<GC> this_value, const std::vector<Value<GC>>& args);
        } // namespace vector
    } // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {

// Re-export vector_double_initializer from vector module
using vector_double_initializer = vector::instance_init;

// Re-export vector functions from vector module
using vector::call_handler;
using vector::get_fixed;
using vector::set_fixed;
using vector::get_length;
using vector::set_length;
using vector::every;
using vector::filter;
using vector::for_each;
using vector::index_of;
using vector::insert_at;
using vector::join;
using vector::last_index_of;
using vector::map;
using vector::pop;
using vector::push;
using vector::remove_at;
using vector::reverse;
using vector::shift;
using vector::slice;
using vector::_some;
using vector::sort;
using vector::splice;
using vector::unshift;

// Re-export vector_double_allocator from object module
template<typename GC>
using vector_double_allocator = ruffle::core::avm2::object::vector_allocator<GC>;

/// Vector.<double>.concat implementation
template<typename GC>
Value<GC> concat(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_VECTOR_DOUBLE_H
