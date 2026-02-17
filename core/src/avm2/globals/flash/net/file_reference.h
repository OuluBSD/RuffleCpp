// C++ translation of core/src/avm2/globals/flash/net/file_reference.rs
//! flash.net.FileReference functionality

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_FILE_REFERENCE_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_FILE_REFERENCE_H

#include <vector>
#include <string>
#include <optional>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
    class AvmString;

    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class Value;
        template<typename T> class Error;
        template<typename T> class Object;
        template<typename T> class ByteArrayObject;
        template<typename T> class DateObject;
        template<typename T> class FileReferenceObject;
        template<typename T> class EventObject;
        template<typename T> class Avm2;

        namespace bytearray {
            class ByteArrayStorage;
        }
    }

    namespace backend {
        namespace ui {
            struct FileFilter {
                std::string description;
                std::string extensions;
                std::optional<std::string> mac_type;
            };
        }
    }

    namespace loader {
        // Forward declarations for loader functions
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace net {
namespace file_reference {

using backend::ui::FileFilter;
using bytearray::ByteArrayStorage;

/// Get the creation date of the file.
///
/// \param activation The current activation
/// \param this_val The FileReference object
/// \param args Function arguments (unused)
/// \return Creation date as Date object or Null
template<typename GC>
Value<GC> get_creation_date(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Get the file data as a ByteArray.
///
/// \param activation The current activation
/// \param this_val The FileReference object
/// \param args Function arguments (unused)
/// \return ByteArray containing file data or Null
template<typename GC>
Value<GC> get_data(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Get the modification date of the file.
///
/// \param activation The current activation
/// \param this_val The FileReference object
/// \param args Function arguments (unused)
/// \return Modification date as Date object or Null
template<typename GC>
Value<GC> get_modification_date(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Get the name of the file.
///
/// \param activation The current activation
/// \param this_val The FileReference object
/// \param args Function arguments (unused)
/// \return File name as string
template<typename GC>
Value<GC> get_name(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Get the size of the file.
///
/// \param activation The current activation
/// \param this_val The FileReference object
/// \param args Function arguments (unused)
/// \return File size as number
template<typename GC>
Value<GC> get_size(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Get the type of the file.
///
/// \param activation The current activation
/// \param this_val The FileReference object
/// \param args Function arguments (unused)
/// \return File type as string
template<typename GC>
Value<GC> get_type(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Browse for a file to open.
///
/// \param activation The current activation
/// \param this_val The FileReference object
/// \param args Function arguments (optional filters array)
/// \return Boolean indicating if dialog was shown
template<typename GC>
Value<GC> browse(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Load the file data.
///
/// \param activation The current activation
/// \param this_val The FileReference object
/// \param args Function arguments (unused)
/// \return Undefined
template<typename GC>
Value<GC> load(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Save file data (internal implementation).
///
/// \param activation The current activation
/// \param this_val The FileReference object
/// \param args Function arguments (data and default filename)
/// \return Undefined or error
template<typename GC>
Value<GC> save_internal(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

// Template implementations

template<typename GC>
Value<GC> get_creation_date(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& /*args*/
) {
    auto this_obj = this_val.as_object().value();
    auto this_file_ref = this_obj.as_file_reference().value();

    auto file_ref = this_file_ref.file_reference();
    if (file_ref->is_none()) {
        return Value<GC>::error(make_error_2037(activation));
    }

    if (auto* dialog_result = file_ref->as_file_dialog_result()) {
        if (auto time = dialog_result->creation_time()) {
            return DateObject<GC>::from_date_time(activation->context, time.value()).into();
        }
    }

    return Value<GC>::null();
}

template<typename GC>
Value<GC> get_data(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& /*args*/
) {
    auto this_obj = this_val.as_object().value();
    auto this_file_ref = this_obj.as_file_reference().value();

    if (!this_file_ref.loaded()) {
        // Contrary to other getters `data` will return null instead of throwing.
        return Value<GC>::null();
    }

    if (auto* dialog_result = this_file_ref.file_reference()->as_file_dialog_result()) {
        auto bytes = dialog_result->contents();
        auto storage = ByteArrayStorage::from_vec(activation->context, bytes);
        auto bytearray = ByteArrayObject<GC>::from_storage(activation->context, storage);
        return bytearray.into();
    }

    return Value<GC>::null();
}

template<typename GC>
Value<GC> get_modification_date(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& /*args*/
) {
    auto this_obj = this_val.as_object().value();
    auto this_file_ref = this_obj.as_file_reference().value();

    auto file_ref = this_file_ref.file_reference();
    if (file_ref->is_none()) {
        return Value<GC>::error(make_error_2037(activation));
    }

    if (auto* dialog_result = file_ref->as_file_dialog_result()) {
        if (auto time = dialog_result->modification_time()) {
            return DateObject<GC>::from_date_time(activation->context, time.value()).into();
        }
    }

    return Value<GC>::null();
}

template<typename GC>
Value<GC> get_name(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& /*args*/
) {
    auto this_obj = this_val.as_object().value();
    auto this_file_ref = this_obj.as_file_reference().value();

    auto file_ref = this_file_ref.file_reference();
    if (file_ref->is_none()) {
        return Value<GC>::error(make_error_2037(activation));
    }

    if (auto* dialog_result = file_ref->as_file_dialog_result()) {
        auto name = dialog_result->file_name().value_or("");
        return AvmString<GC>::new_utf8(activation->gc(), name).into();
    }

    return Value<GC>::null();
}

template<typename GC>
Value<GC> get_size(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& /*args*/
) {
    auto this_obj = this_val.as_object().value();
    auto this_file_ref = this_obj.as_file_reference().value();

    auto file_ref = this_file_ref.file_reference();
    if (file_ref->is_none()) {
        return Value<GC>::error(make_error_2037(activation));
    }

    size_t size = 0;
    if (auto* dialog_result = file_ref->as_file_dialog_result()) {
        size = dialog_result->size().value_or(0);
    }

    return Value<GC>::number(static_cast<double>(size));
}

template<typename GC>
Value<GC> get_type(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& /*args*/
) {
    auto this_obj = this_val.as_object().value();
    auto this_file_ref = this_obj.as_file_reference().value();

    auto file_ref = this_file_ref.file_reference();
    if (file_ref->is_none()) {
        return Value<GC>::error(make_error_2037(activation));
    }

    if (auto* dialog_result = file_ref->as_file_dialog_result()) {
        auto type = dialog_result->file_type().value_or("");
        return AvmString<GC>::new_utf8(activation->gc(), type).into();
    }

    return Value<GC>::null();
}

template<typename GC>
Value<GC> browse(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();
    auto this_file_ref = this_obj.as_file_reference().value();

    std::vector<FileFilter> filters;

    if (args.size() > 0) {
        if (auto obj = args[0].as_object()) {
            if (auto array_storage = obj->as_array_storage()) {
                for (const auto& filter : array_storage->iter()) {
                    if (auto filter_obj = filter.as_object()) {
                        auto filefilter_class = activation->avm2()->classes()->filefilter.inner_class_definition();
                        if (!filter_obj->is_of_type(filefilter_class)) {
                            return Value<GC>::error(make_error_2097(activation));
                        }

                        auto description = filter_obj->get_slot(file_filter_slots::_DESCRIPTION);
                        auto extension = filter_obj->get_slot(file_filter_slots::_EXTENSION);
                        auto mac_type = filter_obj->get_slot(file_filter_slots::_MAC_TYPE);

                        // The description and extension must be non-empty strings.
                        if (auto desc_str = description.as_string()) {
                            if (auto ext_str = extension.as_string()) {
                                if (!desc_str->is_empty() && !ext_str->is_empty()) {
                                    std::optional<std::string> mac_type_str;
                                    if (auto mac_str = mac_type.as_string()) {
                                        if (!mac_str->is_empty()) {
                                            mac_type_str = mac_str->to_string();
                                        }
                                    }

                                    filters.push_back(FileFilter{
                                        desc_str->to_string(),
                                        ext_str->to_string(),
                                        mac_type_str
                                    });
                                } else {
                                    return Value<GC>::error(make_error_2097(activation));
                                }
                            } else {
                                return Value<GC>::error(make_error_2097(activation));
                            }
                        } else {
                            return Value<GC>::error(make_error_2097(activation));
                        }
                    } else {
                        return Value<GC>::error(make_error_2097(activation));
                    }
                }
            }
        }
    }

    auto dialog = activation->context->ui->display_file_open_dialog(filters);
    bool result = false;

    if (dialog.has_value()) {
        auto process = loader::select_file_dialog_avm2(activation->context, this_file_ref, dialog.value());
        activation->context->navigator->spawn_future(process);
        result = true;
    }

    return Value<GC>::bool_(result);
}

template<typename GC>
Value<GC> load(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& /*args*/
) {
    auto this_obj = this_val.as_object().value();
    auto this_file_ref = this_obj.as_file_reference().value();

    // Somewhat unexpectedly, we don't need to load anything here, because
    // that already happened during browse() or save().

    size_t size = 0;
    if (auto* dialog_result = this_file_ref.file_reference()->as_file_dialog_result()) {
        size = dialog_result->size().value_or(0);
    }

    // Dispatch events
    auto open_evt = EventObject<GC>::bare_default_event(activation->context, "open");
    Avm2<GC>::dispatch_event(activation->context, open_evt, this_obj);

    auto progress_evt = EventObject<GC>::progress_event(activation, "progress", 0, size);
    Avm2<GC>::dispatch_event(activation->context, progress_evt, this_obj);

    auto open_evt2 = EventObject<GC>::bare_default_event(activation->context, "open");
    Avm2<GC>::dispatch_event(activation->context, open_evt2, this_obj);

    auto progress_evt2 = EventObject<GC>::progress_event(activation, "progress", size, size);
    Avm2<GC>::dispatch_event(activation->context, progress_evt2, this_obj);

    this_file_ref.set_loaded(true);

    auto complete_evt = EventObject<GC>::bare_default_event(activation->context, "complete");
    Avm2<GC>::dispatch_event(activation->context, complete_evt, this_obj);

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> save_internal(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_obj = this_val.as_object().value();
    auto this_file_ref = this_obj.as_file_reference().value();

    auto byte_array = args[0].as_object().value();
    auto bytearray_mut = byte_array->as_bytearray_mut().value();

    std::vector<uint8_t> data;
    bytearray_mut.swap_storage_with(data);
    // The ByteArray is now empty, and `data` stores the data previously
    // contained in it

    auto file_name = args[1].as_string().value();

    // Create and spawn dialog
    auto dialog = activation->context->ui->display_file_save_dialog(
        file_name->to_string(),
        "Select location to save the file " + file_name->to_string()
    );

    if (dialog.has_value()) {
        auto process = loader::save_file_dialog(activation->context, this_file_ref, dialog.value(), data);
        activation->context->navigator->spawn_future(process);
    } else {
        return Value<GC>::error(make_error_2174(activation));
    }

    return Value<GC>::undefined();
}

} // namespace file_reference
} // namespace net
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_FILE_REFERENCE_H
