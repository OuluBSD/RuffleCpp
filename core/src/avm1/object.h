// C++ translation of core/src/avm1/object.rs
//! Object trait to expose objects to AVM

#ifndef RUFFLE_CORE_AVM1_OBJECT_H
#define RUFFLE_CORE_AVM1_OBJECT_H

#include <memory>
#include <variant>
#include <cstdint>
#include <cstddef>
#include <optional>
#include <functional>
#include <cell>
#include <mutex>

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
    class FunctionObject;
    class UpdateContext;

    namespace gc_arena {
        template<typename T>
        class Gc;
        class Mutation;
    }

    namespace display_object {
        template<typename GC>
        class DisplayObject;
        template<typename GC>
        class MovieClip;
        template<typename GC>
        class Avm1Button;
        template<typename GC>
        class EditText;
        template<typename GC>
        class Video;
    }

    namespace bitmap {
        template<typename GC>
        class BitmapData;
    }

    namespace html {
        template<typename GC>
        class TextFormat;
    }

    namespace streams {
        template<typename GC>
        class NetStream;
    }

    namespace avm1 {
        namespace globals {
            template<typename GC>
            class BevelFilter;
            template<typename GC>
            class BlurFilter;
            template<typename GC>
            class ColorMatrixFilter;
            template<typename GC>
            class ColorTransformObject;
            template<typename GC>
            class ConvolutionFilter;
            template<typename GC>
            class Date;
            template<typename GC>
            class DisplacementMapFilter;
            template<typename GC>
            class DropShadowFilter;
            template<typename GC>
            class FileReferenceObject;
            template<typename GC>
            class GlowFilter;
            template<typename GC>
            class GradientFilter;
            template<typename GC>
            class LocalConnection;
            template<typename GC>
            class NetConnection;
            template<typename GC>
            class SharedObject;
            template<typename GC>
            class Sound;
            template<typename GC>
            class StyleSheetObject;
            template<typename GC>
            class TextSnapshotObject;
            template<typename GC>
            class TransformObject;
            template<typename GC>
            class Xml;
            template<typename GC>
            class XmlSocket;
        }

        namespace xml {
            template<typename GC>
            class XmlNode;
        }

        namespace object {
            class SuperObject;
        }
    }
}
}

namespace smallvec {
    template<typename T, size_t N>
    class SmallVec : public std::vector<T> {};
}

namespace ruffle {
namespace core {
namespace avm1 {

using gc_arena::Gc;
using gc_arena::Mutation;

// Forward declare Object
template<typename GC>
class Object;

/// Small wrapper struct to keep boxed f64s word-sized on every architecture.
template<typename GC>
struct BoxedF64 {
#ifdef TARGET_POINTER_WIDTH_64
    double value;
#else
    Gc<double> value;
#endif

    static BoxedF64<GC> new(Mutation<GC>* mc, double val) {
        return BoxedF64<GC>{
#ifdef TARGET_POINTER_WIDTH_64
            val
#else
            Gc<double>::new(mc, val)
#endif
        };
    }

