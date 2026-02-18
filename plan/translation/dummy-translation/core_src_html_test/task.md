# Task: Translate core/src/html/test.rs

## Description
Translate the Rust file `core/src/html/test.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/html/test.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated HTML module test functions to C++ header
- Translated position tests: test_position_add(), test_position_add_assign()
- Translated rectangle conversion tests: test_into_swf_rectangle(), test_from_swf_rectangle()
- Translated bounds tests: test_bounds_extents(), test_bounds_union(), test_bounds_union_assign()
- Translated bounds reverse tests: test_bounds_union_reverse(), test_bounds_union_assign_reverse()
- Translated bounds position tests: test_bounds_position_add(), test_bounds_position_add_assign()
- Translated bounds size tests: test_bounds_size_add(), test_bounds_size_add_assign()
- Translated bounds with size test: test_bounds_with_size()
- Translated TextFormat tests: test_textformat_merge(), test_textformat_mix()
- Translated FormatSpans tests: test_formatspans_set_default(), test_formatspans_resolve_position()
- Translated FormatSpans break tests: test_formatspans_ensure_span_break(), test_formatspans_ensure_span_break_redundant()
- Translated FormatSpans boundary tests: test_formatspans_span_boundaries(), test_formatspans_get_text_format()
- Translated FormatSpans normalize tests: test_formatspans_normalize_no_spans(), test_formatspans_normalize_no_text()
- Translated FormatSpans span tests: test_formatspans_normalize_short_spans(), test_formatspans_normalize_exact_spans()
- Translated FormatSpans long span tests: test_formatspans_normalize_long_spans(), test_formatspans_normalize_merge_spans()
- Translated FormatSpans merge tests: test_formatspans_normalize_merge_many_spans(), test_formatspans_normalize_long_spans_with_merge()
- Translated FormatSpans set_text_format tests: test_formatspans_normalize_set_text_format_double_cut(), test_formatspans_normalize_set_text_format_single_cut(), test_formatspans_normalize_set_text_format_no_cut()
- Translated FormatSpans replace_text tests: test_formatspans_replace_text_inbounds(), test_formatspans_replace_text_edgebounds(), test_formatspans_replace_text_oob(), test_formatspans_replace_text_degenerate()
- Added run_all_tests() function for running all tests
- Added proper forward declarations for all HTML dimension and text_format types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
