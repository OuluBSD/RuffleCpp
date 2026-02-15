/*
 * C++ header for shared buffer functionality
 * This replaces the functionality of core/common/src/buffer.rs
 */

#ifndef SHARED_BUFFER_H
#define SHARED_BUFFER_H

#include <vector>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <cstdint>
#include <cstring>
#include <iosfwd>

// Forward declarations
class Slice;
class Substream;
class SliceCursor;
class SubstreamCursor;

// A shared data buffer
class Buffer {
private:
    std::shared_ptr<std::shared_mutex> mutex_;
    std::shared_ptr<std::vector<uint8_t>> data_;

public:
    // Constructor
    Buffer() : mutex_(std::make_shared<std::shared_mutex>()), 
               data_(std::make_shared<std::vector<uint8_t>>()) {}

    // Create a new buffer
    static Buffer create_new() {
        return Buffer();
    }

    // Create a buffer with initial capacity
    static Buffer with_capacity(size_t cap) {
        Buffer buf;
        {
            std::unique_lock lock(*buf.mutex_);
            buf.data_->reserve(cap);
        }
        return buf;
    }

    // Get the capacity of the buffer
    size_t capacity() const {
        std::shared_lock lock(*mutex_);
        return data_->capacity();
    }

    // Get the length of the buffer
    size_t len() const {
        std::shared_lock lock(*mutex_);
        return data_->size();
    }

    // Check if the buffer is empty
    bool is_empty() const {
        std::shared_lock lock(*mutex_);
        return data_->empty();
    }

    // Reserve additional space
    void reserve(size_t additional) {
        std::unique_lock lock(*mutex_);
        data_->reserve(data_->size() + additional);
    }

    // Reserve exact additional space
    void reserve_exact(size_t additional) {
        std::unique_lock lock(*mutex_);
        data_->reserve(data_->size() + additional);
    }

    // Get the entire buffer as a slice
    Slice as_slice() const;

    // Append data to the buffer
    void append(std::vector<uint8_t>& other) {
        std::unique_lock lock(*mutex_);
        data_->insert(data_->end(), other.begin(), other.end());
        other.clear();  // Clear the other vector as in Rust
    }

    // Extend buffer from a slice
    void extend_from_slice(const std::vector<uint8_t>& other) {
        std::unique_lock lock(*mutex_);
        data_->insert(data_->end(), other.begin(), other.end());
    }

    // Get a slice of the buffer
    std::optional<Slice> get(size_t start, size_t end) const;

    // Get a full slice of the buffer
    Slice to_full_slice() const;

    // Get an empty slice
    Slice to_empty_slice() const;

    // Equality operator (compares shared pointer identity)
    bool operator==(const Buffer& other) const {
        return data_.get() == other.data_.get();
    }

    // Copy constructor
    Buffer(const Buffer&) = default;

    // Assignment operator
    Buffer& operator=(const Buffer&) = default;

    // Move constructor
    Buffer(Buffer&&) = default;

    // Move assignment operator
    Buffer& operator=(Buffer&&) = default;

    // Constructor from vector
    static Buffer from_vector(std::vector<uint8_t> vec) {
        Buffer buf;
        {
            std::unique_lock lock(*buf.mutex_);
            *buf.data_ = std::move(vec);
        }
        return buf;
    }
};

// Reference to a slice of the buffer data
class SliceRef {
private:
    std::shared_lock<std::shared_mutex> guard_;
    std::shared_ptr<std::vector<uint8_t>> data_;
    size_t start_;
    size_t end_;

public:
    SliceRef(std::shared_lock<std::shared_mutex>&& lock, 
             std::shared_ptr<std::vector<uint8_t>> data, 
             size_t start, size_t end)
        : guard_(std::move(lock)), data_(data), start_(start), end_(end) {}

    // Get pointer to data
    const uint8_t* data() const {
        return data_->data() + start_;
    }

    // Get size
    size_t size() const {
        return end_ - start_;
    }

    // Index operator
    uint8_t operator[](size_t idx) const {
        return data_->at(start_ + idx);
    }

    // Iterator support
    const uint8_t* begin() const { return data(); }
    const uint8_t* end() const { return data() + size(); }

