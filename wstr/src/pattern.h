// C++ translation of wstr/src/pattern.rs
// Pattern matching for WStr (like core::str::pattern::Pattern but for WStr)

#ifndef RUFFLE_WSTR_PATTERN_H
#define RUFFLE_WSTR_PATTERN_H

#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <variant>

// Forward declarations
namespace ruffle {
namespace wstr {
    class WStr;
    template<typename T, typename U>
    struct Units;
}
}

namespace ruffle {
namespace wstr {
namespace pattern {

/// Search step returned by Searcher
enum class SearchStep {
    Match(std::size_t, std::size_t),   // Match found at [start, end)
    Reject(std::size_t, std::size_t),  // No match at [start, end)
    Done                                // Search complete
};

/// A searcher that can find patterns in a WStr
template<typename T>
class Searcher {
public:
    virtual ~Searcher() = default;

    /// Get the next search step
    virtual SearchStep next() = 0;

    /// Get the next search step from the back
    virtual SearchStep next_back() = 0;

    /// Get the next match
    std::optional<std::pair<std::size_t, std::size_t>> next_match() {
        while (true) {
            auto step = next();
            if (auto match = std::get_if<0>(&step)) {
                return *match;
            } else if (std::holds_alternative<2>(step)) {
                return std::nullopt;
            }
            // Continue on Reject
        }
    }

    /// Get the next match from the back
    std::optional<std::pair<std::size_t, std::size_t>> next_match_back() {
        while (true) {
            auto step = next_back();
            if (auto match = std::get_if<0>(&step)) {
                return *match;
            } else if (std::holds_alternative<2>(step)) {
                return std::nullopt;
            }
            // Continue on Reject
        }
    }

    /// Get the next reject
    std::optional<std::pair<std::size_t, std::size_t>> next_reject() {
        while (true) {
            auto step = next();
            if (auto reject = std::get_if<1>(&step)) {
                return *reject;
            } else if (std::holds_alternative<2>(step)) {
                return std::nullopt;
            }
            // Continue on Match
        }
    }

    /// Get the next reject from the back
    std::optional<std::pair<std::size_t, std::size_t>> next_reject_back() {
        while (true) {
            auto step = next_back();
            if (auto reject = std::get_if<1>(&step)) {
                return *reject;
            } else if (std::holds_alternative<2>(step)) {
                return std::nullopt;
            }
            // Continue on Match
        }
    }
};

/// Predicate trait for pattern matching
template<typename T>
class Predicate {
public:
    virtual ~Predicate() = default;
    virtual bool matches(T c) = 0;
};

/// Predicate that matches a single value
template<typename T>
class ValuePredicate : public Predicate<T> {
    T value_;
public:
    explicit ValuePredicate(T value) : value_(value) {}
    bool matches(T c) override { return c == value_; }
};

/// Predicate that matches any value in a set
template<typename T, typename U>
class AnyOfPredicate : public Predicate<T> {
    const U* values_;
    std::size_t count_;
public:
    AnyOfPredicate(const U* values, std::size_t count)
        : values_(values), count_(count) {}

    bool matches(T c) override {
        for (std::size_t i = 0; i < count_; ++i) {
            if (static_cast<U>(c) == values_[i]) {
                return true;
            }
        }
        return false;
    }
};

/// Predicate that uses a function
template<typename F>
class FnPredicate : public Predicate<std::uint16_t> {
    F func_;
public:
    explicit FnPredicate(F func) : func_(func) {}
    bool matches(std::uint16_t c) override { return func_(c); }
};

/// Searcher for predicate-based patterns
template<typename T, typename P>
class PredSearcher : public Searcher<std::variant<std::monostate, std::pair<std::size_t, std::size_t>, std::monostate>> {
    const T* haystack_;
    std::size_t haystack_len_;
    P* predicate_;
    std::size_t front_;
    bool can_match_;

public:
    PredSearcher(bool can_match, const T* haystack, std::size_t len, P* predicate)
        : haystack_(haystack), haystack_len_(len), predicate_(predicate),
          front_(can_match ? 0 : len), can_match_(can_match) {}

