#include "test_helpers.hpp"

#if defined(__cplusplus) && (__cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900))

#if defined(OTLP_FORMAT_ENABLE_METRICS)
#include <vector>

namespace {

static void test_cpp_metrics_wrappers(Tap &t) {
  const std::uint8_t common_bytes[] = {1, 2, 3};

  otlp::AnyValue nested = otlp::AnyValue::string("nested");
  otlp::KeyValue kv;
  kv.set_key("key");
  kv.set_value(nested);

  otlp::InstrumentationScope common_scope;
  common_scope.set_name("scope");
  common_scope.set_version("1.0");
  common_scope.append_attribute(kv);

  otlp::EntityRef entity_ref;
  entity_ref.set_schema_url("entity-schema");
  entity_ref.set_type("service.instance");
  entity_ref.append_id_key("service.instance.id");
  entity_ref.append_description_key("entity");

  otlp::Resource common_resource;
  common_resource.append_attribute(kv);
  common_resource.append_entity_ref(entity_ref);

  otlp::Exemplar exemplar;
  exemplar.set_time_unix_nano(111);
  exemplar.set_as_double(9.5);
  exemplar.set_span_id(common_bytes, sizeof(common_bytes));
  exemplar.set_trace_id(common_bytes, sizeof(common_bytes));
  exemplar.append_filtered_attribute(kv);

  otlp::NumberDataPoint number_point;
  number_point.append_attribute(kv);
  number_point.set_start_time_unix_nano(100);
  number_point.set_time_unix_nano(200);
  number_point.set_as_double(3.14);
  number_point.append_exemplar(exemplar);
  number_point.set_flags(OTLP_DATA_POINT_FLAGS_NO_RECORDED_VALUE_MASK);

  otlp::Gauge gauge;
  gauge.append_data_point(number_point);

  otlp::Metric metric;
  metric.set_name("cpp.metric");
  metric.set_gauge(gauge);

  otlp::Sum sum;
  sum.append_data_point(number_point);
  sum.set_aggregation_temporality(OTLP_AGGREGATION_TEMPORALITY_DELTA);
  sum.set_is_monotonic(true);

  otlp::Metric sum_metric;
  sum_metric.set_name("cpp.sum");
  sum_metric.set_unit("ms");
  sum_metric.append_metadata(kv);
  sum_metric.set_sum(sum);

  otlp::HistogramDataPoint histogram_point;
  histogram_point.append_attribute(kv);
  histogram_point.set_start_time_unix_nano(100);
  histogram_point.set_time_unix_nano(200);
  histogram_point.set_count(2);
  histogram_point.set_sum(6.0);
  histogram_point.append_bucket_count(1);
  histogram_point.append_bucket_count(1);
  histogram_point.append_explicit_bound(4.0);
  histogram_point.append_exemplar(exemplar);
  histogram_point.set_flags(OTLP_DATA_POINT_FLAGS_NO_RECORDED_VALUE_MASK);
  histogram_point.set_min(2.0);
  histogram_point.set_max(4.0);

  otlp::Histogram histogram;
  histogram.append_data_point(histogram_point);
  histogram.set_aggregation_temporality(
      OTLP_AGGREGATION_TEMPORALITY_CUMULATIVE);

  otlp::Metric histogram_metric;
  histogram_metric.set_histogram(histogram);

  otlp::ExponentialHistogramBuckets positive_buckets;
  positive_buckets.set_offset(-1);
  positive_buckets.append_bucket_count(3);

  otlp::ExponentialHistogramBuckets negative_buckets;
  negative_buckets.set_offset(1);
  negative_buckets.append_bucket_count(4);

  otlp::ExponentialHistogramDataPoint exponential_point;
  exponential_point.append_attribute(kv);
  exponential_point.set_start_time_unix_nano(100);
  exponential_point.set_time_unix_nano(200);
  exponential_point.set_count(7);
  exponential_point.set_sum(12.0);
  exponential_point.set_scale(-2);
  exponential_point.set_zero_count(1);
  exponential_point.set_zero_threshold(0.001);
  exponential_point.set_positive(positive_buckets);
  exponential_point.set_negative(negative_buckets);
  exponential_point.append_exemplar(exemplar);
  exponential_point.set_flags(OTLP_DATA_POINT_FLAGS_NO_RECORDED_VALUE_MASK);
  exponential_point.set_min(1.0);
  exponential_point.set_max(8.0);

  otlp::ExponentialHistogram exponential_histogram;
  exponential_histogram.append_data_point(exponential_point);
  exponential_histogram.set_aggregation_temporality(
      OTLP_AGGREGATION_TEMPORALITY_DELTA);

  otlp::Metric exponential_histogram_metric;
  exponential_histogram_metric.set_exponential_histogram(
      exponential_histogram);

  otlp::SummaryDataPointValueAtQuantile quantile;
  quantile.set_quantile(0.5);
  quantile.set_value(5.0);

  otlp::SummaryDataPoint summary_point;
  summary_point.append_attribute(kv);
  summary_point.set_start_time_unix_nano(100);
  summary_point.set_time_unix_nano(200);
  summary_point.set_count(9);
  summary_point.set_sum(45.0);
  summary_point.append_quantile_value(quantile);
  summary_point.set_flags(OTLP_DATA_POINT_FLAGS_NO_RECORDED_VALUE_MASK);

  otlp::Summary summary;
  summary.append_data_point(summary_point);

  otlp::Metric summary_metric;
  summary_metric.set_summary(summary);

  check_equal(t, sum_metric.get()->data_kind, OTLP_METRIC_DATA_SUM,
              "sum metric kind");
  check_equal(t, sum_metric.get()->name.len, static_cast<size_t>(7),
              "sum metric name length");
  check_equal(t, sum_metric.get()->unit.len, static_cast<size_t>(2),
              "sum metric unit length");
  check_equal(t, sum_metric.get()->metadata.len, static_cast<size_t>(1),
              "sum metric metadata count");
  check_equal(t, histogram_metric.get()->data_kind,
              OTLP_METRIC_DATA_HISTOGRAM, "histogram metric kind");
  check_equal(t, exponential_histogram_metric.get()->data_kind,
              OTLP_METRIC_DATA_EXPONENTIAL_HISTOGRAM,
              "exponential histogram metric kind");
  check_equal(t, summary_metric.get()->data_kind, OTLP_METRIC_DATA_SUMMARY,
              "summary metric kind");
  check_equal(t, metric.get()->data.gauge.data_points.len,
              static_cast<size_t>(1), "gauge data point count");
  check_equal(t, sum.get()->data_points.len, static_cast<size_t>(1),
              "sum data point count");
  check_equal(t, histogram.get()->data_points.ptr[0].has_sum, 1,
              "histogram optional sum presence");
  check_equal(t, exponential_histogram.get()->data_points.ptr[0].has_positive,
              1, "exponential histogram positive bucket presence");
  check_equal(t, summary.get()->data_points.ptr[0].quantile_values.len,
              static_cast<size_t>(1), "summary quantile count");

  otlp::ScopeMetrics metrics_scope;
  metrics_scope.set_scope(common_scope);
  metrics_scope.set_schema_url("metrics-scope-schema");
  metrics_scope.append_metric(metric);

  otlp::ResourceMetrics metrics_resource;
  metrics_resource.set_resource(common_resource);
  metrics_resource.set_schema_url("metrics-resource-schema");
  metrics_resource.append_scope_metrics(metrics_scope);

  otlp::MetricsExportRequest metrics_request;
  metrics_request.append_resource_metrics(metrics_resource);

  std::vector<std::uint8_t> encoded_metrics(metrics_request.encoded_size());
  t.not_ok(encoded_metrics.empty(), "metrics request buffer allocated");
  check_equal(t,
              metrics_request.encode(encoded_metrics.data(),
                                     encoded_metrics.size()),
              OTLP_STATUS_OK, "metrics request encode");

  otlp::MetricsExportRequest decoded_metrics;
  check_equal(t,
              decoded_metrics.decode(encoded_metrics.data(),
                                     encoded_metrics.size()),
              OTLP_STATUS_OK, "metrics request decode");
  check_equal(t, decoded_metrics.get()->resource_metrics.len,
              static_cast<size_t>(1),
              "decoded metrics resource count");
  check_equal(t, decoded_metrics.get()->resource_metrics.ptr[0].schema_url.len,
              static_cast<size_t>(23),
              "decoded metrics resource schema length");
  check_equal(
      t,
      decoded_metrics.get()->resource_metrics.ptr[0]
          .scope_metrics.ptr[0]
          .schema_url.len,
      static_cast<size_t>(20), "decoded metrics scope schema length");
}

static void test_cpp_metrics_copy_independence(Tap &t) {
  otlp::AnyValue nested = otlp::AnyValue::string("nested");
  otlp::KeyValue kv;
  kv.set_key("key");
  kv.set_value(nested);

  otlp::InstrumentationScope common_scope;
  common_scope.set_name("scope");
  common_scope.set_version("1.0");
  common_scope.append_attribute(kv);

  otlp::EntityRef entity_ref;
  entity_ref.set_schema_url("entity-schema");
  entity_ref.set_type("service.instance");
  entity_ref.append_id_key("service.instance.id");
  entity_ref.append_description_key("entity");

  otlp::Resource common_resource;
  common_resource.append_attribute(kv);
  common_resource.append_entity_ref(entity_ref);

  otlp::NumberDataPoint point;
  point.set_time_unix_nano(1234);
  point.set_as_double(42.5);

  otlp::Gauge gauge;
  gauge.append_data_point(point);

  otlp::Metric metric;
  metric.set_name("cpu.usage");
  metric.set_gauge(gauge);

  otlp::ScopeMetrics metrics_scope;
  metrics_scope.set_scope(common_scope);
  metrics_scope.append_metric(metric);

  otlp::ResourceMetrics metrics_resource;
  metrics_resource.set_resource(common_resource);
  metrics_resource.set_schema_url("metrics-resource-schema");
  metrics_resource.append_scope_metrics(metrics_scope);

  otlp::MetricsExportRequest metrics_request;
  metrics_request.append_resource_metrics(metrics_resource);

  otlp::MetricsExportRequest copied_metrics = metrics_request;
  metrics_request.append_resource_metrics(metrics_resource);

  check_equal(t, copied_metrics.get()->resource_metrics.len,
              static_cast<size_t>(1),
              "copied metrics request resource count");
  check_equal(t, metrics_request.get()->resource_metrics.len,
              static_cast<size_t>(2),
              "mutated original metrics request resource count");
}

static void test_cpp_metrics_data_and_response(Tap &t) {
  otlp::AnyValue nested = otlp::AnyValue::string("nested");
  otlp::KeyValue kv;
  kv.set_key("key");
  kv.set_value(nested);

  otlp::InstrumentationScope common_scope;
  common_scope.set_name("scope");
  common_scope.set_version("1.0");
  common_scope.append_attribute(kv);

  otlp::EntityRef entity_ref;
  entity_ref.set_schema_url("entity-schema");
  entity_ref.set_type("service.instance");
  entity_ref.append_id_key("service.instance.id");
  entity_ref.append_description_key("entity");

  otlp::Resource common_resource;
  common_resource.append_attribute(kv);
  common_resource.append_entity_ref(entity_ref);

  otlp::NumberDataPoint point;
  point.set_time_unix_nano(1234);
  point.set_as_double(42.5);

  otlp::Gauge gauge;
  gauge.append_data_point(point);

  otlp::Metric metric;
  metric.set_name("cpu.usage");
  metric.set_gauge(gauge);

  otlp::ScopeMetrics metrics_scope;
  metrics_scope.set_scope(common_scope);
  metrics_scope.append_metric(metric);

  otlp::ResourceMetrics metrics_resource;
  metrics_resource.set_resource(common_resource);
  metrics_resource.append_scope_metrics(metrics_scope);

  otlp::MetricsData metrics_data;
  metrics_data.append_resource_metrics(metrics_resource);
  std::vector<std::uint8_t> encoded_metrics_data(metrics_data.encoded_size());
  check_equal(t,
              metrics_data.encode(encoded_metrics_data.data(),
                                  encoded_metrics_data.size()),
              OTLP_STATUS_OK, "metrics data encode");

  otlp::MetricsData decoded_metrics_data;
  check_equal(t,
              decoded_metrics_data.decode(encoded_metrics_data.data(),
                                          encoded_metrics_data.size()),
              OTLP_STATUS_OK, "metrics data decode");

  otlp::MetricsExportResponse metrics_response;
  metrics_response.set_partial_success(5, "metrics-partial");
  std::vector<std::uint8_t> encoded_metrics_response(
      metrics_response.encoded_size());
  check_equal(t,
              metrics_response.encode(encoded_metrics_response.data(),
                                      encoded_metrics_response.size()),
              OTLP_STATUS_OK, "metrics response encode");

  otlp::MetricsExportResponse decoded_metrics_response;
  check_equal(t,
              decoded_metrics_response.decode(encoded_metrics_response.data(),
                                              encoded_metrics_response.size()),
              OTLP_STATUS_OK, "metrics response decode");

  check_equal(t, decoded_metrics_data.get()->resource_metrics.len,
              static_cast<size_t>(1),
              "decoded metrics data resource count");
  check_equal(t, decoded_metrics_response.get()->has_partial_success, 1,
              "decoded metrics response partial success");
}

} // namespace

#endif /* OTLP_FORMAT_ENABLE_METRICS */

#endif /* __cplusplus */

void test_metrics_cpp(Tap &t) {
#if defined(__cplusplus) && (__cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900))
#if defined(OTLP_FORMAT_ENABLE_METRICS)
  t.test("cpp metrics wrappers", [](Tap &sub) {
    test_cpp_metrics_wrappers(sub);
  });
  t.test("cpp metrics copy independence", [](Tap &sub) {
    test_cpp_metrics_copy_independence(sub);
  });
  t.test("cpp metrics data and response", [](Tap &sub) {
    test_cpp_metrics_data_and_response(sub);
  });
#else
  (void)t;
#endif
#else
  (void)t;
#endif
}

int test_metrics_cpp_count(void) {
#if defined(OTLP_FORMAT_ENABLE_METRICS)
  return 3;
#else
  return 0;
#endif
}
