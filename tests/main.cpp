#include <cstdint>
#include <vector>

#define OTLP_FORMAT_IMPLEMENTATION
#include "test_helpers.hpp"

void test_common_cpp(Tap &t);
int test_common_cpp_count(void);

void test_logs_cpp(Tap &t);
int test_logs_cpp_count(void);

void test_profiles_cpp(Tap &t);
int test_profiles_cpp_count(void);

void test_traces_cpp(Tap &t);
int test_traces_cpp_count(void);

void test_metrics_cpp(Tap &t);
int test_metrics_cpp_count(void);

#if defined(__cplusplus) && (__cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900))

int main() {
  Tap t;

  int planned = 0;
  planned += test_common_cpp_count();
  planned += test_logs_cpp_count();
  planned += test_profiles_cpp_count();
  planned += test_traces_cpp_count();
  planned += test_metrics_cpp_count();

  t.plan(planned);

  test_common_cpp(t);
  test_logs_cpp(t);
  test_profiles_cpp(t);
  test_traces_cpp(t);
  test_metrics_cpp(t);

  return t.end();
}

#else

int main() {
  return 0;
}

#endif
