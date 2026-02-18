// C++ translation of core/src/avm2/array.rs
//! Array support types for AVM2

#ifndef RUFFLE_CORE_AVM2_ARRAY_H
#define RUFFLE_CORE_AVM2_ARRAY_H

#include "value.h"

#include <vector>
#include <map>
#include <optional>
#include <cstdint>
#include <cstddef>

namespace ruffle {
namespace core {
namespace avm2 {

/// Minimum length for sparse array storage
constexpr size_t MIN_SPARSE_LENGTH = 32;

/// Maximum length for dense array storage
constexpr size_t MAX_DENSE_LENGTH = 1 << 28;

/// Array storage iterator - yields Optional<Value> (None for holes)
template<typename GC>
class ArrayStorageIterator {
public:
    using value_type = std::optional<Value<GC>>;
    using reference = const value_type&;
    using pointer = const value_type*;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    
    ArrayStorageIterator(
        const class ArrayStorage<GC>* storage,
        size_t index,
        size_t index_back
    );
    
    reference operator*() const;
    pointer operator->() const;
    
    ArrayStorageIterator& operator++();
    ArrayStorageIterator operator++(int);
    ArrayStorageIterator& operator--();
    ArrayStorageIterator operator--(int);
    
    bool operator==(const ArrayStorageIterator& other) const;
    bool operator!=(const ArrayStorageIterator& other) const;

private:
    const class ArrayStorage<GC>* storage_;
    size_t index_;
    size_t index_back_;
};

/// Array storage for AVM2 arrays
/// 
/// Array values may consist of either standard Values or "holes": values
/// which are not properties of the associated object and must be resolved
/// in the prototype.
template<typename GC>
class ArrayStorage {
public:
    /// Construct new array storage with given length
    /// 
    /// All initial elements are holes.
    /// 
    /// @param length Initial array length
    explicit ArrayStorage(size_t length);
    
    /// Construct from a list of values
    /// 
    /// @param values Initial values
    static ArrayStorage from_args(const std::vector<Value<GC>>& values);
    
    /// Construct from existing storage vector
    /// 
    /// @param storage Vector of optional values (None = hole)
    static ArrayStorage from_storage(std::vector<std::optional<Value<GC>>> storage);
    
    /// Construct from iterator range
    /// 
    /// @param begin Begin iterator
    /// @param end End iterator
    template<typename Iter>
    static ArrayStorage from_iter(Iter begin, Iter end);
    
    /// Get the value at given index
    /// 
    /// @param index Array index
    /// @return Optional value (None if hole)
    std::optional<Value<GC>> get(size_t index) const;
    
    /// Set the value at given index
    /// 
    /// @param index Array index
    /// @param value Value to set (None for hole)
    void set(size_t index, std::optional<Value<GC>> value);
    
    /// Delete the value at given index (make it a hole)
    /// 
    /// @param index Array index
    void delete_(size_t index);
    
    /// Check if index contains a hole
    /// 
    /// @param index Array index
    /// @return true if index is a hole
    bool is_hole(size_t index) const;
    
    /// Get the logical length of the array
    /// 
    /// @return Array length
    size_t length() const;
    
    /// Set the logical length of the array
    /// 
    /// @param length New length
    void set_length(size_t length);
    
    /// Get the count of non-hole elements
    /// 
    /// @return Number of occupied slots
    size_t occupied_count() const;
    
    /// Check if array is empty
    /// 
    /// @return true if length is 0
    bool is_empty() const;
    
    /// Clear all elements (set length to 0)
    void clear();
    
    /// Reserve capacity for dense storage
    /// 
    /// @param capacity Minimum capacity
    void reserve(size_t capacity);
    
    /// Truncate array to given length
    /// 
    /// @param new_length New length
    void truncate(size_t new_length);
    
    /// Push a value to the end of the array
    /// 
    /// @param value Value to push
    void push(const Value<GC>& value);
    
    /// Pop a value from the end of the array
    /// 
    /// @return Optional value (None if empty or hole)
    std::optional<Value<GC>> pop();
    
    /// Get begin iterator
    ArrayStorageIterator<GC> begin() const;
    
    /// Get end iterator
    ArrayStorageIterator<GC> end() const;
    
    /// Get reverse begin iterator
    ArrayStorageIterator<GC> rbegin() const;
    
    /// Get reverse end iterator
    ArrayStorageIterator<GC> rend() const;

private:
    /// Dense storage: vector of values with count of non-holes
    struct Dense {
        std::vector<std::optional<Value<GC>>> storage;
        size_t occupied_count;
    };
    
