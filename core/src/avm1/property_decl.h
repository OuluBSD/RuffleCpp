// C++ translation of core/src/avm1/property_decl.rs
//! Declarative property definitions for AVM1

#ifndef RUFFLE_CORE_AVM1_PROPERTY_DECL_H
#define RUFFLE_CORE_AVM1_PROPERTY_DECL_H

#include <cstdint>
#include <vector>
#include <functional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace string {
    template<typename T> class WStr;
    template<typename T> class StringContext;
    template<typename T> class AvmString;
    
    template<typename GC>
    struct HasStringContext {
        virtual const StringContext<GC>& strings_ref() const = 0;
    };
}
namespace avm1 {
    template<typename T> class Object;
    template<typename T> class Value;
    template<typename T> class FunctionObject;
    
    namespace function {
        template<typename GC>
        using NativeFunction = std::function<Value<GC>(void*, Value<GC>, const std::vector<Value<GC>>&)>;
        
        template<typename GC>
        using TableNativeFunction = std::function<Value<GC>(void*, Value<GC>, const std::vector<Value<GC>>&, uint16_t)>;
    }
    
    namespace property {
        enum class Attribute : uint8_t {
            NONE = 0,
            DONT_ENUM = 1,
            DONT_DELETE = 2,
            READ_ONLY = 4,
        };
        
        inline Attribute operator|(Attribute a, Attribute b) {
            return static_cast<Attribute>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
        }
        
        inline Attribute from_bits_truncate(uint8_t bits) {
            return static_cast<Attribute>(bits);
        }
    }
}
}
}

namespace gc_arena {
    template<typename T>
    struct Mutation;
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace property_decl {

/// Declaration context for defining properties
template<typename GC>
class DeclContext : public string::HasStringContext<GC> {
public:
    string::StringContext<GC>* strings;
    Object<GC> object_proto;
    Object<GC> fn_proto;

    DeclContext(
        string::StringContext<GC>* strings,
        Object<GC> object_proto,
        Object<GC> fn_proto
    ) : strings(strings), object_proto(object_proto), fn_proto(fn_proto) {}

    const string::StringContext<GC>& strings_ref() const override {
        return *strings;
    }

    gc_arena::Mutation<GC>* gc() const {
        return strings->gc();
    }

    /// Define properties on an object from declarations
    void define_properties_on(Object<GC> obj, const std::vector<Declaration<GC>>& decls);

    /// Create an empty class
    SystemClass<GC> empty_class(Object<GC> super_proto);

    /// Create a class with a normal constructor
    SystemClass<GC> class_(
        function::NativeFunction<GC> function,
        Object<GC> super_proto
    );

    /// Create a class with a special native constructor
    SystemClass<GC> native_class(
        function::NativeFunction<GC> constructor,
        std::optional<function::NativeFunction<GC>> function,
        Object<GC> super_proto
    );

    /// Create a native class with custom prototype
    SystemClass<GC> native_class_with_proto(
        function::NativeFunction<GC> constructor,
        std::optional<function::NativeFunction<GC>> function,
        Object<GC> proto
    );
};

/// A system class with prototype and constructor
template<typename GC>
struct SystemClass {
    Object<GC> proto;
    Object<GC> constr;
};

/// Kind of declaration
template<typename GC>
class DeclKind {
public:
    enum Type {
        TableProperty,
        Property,
        TableMethod,
        Method,
        TableFunction,
        Function,
        String,
        Value
    };

    Type type;
    
    // TableProperty data
    function::TableNativeFunction<GC> table_native;
    uint16_t getter_index;
    std::optional<uint16_t> setter_index;
    
    // Property data
    function::NativeFunction<GC> getter;
    std::optional<function::NativeFunction<GC>> setter;
    
    // Method/TableMethod data
    function::NativeFunction<GC> method;
    
    // String data
    std::vector<uint8_t> string_value;
    
    // Value data
    Value<GC> value;

    // Constructors for different kinds
    static DeclKind table_property(
        function::TableNativeFunction<GC> native,
        uint16_t getter,
        std::optional<uint16_t> setter
    ) {
        DeclKind result;
        result.type = TableProperty;
        result.table_native = native;
        result.getter_index = getter;
        result.setter_index = setter;
        return result;
    }

    static DeclKind property(
        function::NativeFunction<GC> getter,
        std::optional<function::NativeFunction<GC>> setter
    ) {
        DeclKind result;
        result.type = Property;
        result.getter = getter;
        result.setter = setter;
        return result;
    }

    static DeclKind table_method(
        function::TableNativeFunction<GC> native,
        uint16_t index
    ) {
        DeclKind result;
        result.type = TableMethod;
        result.table_native = native;
        result.getter_index = index;
        return result;
    }

    static DeclKind method(function::NativeFunction<GC> f) {
        DeclKind result;
        result.type = Method;
        result.method = f;
        return result;
    }

    static DeclKind table_function(
        function::TableNativeFunction<GC> native,
        uint16_t index
    ) {
        DeclKind result;
        result.type = TableFunction;
        result.table_native = native;
        result.getter_index = index;
        return result;
    }

    static DeclKind function_(function::NativeFunction<GC> f) {
        DeclKind result;
        result.type = Function;
        result.method = f;
        return result;
    }

    static DeclKind string_(std::vector<uint8_t> s) {
        DeclKind result;
        result.type = String;
        result.string_value = std::move(s);
        return result;
    }

    static DeclKind value(Value<GC> v) {
        DeclKind result;
        result.type = Value;
        result.value = v;
        return result;
    }
};

/// A property/method/field declaration
template<typename GC>
struct Declaration {
    std::vector<uint8_t> name;
    DeclKind<GC> kind;
    property::Attribute attributes;

    /// Define this declaration on an object
    Value<GC> define_on(
        string::StringContext<GC>* context,
        Object<GC> obj,
        Object<GC> fn_proto
    );
};

/// Type alias for static declarations
/// In C++, this is a function that returns a vector of declarations
template<typename GC>
using StaticDeclarations = std::function<std::vector<Declaration<GC>>(DeclContext<GC>*)>;

/// Helper class for converting values to DeclKind
template<typename GC, typename T>
class DeclValueConvertible {
public:
    T value;
    
    explicit DeclValueConvertible(T v) : value(v) {}
    
    DeclKind<GC> convert();
};

// Specializations for common types
template<typename GC>
DeclKind<GC> DeclValueConvertible<GC, int32_t>::convert() {
    return DeclKind<GC>::value(Value<GC>::number(static_cast<double>(value)));
}

template<typename GC>
DeclKind<GC> DeclValueConvertible<GC, double>::convert() {
    return DeclKind<GC>::value(Value<GC>::number(value));
}

template<typename GC>
DeclKind<GC> DeclValueConvertible<GC, bool>::convert() {
    return DeclKind<GC>::value(Value<GC>::boolean(value));
}

template<typename GC>
DeclKind<GC> DeclValueConvertible<GC, Object<GC>*>::convert() {
    return DeclKind<GC>::value(Value<GC>::object(value));
}

template<typename GC>
DeclKind<GC> DeclValueConvertible<GC, const char*>::convert() {
    // Assert ASCII
    std::vector<uint8_t> bytes;
    for (const char* p = value; *p; ++p) {
        bytes.push_back(static_cast<uint8_t>(*p));
    }
    return DeclKind<GC>::string(bytes);
}

} // namespace property_decl
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_PROPERTY_DECL_H
