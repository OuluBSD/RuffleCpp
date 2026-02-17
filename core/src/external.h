// C++ translation of core/src/external.rs
//! External interface for ActionScript to communicate with outside world

#ifndef RUFFLE_CORE_EXTERNAL_H
#define RUFFLE_CORE_EXTERNAL_H

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <memory>
#include <cstdint>
#include <variant>
#include <functional>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class AvmString;
    template<typename GC>
    class UpdateContext;

    namespace avm1 {
        template<typename GC>
        class Value;
        template<typename GC>
        class Object;
        template<typename GC>
        class Activation;
        template<typename GC>
        class Error;
        enum class ActivationIdentifier;
    }

    namespace avm2 {
        template<typename GC>
        class Value;
        template<typename GC>
        class Error;
        template<typename GC>
        class Activation;
        template<typename GC>
        class FunctionObject;
        template<typename GC>
        class Object;
        template<typename GC>
        class ArrayObject;
        template<typename GC>
        class ArrayStorage;

        namespace globals {
            template<typename GC>
            class FlashExternalInterface;
        }
    }

    namespace display_object {
        template<typename GC>
        class DisplayObject;
        template<typename GC>
        class TDisplayObject;
    }

    namespace stage {
        template<typename GC>
        class Stage;
    }

    namespace library {
        template<typename GC>
        class Library;
    }

    namespace gc_arena {
        template<typename T>
        class Gc;
        class Mutation;
    }

    struct Context;
}
}

namespace ruffle {
namespace core {

using gc_arena::Gc;
using gc_arena::Mutation;

/// An intermediate format of representing shared data between ActionScript and elsewhere.
///
/// Regardless of the capabilities of both sides, all data will be translated to this potentially
/// lossy format. Any recursion or additional metadata in ActionScript will not be translated.
template<typename GC>
class ExternalValue {
public:
    enum class Type {
        Undefined,
        Null,
        Bool,
        Number,
        String,
        Object,
        List
    };

private:
    Type type_;

    // Storage for different value types
    struct Storage {
        bool bool_value;
        double number_value;
        std::string string_value;
        std::map<std::string, ExternalValue> object_value;
        std::vector<ExternalValue> list_value;

        Storage() : bool_value(false), number_value(0.0) {}
    };

    Storage storage_;

public:
    // Constructors
    ExternalValue() : type_(Type::Undefined) {}

    static ExternalValue undefined() {
        return ExternalValue();
    }

    static ExternalValue null() {
        ExternalValue val;
        val.type_ = Type::Null;
        return val;
    }

    static ExternalValue from_bool(bool value) {
        ExternalValue val;
        val.type_ = Type::Bool;
        val.storage_.bool_value = value;
        return val;
    }

    static ExternalValue from_number(double value) {
        ExternalValue val;
        val.type_ = Type::Number;
        val.storage_.number_value = value;
        return val;
    }

    static ExternalValue from_string(const std::string& value) {
        ExternalValue val;
        val.type_ = Type::String;
        val.storage_.string_value = value;
        return val;
    }

    static ExternalValue from_object(const std::map<std::string, ExternalValue>& value) {
        ExternalValue val;
        val.type_ = Type::Object;
        val.storage_.object_value = value;
        return val;
    }

    static ExternalValue from_list(const std::vector<ExternalValue>& value) {
        ExternalValue val;
        val.type_ = Type::List;
        val.storage_.list_value = value;
        return val;
    }

    // Type converters from various numeric types
    static ExternalValue from_f32(float value) {
        return from_number(static_cast<double>(value));
    }

    static ExternalValue from_u8(uint8_t value) {
        return from_number(static_cast<double>(value));
    }

    static ExternalValue from_i16(int16_t value) {
        return from_number(static_cast<double>(value));
    }

    static ExternalValue from_u16(uint16_t value) {
        return from_number(static_cast<double>(value));
    }

    static ExternalValue from_i32(int32_t value) {
        return from_number(static_cast<double>(value));
    }

    static ExternalValue from_i64(int64_t value) {
        return from_number(static_cast<double>(value));
    }

    static ExternalValue from_u32(uint32_t value) {
        return from_number(static_cast<double>(value));
    }

    static ExternalValue from_usize(size_t value) {
        return from_number(static_cast<double>(value));
    }

    // Type accessors
    Type type() const { return type_; }

    bool is_undefined() const { return type_ == Type::Undefined; }
    bool is_null() const { return type_ == Type::Null; }
    bool is_bool() const { return type_ == Type::Bool; }
    bool is_number() const { return type_ == Type::Number; }
    bool is_string() const { return type_ == Type::String; }
    bool is_object() const { return type_ == Type::Object; }
    bool is_list() const { return type_ == Type::List; }

    bool as_bool() const { return storage_.bool_value; }
    double as_number() const { return storage_.number_value; }
    const std::string& as_string() const { return storage_.string_value; }
    const std::map<std::string, ExternalValue>& as_object() const { return storage_.object_value; }
    const std::vector<ExternalValue>& as_list() const { return storage_.list_value; }

    // Convert from AVM1 value
    static ExternalValue from_avm1(
        avm1::Activation<GC>* activation,
        avm1::Value<GC> value
    );