    /// Sparse storage: map of index to value with explicit length
    struct Sparse {
        std::map<size_t, Value<GC>> storage;
        size_t length;
    };
    
    /// Storage variant
    std::variant<Dense, Sparse> storage_;
    
    /// Check if should use dense storage for given length
    static bool should_use_dense(size_t length);
    
    /// Convert to dense storage
    void to_dense();
    
    /// Convert to sparse storage
    void to_sparse();
    
    /// Get dense storage reference (panics if sparse)
    Dense& as_dense();
    const Dense& as_dense() const;
    
    /// Get sparse storage reference (panics if dense)
    Sparse& as_sparse();
    const Sparse& as_sparse() const;
};

/// Array object for AVM2
template<typename GC>
class ArrayObject {
public:
    /// Create new array object
    /// 
    /// @param storage Array storage
    explicit ArrayObject(ArrayStorage<GC> storage);
    
    /// Create new array with given length
    /// 
    /// @param length Initial length
    static ArrayObject with_length(size_t length);
    
    /// Create new array from arguments
    /// 
    /// @param values Initial values
    static ArrayObject from_args(const std::vector<Value<GC>>& values);
    
    /// Get the array storage
    /// 
    /// @return Reference to storage
    const ArrayStorage<GC>& storage() const;
    
    /// Get mutable array storage
    /// 
    /// @return Mutable reference to storage
    ArrayStorage<GC>& storage_mut();
    
    /// Get array length
    /// 
    /// @return Array length
    size_t length() const;
    
    /// Set array length
    /// 
    /// @param length New length
    void set_length(size_t length);
    
    /// Get value at index
    /// 
    /// @param index Array index
    /// @return Optional value (None if hole)
    std::optional<Value<GC>> get(size_t index) const;
    
    /// Set value at index
    /// 
    /// @param index Array index
    /// @param value Value to set
    void set(size_t index, const Value<GC>& value);
    
    /// Delete value at index
    /// 
    /// @param index Array index
    void delete_(size_t index);
    
    /// Check if index is a hole
    /// 
    /// @param index Array index
    /// @return true if hole
    bool is_hole(size_t index) const;
    
    /// Push value to end
    /// 
    /// @param value Value to push
    void push(const Value<GC>& value);
    
    /// Pop value from end
    /// 
    /// @return Optional value
    std::optional<Value<GC>> pop();
    
    /// Shift value from beginning
    /// 
    /// @return Optional value
    std::optional<Value<GC>> shift();
    
    /// Unshift value to beginning
    /// 
    /// @param value Value to unshift
    void unshift(const Value<GC>& value);
    
    /// Splice array
    /// 
    /// @param start Start index
    /// @param delete_count Number of elements to delete
    /// @param items Items to insert
    /// @return Deleted elements
    std::vector<Value<GC>> splice(
        size_t start,
        size_t delete_count,
        const std::vector<Value<GC>>& items
    );
    
    /// Slice array
    /// 
    /// @param start Start index
    /// @param end End index
    /// @return New array with sliced elements
    ArrayObject slice(size_t start, size_t end);
    
    /// Concatenate arrays
    /// 
    /// @param arrays Arrays to concatenate
    /// @return New concatenated array
    ArrayObject concat(const std::vector<ArrayObject>& arrays) const;
    
    /// Join array elements
    /// 
    /// @param separator Element separator
    /// @return Joined string
    AvmString<GC> join(const AvmString<GC>& separator) const;
    
    /// Reverse array in place
    void reverse();
    
    /// Sort array
    /// 
    /// @param compare_fn Optional compare function
    void sort(std::optional<std::function<int(const Value<GC>&, const Value<GC>&)>> compare_fn);
    
    /// Sort array with flags
    /// 
    /// @param flags Sort flags
    /// @param compare_fn Optional compare function
    void sort_with_flags(
        uint32_t flags,
        std::optional<std::function<int(const Value<GC>&, const Value<GC>&)>> compare_fn
    );

private:
    ArrayStorage<GC> storage_;
};

/// Sort flags for Array.sort()
namespace array_sort_flags {
    constexpr uint32_t CASEINSENSITIVE = 1;
    constexpr uint32_t DESCENDING = 2;
    constexpr uint32_t UNIQUESORT = 4;
    constexpr uint32_t RETURNINDEXEDARRAY = 8;
    constexpr uint32_t NUMERIC = 16;
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_ARRAY_H
