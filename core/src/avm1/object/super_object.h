// C++ translation of core/src/avm1/object/super_object.rs
//! Special object that implements `super`

#ifndef RUFFLE_CORE_AVM1_OBJECT_SUPER_OBJECT_H
#define RUFFLE_CORE_AVM1_OBJECT_SUPER_OBJECT_H

#include <cstdint>
#include <optional>
#include <vector>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace string {
    template<typename T> class AvmString;
}
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Value;
    template<typename T> class Object;
    template<typename T> class NativeObject;
    namespace function {
        enum class ExecutionReason;
    }
    namespace object {
        class ExecutionName;
        template<typename GC>
        std::optional<std::pair<Value<GC>, uint8_t>> search_prototype(
            Value<GC> proto,
            typename string::AvmString<GC> name,
            Activation<GC>* activation,
            Object<GC> this_obj,
            bool call_resolve_fn
        );
    }
}
namespace common {
namespace utils {
    enum class ZeroU8 : uint8_t;
}
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace object {

/// Special object that implements `super`
///
/// A `SuperObject` references all data from another object, but with one layer
/// of prototyping removed. It's as if the given object had been constructed
/// with its parent class.
template<typename GC>
class SuperObject {
public:
    SuperObject() = default;

    /// Construct a `super` for an incoming stack frame.
    SuperObject(Object<GC> this_obj, uint8_t depth)
        : this_(this_obj)
        , depth_(depth)
        , niche_(common::utils::ZeroU8::Zero)
    {}

    /// Get the `this` object
    Object<GC> this_obj() const {
        return this_;
    }

    /// Get the prototype depth
    uint8_t depth() const {
        return depth_;
    }

    /// Get the base prototype
    std::optional<Object<GC>> base_proto(Activation<GC>* activation);

    /// Get the prototype
    Value<GC> proto(Activation<GC>* activation);

    /// Call the super constructor
    Result<Value<GC>, Error<GC>> call(
        ExecutionName name,
        Activation<GC>* activation,
        const std::vector<Value<GC>>& args
    );

    /// Call a method on the super object
    Result<Value<GC>, Error<GC>> call_method(
        typename string::AvmString<GC> name,
        const std::vector<Value<GC>>& args,
        Activation<GC>* activation,
        function::ExecutionReason reason
    );

private:
    /// The object present as `this` throughout the superchain.
    Object<GC> this_;

    /// The prototype depth of the currently-executing method.
    uint8_t depth_;

    /// Adds a niche, so that enums containing this type can use it for their discriminant.
    common::utils::ZeroU8 niche_;
};

} // namespace object
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_OBJECT_SUPER_OBJECT_H
