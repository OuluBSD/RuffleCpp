// C++ translation of core/src/avm1/globals/file_reference.rs
// flash.net.FileReference object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_FILE_REFERENCE_H
#define RUFFLE_CORE_AVM1_GLOBALS_FILE_REFERENCE_H

#include <cstdint>
#include <memory>
#include <vector>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class NativeObject;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
    namespace globals {
        namespace as_broadcaster {
            template<typename GC>
            struct BroadcasterFunctions;
        }
    }
}
}
namespace backend {
namespace ui {
    struct FileFilter;
    class FileDialogResult;
}
}
namespace string {
    template<typename T> class AvmString;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace file_reference {

/// FileReferenceData structure
///
/// Holds the FileReference properties and file data.
template<typename GC>
class FileReferenceData {
public:
    /// Has this object been initialised from a dialog
    bool is_initialised;

    std::optional<Object<GC>*> creation_date;
    std::optional<string::AvmString<GC>> creator;
    std::optional<Object<GC>*> modification_date;
    std::optional<string::AvmString<GC>> name;
    std::optional<string::AvmString<GC>> post_data;
    std::optional<uint64_t> size;
    std::optional<string::AvmString<GC>> file_type;

    /// The contents of the referenced file
    /// We track this here so that it can be referenced in FileReference.upload
    std::vector<uint8_t> data;

    /// Default constructor
    FileReferenceData()
        : is_initialised(false)
        , creation_date(std::nullopt)
        , creator(std::nullopt)
        , modification_date(std::nullopt)
        , name(std::nullopt)
        , post_data(std::nullopt)
        , size(std::nullopt)
        , file_type(std::nullopt)
        , data()
    {}
};

/// FileReferenceObject wrapper
///
/// AVM1 FileReference object wrapper.
template<typename GC>
class FileReferenceObject {
public:
    /// Initialize from a dialog result
    ///
    /// \param activation The current activation context
    /// \param result The file dialog result
    void init_from_dialog_result(
        Activation<GC>* activation,
        const backend::ui::FileDialogResult* result
    );

    /// Get the underlying data
    FileReferenceData<GC>* data() { return data_.get(); }
    const FileReferenceData<GC>* data() const { return data_.get(); }

private:
    std::shared_ptr<FileReferenceData<GC>> data_;
};

/// Property declarations for FileReference prototype
extern const StaticDeclarations PROTO_DECLS;

/// Property declarations for FileReference object (static properties)
extern const StaticDeclarations OBJECT_DECLS;

/// Create the FileReference class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \param broadcaster_fns Broadcaster functions for event support
/// \param array_proto The Array prototype object
/// \return A SystemClass containing the created FileReference class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto,
    as_broadcaster::BroadcasterFunctions<GC> broadcaster_fns,
    Object<GC>* array_proto
);

/// Implements FileReference.creationDate property getter
///
/// Gets the creation date of the referenced file.
///
/// \param activation The current activation context
/// \param this_obj The FileReference object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The creation date as a Date object, or Value::Undefined
template<typename GC>
Value<GC> creation_date(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements FileReference.creator property getter
///
/// Gets the creator of the referenced file.
///
/// \param activation The current activation context
/// \param this_obj The FileReference object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The creator string, or Value::Undefined
template<typename GC>
Value<GC> creator(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements FileReference.modificationDate property getter
///
/// Gets the modification date of the referenced file.
///
/// \param activation The current activation context
/// \param this_obj The FileReference object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The modification date as a Date object, or Value::Undefined
template<typename GC>
Value<GC> modification_date(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements FileReference.name property getter
///
/// Gets the name of the referenced file.
///
/// \param activation The current activation context
/// \param this_obj The FileReference object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The file name string, or Value::Undefined
template<typename GC>
Value<GC> name(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements FileReference.postData property getter
///
/// Gets the post data for the file upload.
///
/// \param activation The current activation context
/// \param this_obj The FileReference object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The post data string, or empty string
template<typename GC>
Value<GC> post_data(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements FileReference.postData property setter
///
/// Sets the post data for the file upload.
///
/// \param activation The current activation context
/// \param this_obj The FileReference object
/// \param args Arguments (post data string)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> set_post_data(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements FileReference.size property getter
///
/// Gets the size of the referenced file.
///
/// \param activation The current activation context
/// \param this_obj The FileReference object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The file size, or Value::Undefined
template<typename GC>
Value<GC> size(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements FileReference.type property getter
///
/// Gets the type of the referenced file.
///
/// \param activation The current activation context
/// \param this_obj The FileReference object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The file type string, or Value::Undefined
template<typename GC>
Value<GC> file_type(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements FileReference.browse method
///
/// Displays a file open dialog to select a file.
///
/// \param activation The current activation context
/// \param this_obj The FileReference object
/// \param args Arguments (optional file filters array)
/// \param arg_count Number of arguments
/// \return true if dialog was shown, false otherwise
template<typename GC>
Value<GC> browse(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements FileReference.cancel method
///
/// Cancels the current file operation.
///
/// \param activation The current activation context
/// \param this_obj The FileReference object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> cancel(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements FileReference.download method
///
/// Downloads a file from the specified URL.
///
/// \param activation The current activation context
/// \param this_obj The FileReference object
/// \param args Arguments (URL, optional file name)
/// \param arg_count Number of arguments
/// \return true if download was started, false otherwise
template<typename GC>
Value<GC> download(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements FileReference.upload method
///
/// Uploads the referenced file to the specified URL.
///
/// \param activation The current activation context
/// \param this_obj The FileReference object
/// \param args Arguments (URL)
/// \param arg_count Number of arguments
/// \return true if upload was started, false otherwise
template<typename GC>
Value<GC> upload(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// FileReference constructor
///
/// Creates a new FileReference object.
///
/// \param activation The current activation context
/// \param this_obj The FileReference object being constructed
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace file_reference
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_FILE_REFERENCE_H
