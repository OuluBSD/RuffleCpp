// C++ translation of core/src/avm1/globals/xml.rs
// XML class for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_XML_H
#define RUFFLE_CORE_AVM1_GLOBALS_XML_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    struct SystemClass;
    enum class Attribute;
}
namespace string {
    template<typename T> class AvmString;
    template<typename T> class WString;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace xml {

/// XML status codes for parse errors
enum class XmlStatus : int8_t {
    NoError = 0,
    CdataNotTerminated = -2,
    DeclNotTerminated = -3,
    DoctypeNotTerminated = -4,
    CommentNotTerminated = -5,
    ElementMalformed = -6,
    OutOfMemory = -7,
    AttributeNotTerminated = -8,
    MismatchedStart = -9,
    MismatchedEnd = -10
};

/// Property declarations for XML prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the XML class
///
/// @param context The declaration context for creating the class
/// @param super_proto The prototype for the class
/// @return The created SystemClass
template<typename GC>
SystemClass<GC> create_class(DeclContext<GC>* context, Object<GC>* super_proto);

/// XML (document) constructor
///
/// @param activation The current activation context
/// @param this_obj The XML object being constructed
/// @param args Arguments (optional XML string at index 0)
/// @return The constructed XML object
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XML.createElement
///
/// Creates a new XML element node.
///
/// @param activation The current activation context
/// @param this_obj The XML object
/// @param args Arguments (element name at index 0)
/// @return New XMLNode element or Value::Undefined
template<typename GC>
Value<GC> create_element(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XML.createTextNode
///
/// Creates a new XML text node.
///
/// @param activation The current activation context
/// @param this_obj The XML object
/// @param args Arguments (text content at index 0)
/// @return New XMLNode text node or Value::Undefined
template<typename GC>
Value<GC> create_text_node(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XML.getBytesLoaded
///
/// Returns the number of bytes loaded.
///
/// @param activation The current activation context
/// @param this_obj The XML object
/// @param args Arguments (unused)
/// @return Bytes loaded (from _bytesLoaded property)
template<typename GC>
Value<GC> get_bytes_loaded(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XML.getBytesTotal
///
/// Returns the total number of bytes.
///
/// @param activation The current activation context
/// @param this_obj The XML object
/// @param args Arguments (unused)
/// @return Total bytes (from _bytesTotal property)
template<typename GC>
Value<GC> get_bytes_total(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XML.parseXML
///
/// Parses an XML string into the document.
///
/// @param activation The current activation context
/// @param this_obj The XML object
/// @param args Arguments (XML string at index 0)
/// @return Value::Undefined
template<typename GC>
Value<GC> parse_xml(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XML.sendAndLoad
///
/// Sends XML data and loads the response.
///
/// @param activation The current activation context
/// @param this_obj The XML object
/// @param args Arguments (URL at index 0, target XML object at index 1)
/// @return Value::Undefined
template<typename GC>
Value<GC> send_and_load(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XML.load
///
/// Loads XML from a URL.
///
/// @param activation The current activation context
/// @param this_obj The XML object
/// @param args Arguments (URL at index 0)
/// @return Boolean indicating if load started
template<typename GC>
Value<GC> load(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XML.onData
///
/// Event handler for data loading.
///
/// @param activation The current activation context
/// @param this_obj The XML object
/// @param args Arguments (source data at index 0, undefined on error)
/// @return Value::Undefined
template<typename GC>
Value<GC> on_data(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XML.docTypeDecl property getter
///
/// Returns the DOCTYPE declaration.
///
/// @param activation The current activation context
/// @param this_obj The XML object
/// @param args Arguments (unused)
/// @return DOCTYPE string or Value::Undefined
template<typename GC>
Value<GC> doc_type_decl(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XML.xmlDecl property getter
///
/// Returns the XML declaration.
///
/// @param activation The current activation context
/// @param this_obj The XML object
/// @param args Arguments (unused)
/// @return XML declaration string or Value::Undefined
template<typename GC>
Value<GC> xml_decl(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XML.idMap property getter
///
/// Returns the document's ID map object.
///
/// @param activation The current activation context
/// @param this_obj The XML object
/// @param args Arguments (unused)
/// @return ID map object or Value::Undefined
template<typename GC>
Value<GC> id_map(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements XML.status property getter
///
/// Returns the parse status code.
///
/// @param activation The current activation context
/// @param this_obj The XML object
/// @param args Arguments (unused)
/// @return Status code as integer
template<typename GC>
Value<GC> status(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace xml
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_XML_H
