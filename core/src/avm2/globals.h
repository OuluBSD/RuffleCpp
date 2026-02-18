// C++ translation of core/src/avm2/globals.rs
//! AVM2 global classes and builtins

#ifndef RUFFLE_CORE_AVM2_GLOBALS_H
#define RUFFLE_CORE_AVM2_GLOBALS_H

#include <cstdint>
#include <vector>
#include <string>
#include <optional>
#include <memory>
#include <span>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC> class AvmString;
    template<typename GC> class WStr;
    template<typename GC> class WString;
    template<typename GC> class Mutation;
    template<typename GC> class UpdateContext;
    template<typename GC> class StringContext;

    namespace avm2 {
        template<typename GC> class Activation;
        template<typename GC> class Error;
        template<typename GC> class Class;
        template<typename GC> class ClassObject;
        template<typename GC> class ScriptObject;
        template<typename GC> class Scope;
        template<typename GC> class ScopeChain;
        template<typename GC> class Domain;
        template<typename GC> class Multiname;
        template<typename GC> class Namespace;
        template<typename GC> class QName;
        template<typename GC> class Avm2;
        
        namespace api_version {
            enum class ApiVersion;
        }

        namespace script {
            template<typename GC>
            struct TranslationUnit;
        }
    }

    namespace tag_utils {
        template<typename GC>
        struct SwfMovie;
        template<typename GC>
        struct SwfSlice;
        template<typename GC>
        struct SwfStream;
    }
}
}

