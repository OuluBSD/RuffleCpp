// C++ translation of core/src/avm2/object.rs
// AVM2 objects

#ifndef RUFFLE_CORE_AVM2_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_H

#include <memory>
#include <vector>
#include <optional>
#include <cstdint>
#include <functional>
#include <string>

// Forward declarations
namespace ruffle {
namespace core {

template<typename GC>
class AvmString;
template<typename GC>
class Mutation;

namespace avm2 {

template<typename GC>
class Activation;
template<typename GC>
class Value;
template<typename GC>
class Error;
template<typename GC>
class Multiname;
template<typename GC>
class Namespace;
template<typename GC>
class VTable;
template<typename GC>
class Class;
template<typename GC>
class ClassObject;
template<typename GC>
class FunctionObject;
template<typename GC>
class ScriptObject;
template<typename GC>
class ScriptObjectData;
template<typename GC>
class ScriptObjectWrapper;
template<typename GC>
class Domain;
template<typename GC>
class Event;
template<typename GC>
class DispatchList;
template<typename GC>
class RegExp;
template<typename GC>
class Font;
template<typename GC>
class BitmapData;
template<typename GC>
class TextFormat;
template<typename GC>
class NetStream;
template<typename GC>
class DisplayObject;

namespace function {
    template<typename GC>
    struct FunctionArgs;
}

namespace property {
    enum class Property;
}

namespace array {
    template<typename GC>
    class ArrayStorage;
}

namespace bytearray {
    class ByteArrayStorage;
}

namespace vector {
    template<typename GC>
    class VectorStorage;
}

namespace events {
    template<typename GC>
    class DispatchList;
}

// Forward declare all object types
template<typename GC>
class ArrayObject;
template<typename GC>
class BitmapDataObject;
template<typename GC>
class ByteArrayObject;
template<typename GC>
class ClassObject;
template<typename GC>
class Context3DObject;
template<typename GC>
class DateObject;
template<typename GC>
class DictionaryObject;
template<typename GC>
class DispatchObject;
template<typename GC>
class DomainObject;
template<typename GC>
class ErrorObject;
template<typename GC>
class EventObject;
template<typename GC>
class FileReferenceObject;
template<typename GC>
class FontObject;
template<typename GC>
class FunctionObject;
template<typename GC>
class IndexBuffer3DObject;
template<typename GC>
class LoaderInfoObject;
template<typename GC>
class LocalConnectionObject;
template<typename GC>
class MessageChannelObject;
template<typename GC>
class NamespaceObject;
template<typename GC>
class NetConnectionObject;
template<typename GC>
class NetStreamObject;
template<typename GC>
class Program3DObject;
template<typename GC>
class ProxyObject;
template<typename GC>
class QNameObject;
template<typename GC>
class RegExpObject;
template<typename GC>
class ResponderObject;
template<typename GC>
class ScriptObject;
template<typename GC>
class SecurityDomainObject;
template<typename GC>
class ShaderDataObject;
template<typename GC>
class SharedObjectObject;
template<typename GC>
class SocketObject;
template<typename GC>
class SoundObject;
template<typename GC>
class SoundChannelObject;
template<typename GC>
class SoundTransformObject;
template<typename GC>
class Stage3DObject;
template<typename GC>
class StageObject;
template<typename GC>
class StyleSheetObject;
template<typename GC>
class TextFormatObject;
template<typename GC>
class TextureObject;
template<typename GC>
class VectorObject;
template<typename GC>
class VertexBuffer3DObject;
template<typename GC>
class WorkerDomainObject;
template<typename GC>
class WorkerObject;
template<typename GC>
class XmlListObject;
template<typename GC>
class XmlObject;

// Weak object types
template<typename GC>
class ArrayObjectWeak;
template<typename GC>
class BitmapDataObjectWeak;
template<typename GC>
class ByteArrayObjectWeak;
template<typename GC>
class ClassObjectWeak;
template<typename GC>
class Context3DObjectWeak;
template<typename GC>
class DateObjectWeak;
template<typename GC>
class DictionaryObjectWeak;
template<typename GC>
class DispatchObjectWeak;
template<typename GC>
class DomainObjectWeak;
template<typename GC>
class ErrorObjectWeak;
template<typename GC>
class EventObjectWeak;
template<typename GC>
class FileReferenceObjectWeak;
template<typename GC>
class FontObjectWeak;
template<typename GC>
class FunctionObjectWeak;
template<typename GC>
class IndexBuffer3DObjectWeak;
template<typename GC>
class LoaderInfoObjectWeak;
template<typename GC>
class LocalConnectionObjectWeak;
template<typename GC>
class MessageChannelObjectWeak;
template<typename GC>
class NamespaceObjectWeak;
template<typename GC>
class NetConnectionObjectWeak;
template<typename GC>
class NetStreamObjectWeak;
template<typename GC>
class Program3DObjectWeak;
template<typename GC>
class ProxyObjectWeak;
template<typename GC>
class QNameObjectWeak;
template<typename GC>
class RegExpObjectWeak;
template<typename GC>
class ResponderObjectWeak;
template<typename GC>
class ScriptObjectWeak;
template<typename GC>
class SecurityDomainObjectWeak;
template<typename GC>
class ShaderDataObjectWeak;
template<typename GC>
class SharedObjectObjectWeak;
template<typename GC>
class SocketObjectWeak;
template<typename GC>
class SoundObjectWeak;
template<typename GC>
class SoundChannelObjectWeak;
template<typename GC>
class SoundTransformObjectWeak;
template<typename GC>
class Stage3DObjectWeak;
template<typename GC>
class StageObjectWeak;
template<typename GC>
class StyleSheetObjectWeak;
template<typename GC>
class TextFormatObjectWeak;
template<typename GC>
class TextureObjectWeak;
template<typename GC>
class VectorObjectWeak;
template<typename GC>
class VertexBuffer3DObjectWeak;
template<typename GC>
class WorkerDomainObjectWeak;
template<typename GC>
class WorkerObjectWeak;
template<typename GC>
class XmlListObjectWeak;
template<typename GC>
class XmlObjectWeak;

/// Hint for primitive coercion preference
enum class Hint {
    Number,
    String,
    None
};

/// Represents an object that can be directly interacted with by the AVM2 runtime.
/// This is the base trait for all AVM2 object types.
template<typename GC>
class TObject {
public:
    virtual ~TObject() = default;
    
