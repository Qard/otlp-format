#include <stdio.h>
#define OTLP_FORMAT_IMPLEMENTATION
#include "test_helpers.h"

tap_t g_tap;
int g_test_index = 0;
tap_t *g_current_tap = NULL;

int test_common_count(void);
void test_common_run(tap_t *t);

int test_logs_count(void);
void test_logs_run(tap_t *t);

int test_profiles_count(void);
void test_profiles_run(tap_t *t);

int test_traces_count(void);
void test_traces_run(tap_t *t);

int test_metrics_count(void);
void test_metrics_run(tap_t *t);

int main(void) {
  int planned;

  g_tap = tap(stdout);
  g_current_tap = &g_tap;

  planned = test_common_count();
  planned += test_logs_count();
  planned += test_profiles_count();
  planned += test_traces_count();
  planned += test_metrics_count();

  tap_plan(&g_tap, planned);

  test_common_run(&g_tap);
  test_logs_run(&g_tap);
  test_profiles_run(&g_tap);
  test_traces_run(&g_tap);
  test_metrics_run(&g_tap);

  return tap_end(&g_tap);
}