    // Convert to AVM1 value
    avm1::Value<GC> into_avm1(avm1::Activation<GC>* activation) const;

    // Convert from AVM2 value
    static ExternalValue from_avm2(
        avm2::Activation<GC>* activation,
        avm2::Value<GC> value
    );

    // Convert to AVM2 value
    avm2::Value<GC> into_avm2(UpdateContext<GC>* context) const;
};

/// Callback types for external interface
template<typename GC>
class ExternalCallback {
public:
    enum class Type {
        Avm1,
        Avm2
    };

private:
    Type type_;

    // AVM1 callback data
    avm1::Value<GC> avm1_this_;
    avm1::Object<GC> avm1_method_;

    // AVM2 callback data
    avm2::FunctionObject<GC> avm2_method_;

public:
    ExternalCallback() : type_(Type::Avm1) {}

    static ExternalCallback avm1_callback(
        avm1::Value<GC> this_value,
        avm1::Object<GC> method
    ) {
        ExternalCallback cb;
        cb.type_ = Type::Avm1;
        cb.avm1_this_ = this_value;
        cb.avm1_method_ = method;
        return cb;
    }

    static ExternalCallback avm2_callback(
        avm2::FunctionObject<GC> method
    ) {
        ExternalCallback cb;
        cb.type_ = Type::Avm2;
        cb.avm2_method_ = method;
        return cb;
    }

    Type type() const { return type_; }

    /// Call the callback with the given arguments
    ExternalValue<GC> call(
        UpdateContext<GC>* context,
        const std::string& name,
        const std::vector<ExternalValue<GC>>& args
    ) const;
};

/// Trait for FSCommand provider
class FsCommandProvider {
public:
    virtual ~FsCommandProvider() = default;

    /// Handle an FSCommand
    virtual bool on_fs_command(const std::string& command, const std::string& args) = 0;
};

/// Null implementation of FsCommandProvider
class NullFsCommandProvider : public FsCommandProvider {
public:
    bool on_fs_command(const std::string& /*command*/, const std::string& /*args*/) override {
        return false;
    }
};

/// Trait for external interface provider
class ExternalInterfaceProvider {
public:
    virtual ~ExternalInterfaceProvider() = default;

    /// Call a method through the external interface
    virtual ExternalValue<> call_method(
        UpdateContext<>* context,
        const std::string& name,
        const std::vector<ExternalValue<>>& args
    ) = 0;

    /// Notify that a callback is available
    virtual void on_callback_available(const std::string& name) = 0;

    /// Get the ID of this external interface
    virtual std::optional<std::string> get_id() = 0;
};

/// Null implementation of ExternalInterfaceProvider
class NullExternalInterfaceProvider : public ExternalInterfaceProvider {
public:
    ExternalValue<> call_method(
        UpdateContext<>* /*context*/,
        const std::string& /*name*/,
        const std::vector<ExternalValue<>>& /*args*/
    ) override {
        return ExternalValue<>::undefined();
    }

    void on_callback_available(const std::string& /*name*/) override {}

    std::optional<std::string> get_id() override {
        return std::nullopt;
    }
};

/// External interface for ActionScript to communicate with outside world
template<typename GC>
class ExternalInterface {
private:
    std::shared_ptr<ExternalInterfaceProvider> provider_;
    std::map<std::string, ExternalCallback<GC>> callbacks_;
    std::unique_ptr<FsCommandProvider> fs_commands_;

public:
    /// Create a new ExternalInterface
    ExternalInterface(
        std::optional<std::unique_ptr<ExternalInterfaceProvider>> provider,
        std::unique_ptr<FsCommandProvider> fs_commands
    )
        : provider_(provider.has_value() ? std::move(*provider) : nullptr)
        , fs_commands_(std::move(fs_commands))
    {}

    /// Set the provider
    void set_provider(std::optional<std::unique_ptr<ExternalInterfaceProvider>> provider) {
        provider_ = provider.has_value() ? std::move(*provider) : nullptr;
    }

    /// Add a callback
    void add_callback(const std::string& name, ExternalCallback<GC> callback) {
        callbacks_[name] = callback;
        if (provider_) {
            provider_->on_callback_available(name);
        }
    }

    /// Get a callback by name
    std::optional<ExternalCallback<GC>> get_callback(const std::string& name) const {
        auto it = callbacks_.find(name);
        if (it != callbacks_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    /// Call a method through the external interface
    static ExternalValue<GC> call_method(
        UpdateContext<GC>* context,
        const std::string& name,
        const std::vector<ExternalValue<GC>>& args
    ) {
        if (context->external_interface.provider) {
            return context->external_interface.provider->call_method(context, name, args);
        }
        return ExternalValue<GC>::undefined();
    }

    /// Check if external interface is available
    bool available() const {
        return provider_ != nullptr;
    }

    /// Get the ID of this external interface
    std::optional<std::string> get_id() const {
        if (provider_) {
            return provider_->get_id();
        }
        return std::nullopt;
    }

    /// Invoke an FSCommand
    bool invoke_fs_command(const std::string& command, const std::string& args) const {
        return fs_commands_->on_fs_command(command, args);
    }
};

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_EXTERNAL_H
