// C++ translation of core/src/avm2/globals/flash/display/loader_info.rs
//! flash.display.LoaderInfo builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_LOADER_INFO_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_LOADER_INFO_H

#include <vector>
#include <string>
#include <optional>
#include <memory>
#include <map>
#include <sync>

// Forward declarations
namespace ruffle {
namespace core {
    class WStr;
    class AvmString;
    template<typename GC> class Activation;
    template<typename GC> class Value;
    template<typename GC> class Error;
    template<typename GC> class Object;
    template<typename GC> class ScriptObject;
    template<typename GC> class DomainObject;
    template<typename GC> class ByteArrayObject;
    template<typename GC> class LoaderInfoObject;

    namespace display_object {
        template<typename GC> class DisplayObject;
        template<typename GC> class MovieClip;
        template<typename GC> class Bitmap;
        template<typename GC> class TDisplayObject;
    }

    namespace loader {
        enum class ContentType;
        template<typename GC> class Loader;
    }

    namespace swf {
        enum class Compression;
        struct Header;
        template<typename T> class SwfMovie;
    }

    namespace avm2 {
        enum class Endian;
        template<typename GC> class LoaderStream;
    }
}
}

namespace url {
    class Url;
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace display {
namespace loader_info {

/// `actionScriptVersion` getter
///
/// Returns the ActionScript version of the loaded SWF.
/// Returns 3 for AVM2 SWFs, 2 for AVM1 SWFs.
///
/// @param activation The current activation context
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return ActionScript version number, or Error 2099 if not yet loaded
template<typename GC>
Value<GC> get_action_script_version(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `applicationDomain` getter
///
/// Returns the ApplicationDomain of the loaded content.
///
/// @param activation The current activation context
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return ApplicationDomain object, or Null for AVM1 content
template<typename GC>
Value<GC> get_application_domain(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `bytesTotal` getter
///
/// Returns the total number of bytes to be loaded.
///
/// @param activation Unused
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return Total bytes as Number
template<typename GC>
Value<GC> get_bytes_total(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `bytesLoaded` getter
///
/// Returns the number of bytes loaded so far.
///
/// @param activation Unused
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return Loaded bytes as Number
template<typename GC>
Value<GC> get_bytes_loaded(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `content` getter
///
/// Returns the loaded content (DisplayObject).
/// Returns Null if content is not exposed.
///
/// @param activation Unused
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return Content DisplayObject, or Null
template<typename GC>
Value<GC> get_content(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `contentType` getter
///
/// Returns the MIME type of the loaded content.
///
/// @param activation The current activation context
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return Content type string, or Null for unknown types
template<typename GC>
Value<GC> get_content_type(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `frameRate` getter
///
/// Returns the frame rate of the loaded SWF.
///
/// @param activation The current activation context
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return Frame rate as Number, or Error 2099 if not yet loaded
template<typename GC>
Value<GC> get_frame_rate(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `height` getter
///
/// Returns the height of the loaded content in pixels.
///
/// @param activation The current activation context
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return Height as Number, or Error 2099 if not yet loaded
template<typename GC>
Value<GC> get_height(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `isURLInaccessible` getter
///
/// Returns whether the URL is inaccessible due to security restrictions.
///
/// @param activation The current activation context
/// @param this_value Unused
/// @param args Unused
/// @return Boolean (stub - always returns false)
template<typename GC>
Value<GC> get_is_url_inaccessible(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `sameDomain` getter
///
/// Returns whether the loaded content is in the same domain.
///
/// @param activation The current activation context
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return Boolean (stub - always returns false), or Error 2099 if not yet loaded
template<typename GC>
Value<GC> get_same_domain(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `childAllowsParent` getter
///
/// Returns whether the child content allows access from the parent.
///
/// @param activation The current activation context
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return Boolean indicating access permission, or Error 2099 if not yet loaded
template<typename GC>
Value<GC> get_child_allows_parent(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `parentAllowsChild` getter
///
/// Returns whether the parent allows access to the child content.
///
/// @param activation The current activation context
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return Boolean indicating access permission, or Error 2099 if not yet loaded
template<typename GC>
Value<GC> get_parent_allows_child(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `swfVersion` getter
///
/// Returns the SWF version of the loaded content.
///
/// @param activation The current activation context
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return SWF version number, or Error 2099 if not yet loaded
template<typename GC>
Value<GC> get_swf_version(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `url` getter
///
/// Returns the URL of the loaded content.
/// Returns Null if content is not exposed.
///
/// @param activation The current activation context
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return URL string, or Null if not exposed
template<typename GC>
Value<GC> get_url(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `width` getter
///
/// Returns the width of the loaded content in pixels.
///
/// @param activation The current activation context
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return Width as Number, or Error 2099 if not yet loaded
template<typename GC>
Value<GC> get_width(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `bytes` getter
///
/// Returns a ByteArray containing the loaded data.
/// For SWF files, returns the raw SWF data.
/// For images, returns a generated SWF containing the image (stub).
///
/// @param activation The current activation context
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return ByteArray containing the loaded data, or Null if not started loading
template<typename GC>
Value<GC> get_bytes(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `loader` getter
///
/// Returns the Loader object associated with this LoaderInfo.
///
/// @param activation Unused
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return Loader object, or Null if no loader
template<typename GC>
Value<GC> get_loader(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `loaderURL` getter
///
/// Returns the URL of the loader (the SWF that initiated the load).
///
/// @param activation The current activation context
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return Loader URL string
template<typename GC>
Value<GC> get_loader_url(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `parameters` getter
///
/// Returns the parameters object for the loaded content.
///
/// @param activation The current activation context
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return Parameters object with key-value pairs
template<typename GC>
Value<GC> get_parameters(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `sharedEvents` getter
///
/// Returns the SharedEventDispatcher for the loaded content.
///
/// @param activation Unused
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return SharedEventDispatcher object
template<typename GC>
Value<GC> get_shared_events(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// `uncaughtErrorEvents` getter
///
/// Returns the UncaughtErrorEvents for the loaded content.
///
/// @param activation Unused
/// @param this_value The LoaderInfo object (this)
/// @param args Unused
/// @return UncaughtErrorEvents object
template<typename GC>
Value<GC> get_uncaught_error_events(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace loader_info
} // namespace display
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_LOADER_INFO_H
