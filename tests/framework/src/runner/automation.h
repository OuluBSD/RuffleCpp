// C++ translation of tests/framework/src/runner/automation.rs
// Automated event handling for test framework

#ifndef RUFFLE_TESTS_FRAMEWORK_RUNNER_AUTOMATION_H
#define RUFFLE_TESTS_FRAMEWORK_RUNNER_AUTOMATION_H

#include <cstdint>
#include <string>
#include <variant>
#include <optional>
#include <utility>

// Forward declarations
namespace ruffle {
namespace core {
    class Player;
    
    namespace events {
        enum class MouseButton;
        enum class TextControlCode;
        
        struct KeyDescriptor;
        enum class KeyLocation;
        
        class LogicalKey;
        enum class NamedKey;
        class PhysicalKey;
        
        enum class ImeEvent;
        class MouseWheelDelta;
        
        class PlayerEvent;
    }
}

namespace input_format {
    enum class MouseButton;
    enum class TextControlCode;
    
    struct AutomatedKey;
    struct AutomatedEvent;
}
}

namespace ruffle {
namespace tests {
namespace framework {
namespace runner {
namespace automation {

/// Perform an automated event on the player
///
/// \param evt The automated event to perform
/// \param player The player to perform the event on
void perform_automated_event(const input_format::AutomatedEvent& evt, core::Player& player);

/// Convert an automated key to a key descriptor
///
/// \param automated_key The automated key to convert
/// \return Key descriptor for the core event system
core::events::KeyDescriptor automated_key_to_descriptor(const input_format::AutomatedKey& automated_key);

} // namespace automation
} // namespace runner
} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_RUNNER_AUTOMATION_H
