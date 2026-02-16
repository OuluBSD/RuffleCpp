// C++ translation of core/src/avm2/object/file_reference_object.rs
//! Object representation for FileReference objects

#ifndef RUFFLE_CORE_AVM2_OBJECT_FILE_REFERENCE_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_FILE_REFERENCE_OBJECT_H

#include <memory>
#include <variant>
#include <functional>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class ScriptObjectData;
    template<typename T> class TObject;
    template<typename T> class Gc;
    template<typename T> class GcWeak;
    template<typename T> class ClassObject;
    template<typename T> class Object;
}
}
}

namespace ruffle {
namespace backend {
namespace ui {
    class FileDialogResult;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// FileDialogResult wrapper - represents the result of a file dialog
using FileDialogResultPtr = std::unique_ptr<ruffle::backend::ui::FileDialogResult>;

/// FileReference - represents the state of a file reference
enum class FileReferenceState {
    None,
    FileDialogResult
};

/// FileReference - union type for file reference data
template<typename GC>
class FileReference {
public:
    FileReference() : state_(FileReferenceState::None) {}
    explicit FileReference(FileDialogResultPtr result)
        : state_(FileReferenceState::FileDialogResult)
        , result_(std::move(result)) {}

    FileReferenceState state() const { return state_; }
    FileDialogResultPtr& result() { return result_; }
    const FileDialogResultPtr& result() const { return result_; }

private:
    FileReferenceState state_;
    FileDialogResultPtr result_;
};

/// Weak reference to FileReferenceObject data
template<typename GC>
class FileReferenceObjectWeak;

/// Data structure for FileReferenceObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class FileReferenceObjectData {
public:
    FileReferenceObjectData() : loaded_(false) {}

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// File reference data
    FileReference<GC>& file_reference() { return reference_; }
    const FileReference<GC>& file_reference() const { return reference_; }

    /// Loaded state
    bool loaded() const { return loaded_; }
    void set_loaded(bool value) { loaded_ = value; }

private:
    FileReference<GC> reference_;
    bool loaded_;

    // Friend classes for accessing private members
    friend class FileReferenceObject<GC>;
    friend class FileReferenceObjectWeak<GC>;
};

/// FileReferenceObject - AVM2 FileReference object representation
/// Represents a file reference for file upload/download operations
template<typename GC>
class FileReferenceObject {
public:
    using DataType = FileReferenceObjectData<GC>;
    using WeakType = FileReferenceObjectWeak<GC>;

    FileReferenceObject() = default;
    explicit FileReferenceObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create a new FileReferenceObject
    static FileReferenceObject create(ClassObject<GC> class_obj, Activation<GC>* activation);

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

    /// Initialize from dialog result
    void init_from_dialog_result(FileDialogResultPtr result);

    /// Get the file reference
    const FileReference<GC>& file_reference() const {
        return data_->file_reference();
    }

    /// Get the loaded state
    bool loaded() const {
        return data_->loaded();
    }

    /// Set the loaded state
    void set_loaded(bool value) {
        data_->set_loaded(value);
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to FileReferenceObject
template<typename GC>
class FileReferenceObjectWeak {
public:
    FileReferenceObjectWeak() = default;
    explicit FileReferenceObjectWeak(GcWeak<GC, FileReferenceObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, FileReferenceObjectData<GC>> data_;
};

// Template method implementations
template<typename GC>
FileReferenceObject<GC> FileReferenceObject<GC>::create(ClassObject<GC> class_obj, Activation<GC>* activation) {
    auto base = ScriptObjectData<GC>::create(class_obj);
    auto data = Gc<GC, DataType>::create(activation->gc(), DataType{});
    data->base = base;
    return FileReferenceObject<GC>(data);
}

template<typename GC>
void FileReferenceObject<GC>::init_from_dialog_result(FileDialogResultPtr result) {
    data_->reference_ = FileReference<GC>(std::move(result));
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_FILE_REFERENCE_OBJECT_H
