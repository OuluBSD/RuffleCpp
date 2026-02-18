// C++ translation of wstr/src/tests.rs
// WStr unit tests

#ifndef RUFFLE_WSTR_TESTS_H
#define RUFFLE_WSTR_TESTS_H

// Test utilities for WStr
// Note: This is a stub header for test infrastructure
// The actual Rust tests use macros and test framework that don't directly translate

namespace ruffle {
namespace wstr {
namespace tests {

// Test helper macros (conceptual translation of Rust macros)
// bstr!(string) - Create WStr from byte string
// wstr!(chars) - Create WStr from wide characters

// Test functions (stubs for test infrastructure)
void test_roundtrip();
void test_eq();
void test_cmp();
void test_fmt();
void test_buf_concat_bytes();
void test_buf_concat_wide();
void test_offset_in();
void test_char_patterns();
void test_multi_char_patterns();
void test_str_patterns();
void test_split_ascii_prefix();
void test_char_boundary();
void test_utf8_index_mapping();
void test_utf8_index_mapping_empty();
void test_parse();

// Run all WStr tests
void run_all_tests();

} // namespace tests
} // namespace wstr
} // namespace ruffle

#endif // RUFFLE_WSTR_TESTS_H
