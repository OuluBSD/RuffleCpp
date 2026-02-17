// C++ translation of scanner/src/ser_bridge.rs
// Parallel-to-serial iterator bridge

#ifndef RUFFLE_SCANNER_SER_BRIDGE_H
#define RUFFLE_SCANNER_SER_BRIDGE_H

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <functional>
#include <type_traits>

namespace ruffle {
namespace scanner {

/// Parallel-to-serial iterator bridge
///
/// This class provides a way to convert parallel iteration into serial consumption.
/// It spawns a background thread to process items and makes them available through
/// a standard Iterator interface.
///
/// Based on the proposal from https://github.com/rayon-rs/rayon/issues/858
template<typename T>
class SerBridgeImpl {
private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool done_ = false;

public:
    /// Create a new SerBridgeImpl from a parallel iterable
    ///
    /// \tparam ParallelIterable A type that can be converted into a parallel iterator
    /// \param par_iterable The parallel iterable to bridge
    template<typename ParallelIterable>
    static SerBridgeImpl<T> new_bridge(ParallelIterable par_iterable) {
        SerBridgeImpl<T> bridge;
        
        // Spawn a background thread to process the parallel iterator
        std::thread([bridge_ptr = &bridge, par_iterable = std::move(par_iterable)]() mutable {
            // Process each item in the parallel iterable
            for (auto& item : par_iterable) {
                {
                    std::lock_guard<std::mutex> lock(bridge_ptr->mutex_);
                    bridge_ptr->queue_.push(std::move(item));
                }
                bridge_ptr->cv_.notify_one();
            }
            
            // Mark as done when all items are processed
            {
                std::lock_guard<std::mutex> lock(bridge_ptr->mutex_);
                bridge_ptr->done_ = true;
            }
            bridge_ptr->cv_.notify_one();
        }).detach();
        
        return bridge;
    }

    /// Get the next item from the bridge
    ///
    /// \return The next item, or std::nullopt if the bridge is exhausted
    std::optional<T> next() {
        std::unique_lock<std::mutex> lock(mutex_);
        
        // Wait until there's an item or we're done
        cv_.wait(lock, [this]() {
            return !queue_.empty() || done_;
        });
        
        // Return the next item if available
        if (!queue_.empty()) {
            T item = std::move(queue_.front());
            queue_.pop();
            return item;
        }
        
        return std::nullopt;
    }

    /// Check if the bridge has more items
    ///
    /// \return true if there are more items to consume
    bool has_next() {
        std::lock_guard<std::mutex> lock(mutex_);
        return !queue_.empty() || !done_;
    }
};

/// Parallel-to-serial iterator bridge trait
///
/// This trait provides the ser_bridge() method for converting parallel iterators
/// into serial iterators.
///
/// \tparam T The item type of the parallel iterator
template<typename T>
class SerBridge {
public:
    /// Convert this parallel iterator to a serial bridge
    ///
    /// \return A SerBridgeImpl that provides serial access to parallel items
    template<typename ParallelIterator>
    static SerBridgeImpl<T> ser_bridge(ParallelIterator par_iter) {
        return SerBridgeImpl<T>::new_bridge(std::move(par_iter));
    }
};

} // namespace scanner
} // namespace ruffle

#endif // RUFFLE_SCANNER_SER_BRIDGE_H
