// C++ translation of core/src/avm2/flv.rs
// FLV (Flash Video) data conversion to AVM2 values

#ifndef RUFFLE_CORE_AVM2_FLV_H
#define RUFFLE_CORE_AVM2_FLV_H

#include <vector>
#include <string>
#include <variant>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
    class UpdateContext;
    class AvmString;
    
    namespace avm2 {
        template<typename T> class Value;
        
        namespace object {
            class ScriptObject;
            class ArrayObject;
            class DateObject;
            
            template<typename GC>
            class TObject;
        }
    }
}
}

// Placeholder for chrono types
namespace chrono {
    template<typename Rep, typename Period> class duration;
    template<typename Clock, typename Duration> class time_point;
}

// Placeholder for flv_rs types
namespace flv_rs {
    class Value;
    struct Variable {
        std::string name;
        Value data;
    };
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Convert FLV variables to AVM2 object
template<typename GC>
Value<GC> avm2_object_from_flv_variables(
    UpdateContext* context,
    const std::vector<flv_rs::Variable>& variables
);

/// Convert FLV values to AVM2 array
template<typename GC>
Value<GC> avm2_array_from_flv_values(
    UpdateContext* context,
    const std::vector<flv_rs::Value>& values
);

/// Convert FLV date to AVM2 date
template<typename GC>
Value<GC> avm2_date_from_flv_date(
    UpdateContext* context,
    double unix_time,
    int16_t local_offset
);

/// Extension trait for FLV Value to AVM2 Value conversion
template<typename GC>
class FlvValueAvm2Ext {
public:
    /// Convert FLV value to AVM2 value
    static Value<GC> to_avm2_value(const flv_rs::Value& flv_value, UpdateContext* context);
};

/// Convert FLV value to AVM2 value
/// 
/// Handles various FLV data types:
/// - Object/EcmaArray -> ScriptObject
/// - StrictArray -> ArrayObject
/// - String/LongString -> AvmString
/// - Date -> DateObject
/// - Number -> numeric Value
/// - Boolean -> boolean Value
/// - Null -> Null Value
/// - Undefined -> Undefined Value
template<typename GC>
Value<GC> flv_to_avm2_value(const flv_rs::Value& flv_value, UpdateContext* context);

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_FLV_H
