#ifndef TEST_HELPERS_HPP
#define TEST_HELPERS_HPP

#include <cstdint>
#include <string>

#include "tap.h"
#include "otlp_format_bundled.hpp"

#if defined(__cplusplus) && (__cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900))

namespace {

template <typename A, typename B>
static void check_equal(Tap &t, A a, B b, const std::string &description) {
  t.ok(a == static_cast<A>(b), description);
}

} // namespace

#endif

#endif /* TEST_HELPERS_HPP */