    SearchStep next() override {
        if (front_ >= haystack_len_) {
            return SearchStep::Done;
        }

        T c = haystack_[front_];
        std::size_t i = front_;
        front_++;

        if (predicate_->matches(c)) {
            return SearchStep::Match(i, i + 1);
        } else {
            return SearchStep::Reject(i, i + 1);
        }
    }

    SearchStep next_back() override {
        if (front_ >= haystack_len_) {
            return SearchStep::Done;
        }

        T c = haystack_[haystack_len_ - 1];
        std::size_t len = haystack_len_;
        haystack_len_--;

        if (predicate_->matches(c)) {
            return SearchStep::Match(len - 1, len);
        } else {
            return SearchStep::Reject(len - 1, len);
        }
    }
};

/// Searcher for slice patterns
template<typename T>
class SliceSearcher : public Searcher<std::variant<std::monostate, std::pair<std::size_t, std::size_t>, std::monostate>> {
    const T* haystack_;
    std::size_t haystack_len_;
    const T* needle_;
    std::size_t needle_len_;
    std::size_t front_;
    std::size_t back_;

public:
    SliceSearcher(const T* haystack, std::size_t h_len, const T* needle, std::size_t n_len)
        : haystack_(haystack), haystack_len_(h_len), needle_(needle), needle_len_(n_len) {
        if (h_len >= n_len) {
            front_ = 0;
            back_ = h_len - n_len;
        } else {
            front_ = 1;
            back_ = 0;
        }
    }

    SearchStep next() override {
        if (front_ > back_) {
            return SearchStep::Done;
        }

        std::size_t start = front_;
        std::size_t end = front_ + needle_len_;

        bool match = true;
        for (std::size_t i = 0; i < needle_len_; ++i) {
            if (haystack_[start + i] != needle_[i]) {
                match = false;
                break;
            }
        }

        if (match) {
            front_ = end;
            return SearchStep::Match(start, end);
        } else {
            front_++;
            return SearchStep::Reject(start, start + 1);
        }
    }

    SearchStep next_back() override {
        if (front_ > back_) {
            return SearchStep::Done;
        }

        std::size_t start = back_;
        std::size_t end = start + needle_len_;

        bool match = true;
        for (std::size_t i = 0; i < needle_len_; ++i) {
            if (haystack_[start + i] != needle_[i]) {
                match = false;
                break;
            }
        }

        if (match) {
            if (start >= needle_len_) {
                back_ = start - needle_len_;
            } else {
                front_ = 1;
                back_ = 0;
            }
            return SearchStep::Match(start, end);
        } else {
            if (back_ == 0) {
                front_ = 1;
            } else {
                back_--;
            }
            return SearchStep::Reject(end - 1, end);
        }
    }
};

/// Searcher for empty patterns (matches at every position)
class EmptySearcher : public Searcher<std::variant<std::monostate, std::pair<std::size_t, std::size_t>, std::monostate>> {
    std::size_t current_;
    std::size_t len_;

public:
    explicit EmptySearcher(std::size_t len) : current_(0), len_(len + 1) {}

    SearchStep next() override {
        if (current_ >= len_) {
            return SearchStep::Done;
        }
        std::size_t i = current_++;
        return SearchStep::Match(i, i);
    }

    SearchStep next_back() override {
        if (current_ >= len_) {
            return SearchStep::Done;
        }
        std::size_t i = --len_;
        return SearchStep::Match(i, i);
    }
};

/// Either type for holding one of two searcher types
template<typename T, typename U>
class Either {
    std::variant<T, U> data_;

public:
    explicit Either(T value) : data_(value) {}
    explicit Either(U value) : data_(value) {}

    bool is_left() const { return std::holds_alternative<T>(data_); }
    bool is_right() const { return std::holds_alternative<U>(data_); }

    T& left() { return std::get<T>(data_); }
    U& right() { return std::get<U>(data_); }
    const T& left() const { return std::get<T>(data_); }
    const U& right() const { return std::get<U>(data_); }
};

} // namespace pattern
} // namespace wstr
} // namespace ruffle

#endif // RUFFLE_WSTR_PATTERN_H
