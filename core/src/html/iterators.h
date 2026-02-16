// C++ translation of core/src/html/iterators.rs
//! Layout box iterators

#ifndef RUFFLE_CORE_HTML_ITERATORS_H
#define RUFFLE_CORE_HTML_ITERATORS_H

#include <cstddef>
#include <optional>
#include <algorithm>

// Forward declarations
namespace ruffle {
namespace core {
namespace html {
namespace text_format {
    class FormatSpans;
    class TextSpan;
}
namespace string {
    template<typename T> class WStr;
}
}
}
}

namespace ruffle {
namespace core {
namespace html {
namespace iterators {

/// Iterator implementation for the `iter_spans` method of `FormatSpans`.
template<typename GC>
class TextSpanIter {
public:
    /// Constructor
    /// @param base The FormatSpans to iterate over
    TextSpanIter(const text_format::FormatSpans* base)
        : start_pos_(0), base_(base), index_(0) {}

    /// Check if there are more spans to iterate
    /// @return true if there are more spans
    bool has_next() const {
        return base_->span(index_) != nullptr;
    }

    /// Get the next span in the iteration
    /// @return Tuple of (start_pos, end_pos, text_slice, span) or nullopt if done
    std::optional<std::tuple<size_t, size_t, const string::WStr<GC>*, const text_format::TextSpan*>> next() {
        const text_format::TextSpan* span = base_->span(index_);
        if (!span) {
            return std::nullopt;
        }

        index_ = index_ + 1;

        size_t text_len = base_->text()->len();
        size_t start_pos = std::min(start_pos_, text_len);
        size_t end_pos = std::min(start_pos_ + span->span_length, text_len);

        const string::WStr<GC>* text_slice = base_->displayed_text()->slice(start_pos, end_pos);
        if (!text_slice) {
            return std::nullopt;
        }

        start_pos_ = end_pos;

        return std::make_tuple(start_pos, end_pos, text_slice, span);
    }

    /// Reset the iterator to the beginning
    void reset() {
        start_pos_ = 0;
        index_ = 0;
    }

private:
    size_t start_pos_;
    const text_format::FormatSpans* base_;
    size_t index_;
};

} // namespace iterators
} // namespace html
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_HTML_ITERATORS_H