    /// Get the base of this object.
    virtual ScriptObjectWrapper<GC> base() const = 0;
    
    /// Retrieve a local property of the object.
    virtual Result<Value<GC>, Error<GC>> get_property_local(
        const Multiname<GC>& name,
        Activation<GC>* activation
    ) = 0;
    
    /// Get a dynamic property on this Object by name.
    virtual std::optional<Value<GC>> get_dynamic_property(AvmString<GC> local_name) = 0;
    
    /// Set a local property of the object.
    virtual Result<void, Error<GC>> set_property_local(
        const Multiname<GC>& name,
        Value<GC> value,
        Activation<GC>* activation
    ) = 0;
    
    /// Set a dynamic property on this Object by name.
    virtual void set_dynamic_property(
        AvmString<GC> local_name,
        Value<GC> value,
        Mutation<GC>* mc
    ) = 0;
    
    /// Retrieve a slot by its index.
    virtual Value<GC> get_slot(uint32_t id) = 0;
    
    /// Set a slot by its index.
    virtual Result<void, Error<GC>> set_slot(
        uint32_t id,
        Value<GC> value,
        Activation<GC>* activation
    ) = 0;
    
    /// Indicates whether or not a property exists on an object.
    virtual bool has_property(const Multiname<GC>& name) = 0;
    
    /// Indicates whether or not a property exists on an object (own property).
    virtual bool has_own_property(const Multiname<GC>& name) = 0;
    
    /// Returns true if an object has one or more traits of a given name.
    virtual bool has_trait(const Multiname<GC>& name) = 0;
    