    // Comparison operator
    bool operator==(const SliceRef& other) const {
        if (this == &other) return true;
        if (size() != other.size()) return false;
        return std::equal(begin(), end(), other.begin());
    }
};

// A reference into a shared data buffer
class Slice {
    friend class Buffer;  // Allow Buffer to access private members

private:
    Buffer buf_;
    size_t start_;
    size_t end_;

public:
    // Constructor (private, only Buffer can create slices)
    Slice(Buffer buf, size_t start, size_t end) 
        : buf_(std::move(buf)), start_(start), end_(end) {}

    // Create a subslice of this buffer slice
    Slice to_subslice(const std::vector<uint8_t>& slice) const;

    // Create a subslice without bounds checking
    Slice to_unbounded_subslice(const std::vector<uint8_t>& slice) const;

    // Create a slice from start and end positions
    Slice to_start_and_end(size_t start, size_t end) const;

    // Get a subslice with range bounds
    std::optional<Slice> get(size_t start, size_t end) const;

    // Check if slice is empty
    bool is_empty() const {
        return end_ == start_;
    }

    // Get the length of the slice
    size_t len() const {
        return end_ - start_;
    }

    // Get start position
    size_t start() const {
        return start_;
    }

    // Get end position
    size_t end() const {
        return end_;
    }

    // Get the data reference
    SliceRef data() const {
        std::shared_lock lock(*buf_.mutex_);
        return SliceRef(std::move(lock), buf_.data_, start_, end_);
    }

    // Get the buffer
    const Buffer& buffer() const {
        return buf_;
    }

    // Create a cursor for reading
    SliceCursor as_cursor() const;

    // Copy constructor
    Slice(const Slice&) = default;

    // Assignment operator
    Slice& operator=(const Slice&) = default;

    // Move constructor
    Slice(Slice&&) = default;

    // Move assignment operator
    Slice& operator=(Slice&&) = default;
};

// A readable cursor into a buffer slice
class SliceCursor {
private:
    Slice slice_;
    size_t pos_;

public:
    explicit SliceCursor(Slice slice) : slice_(std::move(slice)), pos_(0) {}

    // Read data from the cursor
    size_t read(std::vector<uint8_t>& data) {
        size_t copy_count = std::min(data.size(), slice_.len() - pos_);
        if (copy_count == 0) return 0;

        auto slice_data = slice_.data();
        std::copy(slice_data.begin() + pos_, 
                  slice_data.begin() + pos_ + copy_count, 
                  data.begin());
        
        pos_ += copy_count;
        return copy_count;
    }

    // Get current position
    size_t position() const {
        return pos_;
    }

    // Reset position
    void reset() {
        pos_ = 0;
    }

    // Copy constructor
    SliceCursor(const SliceCursor& other) : slice_(other.slice_), pos_(other.pos_) {}

    // Assignment operator
    SliceCursor& operator=(const SliceCursor& other) {
        if (this != &other) {
            slice_ = other.slice_;
            pos_ = other.pos_;
        }
        return *this;
    }
};

// Error types for substream operations
enum class SubstreamError {
    FOREIGN_BUFFER,
    SUCCESS
};

// A list of multiple slices of the same buffer
class Substream {
private:
    Buffer buf_;
    std::shared_ptr<std::shared_mutex> chunks_mutex_;
    std::shared_ptr<std::vector<std::pair<size_t, size_t>>> chunks_;

public:
    explicit Substream(Buffer buf) 
        : buf_(std::move(buf)), 
          chunks_mutex_(std::make_shared<std::shared_mutex>()),
          chunks_(std::make_shared<std::vector<std::pair<size_t, size_t>>>()) {}

    // Create a new substream
    static Substream create(Buffer buf) {
        return Substream(std::move(buf));
    }

    // Append a slice to the substream
    SubstreamError append(Slice slice) {
        std::unique_lock lock(*chunks_mutex_);
        if (buf_ == slice.buffer()) {
            chunks_->emplace_back(slice.start(), slice.end());
            return SubstreamError::SUCCESS;
        } else {
            return SubstreamError::FOREIGN_BUFFER;
        }
    }

    // Get number of chunks
    size_t num_chunks() const {
        std::shared_lock lock(*chunks_mutex_);
        return chunks_->size();
    }

    // Create a cursor for reading
    SubstreamCursor as_cursor() const;