namespace swf {
    enum class TagCode : uint8_t;
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {

/// This structure represents all system builtin classes.
template<typename GC>
struct SystemClasses {
    ClassObject<GC> object;
    ClassObject<GC> function;
    ClassObject<GC> class_;
    ClassObject<GC> string;
    ClassObject<GC> boolean;
    ClassObject<GC> number;
    ClassObject<GC> int_;
    ClassObject<GC> uint;
    ClassObject<GC> namespace_;
    ClassObject<GC> array;
    ClassObject<GC> movieclip;
    ClassObject<GC> framelabel;
    ClassObject<GC> scene;
    ClassObject<GC> application_domain;
    ClassObject<GC> event;
    ClassObject<GC> fullscreenevent;
    ClassObject<GC> video;
    ClassObject<GC> xml;
    ClassObject<GC> xml_list;
    ClassObject<GC> display_object;
    ClassObject<GC> shape;
    ClassObject<GC> textfield;
    ClassObject<GC> textformat;
    ClassObject<GC> graphics;
    ClassObject<GC> loader;
    ClassObject<GC> loaderinfo;
    ClassObject<GC> bytearray;
    ClassObject<GC> stage;
    ClassObject<GC> sprite;
    ClassObject<GC> simplebutton;
    ClassObject<GC> regexp;
    // the generic Vector class, useless until you .apply() type arg onto it
    ClassObject<GC> generic_vector;
    // Vector.<*>, NOT Vector.<Object>. Used as base class for new Vector<T>.
    ClassObject<GC> object_vector;
    ClassObject<GC> soundtransform;
    ClassObject<GC> soundchannel;
    ClassObject<GC> bitmap;
    ClassObject<GC> bitmapdata;
    ClassObject<GC> date;
    ClassObject<GC> qname;
    ClassObject<GC> mouseevent;
    ClassObject<GC> progressevent;
    ClassObject<GC> httpstatusevent;
    ClassObject<GC> textevent;
    ClassObject<GC> errorevent;
    ClassObject<GC> ioerrorevent;
    ClassObject<GC> securityerrorevent;
    ClassObject<GC> transform;
    ClassObject<GC> colortransform;
    ClassObject<GC> matrix;
    ClassObject<GC> matrix3d;
    ClassObject<GC> perspectiveprojection;
    ClassObject<GC> illegaloperationerror;
    ClassObject<GC> eventdispatcher;
    ClassObject<GC> rectangle;
    ClassObject<GC> keyboardevent;
    ClassObject<GC> point;
    ClassObject<GC> evalerror;
    ClassObject<GC> rangeerror;
    ClassObject<GC> referenceerror;
    ClassObject<GC> argumenterror;
    ClassObject<GC> syntaxerror;
    ClassObject<GC> typeerror;
    ClassObject<GC> verifyerror;
    ClassObject<GC> definitionerror;
    ClassObject<GC> uninitializederror;
    ClassObject<GC> ioerror;
    ClassObject<GC> eoferror;
    ClassObject<GC> urierror;
    ClassObject<GC> securityerror;
    ClassObject<GC> error;
    ClassObject<GC> uncaughterrorevents;
    ClassObject<GC> statictext;
    ClassObject<GC> textlinemetrics;
    ClassObject<GC> stage3d;
    ClassObject<GC> context3d;
    ClassObject<GC> indexbuffer3d;
    ClassObject<GC> vertexbuffer3d;
    ClassObject<GC> program3d;
    ClassObject<GC> urlvariables;
    ClassObject<GC> bevelfilter;
    ClassObject<GC> bitmapfilter;
    ClassObject<GC> blurfilter;
    ClassObject<GC> colormatrixfilter;
    ClassObject<GC> convolutionfilter;
    ClassObject<GC> displacementmapfilter;
    ClassObject<GC> dropshadowfilter;
    ClassObject<GC> glowfilter;
    ClassObject<GC> gradientbevelfilter;
    ClassObject<GC> gradientglowfilter;
    ClassObject<GC> texture;
    ClassObject<GC> cubetexture;
    ClassObject<GC> rectangletexture;
    ClassObject<GC> morphshape;
    ClassObject<GC> shader;
    ClassObject<GC> shaderinput;
    ClassObject<GC> shaderparameter;
    ClassObject<GC> netstatusevent;
    ClassObject<GC> shaderfilter;
    ClassObject<GC> statusevent;
    ClassObject<GC> asyncerrorevent;
    ClassObject<GC> contextmenuevent;
    ClassObject<GC> filereference;
    ClassObject<GC> filefilter;
    ClassObject<GC> font;
    ClassObject<GC> textline;
    ClassObject<GC> sampledataevent;
    ClassObject<GC> avm1movie;
    ClassObject<GC> focusevent;
    ClassObject<GC> dictionary;
    ClassObject<GC> id3info;
    ClassObject<GC> textrun;
    ClassObject<GC> sharedobject;
    ClassObject<GC> worker;
    ClassObject<GC> workerdomain;
    ClassObject<GC> messagechannel;
    ClassObject<GC> securitydomain;
};

/// This structure represents all system builtin class definitions.
template<typename GC>
struct SystemClassDefs {
    Class<GC> object;
    Class<GC> class_;
    Class<GC> function;
    Class<GC> null;
    Class<GC> void_;

    Class<GC> array;
    Class<GC> boolean;
    Class<GC> int_;
    Class<GC> generic_vector;
    Class<GC> namespace_;
    Class<GC> number;
    Class<GC> string;
    Class<GC> uint;
    Class<GC> xml;
    Class<GC> xml_list;

    // Vector.<Number> aka Vector$double
    Class<GC> number_vector;
    // Vector.<int> aka Vector$int
    Class<GC> int_vector;
    // Vector.<uint> aka Vector$uint
    Class<GC> uint_vector;
    // Vector.<*> aka Vector$object
    Class<GC> object_vector;

    Class<GC> bitmap;
    Class<GC> bitmapdata;
    Class<GC> igraphicsdata;
    Class<GC> graphicsbitmapfill;
    Class<GC> graphicsendfill;
    Class<GC> graphicsgradientfill;
    Class<GC> graphicspath;
    Class<GC> graphicstrianglepath;
    Class<GC> graphicssolidfill;
    Class<GC> graphicsshaderfill;
    Class<GC> graphicsstroke;
    Class<GC> cubetexture;
    Class<GC> rectangletexture;
    Class<GC> display_object;
    Class<GC> sprite;
    Class<GC> urlrequestheader;
    Class<GC> contextmenuitem;
};

/// Setup the Object, Class, and void classes, which are special "early
/// classes". This step of VM initialization must be done before everything else,
/// including the construction of the first Script and loading of any other classes.
template<typename GC>
Result<void, Error<GC>> init_early_classes(
    Activation<GC>* activation,
    script::TranslationUnit<GC> tunit
) {
    // We know that Object is class #0 and Class is class #1 in the builtin ABC
    constexpr uint32_t OBJECT_IDX = 0;
    constexpr uint32_t CLASS_IDX = 1;

    auto mc = activation->gc();

    // We need to load Object's i_class before we do anything else, even
    // initialize the script.
    // Object's i_class has no superclass, so we load it first.
    auto object_i_class = Class<GC>::instance_from_abc_index(tunit, OBJECT_IDX, activation);
    object_i_class.load_instance_traits(activation, tunit, OBJECT_IDX);
    object_i_class.init_vtable(activation);

    // We're going to need the Object class registered in the domain for the
    // Class class to load. These will be overwritten when we properly load
    // the rest of the classes, but it doesn't matter since it'll be overwritten
    // with the same class anyways.
    activation->domain().export_class(object_i_class.name(), object_i_class, mc);

    // Now we can load Class's i_class:
    auto class_i_class = Class<GC>::instance_from_abc_index(tunit, CLASS_IDX, activation);
    class_i_class.load_instance_traits(activation, tunit, CLASS_IDX);
    class_i_class.init_vtable(activation);

    // Register the Class class in the domain
    activation->domain().export_class(class_i_class.name(), class_i_class, mc);

    // Now we can load the c_classes for Object and Class safely.
    auto object_c_class = Class<GC>::class_from_abc_index(tunit, OBJECT_IDX, class_i_class, activation);
    object_c_class.load_class_traits(activation, tunit, OBJECT_IDX);
    object_c_class.init_vtable(activation);

    auto class_c_class = Class<GC>::class_from_abc_index(tunit, CLASS_IDX, class_i_class, activation);
    class_c_class.load_class_traits(activation, tunit, CLASS_IDX);
    class_c_class.init_vtable(activation);

    // Now we link the i_classes and c_classes with each other:
    object_i_class.link_with_c_class(mc, object_c_class);
    class_i_class.link_with_c_class(mc, class_c_class);

    // Set the classes on the TranslationUnit to prevent TranslationUnit::load_class
    // from creating duplicate classes for them
    tunit.set_class(mc, static_cast<size_t>(OBJECT_IDX), object_i_class);
    tunit.set_class(mc, static_cast<size_t>(CLASS_IDX), class_i_class);

    // Set up the null and void classes and initialize SystemClasses

    // This is a weird internal class in avmplus, but it allows for implementing
    // describeType(null) in a cleaner way
    auto null_def = null::create_class(activation);

    // void doesn't have a ClassObject
    auto void_def = void_::create_class(activation);
    activation->domain().export_class(void_def.name(), void_def, mc);

    activation->avm2().system_class_defs = SystemClassDefs<GC>{
        object_i_class,
        class_i_class,
        null_def,
        void_def,
        // temporary initialization for rest
    };

    // NOTE: We don't create correct outer ScopeChains for Object and Class
    // here. This could cause unexpected behavior in their code, but the current
    // AS method implementations don't ever use properties from the outer ScopeChain.
    // However, we do need to ensure that the outer ScopeChain isn't zero-sized.
    // We will need to replace this ScopeChain once the script is created.
    auto dummy_object = ScriptObject<GC>::custom_object(
        mc, object_i_class, std::nullopt, object_i_class.vtable()
    );

    auto empty_scope = ScopeChain<GC>::new_(tunit.domain());
    auto dummy_scope = empty_scope.chain(mc, std::span{Scope<GC>::new(dummy_object.into())});
    activation->set_outer(dummy_scope);

    // Finally, we can actually create the ClassObjects for Object and Class.
    auto object_class = ClassObject<GC>::from_class_minimal(activation, object_i_class, std::nullopt);
    auto object_proto = ScriptObject<GC>::custom_object(
        mc, object_i_class, std::nullopt, object_class.instance_vtable()
    );

    auto class_class = ClassObject<GC>::from_class_minimal(activation, class_i_class, object_class);
    auto class_proto = ScriptObject<GC>::custom_object(
        mc, object_i_class, object_proto, object_class.instance_vtable()
    );

    // Now to weave the Gordian knot...
    object_class.link_prototype(activation->context(), object_proto);
    object_class.link_type(mc, class_proto);

    class_class.link_prototype(activation->context(), class_proto);
    class_class.link_type(mc, class_proto);

    // At this point, we need both early classes to be available in SystemClasses
    // in order to call into_finished_class on both ClassObjects.
    activation->avm2().system_classes = SystemClasses<GC>{
        object_class,
        class_class,
        // temporary initialization for rest - all set to object_class
    };

    // Construct the ClassObjects. We will run the class initializers later.
    class_class.into_finished_class(activation);
    object_class.into_finished_class(activation);

    // We don't need to validate the classes, as we already know that the
    // Object and Class classes are valid

    // However, we do need to bind their methods
    object_class.bind_methods(activation);
    class_class.bind_methods(activation);

    // Reset the Activation's outer scope.
    activation->set_outer(empty_scope);

    return {};
}

/// This file is built by 'core/build_playerglobal/'
/// See that tool, and 'core/src/avm2/globals/README.md', for more details
inline const std::span<const uint8_t> PLAYERGLOBAL = /* include_bytes!(...) */ std::span<const uint8_t>{};

/// Do the same for methods
namespace methods {
    // Native methods would be declared here
    // pub use super::native::methods::*;
}

/// Allow accessing slots that are meant to be accessed natively by using
/// avm2::globals::slots::*;
namespace slots {
    // Native slots would be declared here
    // pub use super::native::slots::*;
}

/// Initialize builtin system classes from playerglobal
template<typename GC>
void init_builtin_system_classes(Activation<GC>* activation) {
    // We don't include Function here because it registers itself manually
    // in its class initializer
    
    // List of classes to initialize from playerglobal
    static constexpr std::pair<const char*, const char*> classes[] = {
        {"", "ArgumentError"},
        {"", "Array"},
        {"", "Boolean"},
        {"", "DefinitionError"},
        {"", "Error"},
        {"", "EvalError"},
        {"", "int"},
        {"", "Namespace"},
        {"", "Number"},
        {"", "QName"},
        {"", "RangeError"},
        {"", "ReferenceError"},
        {"", "SecurityError"},
        {"", "String"},
        {"", "SyntaxError"},
        {"", "TypeError"},
        {"", "uint"},
        {"", "UninitializedError"},
        {"", "URIError"},
        {"", "VerifyError"},
        {"", "XML"},
        {"", "XMLList"},
        {"__AS3__.vec", "Vector"},
    };

    for (const auto& [package, class_name] : classes) {
        // Package and class names are ASCII
        auto package_str = WStr<GC>::from_units(reinterpret_cast<const uint8_t*>(package));
        auto class_name_str = WStr<GC>::from_units(reinterpret_cast<const uint8_t*>(class_name));

        auto package_interned = activation->strings().intern_static(package_str);
        auto class_name_interned = activation->strings().intern_static(class_name_str);

        // Lookup with the highest version, so we see all defined classes here
        auto ns = Namespace<GC>::package(package_interned, api_version::ApiVersion::VM_INTERNAL, activation->strings());
        auto name = QName<GC>::new_(ns, class_name_interned);
        
        auto class_object = activation->domain().get_defined_value(activation, name);
        if (class_object.is_object()) {
            if (auto class_obj = class_object.as_object()->as_class_object()) {
                // Set the appropriate field in system_classes
                // This would be done via a macro in Rust, but in C++ we'd need
                // to manually set each field
            }
        }
    }

    // Initialize vector class objects
    vector::init_vector_class_objects(activation);
}

/// Initialize builtin system class definitions
template<typename GC>
void init_builtin_system_class_defs(Activation<GC>* activation) {
    // Mark all the special builtin classes; see the documentation on
    // Class.builtin_type for more information
    auto class_defs = activation->avm2().class_defs();
    class_defs.int_.mark_builtin_type(BuiltinType::Int);
    class_defs.uint.mark_builtin_type(BuiltinType::Uint);
    class_defs.number.mark_builtin_type(BuiltinType::Number);
    class_defs.boolean.mark_builtin_type(BuiltinType::Boolean);
    class_defs.object.mark_builtin_type(BuiltinType::Object);
    class_defs.string.mark_builtin_type(BuiltinType::String);
    class_defs.void_.mark_builtin_type(BuiltinType::Void);

    // Initialize vector class definitions
    vector::init_vector_class_defs(activation);
}

/// Initialize native system classes
template<typename GC>
void init_native_system_classes(Activation<GC>* activation) {
    // This would initialize classes that have native implementations
    // like Date, RegExp, and various flash.* classes
}

// Re-export functions from toplevel module
using toplevel::decode_uri;
using toplevel::decode_uri_component;
using toplevel::encode_uri;
using toplevel::encode_uri_component;
using toplevel::escape;
using toplevel::is_finite;
using toplevel::is_nan;
using toplevel::is_xml_name;
using toplevel::parse_float;
using toplevel::parse_int;
using toplevel::trace;
using toplevel::unescape;

} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_H
