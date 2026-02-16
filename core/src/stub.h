// C++ translation of core/src/stub.rs
//! Stub tracking for unimplemented features

#ifndef RUFFLE_CORE_STUB_H
#define RUFFLE_CORE_STUB_H

#include <string>
#include <string_view>
#include <optional>
#include <unordered_set>
#include <vector>
#include <fmt/format.h>

// Forward declarations
namespace ruffle {
namespace core {
namespace stub {

/// Stub types for tracking unimplemented features
enum class StubType {
    Avm1Method,
    Avm1Constructor,
    Avm2Method,
    Avm2Getter,
    Avm2Setter,
    Avm2Constructor,
    Other,
};

/// Represents an unimplemented stub
class Stub {
public:
    StubType type;
    std::string class_name;
    std::string method_or_property;
    std::optional<std::string> specifics;

    Stub(StubType t, std::string cls, std::string method, std::optional<std::string> spec = std::nullopt)
        : type(t)
        , class_name(std::move(cls))
        , method_or_property(std::move(method))
        , specifics(std::move(spec))
    {}

    /// Get the AVM2 class name if applicable
    std::optional<std::string> avm2_class() const {
        switch (type) {
            case StubType::Avm2Method:
            case StubType::Avm2Getter:
            case StubType::Avm2Setter:
            case StubType::Avm2Constructor:
                return class_name;
            default:
                return std::nullopt;
        }
    }

    /// String representation
    std::string to_string() const {
        switch (type) {
            case StubType::Avm1Method:
                if (specifics.has_value()) {
                    return fmt::format("AVM1 {}.{}() {}", class_name, method_or_property, specifics.value());
                }
                return fmt::format("AVM1 {}.{}()", class_name, method_or_property);

            case StubType::Avm1Constructor:
                return fmt::format("AVM1 {}() constructor", class_name);

            case StubType::Avm2Method:
                if (specifics.has_value()) {
                    return fmt::format("AVM2 {}.{}() {}", class_name, method_or_property, specifics.value());
                }
                return fmt::format("AVM2 {}.{}()", class_name, method_or_property);

            case StubType::Avm2Getter:
                return fmt::format("AVM2 {}.{} getter", class_name, method_or_property);

            case StubType::Avm2Setter:
                return fmt::format("AVM2 {}.{} setter", class_name, method_or_property);

            case StubType::Avm2Constructor:
                if (specifics.has_value()) {
                    return fmt::format("AVM2 {} constructor {}", class_name, specifics.value());
                }
                return fmt::format("AVM2 {} constructor", class_name);

            case StubType::Other:
                return class_name;
        }
        return "Unknown stub";
    }

    /// Hash function for unordered_set
    struct Hash {
        size_t operator()(const Stub& s) const {
            size_t h1 = std::hash<std::string>{}(s.class_name);
            size_t h2 = std::hash<std::string>{}(s.method_or_property);
            size_t h3 = s.specifics.has_value()
                ? std::hash<std::string>{}(s.specifics.value())
                : 0;
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };

    /// Equality operator
    bool operator==(const Stub& other) const {
        return type == other.type
            && class_name == other.class_name
            && method_or_property == other.method_or_property
            && specifics == other.specifics;
    }
};

/// Collection of encountered stubs
class StubCollection {
public:
    StubCollection() = default;

    /// Record an encountered stub
    void encounter(const Stub& stub) {
        if (stubs_.find(stub) == stubs_.end()) {
            // Log warning about encountered stub
            // tracing::warn!("Encountered stub: {}", stub.to_string());
            stubs_.insert(stub);
        }
    }

    /// Iterate over all encountered stubs
    const std::unordered_set<Stub, Stub::Hash>& stubs() const {
        return stubs_;
    }

    /// Get the number of unique stubs encountered
    size_t size() const {
        return stubs_.size();
    }

private:
    std::unordered_set<Stub, Stub::Hash> stubs_;
};

} // namespace stub

/// Context for stub tracking
namespace context {

class UpdateContext {
public:
    stub::StubCollection& stub_tracker() {
        return stub_tracker_;
    }

private:
    stub::StubCollection stub_tracker_;
};

} // namespace context
} // namespace core
} // namespace ruffle

// Stub tracking macros
// These macros track unimplemented methods/constructors/getters/setters

#define CONTEXT_STUB(context, message) \
    do { \
        static const ::ruffle::core::stub::Stub STUB( \
            ::ruffle::core::stub::StubType::Other, \
            std::string(message), \
            "" \
        ); \
        (context).stub_tracker().encounter(STUB); \
    } while (0)

#endif // RUFFLE_CORE_STUB_H
