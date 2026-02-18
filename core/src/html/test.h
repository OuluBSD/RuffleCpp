// C++ translation of core/src/html/test.rs
// Tests for HTML module

#ifndef RUFFLE_CORE_HTML_TEST_H
#define RUFFLE_CORE_HTML_TEST_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace swf {
    struct Rectangle;
    class Twips;
}
namespace core {
namespace html {
namespace dimensions {
    struct Position;
    struct Size;
    struct BoxBounds;
}
namespace text_format {
    struct TextFormat;
    struct TextSpan;
    struct FormatSpans;
}
}
}
}

namespace ruffle {
namespace core {
namespace html {
namespace test {

// Position tests
void test_position_add();
void test_position_add_assign();

// Rectangle conversion tests
void test_into_swf_rectangle();
void test_from_swf_rectangle();

// Bounds tests
void test_bounds_extents();
void test_bounds_union();
void test_bounds_union_assign();
void test_bounds_union_reverse();
void test_bounds_union_assign_reverse();
void test_bounds_position_add();
void test_bounds_position_add_assign();
void test_bounds_size_add();
void test_bounds_size_add_assign();
void test_bounds_with_size();

// TextFormat tests
void test_textformat_merge();
void test_textformat_mix();

// FormatSpans tests
void test_formatspans_set_default();
void test_formatspans_resolve_position();
void test_formatspans_ensure_span_break();
void test_formatspans_ensure_span_break_redundant();
void test_formatspans_span_boundaries();
void test_formatspans_get_text_format();
void test_formatspans_normalize_no_spans();
void test_formatspans_normalize_no_text();
void test_formatspans_normalize_short_spans();
void test_formatspans_normalize_exact_spans();
void test_formatspans_normalize_long_spans();
void test_formatspans_normalize_merge_spans();
void test_formatspans_normalize_merge_many_spans();
void test_formatspans_normalize_long_spans_with_merge();
void test_formatspans_normalize_set_text_format_double_cut();
void test_formatspans_normalize_set_text_format_single_cut();
void test_formatspans_normalize_set_text_format_no_cut();
void test_formatspans_replace_text_inbounds();
void test_formatspans_replace_text_edgebounds();
void test_formatspans_replace_text_oob();
void test_formatspans_replace_text_degenerate();

/// Run all HTML module tests
void run_all_tests();

} // namespace test
} // namespace html
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_HTML_TEST_H
