// C++ translation of tests/framework/src/image_trigger.rs
//! Image trigger types for test framework

#ifndef RUFFLE_TESTS_FRAMEWORK_IMAGE_TRIGGER_H
#define RUFFLE_TESTS_FRAMEWORK_IMAGE_TRIGGER_H

#include <string>
#include <cstdint>
#include <variant>
#include <stdexcept>

namespace ruffle {
namespace tests {
namespace framework {

/// Image trigger for test framework
/// Determines when to capture an image during test execution
class ImageTrigger {
public:
    /// The type of image trigger
    enum class Type {
        LastFrame,           // Capture on the last frame
        SpecificIteration,   // Capture on a specific iteration/frame
        FsCommand            // Capture on fs_command
    };

    /// Default constructor - LastFrame
    ImageTrigger() : type_(Type::LastFrame), iteration_(0) {}

    /// Constructor for SpecificIteration
    explicit ImageTrigger(uint32_t iteration)
        : type_(Type::SpecificIteration), iteration_(iteration) {}

    /// Constructor for FsCommand
    static ImageTrigger fs_command() {
        ImageTrigger trigger;
        trigger.type_ = Type::FsCommand;
        trigger.iteration_ = 0;
        return trigger;
    }

    /// Constructor for LastFrame
    static ImageTrigger last_frame() {
        return ImageTrigger();
    }

    /// Get the trigger type
    Type type() const { return type_; }

    /// Get the iteration number (only valid for SpecificIteration)
    uint32_t iteration() const { return iteration_; }

    /// Check if this is a SpecificIteration trigger
    bool is_specific_iteration() const {
        return type_ == Type::SpecificIteration;
    }

    /// Check if this is a LastFrame trigger
    bool is_last_frame() const {
        return type_ == Type::LastFrame;
    }

    /// Check if this is a FsCommand trigger
    bool is_fs_command() const {
        return type_ == Type::FsCommand;
    }

    /// Equality operator
    bool operator==(const ImageTrigger& other) const {
        if (type_ != other.type_) {
            return false;
        }
        if (type_ == Type::SpecificIteration) {
            return iteration_ == other.iteration_;
        }
        return true;
    }

    /// Inequality operator
    bool operator!=(const ImageTrigger& other) const {
        return !(*this == other);
    }

private:
    Type type_;
    uint32_t iteration_;
};

/// Parse an ImageTrigger from a string representation
/// Supports:
/// - "last_frame" -> ImageTrigger::LastFrame
/// - "fs_command" -> ImageTrigger::FsCommand
/// - numeric string -> ImageTrigger::SpecificIteration(value)
inline ImageTrigger parse_image_trigger(const std::string& value) {
    // Check for special strings
    if (value == "last_frame") {
        return ImageTrigger::last_frame();
    }
    if (value == "fs_command") {
        return ImageTrigger::fs_command();
    }

    // Try to parse as number
    try {
        uint32_t iteration = std::stoul(value);
        return ImageTrigger(iteration);
    } catch (const std::exception&) {
        throw std::invalid_argument(
            "Invalid image trigger: expected 'last_frame', 'fs_command', or a frame/tick number"
        );
    }
}

/// Parse an ImageTrigger from an integer value
inline ImageTrigger parse_image_trigger(int64_t value) {
    if (value < 0 || value > static_cast<int64_t>(UINT32_MAX)) {
        throw std::out_of_range("Image trigger value out of range");
    }
    return ImageTrigger(static_cast<uint32_t>(value));
}

/// Parse an ImageTrigger from an unsigned integer value
inline ImageTrigger parse_image_trigger(uint64_t value) {
    if (value > static_cast<uint64_t>(UINT32_MAX)) {
        throw std::out_of_range("Image trigger value out of range");
    }
    return ImageTrigger(static_cast<uint32_t>(value));
}

} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_IMAGE_TRIGGER_H
