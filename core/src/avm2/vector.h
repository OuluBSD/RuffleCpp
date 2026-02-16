// C++ translation of core/src/avm2/vector.rs
// Storage for AS3 Vectors

#ifndef RUFFLE_CORE_AVM2_VECTOR_H
#define RUFFLE_CORE_AVM2_VECTOR_H

#include <cstdint>
#include <vector>
#include <optional>
#include <algorithm>
#include <stdexcept>

namespace ruffle {
namespace core {
namespace avm2 {

// Forward declarations
template<typename GC>
class Value;
template<typename GC>
class Activation;
template<typename GC>
class Error;
template<typename GC>
class Class;

/// The vector storage portion of a vector object.
///
/// Vector values are restricted to a single type, decided at the time of the
/// construction of the vector's storage. The type is determined by the type
/// argument associated with the class of the vector. Vector holes are
/// evaluated to a default value based on the type of the vector.
///
/// A vector may also be configured to have a fixed size; when this is enabled,
/// attempts to modify the length fail.
template<typename GC>
class VectorStorage {
private:
    /// The storage for vector values.
    std::vector<Value<GC>> storage_;
    
    /// Whether or not the array length is fixed.
    bool is_fixed_;
    
    /// The allowed type of the contents of the vector, in the form of a class
    /// std::nullopt represents a Vector.<*>.
    std::optional<Class<GC>> value_type_;
    
public:
    /// Create a new vector storage
    VectorStorage(size_t length, bool is_fixed, std::optional<Class<GC>> value_type)
        : is_fixed_(is_fixed), value_type_(value_type) {
        storage_.resize(length, default_value());
    }
    
    /// Check if vector is fixed and throw error if so
    void check_fixed(Activation<GC>* activation) const {
        if (is_fixed_) {
            throw make_error_1126(activation);
        }
    }
    
    /// Create a new vector storage from a list of values.
    /// The values are assumed to already have been coerced to the value type given.
    static VectorStorage from_values(
        std::vector<Value<GC>> storage,
        bool is_fixed,
        std::optional<Class<GC>> value_type
    ) {
        return VectorStorage {
            .storage_ = std::move(storage),
            .is_fixed_ = is_fixed,
            .value_type_ = value_type
        };
    }
    
    /// Check if the vector is fixed
    bool is_fixed() const {
        return is_fixed_;
    }
    
    /// Set whether the vector is fixed
    void set_is_fixed(bool is_fixed) {
        is_fixed_ = is_fixed;
    }
    
    /// Get the length of the vector
    size_t length() const {
        return storage_.size();
    }
    
    /// Reserve capacity
    void reserve_exact(size_t length) {
        storage_.reserve(length);
    }
    
    /// Resize the vector
    void resize(size_t new_length, Activation<GC>* activation) {
        check_fixed(activation);
        storage_.resize(new_length, default_value());
    }
    
    /// Get the default value for this vector
    Value<GC> default_value() const {
        if (value_type_.has_value()) {
            if (value_type_->is_builtin_numeric()) {
                return Value<GC>::Integer(0);
            } else {
                return Value<GC>::Null();
            }
        }
        return Value<GC>::Null();
    }
    
    /// Get the value type stored in this vector (same as the class <T> type)
    std::optional<Class<GC>> value_type() const {
        return value_type_;
    }
    
    /// Get the value type this vector coerces things to
    Class<GC> value_type_for_coercion(Activation<GC>* activation) const {
        return value_type_.value_or(activation->avm2()->class_defs()->object);
    }
    
    /// Check if a vector index is in bounds
    bool is_in_range(size_t pos) const {
        return pos < storage_.size();
    }
    
    /// Change an arbitrary i32 into a positive parameter index.
    /// This converts negative indices into positive indices indexed from the
    /// end of the array. Negative indices that point before the start of the
    /// array are clamped to zero.
    size_t clamp_parameter_index(int32_t pos) const {
        if (pos < 0) {
            return static_cast<size_t>(std::max(pos + static_cast<int32_t>(storage_.size()), 0));
        } else {
            return std::min(static_cast<size_t>(pos), storage_.size());
        }
    }
    
    /// Retrieve a value from the vector
    Value<GC> get(size_t pos, Activation<GC>* activation) const {
        auto val = get_optional(pos);
        if (val.has_value()) {
            return *val;
        } else {
            throw make_error_1125(activation, static_cast<double>(pos), length());
        }
    }
    
    /// Retrieve a value from the vector or std::nullopt for out-of-bounds
    std::optional<Value<GC>> get_optional(size_t index) const {
        if (index < storage_.size()) {
            return storage_[index];
        }
        return std::nullopt;
    }
    
    /// Store a value into the vector.
    /// This function does no coercion as calling it requires mutably borrowing
    /// the vector (and thus it is unwise to reenter the AVM2 runtime to coerce
    /// things). You must use the associated coerce fn before storing things
    /// in the vector.
    /// This function yields an error if the position is outside the length of
    /// the vector.
    void set(size_t pos, Value<GC> value, Activation<GC>* activation) {
        if (!is_fixed_ && pos == length()) {
            storage_.resize(pos + 1, default_value());
        }
        
        if (pos < storage_.size()) {
            storage_[pos] = value;
        } else {
            throw make_error_1125(activation, static_cast<double>(pos), length());
        }
    }
    
    /// Push a value to the end of the vector.
    /// This function returns an error if the vector is fixed.
    /// This function does no coercion as calling it requires mutably borrowing
    /// the vector (and thus it is unwise to reenter the AVM2 runtime to coerce
    /// things). You must use the associated coerce fn before storing things
    /// in the vector.
    void push(Value<GC> value, Activation<GC>* activation) {
        check_fixed(activation);
        storage_.push_back(value);
    }
    
