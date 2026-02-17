// C++ translation of render/wgpu/src/utils.rs
// Utility functions for wgpu rendering

#ifndef RUFFLE_RENDER_WGPU_UTILS_H
#define RUFFLE_RENDER_WGPU_UTILS_H

#include <cstdint>
#include <cstddef>
#include <optional>
#include <functional>
#include <vector>
#include <string>
#include <array>
#include <syncstream>
#include <mutex>
#include <condition_variable>

// Forward declarations
namespace wgpu {
    struct TextureFormat;
    struct Buffer;
    struct Device;
    struct CommandEncoder;
    struct TextureView;
    struct BindGroup;
    struct Extent3d;
    enum class TextureFormat : uint32_t;
    enum class AstcChannel : uint8_t;
    struct AstcBlock;
    class SubmissionIndex;
}

namespace ruffle {
namespace core {
    template<typename GC> class UpdateContext;
}
namespace render {
    namespace utils {
        void unmultiply_alpha_rgba(std::vector<uint8_t>& bytes);
    }
}
}

namespace ruffle {
namespace render {
namespace wgpu {

// Forward declarations
struct BufferDescription;
struct Descriptors;
struct Globals;

/// Create a debug label if the render_debug_labels feature is enabled
///
/// In C++, this is controlled at compile time via RUFFLE_DEBUG_LABELS
#ifdef RUFFLE_DEBUG_LABELS
inline std::optional<std::string> create_debug_label(const char* format, ...) {
    // Implementation would use variadic args to format the string
    // For now, return a placeholder
    return std::optional<std::string>();
}
#else
inline std::optional<std::string> create_debug_label(const char*, ...) {
    return std::nullopt;
}
#endif

/// Remove sRGB suffix from a texture format
///
/// Converts sRGB texture formats to their linear counterparts.
///
/// \param format The texture format to convert
/// \return The non-sRGB version of the format
wgpu::TextureFormat remove_srgb(wgpu::TextureFormat format);

/// Format a list of strings with a connector
///
/// Formats a list like "A, B and C" or "A and B" depending on count.
///
/// \param values Array of string values to join
/// \param connector The connector string (e.g., "and", "or")
/// \return Formatted string
std::string format_list(const std::vector<std::string_view>& values, std::string_view connector);

/// Get backend names from wgpu backends flags
///
/// \param backends The backend flags
/// \return Vector of backend names
std::vector<std::string_view> get_backend_names(uint32_t backends);

/// Create a buffer with initial data
///
/// Creates a wgpu buffer initialized with the provided data.
///
/// \param device The wgpu device
/// \param data Pointer to the data to copy
/// \param size Size of the data in bytes
/// \param usage Buffer usage flags
/// \param label Optional debug label
/// \return The created buffer
wgpu::Buffer* create_buffer_with_data(
    wgpu::Device* device,
    const uint8_t* data,
    size_t size,
    uint32_t usage,
    std::optional<std::string> label
);

/// Buffer dimensions for texture capture
///
/// Stores the dimensions and row padding information needed
/// for capturing texture data from the GPU.
struct BufferDimensions {
    size_t width;
    size_t height;
    size_t unpadded_bytes_per_row;
    uint32_t padded_bytes_per_row;

    /// Default constructor
    BufferDimensions() = default;

    /// Create buffer dimensions for a texture
    ///
    /// \param width Texture width in pixels
    /// \param height Texture height in pixels
    /// \param format The texture format
    BufferDimensions(size_t width, size_t height, wgpu::TextureFormat format);

    /// Get the total buffer size in bytes
    ///
    /// \return Total size in bytes
    uint64_t size() const {
        return static_cast<uint64_t>(padded_bytes_per_row) * static_cast<uint64_t>(height);
    }
};

/// Buffer description trait for BufferDimensions
///
/// Implements cost calculation for buffer pool usage.
struct BufferDimensionsDescription : public BufferDescription {
    using Cost = uint64_t;

