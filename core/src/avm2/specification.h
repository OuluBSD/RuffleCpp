// C++ translation of core/src/avm2/specification.rs
//! AVM2 specification capture utility

#ifndef RUFFLE_CORE_AVM2_SPECIFICATION_H
#define RUFFLE_CORE_AVM2_SPECIFICATION_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <memory>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class Activation;
    template<typename GC>
    class Value;
    template<typename GC>
    class AvmString;
    template<typename GC>
    class Method;
    template<typename GC>
    class ClassObject;
    template<typename GC>
    class Trait;
    template<typename GC>
    class TraitKind;
    template<typename GC>
    class Multiname;

    namespace avm2 {
        template<typename GC>
        class Avm2;
        template<typename GC>
        class BoundMethod;
        struct ParamConfig;
    }

    struct UpdateContext;
    class Stub;

    namespace gc_arena {
        class Mutation;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

using gc_arena::Mutation;

/// Parameter information for serialization
struct ParamInfo {
    std::string type_info;
    std::optional<std::string> value;
    bool variadic = false;

    bool should_skip_variadic() const { return !variadic; }
    bool should_skip_value() const { return !value.has_value(); }
};

/// Class information for serialization
struct ClassInfo {
    bool dynamic = false;
    std::optional<std::string> extends;
    std::optional<std::string> implements;
    bool is_final = false;

    bool should_skip_dynamic() const { return !dynamic; }
    bool should_skip_extends() const { return !extends.has_value(); }
    bool should_skip_implements() const { return !implements.has_value(); }
    bool should_skip_final() const { return !is_final; }
};

/// Variable information for serialization
struct VariableInfo {
    std::optional<std::string> type_info;
    std::optional<std::string> value;
    bool stubbed = false;

    bool should_skip_type() const { return !type_info.has_value(); }
    bool should_skip_value() const { return !value.has_value(); }
    bool should_skip_stubbed() const { return !stubbed; }

    static VariableInfo from_value(Value value, Activation* activation);
};

/// Function information for serialization
struct FunctionInfo {
    std::vector<ParamInfo> args;
    std::string returns;
    bool stubbed = false;

    bool should_skip_stubbed() const { return !stubbed; }

    static FunctionInfo from_method(Method method, bool stubbed);
    static FunctionInfo from_bound_method(const BoundMethod* executable, bool stubbed);
};

/// Trait list for serialization
struct TraitList {
    std::unordered_map<std::string, VariableInfo> constants;
    std::unordered_map<std::string, VariableInfo> variables;
    std::unordered_map<std::string, FunctionInfo> functions;
    std::unordered_map<std::string, VariableInfo> getters;
    std::unordered_map<std::string, VariableInfo> setters;

    bool constants_empty() const { return constants.empty(); }
    bool variables_empty() const { return variables.empty(); }
    bool functions_empty() const { return functions.empty(); }
    bool getters_empty() const { return getters.empty(); }
    bool setters_empty() const { return setters.empty(); }
};

/// Definition for serialization
struct Definition {
    std::optional<ClassInfo> classinfo;
    std::optional<TraitList> static_traits;
    std::optional<TraitList> instance_traits;
    std::optional<TraitList> prototype;

    bool should_skip_classinfo() const { return !classinfo.has_value(); }
    bool should_skip_static() const { return !static_traits.has_value(); }
    bool should_skip_instance() const { return !instance_traits.has_value(); }
    bool should_skip_prototype() const { return !prototype.has_value(); }
};

/// Class stubs tracker
class ClassStubs {
private:
    std::unordered_set<std::string> methods_;
    std::unordered_set<std::string> getters_;
    std::unordered_set<std::string> setters_;

public:
    ClassStubs() = default;

    static ClassStubs for_class(
        const std::string& class_name,
        const std::unordered_set<const Stub*>& stubs
    );

    bool has_method(const std::string& name) const {
        return methods_.find(name) != methods_.end();
    }

    bool has_getter(const std::string& name) const {
        return getters_.find(name) != getters_.end();
    }

    bool has_setter(const std::string& name) const {
        return setters_.find(name) != setters_.end();
    }
};

/// Capture specification data and write to JSON
void capture_specification(UpdateContext* context, const std::string& output_path);

/// Helper function to check if false (for serialization skip logic)
inline bool is_false(bool b) {
    return !b;
}

/// Escape a string for JSON output
std::string escape_string(AvmString string);

/// Format a value for serialization
std::optional<std::string> format_value(const Value& value);

/// Format method signature
std::vector<ParamInfo> format_signature(const std::vector<ParamConfig>& params, bool is_variadic);

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_SPECIFICATION_H
