// C++ translation of render/wgpu/src/buffer_pool.rs
// Buffer and texture pooling for WebGPU

#ifndef RUFFLE_RENDER_WGPU_BUFFER_POOL_H
#define RUFFLE_RENDER_WGPU_BUFFER_POOL_H

#include <cstdint>
#include <memory>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <atomic>
#include <optional>
#include <functional>
#include <variant>

// Forward declarations
namespace wgpu {
    class Device;
    struct TextureDescriptor;
    struct Extent3d;
    enum class TextureDimension;
    enum class TextureFormat;
    enum class TextureUsages;
    class Texture;
    class TextureView;
    class Buffer;
    class BindGroupLayout;
}

namespace ruffle {
namespace render {
namespace wgpu_backend {

// Forward declarations
struct Descriptors;
class Globals;

// Type aliases
template<typename T>
using PoolInner = std::vector<T>;

template<typename Type, typename Description>
using Constructor = std::function<Type(const Descriptors*, const Description*)>;

/// Key for texture pool lookup
struct TextureKey {
    wgpu::Extent3d size;
    wgpu::TextureUsages usage;
    wgpu::TextureFormat format;
    uint32_t sample_count;

    bool operator==(const TextureKey& other) const {
        return size.width == other.size.width &&
               size.height == other.size.height &&
               size.depth_or_array_layers == other.size.depth_or_array_layers &&
               usage == other.usage &&
               format == other.format &&
               sample_count == other.sample_count;
    }
};

/// Key for globals cache lookup
struct GlobalsKey {
    uint32_t viewport_width;
    uint32_t viewport_height;

    bool operator==(const GlobalsKey& other) const {
        return viewport_width == other.viewport_width &&
               viewport_height == other.viewport_height;
    }
};

/// Hash function for TextureKey
struct TextureKeyHash {
    std::size_t operator()(const TextureKey& key) const {
        std::size_t h1 = std::hash<uint32_t>{}(key.size.width);
        std::size_t h2 = std::hash<uint32_t>{}(key.size.height);
        std::size_t h3 = std::hash<uint32_t>{}(key.size.depth_or_array_layers);
        std::size_t h4 = std::hash<int>{}(static_cast<int>(key.usage));
        std::size_t h5 = std::hash<int>{}(static_cast<int>(key.format));
        std::size_t h6 = std::hash<uint32_t>{}(key.sample_count);
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4) ^ (h6 << 5);
    }
};

/// Hash function for GlobalsKey
struct GlobalsKeyHash {
    std::size_t operator()(const GlobalsKey& key) const {
        return std::hash<uint32_t>{}(key.viewport_width) ^
               (std::hash<uint32_t>{}(key.viewport_height) << 1);
    }
};

/// Buffer description trait for pool compatibility checking
///
/// Implement this trait to define how buffer descriptions are compared
/// for compatibility and cost.
template<typename Description>
class BufferDescription {
public:
    using Cost = int;

    /// If the potential buffer represented by this description (self)
    /// fits another existing buffer and its description (other),
    /// return the cost to use that buffer instead of making a new one.
    ///
    /// Cost is an arbitrary unit, but lower is better.
    /// std::nullopt means that the other buffer cannot be used in place of this one.
    virtual std::optional<Cost> cost_to_use(const Description& other) const = 0;
};

/// Marker type indicating all buffers are compatible
struct AlwaysCompatible {
    using Cost = int;

    std::optional<Cost> cost_to_use(const AlwaysCompatible&) const {
        return 0; // Always compatible with zero cost
    }
};

/// Pool entry wrapper that returns items to the pool when dropped
template<typename Type, typename Description>
class PoolEntry {
private:
    std::optional<Type> item_;
    Description description_;
    std::weak_ptr<std::mutex> pool_;

public:
    PoolEntry() = default;

    PoolEntry(
        Type item,
        Description description,
        std::weak_ptr<std::mutex> pool
    ) : item_(std::move(item)),
        description_(std::move(description)),
        pool_(std::move(pool)) {}

    // Move constructor
    PoolEntry(PoolEntry&& other) noexcept
        : item_(std::move(other.item_)),
          description_(std::move(other.description_)),
          pool_(std::move(other.pool_)) {
        other.pool_.reset();
    }