    /// Retrieve the __proto__ of a given object.
    virtual std::optional<Object<GC>> proto() const = 0;
    
    /// Change the __proto__ on this object.
    virtual void set_proto(Mutation<GC>* mc, Object<GC> proto) = 0;
    
    /// Get the next enumerant index in enumerant space.
    virtual Result<uint32_t, Error<GC>> get_next_enumerant(
        uint32_t last_index,
        Activation<GC>* activation
    ) = 0;
    
    /// Retrieve a given enumerable name by index.
    virtual Result<Value<GC>, Error<GC>> get_enumerant_name(
        uint32_t index,
        Activation<GC>* activation
    ) = 0;
    
    /// Retrieve a given enumerable value by index.
    virtual Result<Value<GC>, Error<GC>> get_enumerant_value(
        uint32_t index,
        Activation<GC>* activation
    ) = 0;
    
    /// Determine if a property is currently enumerable.
    virtual bool property_is_enumerable(AvmString<GC> name) const = 0;
    
    /// Mark a dynamic property on this object as enumerable.
    virtual void set_local_property_is_enumerable(
        Mutation<GC>* mc,
        AvmString<GC> name,
        bool is_enumerable
    ) = 0;
    
    /// Determine the type of primitive coercion this object would prefer.
    virtual Hint default_hint() const = 0;
    
    /// Implement the result of calling Object.prototype.toString on this object.
    virtual AvmString<GC> to_string(Mutation<GC>* mc) const = 0;
    
    /// Determine if this object is an instance of a given type.
    virtual bool is_of_type(Class<GC> test_class) const = 0;
    
    /// Get this object's vtable.
    virtual VTable<GC> vtable() const = 0;
    
    /// Get this object's class.
    virtual Class<GC> instance_class() const = 0;
    
    /// Get this object's class's name.
    virtual AvmString<GC> instance_of_class_name(Mutation<GC>* mc) const = 0;
    
    /// Sets a different vtable for object.
    virtual void set_vtable(Mutation<GC>* mc, VTable<GC> vtable) = 0;
    
    /// Get a raw pointer value for this object.
    virtual const void* as_ptr() const = 0;
};

/// Object enum - represents all AVM2 object types
template<typename GC>
class Object {
public:
    enum class Type {
        ScriptObject,
        FunctionObject,
        NamespaceObject,
        ArrayObject,
        StageObject,
        DomainObject,
        EventObject,
        DispatchObject,
        XmlObject,
        XmlListObject,
        RegExpObject,
        ByteArrayObject,
        LoaderInfoObject,
        ClassObject,
        VectorObject,
        SoundObject,
        SoundChannelObject,
        BitmapDataObject,
        DateObject,
        DictionaryObject,
        QNameObject,
        TextFormatObject,
        ProxyObject,
        ErrorObject,
        Stage3DObject,
        Context3DObject,
        IndexBuffer3DObject,
        VertexBuffer3DObject,
        TextureObject,
        Program3DObject,
        NetStreamObject,
        NetConnectionObject,
        ResponderObject,
        ShaderDataObject,
        SocketObject,
        FileReferenceObject,
        FontObject,
        LocalConnectionObject,
        SharedObjectObject,
        SoundTransformObject,
        StyleSheetObject,
        WorkerObject,
        WorkerDomainObject,
        MessageChannelObject,
        SecurityDomainObject
    };
    
private:
    Type type_;
    