    double value() const {
#ifdef TARGET_POINTER_WIDTH_64
        return value;
#else
        return *value;
#endif
    }
};

/// Native object types that can be wrapped by AVM1 Object
template<typename GC>
class NativeObject {
public:
    enum class Type {
        None,
        Super,
        Bool,
        Number,
        String,
        Array,
        Function,
        MovieClip,
        Button,
        EditText,
        Video,
        Date,
        BlurFilter,
        BevelFilter,
        GlowFilter,
        DropShadowFilter,
        ColorMatrixFilter,
        DisplacementMapFilter,
        ConvolutionFilter,
        GradientBevelFilter,
        GradientGlowFilter,
        ColorTransform,
        Transform,
        TextFormat,
        NetStream,
        BitmapData,
        Xml,
        XmlNode,
        SharedObject,
        XmlSocket,
        FileReference,
        NetConnection,
        LocalConnection,
        Sound,
        StyleSheet,
        TextSnapshot
    };

private:
    using VariantType = std::variant<
        std::monostate,                           // None
        display_object::SuperObject<GC>,          // Super
        bool,                                     // Bool
        BoxedF64<GC>,                             // Number
        AvmString<GC>,                            // String
        std::monostate,                           // Array
        Gc<FunctionObject<GC>>,                   // Function
        display_object::MovieClip<GC>,            // MovieClip
        display_object::Avm1Button<GC>,           // Button
        display_object::EditText<GC>,             // EditText
        display_object::Video<GC>,                // Video
        Gc<std::cell::Cell<Date<GC>>>,            // Date
        globals::BlurFilter<GC>,                  // BlurFilter
        globals::BevelFilter<GC>,                 // BevelFilter
        globals::GlowFilter<GC>,                  // GlowFilter
        globals::DropShadowFilter<GC>,            // DropShadowFilter
        globals::ColorMatrixFilter<GC>,           // ColorMatrixFilter
        globals::DisplacementMapFilter<GC>,       // DisplacementMapFilter
        globals::ConvolutionFilter<GC>,           // ConvolutionFilter
        globals::GradientFilter<GC>,              // GradientBevelFilter
        globals::GradientFilter<GC>,              // GradientGlowFilter
        Gc<ColorTransformObject<GC>>,             // ColorTransform
        globals::TransformObject<GC>,             // Transform
        Gc<std::mutex<TextFormat<GC>>>,           // TextFormat
        streams::NetStream<GC>,                   // NetStream
        bitmap::BitmapData<GC>,                   // BitmapData
        globals::Xml<GC>,                         // Xml
        xml::XmlNode<GC>,                         // XmlNode
        Gc<std::mutex<SharedObject<GC>>>,         // SharedObject
        globals::XmlSocket<GC>,                   // XmlSocket
        FileReferenceObject<GC>,                  // FileReference
        globals::NetConnection<GC>,               // NetConnection
        globals::LocalConnection<GC>,             // LocalConnection
        globals::Sound<GC>,                       // Sound
        globals::StyleSheetObject<GC>,            // StyleSheet
        globals::TextSnapshotObject<GC>           // TextSnapshot
    >;

    VariantType data_;
    Type type_;

public:
    static constexpr size_t MAX_SIZE = 2 * sizeof(uintptr_t);
    static_assert(sizeof(VariantType) <= MAX_SIZE, "NativeObject must fit in two pointers");

    NativeObject() : data_(std::monostate{}), type_(Type::None) {}

    Type type() const { return type_; }

    /// Get the underlying display object, if this is a display object
    std::optional<display_object::DisplayObject<GC>> as_display_object() const {
        switch (type_) {
            case Type::MovieClip:
                return display_object::DisplayObject<GC>::MovieClip(std::get<display_object::MovieClip<GC>>(data_));
            case Type::Button:
                return display_object::DisplayObject<GC>::Avm1Button(std::get<display_object::Avm1Button<GC>>(data_));
            case Type::EditText:
                return display_object::DisplayObject<GC>::EditText(std::get<display_object::EditText<GC>>(data_));
            case Type::Video:
                return display_object::DisplayObject<GC>::Video(std::get<display_object::Video<GC>>(data_));
            default:
                return std::nullopt;
        }
    }

    /// Check if this is a Super object
    bool is_super() const { return type_ == Type::Super; }

    /// Get as Super object
    std::optional<display_object::SuperObject<GC>> as_super_object() const {
        if (type_ == Type::Super) {
            return std::get<display_object::SuperObject<GC>>(data_);
        }
        return std::nullopt;
    }
};

/// Execution reason for function calls
enum class ExecutionReason {
    Normal,
    Return,
    Throw,
    Break,
    Continue
};

/// Execution name for function calls
class ExecutionName {
public:
    enum class Kind {
        Static,
        Dynamic
    };

private:
    Kind kind_;
    const char* static_name_;
    AvmString<GC> dynamic_name_;

public:
    static ExecutionName Static(const char* name) {
        return ExecutionName{Kind::Static, name, AvmString<GC>{}};
    }

