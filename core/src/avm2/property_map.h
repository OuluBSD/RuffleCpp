// C++ translation of core/src/avm2/property_map.rs
//! Property map

#ifndef RUFFLE_CORE_AVM2_PROPERTY_MAP_H
#define RUFFLE_CORE_AVM2_PROPERTY_MAP_H

#include <unordered_map>
#include <vector>
#include <optional>
#include <memory>
#include <algorithm>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC> class AvmString;
    template<typename GC> class Multiname;
    template<typename GC> class Namespace;
    template<typename GC> class QName;
    template<typename GC> class Mutation;

    namespace fnv {
        // FNV hash builder
        struct FnvBuildHasher {};
    }

    namespace smallvec {
        // Small vector optimization
        template<typename T, size_t N>
        class SmallVec : public std::vector<T> {
        public:
            using std::vector<T>::vector;
        };
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

using fnv::FnvBuildHasher;
using smallvec::SmallVec;

/// Type which represents named properties on an object.
///
/// This type exposes interfaces akin to `HashMap<QName<'gc>, V>`, and is
/// intended to serve as a drop-in replacement optimized for objects where few
/// properties have overlapping local names. However, we have made slight
/// changes to the API in the following cases:
///
///  * Iterators return tuples of namespace, local-name, and value; rather than
///    a qualified name and value pair.
///  * Only `HashMap` methods that we need are implemented.
///
/// The internal structure of the `PropertyMap` technically allows storage of
/// multiple values per `QName`. It's implementation enforces the invariant
/// that each `QName` only have one associated `V`.
template<typename GC, typename V>
class PropertyMap {
private:
    // HashMap<AvmString<'gc>, SmallVec<[(Namespace<'gc>, V); 2]>, FnvBuildHasher>
    using BucketType = SmallVec<std::pair<Namespace<GC>, V>, 2>;
    std::unordered_map<AvmString<GC>, BucketType> map_;

public:
    PropertyMap() = default;

    static PropertyMap<GC, V> new_() {
        return PropertyMap<GC, V>();
    }

    /// Get a value by QName
    std::optional<std::reference_wrapper<const V>> get(QName<GC> name) const {
        auto it = map_.find(name.local_name());
        if (it != map_.end()) {
            const auto& bucket = it->second;
            for (const auto& [ns, value] : bucket) {
                if (ns.matches_ns(name.namespace_())) {
                    return std::ref(value);
                }
            }
        }
        return std::nullopt;
    }

    /// Get a value by Multiname
    std::optional<std::reference_wrapper<const V>> get_for_multiname(const Multiname<GC>& name) const {
        if (name.has_lazy_component()) {
            // In real implementation, this would be an unreachable!()
            return std::nullopt;
        }

        auto local_name = name.local_name();
        if (!local_name.has_value()) {
            return std::nullopt;
        }

        auto it = map_.find(local_name.value());
        if (it != map_.end()) {
            const auto& bucket = it->second;
            const auto& ns_set = name.namespace_set();
            for (const auto& [ns, value] : bucket) {
                for (const auto& check_ns : ns_set) {
                    if (ns.matches_ns(check_ns)) {
                        return std::ref(value);
                    }
                }
            }
        }
        return std::nullopt;
    }

    /// Get a value and namespace by Multiname
    std::optional<std::pair<Namespace<GC>, std::reference_wrapper<const V>>> 
    get_with_ns_for_multiname(const Multiname<GC>& name) const {
        if (name.has_lazy_component()) {
            // In real implementation, this would be an unreachable!()
            return std::nullopt;
        }

        auto local_name = name.local_name();
        if (!local_name.has_value()) {
            return std::nullopt;
        }

        auto it = map_.find(local_name.value());
        if (it != map_.end()) {
            const auto& bucket = it->second;
            const auto& ns_set = name.namespace_set();
            for (const auto& [ns, value] : bucket) {
                for (const auto& check_ns : ns_set) {
                    if (ns.matches_ns(check_ns)) {
                        return std::make_pair(ns, std::ref(value));
                    }
                }
            }
        }
        return std::nullopt;
    }

    /// Get a mutable reference to a value by QName
    std::optional<std::reference_wrapper<V>> get_mut(QName<GC> name) {
        auto it = map_.find(name.local_name());
        if (it != map_.end()) {
            auto& bucket = it->second;
            for (auto& [ns, value] : bucket) {
                if (ns.matches_ns(name.namespace_())) {
                    return std::ref(value);
                }
            }
        }
        return std::nullopt;
    }

    /// Check if the map contains a key
    bool contains_key(QName<GC> name) const {
        auto it = map_.find(name.local_name());
        if (it != map_.end()) {
            const auto& bucket = it->second;
            for (const auto& [ns, _] : bucket) {
                if (ns.matches_ns(name.namespace_())) {
                    return true;
                }
            }
        }
        return false;
    }

    /// Iterator item type: (AvmString, Namespace, &V)
    struct IteratorItem {
        AvmString<GC> key;
        Namespace<GC> ns;
        const V& value;
    };

    /// Iterator for the property map
    class Iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = IteratorItem;
        using difference_type = std::ptrdiff_t;
        using pointer = const IteratorItem*;
        using reference = const IteratorItem&;

        Iterator(const std::unordered_map<AvmString<GC>, BucketType>& map)
            : map_(map), outer_it_(map.begin()), inner_it_(nullptr), outer_end_(map.end()) {
            advance_to_valid();
        }

        Iterator begin() const { return Iterator(map_); }
        Iterator end() const { 
            Iterator it(map_);
            it.outer_it_ = map_.end();
            return it;
        }

        bool has_next() const { return outer_it_ != outer_end_; }

        IteratorItem next() {
            if (outer_it_ == outer_end_) {
                // Return default item
                return IteratorItem{AvmString<GC>(), Namespace<GC>(), *reinterpret_cast<const V*>(nullptr)};
            }

            auto item = IteratorItem{
                outer_it_->first,
                inner_it_->first,
                inner_it_->second
            };

            ++inner_idx_;
            if (inner_idx_ >= inner_it_->size()) {
                ++outer_it_;
                inner_idx_ = 0;
                advance_to_valid();
            }

            return item;
        }

    private:
        const std::unordered_map<AvmString<GC>, BucketType>& map_;
        typename std::unordered_map<AvmString<GC>, BucketType>::const_iterator outer_it_;
        const BucketType* inner_it_;
        typename std::unordered_map<AvmString<GC>, BucketType>::const_iterator outer_end_;
        size_t inner_idx_ = 0;

        void advance_to_valid() {
            while (outer_it_ != outer_end_) {
                if (!outer_it_->second.empty()) {
                    inner_it_ = &outer_it_->second;
                    inner_idx_ = 0;
                    return;
                }
                ++outer_it_;
            }
        }
    };

    /// Iterate over all properties
    Iterator iter() const {
        return Iterator(map_);
    }

    /// Insert a value by QName, returns old value if exists
    std::optional<V> insert(QName<GC> name, V value) {
        auto& bucket = map_[name.local_name()];

        for (auto& [ns, old_value] : bucket) {
            if (ns.matches_ns(name.namespace_())) {
                std::swap(old_value, value);
                return value;
            }
        }

        bucket.push_back({name.namespace_(), std::move(value)});
        return std::nullopt;
    }

    /// Insert a value with explicit namespace
    std::optional<V> insert_with_namespace(Namespace<GC> ns, AvmString<GC> name, V value) {
        auto& bucket = map_[name];

        for (auto& [existing_ns, old_value] : bucket) {
            if (existing_ns.matches_ns(ns)) {
                std::swap(old_value, value);
                return value;
            }
        }

        bucket.push_back({ns, std::move(value)});
        return std::nullopt;
    }

    /// Remove a value by QName
    std::optional<V> remove(QName<GC> name) {
        auto it = map_.find(name.local_name());
        if (it != map_.end()) {
            auto& bucket = it->second;
            for (size_t i = 0; i < bucket.size(); ++i) {
                if (bucket[i].first.matches_ns(name.namespace_())) {
                    V value = std::move(bucket[i].second);
                    bucket.erase(bucket.begin() + i);
                    return value;
                }
            }
        }
        return std::nullopt;
    }
};

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_PROPERTY_MAP_H