    union Data {
        ScriptObject<GC>* script_object;
        FunctionObject<GC>* function_object;
        NamespaceObject<GC>* namespace_object;
        ArrayObject<GC>* array_object;
        StageObject<GC>* stage_object;
        DomainObject<GC>* domain_object;
        EventObject<GC>* event_object;
        DispatchObject<GC>* dispatch_object;
        XmlObject<GC>* xml_object;
        XmlListObject<GC>* xml_list_object;
        RegExpObject<GC>* regexp_object;
        ByteArrayObject<GC>* bytearray_object;
        LoaderInfoObject<GC>* loaderinfo_object;
        ClassObject<GC>* class_object;
        VectorObject<GC>* vector_object;
        SoundObject<GC>* sound_object;
        SoundChannelObject<GC>* sound_channel_object;
        BitmapDataObject<GC>* bitmapdata_object;
        DateObject<GC>* date_object;
        DictionaryObject<GC>* dictionary_object;
        QNameObject<GC>* qname_object;
        TextFormatObject<GC>* textformat_object;
        ProxyObject<GC>* proxy_object;
        ErrorObject<GC>* error_object;
        Stage3DObject<GC>* stage3d_object;
        Context3DObject<GC>* context3d_object;
        IndexBuffer3DObject<GC>* index_buffer_3d_object;
        VertexBuffer3DObject<GC>* vertex_buffer_3d_object;
        TextureObject<GC>* texture_object;
        Program3DObject<GC>* program_3d_object;
        NetStreamObject<GC>* netstream_object;
        NetConnectionObject<GC>* netconnection_object;
        ResponderObject<GC>* responder_object;
        ShaderDataObject<GC>* shader_data_object;
        SocketObject<GC>* socket_object;
        FileReferenceObject<GC>* file_reference_object;
        FontObject<GC>* font_object;
        LocalConnectionObject<GC>* local_connection_object;
        SharedObjectObject<GC>* shared_object_object;
        SoundTransformObject<GC>* soundtransform_object;
        StyleSheetObject<GC>* stylesheet_object;
        WorkerObject<GC>* worker_object;
        WorkerDomainObject<GC>* worker_domain_object;
        MessageChannelObject<GC>* message_channel_object;
        SecurityDomainObject<GC>* security_domain_object;
        
        Data() {}
        ~Data() {}
    } data_;
    
public:
    /// Check if two objects are pointer-equal
    static bool ptr_eq(const Object<GC>& a, const Object<GC>& b) {
        return a.as_ptr() == b.as_ptr();
    }
    
    /// Downcast methods
    std::optional<ScriptObject<GC>> as_script_object() const;
    std::optional<ClassObject<GC>> as_class_object() const;
    std::optional<FunctionObject<GC>> as_function_object() const;
    std::optional<NamespaceObject<GC>> as_namespace_object() const;
    std::optional<QNameObject<GC>> as_qname_object() const;
    std::optional<LoaderInfoObject<GC>> as_loader_info_object() const;
    std::optional<ArrayObject<GC>> as_array_object() const;
    std::optional<ByteArrayObject<GC>> as_bytearray_object() const;
    std::optional<VectorObject<GC>> as_vector_object() const;
    std::optional<StageObject<GC>> as_stage_object() const;
    std::optional<DomainObject<GC>> as_domain_object() const;
    std::optional<EventObject<GC>> as_event_object() const;
    std::optional<DispatchObject<GC>> as_dispatch_object() const;
    std::optional<FontObject<GC>> as_font_object() const;
    std::optional<RegExpObject<GC>> as_regexp_object() const;
    std::optional<SoundObject<GC>> as_sound_object() const;
    std::optional<SoundChannelObject<GC>> as_sound_channel() const;
    std::optional<BitmapDataObject<GC>> as_bitmap_data_object() const;
    std::optional<ShaderDataObject<GC>> as_shader_data() const;
    std::optional<DateObject<GC>> as_date_object() const;
    std::optional<DictionaryObject<GC>> as_dictionary_object() const;
    std::optional<TextFormatObject<GC>> as_text_format_object() const;
    std::optional<ErrorObject<GC>> as_error_object() const;
    std::optional<XmlObject<GC>> as_xml_object() const;
    std::optional<XmlListObject<GC>> as_xml_list_object() const;
    std::optional<Context3DObject<GC>> as_context_3d() const;
    std::optional<IndexBuffer3DObject<GC>> as_index_buffer() const;
    std::optional<VertexBuffer3DObject<GC>> as_vertex_buffer() const;
    std::optional<Program3DObject<GC>> as_program_3d() const;
    std::optional<Stage3DObject<GC>> as_stage_3d() const;
    std::optional<TextureObject<GC>> as_texture() const;
    std::optional<NetStreamObject<GC>> as_netstream_object() const;
    std::optional<ResponderObject<GC>> as_responder() const;
    std::optional<NetConnectionObject<GC>> as_net_connection() const;
    std::optional<SocketObject<GC>> as_socket() const;
    std::optional<LocalConnectionObject<GC>> as_local_connection_object() const;
    std::optional<FileReferenceObject<GC>> as_file_reference() const;
    std::optional<SharedObjectObject<GC>> as_shared_object() const;
    std::optional<SoundTransformObject<GC>> as_sound_transform() const;
    std::optional<StyleSheetObject<GC>> as_style_sheet() const;
    
