// C++ translation of render/wgpu/src/buffer_builder.rs
// Buffer builder for WebGPU buffer construction

#ifndef RUFFLE_RENDER_WGPU_BUFFER_BUILDER_H
#define RUFFLE_RENDER_WGPU_BUFFER_BUILDER_H

#include <vector>
#include <cstdint>
#include <optional>

// External dependencies
#include <wgpu/wgpu.h>
#include <wgpu/util/buffer.h>

namespace ruffle {
namespace render_wgpu {

/// Error indicating the buffer builder has reached its limit
struct BufferFull {};

/// Buffer builder for constructing WebGPU buffers
///
/// Provides a convenient way to build up buffer data with proper alignment
/// before creating the final wgpu::Buffer.
class BufferBuilder {
private:
    /// Internal buffer storage
    std::vector<uint8_t> inner;

    /// Alignment mask (alignment - 1) for padding calculations
    size_t align_mask;

    /// Maximum buffer size limit
    uint64_t limit;

public:
    /// Create a buffer builder for vertex data
    /// @param limits WebGPU device limits
    static BufferBuilder new_for_vertices(const wgpu::Limits* limits);

    /// Create a buffer builder for uniform data
    /// @param limits WebGPU device limits
    static BufferBuilder new_for_uniform(const wgpu::Limits* limits);

    /// Set the buffer size limit
    /// @param limit Maximum buffer size in bytes
    void set_buffer_limit(uint64_t limit);

    /// Add data to the buffer builder
    /// @tparam T Type of data (must be NoUninit + AnyBitPattern)
    /// @param value Slice of data to add
    /// @return Range of buffer addresses, or BufferFull error if limit exceeded
    template<typename T>
    std::optional<wgpu::BufferAddressRange> add(const std::vector<T>& value);

    /// Finish building and create the final buffer
    /// @param device WebGPU device
    /// @param label Optional debug label
    /// @param usage Buffer usage flags
    /// @return The created wgpu::Buffer
    wgpu::Buffer finish(
        wgpu::Device* device,
        const std::string& label,
        wgpu::BufferUsages usage);

    /// Copy the buffer data to an existing buffer using staging belt
    /// @param staging_belt Staging belt for buffer uploads
    /// @param device WebGPU device
    /// @param encoder Command encoder
    /// @param buffer Destination buffer
    void copy_to(
        wgpu::util::StagingBelt* staging_belt,
        wgpu::Device* device,
        wgpu::CommandEncoder* encoder,
        wgpu::Buffer* buffer);
};

} // namespace render_wgpu
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_BUFFER_BUILDER_H