    // Get total length
    size_t len() const {
        std::shared_lock lock(*chunks_mutex_);
        size_t tally = 0;
        for (const auto& chunk : *chunks_) {
            tally += chunk.second - chunk.first;
        }
        return tally;
    }

    // Check if empty
    bool is_empty() const {
        return len() == 0;
    }

    // Get buffer
    const Buffer& buffer() const {
        return buf_;
    }

    // Get first chunk
    std::optional<Slice> first_chunk() const {
        std::shared_lock lock(*chunks_mutex_);
        if (!chunks_->empty()) {
            auto& first = chunks_->front();
            return Slice(buf_, first.first, first.second);
        }
        return std::nullopt;
    }

    // Get last chunk
        std::optional<Slice> last_chunk() const {
        std::shared_lock lock(*chunks_mutex_);
        if (!chunks_->empty()) {
            auto& last = chunks_->back();
            return Slice(buf_, last.first, last.second);
        }
        return std::nullopt;
    }

    // Copy constructor
    Substream(const Substream&) = default;

    // Assignment operator
    Substream& operator=(const Substream&) = default;

    // Move constructor
    Substream(Substream&&) = default;

    // Move assignment operator
    Substream& operator=(Substream&&) = default;
};

// A readable cursor into a buffer substream
class SubstreamCursor {
private:
    Substream substream_;
    size_t chunk_pos_;
    size_t bytes_pos_;

public:
    explicit SubstreamCursor(Substream substream) 
        : substream_(std::move(substream)), chunk_pos_(0), bytes_pos_(0) {}

    // Read data from the cursor
    size_t read(std::vector<uint8_t>& data) {
        size_t out_count = 0;
        
        auto buf_data = substream_.buffer().data_;
        std::shared_lock buf_lock(*substream_.buffer().mutex_);
        
        std::shared_lock chunks_lock(*substream_.chunks_mutex_);
        const auto& chunks = *substream_.chunks_;

        while (out_count < data.size()) {
            if (chunk_pos_ >= chunks.size()) {
                // Out of chunks to read
                break;
            }

            auto& cur_chunk = chunks[chunk_pos_];
            size_t chunk_len = cur_chunk.second - cur_chunk.first;
            size_t copy_count = std::min(data.size() - out_count, chunk_len - bytes_pos_);

            // Copy data from current chunk
            std::copy(buf_data->begin() + cur_chunk.first + bytes_pos_,
                      buf_data->begin() + cur_chunk.first + bytes_pos_ + copy_count,
                      data.begin() + out_count);

            bytes_pos_ += copy_count;
            out_count += copy_count;

            if (bytes_pos_ < chunk_len) {
                // Data buffer is full
                break;
            }

            // Move to next chunk
            chunk_pos_++;
            bytes_pos_ = 0;
        }

        return out_count;
    }

    // Reset position
    void reset() {
        chunk_pos_ = 0;
        bytes_pos_ = 0;
    }

    // Copy constructor
    SubstreamCursor(const SubstreamCursor& other) 
        : substream_(other.substream_), chunk_pos_(other.chunk_pos_), bytes_pos_(other.bytes_pos_) {}

    // Assignment operator
    SubstreamCursor& operator=(const SubstreamCursor& other) {
        if (this != &other) {
            substream_ = other.substream_;
            chunk_pos_ = other.chunk_pos_;
            bytes_pos_ = other.bytes_pos_;
        }
        return *this;
    }
};

// Implementations of methods that depend on other classes
inline Slice Buffer::to_full_slice() const {
    std::shared_lock lock(*mutex_);
    return Slice(*this, 0, data_->size());
}

inline Slice Buffer::to_empty_slice() const {
    return Slice(*this, 0, 0);
}

inline std::optional<Slice> Buffer::get(size_t start, size_t end) const {
    std::shared_lock lock(*mutex_);
    if (start <= data_->size() && end <= data_->size() && start <= end) {
        return Slice(*this, start, end);
    }
    return std::nullopt;
}

inline SliceCursor Slice::as_cursor() const {
    return SliceCursor(*this);
}

inline SubstreamCursor Substream::as_cursor() const {
    return SubstreamCursor(*this);
}

#endif // SHARED_BUFFER_H