    static ExecutionName Dynamic(AvmString<GC> name) {
        return ExecutionName{Kind::Dynamic, nullptr, name};
    }
};

/// Object handle - shared pointer to Object
template<typename GC>
using ObjectHandle = std::shared_ptr<Object<GC>>;

/// Weak reference to Object
template<typename GC>
using ObjectWeak = std::weak_ptr<Object<GC>>;

/// Object trait to expose objects to AVM1
template<typename GC>
class Object {
public:
    using Native = NativeObject<GC>;

private:
    // Object implementation details would go here
    // This is a forward declaration with key methods

public:
    /// Retrieve a named property from the object, or its prototype.
    /// Returns `None` if the property couldn't be found.
    std::optional<Value<GC>> get_opt(
        AvmString<GC> name,
        Activation<GC>* activation,
        bool call_resolve_fn
    ) {
        // Implementation would search prototype chain
        return std::nullopt;
    }

    /// Retrieve a named property from the object, or its prototype.
    /// If the property couldn't be found, try to find and call a `__resolve` handler.
    Value<GC> get(
        AvmString<GC> name,
        Activation<GC>* activation
    ) {
        return get_opt(name, activation, true).value_or(Value<GC>::undefined());
    }

    /// Retrieve a non-virtual property from the object, or its prototype.
    Value<GC> get_stored(
        AvmString<GC> name,
        Activation<GC>* activation
    ) {
        // Implementation would search without calling virtual getters
        return Value<GC>::undefined();
    }

    /// Set a named property on this object, or its prototype.
    void set(
        AvmString<GC> name,
        Value<GC> value,
        Activation<GC>* activation
    ) -> Result<void, Error<GC>> {
        if (name.is_empty()) {
            return Ok();
        }

        // Implementation would set property, handling prototype chain and watchers
        return Ok();
    }

    /// Call a method on the object.
    ///
    /// It is highly recommended to use this convenience method to perform
    /// method calls. It is morally equivalent to an AVM1 `ActionCallMethod`
    /// opcode.
    Value<GC> call_method(
        AvmString<GC> name,
        const std::vector<Value<GC>>& args,
        Activation<GC>* activation,
        ExecutionReason reason
    ) {
        // Implementation would call method with proper this binding
        return Value<GC>::undefined();
    }

    /// Determine if this object is an instance of a class.
    bool is_instance_of(
        Activation<GC>* activation,
        ObjectHandle<GC> prototype
    ) -> Result<bool, Error<GC>> {
        // Implementation would check prototype chain and interfaces
        return Ok(false);
    }

    /// Get the underlying XML node for this object, if it exists.
    std::optional<xml::XmlNode<GC>> as_xml_node() const {
        // Implementation would return XML node if applicable
        return std::nullopt;
    }

    /// Check if this object is in the prototype chain of the specified test object.
    bool is_prototype_of(Activation<GC>* activation, ObjectHandle<GC> other) const {
        // Implementation would check prototype chain
        return false;
    }

    /// Check pointer equality of two objects
    static bool ptr_eq(ObjectHandle<GC> a, ObjectHandle<GC> b) {
        return a.get() == b.get();
    }

    /// Get raw pointer for comparison
    const Object<GC>* as_ptr() const {
        return this;
    }

    /// Get native object type
    std::optional<Native> native() const {
        // Implementation would return native type
        return std::nullopt;
    }

    /// Get native object type, excluding super objects
    std::optional<Native> native_no_super() const {
        // Implementation would return native type
        return std::nullopt;
    }

    /// Check if object has own property
    bool has_own_property(Activation<GC>* activation, AvmString<GC> name) const {
        // Implementation would check for own property
        return false;
    }

    /// Check if object has own virtual property
    bool has_own_virtual(Activation<GC>* activation, AvmString<GC> name) const {
        // Implementation would check for virtual property
        return false;
    }

    /// Get getter for property
    std::optional<ObjectHandle<GC>> getter(AvmString<GC> name, Activation<GC>* activation) const {
        // Implementation would return getter if exists
        return std::nullopt;
    }

