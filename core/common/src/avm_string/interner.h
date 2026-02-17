// C++ translation of core/common/src/avm_string/interner.rs
// AVM String interner

#ifndef RUFFLE_CORE_COMMON_AVM_STRING_INTERNER_H
#define RUFFLE_CORE_COMMON_AVM_STRING_INTERNER_H

#include <cstdint>
#include <memory>
#include <optional>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>

// Forward declarations
namespace ruffle {
namespace wstr {
    class WStr;
}
namespace core {
namespace avm_string {
    template<typename T> class AvmString;
    template<typename T> class AvmStringRepr;
    template<typename T> class CommonStrings;
    class Mutation;

    /// An interned `AvmString`, with fast by-pointer equality and hashing.
    template<typename GC>
    class AvmAtom {
    public:
        AvmAtom() = default;
        explicit AvmAtom(std::shared_ptr<AvmStringRepr<GC>> repr) : repr_(std::move(repr)) {}

        bool operator==(const AvmAtom& other) const {
            return repr_ == other.repr_;
        }

        bool operator!=(const AvmAtom& other) const {
            return !(*this == other);
        }

        const wstr::WStr* as_wstr() const {
            return repr_.get();
        }

        std::shared_ptr<AvmStringRepr<GC>> repr() const {
            return repr_;
        }

    private:
        std::shared_ptr<AvmStringRepr<GC>> repr_;
    };

    /// A set holding weakly to its elements.
    ///
    /// Stale entries get regularly cleaned up in response to memory pressure:
    /// - in the tracing phase of each GC cycle;
    /// - upon insertion when the set is at capacity.
    template<typename GC, typename T>
    class WeakSet {
    public:
        WeakSet() = default;

        /// Entry types for hash table operations
        enum class EntryType {
            Occupied,
            Vacant
        };

        /// Get or create entry for key
        std::optional<std::shared_ptr<T>> entry(const wstr::WStr* key) {
            auto it = table_.find(key);
            if (it != table_.end()) {
                if (auto locked = it->lock()) {
                    return locked;
                } else {
                    table_.erase(it);
                }
            }
            return std::nullopt;
        }

        /// Insert a fresh key
        std::shared_ptr<T> insert_fresh(std::shared_ptr<T> key) {
            if (table_.size() >= table_.bucket_count()) {
                prune_and_grow();
            }
            table_.insert(key.get());
            return key;
        }

        /// Prune stale entries and/or resize the table
        void prune_and_grow() {
            // Remove dead references
            std::unordered_set<const wstr::WStr*> new_table;
            for (const auto& weak : table_) {
                if (auto locked = weak.lock()) {
                    new_table.insert(locked.get());
                }
            }
            table_ = std::move(new_table);
        }

    private:
        std::unordered_set<const wstr::WStr*> table_;
    };

    /// AVM String Interner
    /// Manages string interning for fast comparison and memory efficiency
    template<typename GC>
    class AvmStringInterner {
    public:
        AvmStringInterner() = default;

        /// Create new interner
        static AvmStringInterner create() {
            AvmStringInterner result;
            
            // Initialize common strings
            result.common_ = CommonStrings<GC>::create(
                [&result](const std::vector<uint8_t>& s) -> AvmAtom<GC> {
                    // Create interned string from static data
                    auto wstr = wstr::WStr::from_units(s);
                    auto repr = std::make_shared<AvmStringRepr<GC>>(wstr, true);
                    return AvmAtom<GC>(result.interned_.insert_fresh(repr));
                }
            );
            
            return result;
        }

        /// Intern a string
        /// The string returned by `f` should be interned, and equivalent to `s`.
        template<typename S, typename F>
        AvmAtom<GC> intern_inner(const S& s, F f)
        where
            S: std::dereferenceable<wstr::WStr>,
            F: std::function<std::shared_ptr<AvmStringRepr<GC>>(const S&)>
        {
            if (auto existing = interned_.entry(&s)) {
                return AvmAtom<GC>(*existing);
            } else {
                return AvmAtom<GC>(interned_.insert_fresh(f(s)));
            }
        }

        /// Get an interned string if it exists
        std::optional<AvmAtom<GC>> get(const wstr::WStr* s) {
            if (auto existing = interned_.entry(s)) {
                return AvmAtom<GC>(*existing);
            }
            return std::nullopt;
        }

        /// Create a substring
        AvmString<GC> substring(
            AvmString<GC> s,
            size_t start_index,
            size_t end_index
        ) {
            // It's assumed that start<=end. This is tested later via a range check.
            if (start_index == end_index) {
                return common_.str_; // this is the empty string
            } else if (end_index == start_index + 1) {
                if (auto c = s.get(start_index)) {
                    if (auto char_str = common_.ascii_chars.get(*c)) {
                        return *char_str;
                    }
                }
            }
            return AvmString<GC>::substring(s, start_index, end_index);
        }

        /// Get common strings
        const CommonStrings<GC>& common() const {
            return common_;
        }

        CommonStrings<GC>& common() {
            return common_;
        }

    private:
        WeakSet<GC, AvmStringRepr<GC>> interned_;
        CommonStrings<GC> common_;
    };

} // namespace avm_string
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_COMMON_AVM_STRING_INTERNER_H
