// C++ translation of tests/tests/external_interface/mod.rs
//! External interface test module

#ifndef RUFFLE_TESTS_EXTERNAL_INTERFACE_H
#define RUFFLE_TESTS_EXTERNAL_INTERFACE_H

#include <optional>
#include <string>
#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class UpdateContext;

    namespace external {
        template<typename GC>
        class ExternalInterfaceProvider;

        template<typename GC>
        class Value;
    }
}
}

namespace ruffle {
namespace tests {
namespace external_interface {

/// External interface test provider for testing ExternalInterface functionality
template<typename GC>
class ExternalInterfaceTestProvider {
public:
    ExternalInterfaceTestProvider() = default;

    /// Create a new ExternalInterfaceTestProvider
    static ExternalInterfaceTestProvider create() {
        return ExternalInterfaceTestProvider();
    }
};

/// Trace handler for ExternalInterface tests
/// Outputs traced arguments and returns "Traced!"
template<typename GC>
Value<GC> do_trace(UpdateContext<GC>* context, const std::vector<Value<GC>>& args) {
    // Trace the arguments
    context->avm_trace("[ExternalInterface] trace: " + format_args(args));
    return Value<GC>::from_string("Traced!");
}

/// Ping handler for ExternalInterface tests
/// Outputs ping message and returns "Pong!"
template<typename GC>
Value<GC> do_ping(UpdateContext<GC>* context, const std::vector<Value<GC>>& args) {
    context->avm_trace("[ExternalInterface] ping");
    return Value<GC>::from_string("Pong!");
}

/// Reentry handler for ExternalInterface tests
/// Tests reentrant callback functionality
template<typename GC>
Value<GC> do_reentry(UpdateContext<GC>* context, const std::vector<Value<GC>>& args) {
    context->avm_trace("[ExternalInterface] starting reentry");

    auto callback = context->external_interface.get_callback("callWith");
    if (callback.has_value()) {
        std::vector<Value<GC>> call_args = {
            Value<GC>::from_string("trace"),
            Value<GC>::from_string("successful reentry!")
        };
        return callback.value()->call(context, "callWith", call_args);
    }

    return Value<GC>::null();
}

/// ExternalInterfaceProvider implementation for ExternalInterfaceTestProvider
template<typename GC>
class ExternalInterfaceTestProviderImpl : public ExternalInterfaceProvider<GC> {
public:
    /// Call a method by name with the given arguments
    Value<GC> call_method(
        UpdateContext<GC>* context,
        const std::string& name,
        const std::vector<Value<GC>>& args
    ) override {
        if (name == "trace") {
            return do_trace(context, args);
        } else if (name == "ping") {
            return do_ping(context, args);
        } else if (name == "reentry") {
            return do_reentry(context, args);
        }
        return Value<GC>::null();
    }

    /// Called when a callback becomes available
    void on_callback_available(const std::string& name) override {
        // No-op for test provider
    }

    /// Get the ID of this provider
    std::optional<std::string> get_id() override {
        return std::nullopt;
    }
};

} // namespace external_interface
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_EXTERNAL_INTERFACE_H