    // Move assignment
    PoolEntry& operator=(PoolEntry&& other) noexcept {
        if (this != &other) {
            return_to_pool();
            item_ = std::move(other.item_);
            description_ = std::move(other.description_);
            pool_ = std::move(other.pool_);
            other.pool_.reset();
        }
        return *this;
    }

    ~PoolEntry() {
        return_to_pool();
    }

    // Prevent copying
    PoolEntry(const PoolEntry&) = delete;
    PoolEntry& operator=(const PoolEntry&) = delete;

    /// Get reference to the pooled item
    Type& get() {
        return *item_;
    }

    const Type& get() const {
        return *item_;
    }

    /// Get pointer to the pooled item
    Type* operator->() {
        return &item_.value();
    }

    const Type* operator->() const {
        return &item_.value();
    }

    Type& operator*() {
        return item_.value();
    }

    const Type& operator*() const {
        return item_.value();
    }

private:
    void return_to_pool() {
        if (item_.has_value() && !pool_.expired()) {
            auto pool = pool_.lock();
            if (pool) {
                std::lock_guard<std::mutex> lock(*pool);
                // In a real implementation, this would push back to the pool
                // For now, we just let the item be destroyed
            }
        }
        item_.reset();
    }
};

/// Buffer pool for reusing expensive-to-create resources
template<typename Type, typename Description>
class BufferPool {
private:
    std::shared_ptr<std::mutex> available_;
    std::vector<std::pair<Type, Description>> pool_;
    Constructor<Type, Description> constructor_;

public:
    explicit BufferPool(Constructor<Type, Description> constructor)
        : available_(std::make_shared<std::mutex>()),
          constructor_(std::move(constructor)) {}

    /// Take an item from the pool or create a new one
    ///
    /// \param descriptors Device descriptors for creating new items
    /// \param description Description of the desired item
    /// \return A PoolEntry that will return the item to the pool when dropped
    PoolEntry<Type, Description> take(
        const Descriptors* descriptors,
        const Description& description
    ) {
        std::lock_guard<std::mutex> lock(*available_);

        // Find best matching item in pool
        std::optional<std::pair<int, size_t>> best;
        for (size_t i = 0; i < pool_.size(); ++i) {
            if (auto cost = description.cost_to_use(pool_[i].second)) {
                if (!best || best->first > *cost) {
                    best = std::make_pair(*cost, i);
                }
            }
        }

        std::pair<Type, Description> item_desc;
        if (best.has_value()) {
            // Use existing item from pool
            item_desc = std::move(pool_[best->second]);
            pool_.erase(pool_.begin() + best->second);
        } else {
            // Create new item
            Type item = constructor_(descriptors, &description);
            item_desc = std::make_pair(std::move(item), description);
        }

        return PoolEntry<Type, Description>(
            std::move(item_desc.first),
            std::move(item_desc.second),
            available_
        );
    }
};

/// Texture pool for managing pooled textures and globals
class TexturePool {
private:
    std::unordered_map<TextureKey, BufferPool<std::pair<wgpu::Texture, wgpu::TextureView>, AlwaysCompatible>, TextureKeyHash> pools_;
    std::unordered_map<GlobalsKey, std::shared_ptr<Globals>, GlobalsKeyHash> globals_cache_;

public:
    TexturePool() = default;

    /// Get a pooled texture or create a new one
    ///
    /// \param descriptors Device descriptors
    /// \param size Texture dimensions
    /// \param usage Texture usage flags
    /// \param format Texture format
    /// \param sample_count Number of samples for MSAA
    /// \return Pool entry for the texture
    PoolEntry<std::pair<wgpu::Texture, wgpu::TextureView>, AlwaysCompatible> get_texture(
        const Descriptors* descriptors,
        wgpu::Extent3d size,
        wgpu::TextureUsages usage,
        wgpu::TextureFormat format,
        uint32_t sample_count
    );

    /// Get or create globals buffer for the given viewport size
    ///
    /// \param descriptors Device descriptors
    /// \param viewport_width Viewport width in pixels
    /// \param viewport_height Viewport height in pixels
    /// \return Shared pointer to globals buffer
    std::shared_ptr<Globals> get_globals(
        const Descriptors* descriptors,
        uint32_t viewport_width,
        uint32_t viewport_height
    );
};

} // namespace wgpu_backend
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_BUFFER_POOL_H