    /// Get underlying storage types
    std::optional<typename array::ArrayStorage<GC>> as_array_storage() const;
    std::optional<typename bytearray::ByteArrayStorage> as_bytearray() const;
    std::optional<typename vector::VectorStorage<GC>> as_vector_storage() const;
    
    /// Get DisplayObject if this object has one
    std::optional<DisplayObject<GC>> as_display_object() const;
    
    /// Get application domain if this object has one
    std::optional<Domain<GC>> as_application_domain() const;
    
    /// Get event if this object has one
    std::optional<typename events::DispatchList<GC>> as_dispatch_mut(Mutation<GC>* mc) const;
    
    /// Get font if this object has one
    std::optional<Font<GC>> as_font() const;
    
    /// Get regexp if this object has one
    std::optional<RegExp<GC>> as_regexp() const;
    
    /// Get bitmap data if this object has one
    std::optional<BitmapData<GC>> as_bitmap_data() const;
    
    /// Get text format if this object has one
    std::optional<TextFormat> as_text_format() const;
    
    /// Get netstream if this object has one
    std::optional<NetStream<GC>> as_netstream() const;
    
    /// Get raw pointer
    const void* as_ptr() const;
    
    /// Equality operators
    bool operator==(const Object<GC>& other) const {
        return ptr_eq(*this, other);
    }
    
    bool operator!=(const Object<GC>& other) const {
        return !(*this == other);
    }
};

/// Weak object enum - weak references to AVM2 objects
template<typename GC>
class WeakObject {
public:
    enum class Type {
        ScriptObject,
        FunctionObject,
        NamespaceObject,
        ArrayObject,
        StageObject,
        DomainObject,
        EventObject,
        DispatchObject,
        XmlObject,
        XmlListObject,
        RegExpObject,
        ByteArrayObject,
        LoaderInfoObject,
        ClassObject,
        VectorObject,
        SoundObject,
        SoundChannelObject,
        BitmapDataObject,
        DateObject,
        DictionaryObject,
        QNameObject,
        TextFormatObject,
        ProxyObject,
        ErrorObject,
        Stage3DObject,
        Context3DObject,
        IndexBuffer3DObject,
        VertexBuffer3DObject,
        TextureObject,
        Program3DObject,
        NetStreamObject,
        NetConnectionObject,
        ResponderObject,
        ShaderDataObject,
        SocketObject,
        FileReferenceObject,
        FontObject,
        LocalConnectionObject,
        SharedObjectObject,
        SoundTransformObject,
        StyleSheetObject,
        WorkerObject,
        WorkerDomainObject,
        MessageChannelObject,
        SecurityDomainObject
    };
    
    /// Get raw pointer
    const void* as_ptr() const;
    
    /// Upgrade weak reference to strong reference
    std::optional<Object<GC>> upgrade(Mutation<GC>* mc) const;
};

/// Abstract class allocator for classes that are not constructible
template<typename GC>
Result<Object<GC>, Error<GC>> abstract_class_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
);

/// Construct call handler for classes that are constructed when called
template<typename GC>
Result<Value<GC>, Error<GC>> construct_call_handler(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_H
