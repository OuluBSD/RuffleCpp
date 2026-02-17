// C++ translation of core/src/avm2/amf.rs
//! AMF (Action Message Format) serialization and deserialization

#ifndef RUFFLE_CORE_AVM2_AMF_H
#define RUFFLE_CORE_AVM2_AMF_H

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class AvmString;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    template<typename GC>
    class Activation;
    template<typename GC>
    class Object;
    template<typename GC>
    class ArrayObject;
    template<typename GC>
    class ArrayStorage;
    template<typename GC>
    class ByteArrayObject;
    template<typename GC>
    class ByteArrayStorage;
    template<typename GC>
    class VectorObject;
    template<typename GC>
    class VectorStorage;
    template<typename GC>
    class Class;
    template<typename GC>
    class ClassObject;
    template<typename GC>
    class ScriptObject;
    template<typename GC>
    class DateObject;
    template<typename GC>
    class XmlObject;
    template<typename GC>
    class DictionaryObject;

    namespace avm2 {
        class Avm2;
    }

    namespace gc_arena {
        template<typename T>
        class Mutation;
    }
}
}

// Forward declaration for flash_lso types
namespace flash_lso {
    namespace types {
        enum class AMFVersion;
        class Value;
        class Element;
        class Lso;
        class Attribute;
        class ClassDefinition;
        struct ObjectId;
    }
}

namespace fnv {
    template<typename K, typename V, typename H>
    class FnvHashMap;
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace amf {

/// Object table for tracking serialized objects
template<typename GC>
using ObjectTable = std::unordered_map<
    Object<GC>,
    std::shared_ptr<flash_lso::types::Value>
>;

/// Serialize a Value to an AmfValue
template<typename GC>
std::optional<flash_lso::types::Value> serialize_value(
    Activation<GC>* activation,
    Value<GC> elem,
    flash_lso::types::AMFVersion amf_version,
    ObjectTable<GC>& object_table
);

/// Serialize an Object and any children to an AMF object
template<typename GC>
void recursive_serialize(
    Activation<GC>* activation,
    Object<GC> obj,
    std::vector<flash_lso::types::Element>& elements,
    std::vector<std::string>* static_properties,
    flash_lso::types::AMFVersion amf_version,
    ObjectTable<GC>& object_table
);

/// Get or create an element for serialization
template<typename GC>
std::optional<flash_lso::types::Element> get_or_create_element(
    Activation<GC>* activation,
    const std::string& name,
    Value<GC> val,
    ObjectTable<GC>& object_table,
    flash_lso::types::AMFVersion amf_version
);

/// Get or create a value for serialization
template<typename GC>
std::optional<std::shared_ptr<flash_lso::types::Value>> get_or_create_value(
    Activation<GC>* activation,
    Value<GC> val,
    ObjectTable<GC>& object_table,
    flash_lso::types::AMFVersion amf_version
);

/// Deserialize an AmfValue to a Value
template<typename GC>
Value<GC> deserialize_value(
    Activation<GC>* activation,
    const flash_lso::types::Value& val
);

/// Deserialize an AmfValue to a Value (internal implementation)
template<typename GC>
Value<GC> deserialize_value_impl(
    Activation<GC>* activation,
    const flash_lso::types::Value& val,
    std::map<flash_lso::types::ObjectId, Object<GC>>& object_map
);

/// Deserialize an LSO into an object containing the properties stored
template<typename GC>
Object<GC> deserialize_lso(
    Activation<GC>* activation,
    const flash_lso::types::Lso& lso
);

/// Convert an alias string to a Class object
template<typename GC>
ClassObject<GC> alias_to_class(
    Activation<GC>* activation,
    AvmString<GC> alias
);

/// Convert a Class to an alias string
template<typename GC>
std::string class_to_alias(
    Activation<GC>* activation,
    Class<GC> class_
);

} // namespace amf
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_AMF_H
