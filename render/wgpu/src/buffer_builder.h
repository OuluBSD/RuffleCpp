// C++ translation of render/wgpu/src/buffer_builder.rs
// Buffer builder for WebGPU

#ifndef RUFFLE_RENDER_WGPU_BUFFER_BUILDER_H
#define RUFFLE_RENDER_WGPU_BUFFER_BUILDER_H

#include <vector>
#include <cstdint>
#include <optional>
#include <ranges>
#include <webgpu/webgpu.h>

namespace ruffle {
namespace render {
namespace wgpu {

/// Error indicating the buffer is full
struct BufferFull {
    BufferFull() = default;
};

/// Buffer builder for WebGPU
///
/// This class helps build WebGPU buffers by managing alignment and size limits.
/// It supports both vertex and uniform buffer alignment requirements.
class BufferBuilder {
private:
    std::vector<uint8_t> inner_;
    size_t align_mask_;
    uint64_t limit_;

public:
    /// Create a new buffer builder for vertex data
    ///
    /// \param limits The WebGPU device limits
    /// \return A new BufferBuilder configured for vertex data
    static BufferBuilder new_for_vertices(const WGPULimits& limits) {
        BufferBuilder builder;
        builder.inner_ = std::vector<uint8_t>();
        builder.align_mask_ = 0;
        builder.limit_ = limits.maxBufferSize;
        return builder;
    }

    /// Create a new buffer builder for uniform data
    ///
    /// \param limits The WebGPU device limits
    /// \return A new BufferBuilder configured for uniform data
    static BufferBuilder new_for_uniform(const WGPULimits& limits) {
        BufferBuilder builder;
        builder.inner_ = std::vector<uint8_t>();
        if (limits.minUniformBufferOffsetAlignment > 0) {
            builder.align_mask_ = static_cast<size_t>(limits.minUniformBufferOffsetAlignment - 1);
        } else {
            builder.align_mask_ = 0;
        }
        builder.limit_ = limits.maxBufferSize;
        return builder;
    }

    /// Set the buffer size limit
    ///
    /// \param limit The maximum buffer size in bytes
    void set_buffer_limit(uint64_t limit) {
        limit_ = limit;
    }

    /// Add data to the buffer
    ///
    /// \tparam T The type of data to add (must be trivially copyable)
    /// \param data The data to add
    /// \return The byte range of the added data, or BufferFull error if limit exceeded
    template<typename T>
    std::optional<std::pair<uint64_t, uint64_t>> add(const std::vector<T>& data) {
        size_t start_pos;
        
        if (!inner_.empty()) {
            if (align_mask_ > 0) {
                // Pad the internal buffer to match alignment requirements
                // Pad on creation so that we don't wastefully pad the end of the buffer
                start_pos = (inner_.size() + align_mask_) & ~align_mask_;
            } else {
                start_pos = inner_.size();
            }
        } else {
            start_pos = 0;
        }

        size_t slice_size = data.size() * sizeof(T);
        if ((start_pos + slice_size) > static_cast<size_t>(limit_)) {
            return std::nullopt; // BufferFull error
        }

        if (start_pos > 0 && align_mask_ > 0) {
            inner_.resize(start_pos, 0);
        }

        // Copy data into the buffer
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(data.data());
        inner_.insert(inner_.end(), bytes, bytes + slice_size);
        
        return std::make_pair(
            static_cast<uint64_t>(start_pos),
            static_cast<uint64_t>(inner_.size())
        );
    }

    /// Finish building and create the WebGPU buffer
    ///
    /// \param device The WebGPU device
    /// \param label Optional label for the buffer
    /// \param usage The buffer usage flags
    /// \return The created WebGPU buffer
    WGPUBuffer finish(WGPUDevice device, const char* label, WGPUBufferUsage usage) {
        WGPUBufferDescriptor desc{};
        desc.label = label;
        desc.size = inner_.size();
        desc.usage = usage;
        desc.mappedAtCreation = false;
        
        return wgpuDeviceCreateBuffer(device, &desc);
    }

    /// Copy the buffer contents to an existing WebGPU buffer
    ///
    /// \param device The WebGPU device
    /// \param encoder The command encoder
    /// \param buffer The destination buffer
    void copy_to(WGPUDevice device, WGPUCommandEncoder encoder, WGPUBuffer buffer) {
        if (inner_.empty()) {
            return;
        }
        
        // Write buffer contents via staging belt
        WGPUImageCopyBuffer copy_buffer{};
        copy_buffer.buffer = buffer;
        copy_buffer.offset = 0;
        copy_buffer.bytesPerRow = static_cast<uint32_t>(inner_.size());
        copy_buffer.rowsPerImage = 1;
        
        WGPUExtent3D size{};
        size.width = static_cast<uint32_t>(inner_.size());
        size.height = 1;
        size.depthOrArrayLayers = 1;
        
        wgpuCommandEncoderWriteBuffer(
            encoder,
            buffer,
            0,
            inner_.data(),
            inner_.size()
        );
    }

    /// Get the current buffer size
    ///
    /// \return The current size in bytes
    size_t size() const {
        return inner_.size();
    }
};

} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_BUFFER_BUILDER_H