    /// Calculate the cost to use another buffer dimension
    ///
    /// \param other The other buffer dimensions to compare against
    /// \return Optional cost (difference in size) if this fits in other
    std::optional<Cost> cost_to_use(const BufferDimensions& other) const {
        if (size() <= other.size()) {
            return other.size() - size();
        }
        return std::nullopt;
    }
};

/// Capture an image from a GPU buffer
///
/// Maps the buffer and calls the provided function with the pixel data.
///
/// \tparam F Function type that takes (const uint8_t* rgba, uint32_t buffer_width)
/// \param device The wgpu device
/// \param buffer The buffer to read from
/// \param dimensions The buffer dimensions
/// \param index Optional submission index to wait for
/// \param with_rgba Function to call with the mapped data
/// \return Result from the function
template<typename F>
auto capture_image(
    wgpu::Device* device,
    wgpu::Buffer* buffer,
    const BufferDimensions& dimensions,
    std::optional<wgpu::SubmissionIndex*> index,
    F with_rgba
) {
    // Channel for synchronization
    std::promise<void> promise;
    std::future<void> future = promise.get_future();

    // Map the buffer
    // In real implementation, this would call buffer->map_async()
    // and wait for the callback

    // For now, placeholder implementation
    device->tick(); // Poll the device

    // Call the function with placeholder data
    std::vector<uint8_t> placeholder(dimensions.padded_bytes_per_row * dimensions.height);
    auto result = with_rgba(placeholder.data(), dimensions.padded_bytes_per_row);

    // Unmap the buffer
    buffer->unmap();

    return result;
}

/// Convert a GPU buffer to an RGBA image
///
/// Captures the buffer data and converts it to an image::RgbaImage.
///
/// \param device The wgpu device
/// \param buffer The buffer to read from
/// \param dimensions The buffer dimensions
/// \param index Optional submission index to wait for
/// \param size The extent of the texture
/// \return The RGBA image (optional, may be empty if conversion fails)
std::optional<std::vector<uint8_t>> buffer_to_image(
    wgpu::Device* device,
    wgpu::Buffer* buffer,
    const BufferDimensions& dimensions,
    std::optional<wgpu::SubmissionIndex*> index,
    const wgpu::Extent3d& size
);

/// Get supported sample count for a texture format
///
/// Keeps halving the sample count until finding one that's supported.
///
/// \param adapter The wgpu adapter
/// \param sample_count The desired sample count
/// \param format The texture format
/// \return The supported sample count (may be reduced from input)
uint32_t supported_sample_count(
    const wgpu::Adapter* adapter,
    uint32_t sample_count,
    wgpu::TextureFormat format
);

/// Run the copy pipeline to copy texture to frame
///
/// Copies the rendered texture to the swapchain frame.
///
/// \param descriptors The render descriptors
/// \param format The target texture format
/// \param actual_surface_format The actual surface format
/// \param frame_view The frame texture view
/// \param input The input texture view
/// \param whole_frame_bind_group The bind group for the whole frame
/// \param globals The global uniforms
/// \param sample_count The MSAA sample count
/// \param encoder The command encoder
void run_copy_pipeline(
    Descriptors* descriptors,
    wgpu::TextureFormat format,
    wgpu::TextureFormat actual_surface_format,
    wgpu::TextureView* frame_view,
    wgpu::TextureView* input,
    wgpu::BindGroup* whole_frame_bind_group,
    Globals* globals,
    uint32_t sample_count,
    wgpu::CommandEncoder* encoder
);

/// Sample count map for storing per-sample-count resources
///
/// Stores resources for sample counts 1, 2, 4, 8, and 16.
///
/// \tparam T The type of resource to store
template<typename T>
class SampleCountMap {
private:
    T one_;
    T two_;
    T four_;
    T eight_;
    T sixteen_;

public:
    /// Default constructor
    SampleCountMap() = default;

    /// Get the resource for a sample count
    ///
    /// \param sample_count The sample count (must be 1, 2, 4, 8, or 16)
    /// \return Reference to the resource
    const T& get(uint32_t sample_count) const {
        switch (sample_count) {
            case 1: return one_;
            case 2: return two_;
            case 4: return four_;
            case 8: return eight_;
            case 16: return sixteen_;
            default:
                // unreachable!("Sample counts must be powers of two between 1..=16")
                abort();
        }
    }

    /// Get or initialize the resource for a sample count
    ///
    /// \tparam F Function type for initialization
    /// \param sample_count The sample count
    /// \param init Initialization function
    /// \return Reference to the resource
    template<typename F>
    const T& get_or_init(uint32_t sample_count, F init) {
        switch (sample_count) {
            case 1:
                if (!one_) one_ = init();
                return one_;
            case 2:
                if (!two_) two_ = init();
                return two_;
            case 4:
                if (!four_) four_ = init();
                return four_;
            case 8:
                if (!eight_) eight_ = init();
                return eight_;
            case 16:
                if (!sixteen_) sixteen_ = init();
                return sixteen_;
            default:
                abort();
        }
    }
};

/// Specialization for std::optional (similar to OnceLock)
template<typename T>
class SampleCountMap<std::optional<T>> {
private:
    std::optional<T> one_;
    std::optional<T> two_;
    std::optional<T> four_;
    std::optional<T> eight_;
    std::optional<T> sixteen_;

public:
    const T& get_or_init(uint32_t sample_count, std::function<T()> init) {
        switch (sample_count) {
            case 1:
                if (!one_) one_ = init();
                return *one_;
            case 2:
                if (!two_) two_ = init();
                return *two_;
            case 4:
                if (!four_) four_ = init();
                return *four_;
            case 8:
                if (!eight_) eight_ = init();
                return *eight_;
            case 16:
                if (!sixteen_) sixteen_ = init();
                return *sixteen_;
            default:
                abort();
        }
    }
};

} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_UTILS_H
