// C++ translation of core/src/avm2/stubs.rs
// AVM2 Stub tracking macros

#ifndef RUFFLE_CORE_AVM2_STUBS_H
#define RUFFLE_CORE_AVM2_STUBS_H

#include <string>
#include <string_view>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
namespace stub {

/// Stub types for tracking unimplemented features
enum class StubType {
    Avm2Method,
    Avm2Constructor,
    Avm2Getter,
    Avm2Setter,
};

/// Represents an unimplemented stub
struct Stub {
    StubType type;
    std::string_view class_name;
    std::string_view method_or_property;
    std::optional<std::string_view> specifics;
};

/// Stub tracker - tracks encountered stubs
class StubTracker {
public:
    void encounter(const Stub* stub);
};

} // namespace stub

namespace avm2 {

/// Context for stub tracking
struct StubContext {
    stub::StubTracker stub_tracker;
};

} // namespace avm2
} // namespace core
} // namespace ruffle

// Stub tracking macros
// These macros track unimplemented methods/constructors/getters/setters

#define AVM2_STUB_METHOD_CONTEXT(context, class_name, method_name) \
    do { \
        static const ::ruffle::core::stub::Stub STUB = { \
            ::ruffle::core::stub::StubType::Avm2Method, \
            class_name, \
            method_name, \
            std::nullopt \
        }; \
        (context).stub_tracker.encounter(&STUB); \
    } while (0)

#define AVM2_STUB_METHOD_CONTEXT_WITH_SPECIFICS(context, class_name, method_name, specifics) \
    do { \
        static const ::ruffle::core::stub::Stub STUB = { \
            ::ruffle::core::stub::StubType::Avm2Method, \
            class_name, \
            method_name, \
            std::make_optional<std::string_view>(specifics) \
        }; \
        (context).stub_tracker.encounter(&STUB); \
    } while (0)

#define AVM2_STUB_METHOD(activation, class_name, method_name) \
    AVM2_STUB_METHOD_CONTEXT((activation).context, class_name, method_name)

#define AVM2_STUB_METHOD_WITH_SPECIFICS(activation, class_name, method_name, specifics) \
    AVM2_STUB_METHOD_CONTEXT_WITH_SPECIFICS((activation).context, class_name, method_name, specifics)

#define AVM2_STUB_CONSTRUCTOR(activation, class_name) \
    do { \
        static const ::ruffle::core::stub::Stub STUB = { \
            ::ruffle::core::stub::StubType::Avm2Constructor, \
            class_name, \
            "", \
            std::nullopt \
        }; \
        (activation).context.stub_tracker.encounter(&STUB); \
    } while (0)

#define AVM2_STUB_CONSTRUCTOR_WITH_SPECIFICS(activation, class_name, specifics) \
    do { \
        static const ::ruffle::core::stub::Stub STUB = { \
            ::ruffle::core::stub::StubType::Avm2Constructor, \
            class_name, \
            "", \
            std::make_optional<std::string_view>(specifics) \
        }; \
        (activation).context.stub_tracker.encounter(&STUB); \
    } while (0)

#define AVM2_STUB_GETTER(activation, class_name, property) \
    do { \
        static const ::ruffle::core::stub::Stub STUB = { \
            ::ruffle::core::stub::StubType::Avm2Getter, \
            class_name, \
            property, \
            std::nullopt \
        }; \
        (activation).context.stub_tracker.encounter(&STUB); \
    } while (0)

#define AVM2_STUB_SETTER(activation, class_name, property) \
    do { \
        static const ::ruffle::core::stub::Stub STUB = { \
            ::ruffle::core::stub::StubType::Avm2Setter, \
            class_name, \
            property, \
            std::nullopt \
        }; \
        (activation).context.stub_tracker.encounter(&STUB); \
    } while (0)

#endif // RUFFLE_CORE_AVM2_STUBS_H
