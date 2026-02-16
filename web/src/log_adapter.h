// C++ translation of web/src/log_adapter.rs
//! Web-based log backend for Ruffle

#ifndef RUFFLE_WEB_LOG_ADAPTER_H
#define RUFFLE_WEB_LOG_ADAPTER_H

#include <memory>
#include <string>
#include <string_view>

// Forward declarations
namespace ruffle {
namespace core {
namespace backend {
namespace log {
    class LogBackend;
}
}
}
}

// Placeholder for JavaScript interop types
namespace js_sys {
    class Function;
}

namespace wasm_bindgen {
    class JsValue;
}

namespace ruffle {
namespace web {

/// WebLogBackend - Log backend for web/Emscripten builds
/// 
/// This backend forwards AVM trace messages to a JavaScript observer
/// function, allowing integration with browser console or custom logging.
class WebLogBackend : public ruffle::core::backend::log::LogBackend {
private:
    std::shared_ptr<wasm_bindgen::JsValue> trace_observer_;

public:
    /// Create a new WebLogBackend
    /// 
    /// \param trace_observer A JavaScript function to receive trace messages
    explicit WebLogBackend(std::shared_ptr<wasm_bindgen::JsValue> trace_observer)
        : trace_observer_(std::move(trace_observer)) {}

    /// Trace a message from AVM execution
    /// 
    /// Sends the message to both the tracing system and the JavaScript observer.
    /// 
    /// \param message The message to trace
    void avm_trace(std::string_view message) override;

    /// Log a warning from AVM execution
    /// 
    /// \param message The warning message
    void avm_warning(std::string_view message) override;
};

} // namespace web
} // namespace ruffle

#endif // RUFFLE_WEB_LOG_ADAPTER_H