    /// Get setter for property
    std::optional<ObjectHandle<GC>> setter(AvmString<GC> name, Activation<GC>* activation) const {
        // Implementation would return setter if exists
        return std::nullopt;
    }

    /// Get local stored property
    std::optional<Value<GC>> get_local_stored(AvmString<GC> name, Activation<GC>* activation) const {
        // Implementation would return stored property
        return std::nullopt;
    }

    /// Set local property
    Result<void, Error<GC>> set_local(
        AvmString<GC> name,
        Value<GC> value,
        Activation<GC>* activation,
        ObjectHandle<GC> this_obj
    ) {
        // Implementation would set local property
        return Ok();
    }

    /// Call watcher
    Result<void, Error<GC>> call_watcher(
        Activation<GC>* activation,
        AvmString<GC> name,
        Value<GC>* value,
        ObjectHandle<GC> this_obj
    ) {
        // Implementation would call watcher if exists
        return Ok();
    }

    /// Get prototype
    Value<GC> proto(Activation<GC>* activation) const {
        // Implementation would return prototype
        return Value<GC>::null();
    }

    /// Get interfaces
    const std::vector<ObjectHandle<GC>>& interfaces() const {
        // Implementation would return interfaces
        static std::vector<ObjectHandle<GC>> empty;
        return empty;
    }
};

/// Tag type for Object pointer
struct ObjectPtr {};

/// Perform a prototype lookup of a given object.
///
/// This function returns both the `Value` and the prototype depth from which
/// it was grabbed from. If the property did not resolve, then it returns
/// `Ok(None)`.
///
/// The prototype depth can and should be used to populate the `depth`
/// parameter necessary to make `super` work.
template<typename GC>
std::optional<std::pair<Value<GC>, uint8_t>> search_prototype(
    Value<GC> proto,
    AvmString<GC> name,
    Activation<GC>* activation,
    ObjectHandle<GC> this_obj,
    bool call_resolve_fn
) {
    uint8_t depth = 0;
    Value<GC> orig_proto = proto;

    while (auto p = proto.as_object()) {
        if (depth == 255) {
            return Error<GC>::prototype_recursion_limit();
        }

        // Check for getter
        if (auto getter = p->getter(name, activation)) {
            if (auto exec = getter->as_function()) {
                auto result = exec->exec(
                    ExecutionName::Static("[Getter]"),
                    activation,
                    this_obj->into(),
                    1,
                    {},
                    ExecutionReason::Special,
                    *getter
                );

                if (result.is_thrown_value()) {
                    return result.error();
                } else if (result.is_special_recursion_limit()) {
                    // Fall back to local resolution for compatibility with SWF<7
                } else {
                    return std::make_pair(result.unwrap_or(Value<GC>::undefined()), depth);
                }
            }
        }

        // Check for stored value
        if (auto value = p->get_local_stored(name, activation)) {
            return std::make_pair(*value, depth);
        }

        proto = p->proto(activation);
        depth++;
    }

    // Check for __resolve
    if (call_resolve_fn) {
        if (auto resolve = find_resolve_method(orig_proto, activation)) {
            auto result = resolve->call(
                atom("__resolve"),
                activation,
                this_obj->into(),
                {name.into()}
            );
            return std::make_pair(result, 0);
        }
    }

    return std::nullopt;
}

/// Finds the appropriate `__resolve` method for an object, searching its hierarchy too.
template<typename GC>
std::optional<ObjectHandle<GC>> find_resolve_method(
    Value<GC> proto,
    Activation<GC>* activation
) {
    uint8_t depth = 0;

    while (auto p = proto.as_object()) {
        if (depth == 255) {
            return Error<GC>::prototype_recursion_limit();
        }

        auto resolve = p->get_local_stored(atom("__resolve"), activation);
        // FP completely skips over primitives (but not over non-function objects)
        if (auto value = resolve.as_object()) {
            return *value;
        }

        proto = p->proto(activation);
        depth++;
    }

    return std::nullopt;
}

} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_OBJECT_H
