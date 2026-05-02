#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "tap.h"
#include "otlp_format_bundled.h"

extern tap_t g_tap;
extern int g_test_index;
extern tap_t *g_current_tap;

#define CHECK(expr) \
  tap_ok(g_current_tap, (expr), #expr)

static inline int otlp_require_ptr(const void *ptr, const char *description) {
  tap_ok(g_current_tap, ptr != NULL, description);
  return ptr != NULL;
}

#define OTLP_DEFINE_SUBTEST(name) \
  static void name##_subtest(tap_t *subtest_t) { \
    tap_t *otlp_prev_tap = g_current_tap; \
    g_current_tap = subtest_t; \
    name(); \
    g_current_tap = otlp_prev_tap; \
  }

static inline void otlp_test_ok(const char *name) {
  ++g_test_index;
  tap_pass(g_current_tap, name);
}

static inline size_t otlp_test_varint_size(uint64_t value) {
  size_t size = 1;
  while (value >= 128U) {
    value >>= 7U;
    ++size;
  }
  return size;
}

static inline void otlp_test_write_varint(uint8_t *dst, size_t *off, uint64_t value) {
  while (value >= 128U) {
    dst[(*off)++] = (uint8_t) ((value & 0x7FU) | 0x80U);
    value >>= 7U;
  }
  dst[(*off)++] = (uint8_t) value;
}

static inline uint8_t *otlp_test_append_unknown_varint_field(
  const uint8_t *src,
  size_t src_len,
  uint32_t field_number,
  uint64_t value,
  size_t *out_len
) {
  size_t off = src_len;
  size_t extra = otlp_test_varint_size(((uint64_t) field_number << 3U)) +
    otlp_test_varint_size(value);
  uint8_t *out = (uint8_t *) malloc(src_len + extra);
  if (!otlp_require_ptr(out, "out != NULL")) {
    return NULL;
  }
  memcpy(out, src, src_len);
  otlp_test_write_varint(out, &off, ((uint64_t) field_number << 3U));
  otlp_test_write_varint(out, &off, value);
  *out_len = off;
  return out;
}

static inline int otlp_test_contains_bytes(
  const uint8_t *haystack,
  size_t haystack_len,
  const uint8_t *needle,
  size_t needle_len
) {
  size_t i;
  if (needle_len == 0 || needle_len > haystack_len) {
    return 0;
  }
  for (i = 0; i <= haystack_len - needle_len; ++i) {
    if (memcmp(haystack + i, needle, needle_len) == 0) {
      return 1;
    }
  }
  return 0;
}

#endif /* TEST_HELPERS_H */