    /// Pop a value off the end of the vector.
    /// This function returns an error if the vector is fixed.
    Value<GC> pop(Activation<GC>* activation) {
        check_fixed(activation);
        
        if (!storage_.empty()) {
            Value<GC> val = storage_.back();
            storage_.pop_back();
            return val;
        } else if (value_type_.has_value()) {
            if (value_type_->is_builtin_numeric()) {
                return Value<GC>::Integer(0);
            } else {
                return Value<GC>::Undefined();
            }
        } else {
            return Value<GC>::Undefined();
        }
    }
    
    /// Push a value to the start of the vector (unshift).
    /// This function returns an error if the vector is fixed.
    void unshift(Value<GC> value, Activation<GC>* activation) {
        check_fixed(activation);
        storage_.insert(storage_.begin(), value);
    }
    
    /// Pop a value off the start of the vector (shift).
    /// This function returns an error if the vector is fixed.
    Value<GC> shift(Activation<GC>* activation) {
        check_fixed(activation);
        
        if (!storage_.empty()) {
            Value<GC> val = storage_.front();
            storage_.erase(storage_.begin());
            return val;
        } else if (value_type_.has_value()) {
            if (value_type_->is_builtin_numeric()) {
                return Value<GC>::Integer(0);
            } else {
                return Value<GC>::Undefined();
            }
        } else {
            return Value<GC>::Undefined();
        }
    }
    
    /// Insert a value at a specific position in the vector.
    /// This function returns an error if the vector is fixed.
    /// Negative bounds are supported and treated as indexing from the end of
    /// the array, backwards.
    void insert(int32_t position, Value<GC> value, Activation<GC>* activation) {
        check_fixed(activation);
        
        size_t pos = clamp_parameter_index(position);
        if (pos >= storage_.size()) {
            storage_.push_back(value);
        } else {
            storage_.insert(storage_.begin() + pos, value);
        }
    }
    
    /// Remove a value from a specific position in the vector.
    /// This function returns an error if the vector is fixed, empty, or being
    /// indexed out of bounds. Otherwise, it returns the removed value.
    /// Negative bounds are supported and treated as indexing from the end of
    /// the array, backwards. Negative arrays are *not* subject to the bounds
    /// check error.
    Value<GC> remove(int32_t position, Activation<GC>* activation) {
        check_fixed(activation);
        
        size_t pos = (position < 0) 
            ? static_cast<size_t>(std::max(position + static_cast<int32_t>(storage_.size()), 0))
            : static_cast<size_t>(position);
        
        if (pos >= storage_.size()) {
            throw make_error_1125(activation, static_cast<double>(pos), length());
        }
        
        Value<GC> val = storage_[pos];
        storage_.erase(storage_.begin() + pos);
        return val;
    }
    
    /// Reverse the vector's storage
    void reverse() {
        std::reverse(storage_.begin(), storage_.end());
    }
    
    /// Iterate over vector values
    class Iterator {
    private:
        typename std::vector<Value<GC>>::const_iterator it_;
        
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = Value<GC>;
        using difference_type = std::ptrdiff_t;
        using pointer = const Value<GC>*;
        using reference = const Value<GC>&;
        
        explicit Iterator(typename std::vector<Value<GC>>::const_iterator it) : it_(it) {}
        
        reference operator*() const { return *it_; }
        pointer operator->() const { return &*it_; }
        
        Iterator& operator++() { ++it_; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        Iterator& operator--() { --it_; return *this; }
        Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }
        
        Iterator operator+(difference_type n) const { return Iterator(it_ + n); }
        Iterator operator-(difference_type n) const { return Iterator(it_ - n); }
        difference_type operator-(const Iterator& other) const { return it_ - other.it_; }
        
        bool operator==(const Iterator& other) const { return it_ == other.it_; }
        bool operator!=(const Iterator& other) const { return it_ != other.it_; }
        bool operator<(const Iterator& other) const { return it_ < other.it_; }
        bool operator<=(const Iterator& other) const { return it_ <= other.it_; }
        bool operator>(const Iterator& other) const { return it_ > other.it_; }
        bool operator>=(const Iterator& other) const { return it_ >= other.it_; }
    };
    
    Iterator begin() const { return Iterator(storage_.begin()); }
    Iterator end() const { return Iterator(storage_.end()); }
    size_t size() const { return storage_.size(); }
    
    /// Replace this vector's storage with new values
    void replace_storage(std::vector<Value<GC>> new_storage) {
        storage_ = std::move(new_storage);
    }
    
    /// Replace the contents of the vector's storage with the contents of an iterator
    template<typename Iter>
    void replace_storage_with_iter(Iter begin, Iter end) {
        storage_.clear();
        storage_.insert(storage_.end(), begin, end);
    }
    
    /// Splice operation - replace a range with new values
    std::vector<Value<GC>> splice(
        size_t start,
        size_t count,
        std::vector<Value<GC>> replace_with
    ) {
        // Note: no fixed check here for bug compatibility
        std::vector<Value<GC>> removed;
        auto it = storage_.begin() + start;
        for (size_t i = 0; i < count && it != storage_.end(); ++i, ++it) {
            removed.push_back(*it);
        }
        storage_.erase(storage_.begin() + start, storage_.begin() + start + count);
        storage_.insert(storage_.begin() + start, replace_with.begin(), replace_with.end());
        return removed;
    }
    
    /// Get direct access to underlying storage
    const std::vector<Value<GC>>& storage() const {
        return storage_;
    }
};

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_VECTOR_H
