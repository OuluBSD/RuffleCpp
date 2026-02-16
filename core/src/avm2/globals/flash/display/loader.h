// C++ translation of core/src/avm2/globals/flash/display/loader.rs
// flash.display.Loader builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_LOADER_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_LOADER_H

#include <vector>
#include <string>
#include <cstdint>
#include <map>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class ClassObject;
    template<typename T> class TObject;
    namespace object {
        template<typename T> class LoaderInfoObject;
        template<typename T> class LoaderStream;
        template<typename T> class Object;
    }
}
namespace backend {
namespace navigator {
    enum class NavigationMethod;
    class Request;
}
}
namespace display_object {
    template<typename T> class LoaderDisplay;
    template<typename T> class MovieClip;
}
namespace loader {
    class LoadManager;
    template<typename T> struct MovieLoaderVMData;
}
namespace tag_utils {
    class SwfMovie;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace slots {
    // Forward declare slot constants from flash_display_loader
    namespace flash_display_loader {
        extern const int _CONTENT_LOADER_INFO;
    }
    
    // Forward declare slot constants from flash_net_url_request
    namespace flash_net_url_request {
        extern const int _URL;
        extern const int _METHOD;
        extern const int _REQUEST_HEADERS;
        extern const int _DATA;
        extern const int _CONTENT_TYPE;
    }
    
    // Forward declare slot constants from flash_net_url_request_header
    namespace flash_net_url_request_header {
        extern const int NAME;
        extern const int VALUE;
    }
}
}
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace display {
namespace loader {

/// Allocator function for flash.display.Loader
/// Creates a new Loader display object with initialized contentLoaderInfo
template<typename GC>
typename avm2::object::Object<GC>* loader_allocator(
    ClassObject<GC>* class_obj,
    Activation<GC>* activation
);

/// Implements Loader.load
/// Loads content from a URLRequest into the loader
/// @param request The URLRequest to load
/// @param context Optional LoaderContext for load settings
template<typename GC>
Value<GC> load(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Helper function to convert URLRequest to internal Request
/// Extracts URL, method, headers, and data from URLRequest
template<typename GC>
typename backend::navigator::Request request_from_url_request(
    Activation<GC>* activation,
    typename avm2::object::Object<GC>* url_request
);

/// Implements Loader.loadBytes
/// Loads content from a byte array into the loader
/// @param data The byte array containing SWF data
/// @param context Optional LoaderContext for load settings
template<typename GC>
Value<GC> load_bytes(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Loader.unload
/// Unloads the content from the loader
/// Currently a stub implementation
template<typename GC>
Value<GC> unload(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace loader
} // namespace display
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_LOADER_H
