#include "test_helpers.h"

#if defined(OTLP_FORMAT_ENABLE_METRICS)

static void test_metrics_proto_wire_compatibility(void) {
  otlp_metric metric;
  otlp_number_data_point number_point;
  otlp_exemplar exemplar;
  otlp_histogram_data_point histogram_point;
  otlp_exponential_histogram_data_point exponential_point;
  otlp_exponential_histogram_buckets buckets;
  otlp_export_metrics_partial_success partial_success;
  otlp_export_metrics_service_response response;
  otlp__writer writer;
  uint8_t encoded_metric[128];
  uint8_t encoded_histogram_point[128];
  uint8_t encoded_exponential_point[128];
  uint8_t encoded_buckets[32];
  uint8_t encoded_response[64];
  const uint8_t exponential_histogram_field[] = { 0x52, 0x02, 0x10, 0x00 };
  const uint8_t legacy_exponential_histogram_field[] = { 0x5a, 0x02, 0x10, 0x00 };
  const uint8_t summary_field[] = { 0x5a, 0x00 };
  const uint8_t metadata_field[] = { 0x62, 0x00 };
  const uint8_t bucket_counts_packed[] = { 0x12, 0x03, 0x01, 0x82, 0x01 };
  const uint8_t histogram_bucket_counts_packed[] = {
    0x32, 0x10,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  const uint8_t histogram_bounds_packed[] = {
    0x3a, 0x10,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x3f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40
  };
  const uint8_t histogram_bucket_counts_unpacked[] = {
    0x31, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x31, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  const uint8_t histogram_bounds_unpacked[] = {
    0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x3f,
    0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40
  };
  const uint8_t bucket_counts_unpacked[] = { 0x10, 0x01, 0x10, 0x82, 0x01 };
  const uint8_t split_positive_buckets[] = {
    0x42, 0x02, 0x08, 0x01,
    0x42, 0x02, 0x10, 0x03
  };
  const uint8_t metric_oneof_last_wins[] = { 0x2a, 0x00, 0x3a, 0x00 };
  const uint8_t number_point_oneof_last_wins[] = {
    0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x40,
    0x31, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  const uint8_t exemplar_oneof_last_wins[] = {
    0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x40,
    0x31, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  const uint8_t exponential_scale_minus_one[] = { 0x30, 0x01 };
  const uint8_t histogram_sum_zero[] = {
    0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  const uint8_t histogram_min_zero[] = {
    0x59, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  const uint8_t histogram_max_zero[] = {
    0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  const uint8_t exponential_min_zero[] = {
    0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  const uint8_t exponential_max_zero[] = {
    0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  const uint8_t bucket_offset_minus_one[] = { 0x08, 0x01 };
  const uint8_t rejected_minus_one[] = {
    0x08, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01
  };
  size_t metric_size;
  size_t point_size;
  size_t buckets_size;
  size_t response_size;
  uint64_t bucket_count;
  double explicit_bound;

  CHECK(OTLP_DATA_POINT_FLAGS_DO_NOT_USE == 0U);
  CHECK(OTLP_DATA_POINT_FLAGS_NO_RECORDED_VALUE_MASK == 1U);

  otlp_metric_init(&metric);
  metric.data_kind = OTLP_METRIC_DATA_EXPONENTIAL_HISTOGRAM;
  otlp_exponential_histogram_init(&metric.data.exponential_histogram);
  metric_size = otlp_metric_encoded_size_inner(&metric);
  otlp__writer_init(&writer, encoded_metric, sizeof(encoded_metric));
  otlp_metric_encode_field(&writer, 1, &metric);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, metric_size));
  CHECK(otlp_test_contains_bytes(encoded_metric, writer.off, exponential_histogram_field, sizeof(exponential_histogram_field)));
  CHECK(!otlp_test_contains_bytes(encoded_metric, writer.off, legacy_exponential_histogram_field, sizeof(legacy_exponential_histogram_field)));
  otlp_metric_destroy(&metric);

  otlp_metric_init(&metric);
  metric.data_kind = OTLP_METRIC_DATA_SUMMARY;
  otlp_summary_init(&metric.data.summary);
  metric_size = otlp_metric_encoded_size_inner(&metric);
  otlp__writer_init(&writer, encoded_metric, sizeof(encoded_metric));
  otlp_metric_encode_field(&writer, 1, &metric);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(otlp_test_contains_bytes(encoded_metric, writer.off, summary_field, sizeof(summary_field)));
  otlp_metric_destroy(&metric);

  otlp_metric_init(&metric);
  {
    otlp_key_value metadata;
    otlp_key_value_init(&metadata);
    CHECK(otlp__append_primitive((void **)&metric.metadata.ptr,
                                  &metric.metadata.len,
                                  &metric.metadata.cap,
                                  sizeof(metadata),
                                  &metadata) == OTLP_STATUS_OK);
  }
  metric_size = otlp_metric_encoded_size_inner(&metric);
  otlp__writer_init(&writer, encoded_metric, sizeof(encoded_metric));
  otlp_metric_encode_field(&writer, 1, &metric);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(otlp_test_contains_bytes(encoded_metric, writer.off, metadata_field, sizeof(metadata_field)));
  otlp_metric_destroy(&metric);

  otlp_exponential_histogram_buckets_init(&buckets);
  buckets.offset = -1;
  bucket_count = 1;
  CHECK(otlp__append_primitive((void **)&buckets.bucket_counts.ptr,
                                &buckets.bucket_counts.len,
                                &buckets.bucket_counts.cap,
                                sizeof(bucket_count),
                                &bucket_count) == OTLP_STATUS_OK);
  bucket_count = 130;
  CHECK(otlp__append_primitive((void **)&buckets.bucket_counts.ptr,
                                &buckets.bucket_counts.len,
                                &buckets.bucket_counts.cap,
                                sizeof(bucket_count),
                                &bucket_count) == OTLP_STATUS_OK);
  buckets_size = otlp_exponential_histogram_buckets_encoded_size_inner(&buckets);
  otlp__writer_init(&writer, encoded_buckets, sizeof(encoded_buckets));
  otlp_exponential_histogram_buckets_encode_field(&writer, 1, &buckets);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, buckets_size));
  CHECK(otlp_test_contains_bytes(encoded_buckets, writer.off, bucket_counts_packed, sizeof(bucket_counts_packed)));
  CHECK(otlp_test_contains_bytes(encoded_buckets, writer.off, bucket_offset_minus_one, sizeof(bucket_offset_minus_one)));
  otlp_exponential_histogram_buckets_destroy(&buckets);

  otlp_exponential_histogram_data_point_init(&exponential_point);
  CHECK(otlp_exponential_histogram_data_point_decode_inner(
             &exponential_point, split_positive_buckets,
             sizeof(split_positive_buckets)) == OTLP_STATUS_OK);
  CHECK(exponential_point.has_positive == 1);
  CHECK(exponential_point.positive.offset == -1);
  CHECK(exponential_point.positive.bucket_counts.len == 1);
  CHECK(exponential_point.positive.bucket_counts.ptr[0] == 3);
  otlp_exponential_histogram_data_point_destroy(&exponential_point);

  otlp_histogram_data_point_init(&histogram_point);
  bucket_count = 1;
  CHECK(otlp__append_primitive((void **)&histogram_point.bucket_counts.ptr,
                                &histogram_point.bucket_counts.len,
                                &histogram_point.bucket_counts.cap,
                                sizeof(bucket_count),
                                &bucket_count) == OTLP_STATUS_OK);
  bucket_count = 2;
  CHECK(otlp__append_primitive((void **)&histogram_point.bucket_counts.ptr,
                                &histogram_point.bucket_counts.len,
                                &histogram_point.bucket_counts.cap,
                                sizeof(bucket_count),
                                &bucket_count) == OTLP_STATUS_OK);
  explicit_bound = 1.0;
  CHECK(otlp__append_primitive((void **)&histogram_point.explicit_bounds.ptr,
                                &histogram_point.explicit_bounds.len,
                                &histogram_point.explicit_bounds.cap,
                                sizeof(explicit_bound),
                                &explicit_bound) == OTLP_STATUS_OK);
  explicit_bound = 2.0;
  CHECK(otlp__append_primitive((void **)&histogram_point.explicit_bounds.ptr,
                                &histogram_point.explicit_bounds.len,
                                &histogram_point.explicit_bounds.cap,
                                sizeof(explicit_bound),
                                &explicit_bound) == OTLP_STATUS_OK);
  point_size = otlp_histogram_data_point_encoded_size_inner(&histogram_point);
  otlp__writer_init(&writer, encoded_histogram_point, sizeof(encoded_histogram_point));
  otlp_histogram_data_point_encode_field(&writer, 1, &histogram_point);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, point_size));
  CHECK(otlp_test_contains_bytes(encoded_histogram_point, writer.off, histogram_bucket_counts_packed, sizeof(histogram_bucket_counts_packed)));
  CHECK(otlp_test_contains_bytes(encoded_histogram_point, writer.off, histogram_bounds_packed, sizeof(histogram_bounds_packed)));
  otlp_histogram_data_point_destroy(&histogram_point);

  otlp_histogram_data_point_init(&histogram_point);
  CHECK(otlp_histogram_data_point_decode_inner(&histogram_point, histogram_bucket_counts_packed, sizeof(histogram_bucket_counts_packed)) == OTLP_STATUS_OK);
  CHECK(histogram_point.bucket_counts.len == 2);
  CHECK(histogram_point.bucket_counts.ptr[1] == 2);
  otlp_histogram_data_point_destroy(&histogram_point);

  otlp_histogram_data_point_init(&histogram_point);
  CHECK(otlp_histogram_data_point_decode_inner(&histogram_point, histogram_bucket_counts_unpacked, sizeof(histogram_bucket_counts_unpacked)) == OTLP_STATUS_OK);
  CHECK(histogram_point.bucket_counts.len == 2);
  CHECK(histogram_point.bucket_counts.ptr[1] == 2);
  otlp_histogram_data_point_destroy(&histogram_point);

  otlp_histogram_data_point_init(&histogram_point);
  CHECK(otlp_histogram_data_point_decode_inner(&histogram_point, histogram_bounds_unpacked, sizeof(histogram_bounds_unpacked)) == OTLP_STATUS_OK);
  CHECK(histogram_point.explicit_bounds.len == 2);
  CHECK(histogram_point.explicit_bounds.ptr[1] == 2.0);
  otlp_histogram_data_point_destroy(&histogram_point);

  otlp_exponential_histogram_buckets_init(&buckets);
  CHECK(otlp_exponential_histogram_buckets_decode_inner(&buckets, bucket_counts_unpacked, sizeof(bucket_counts_unpacked)) == OTLP_STATUS_OK);
  CHECK(buckets.bucket_counts.len == 2);
  CHECK(buckets.bucket_counts.ptr[1] == 130);
  otlp_exponential_histogram_buckets_destroy(&buckets);

  otlp_metric_init(&metric);
  CHECK(otlp_metric_decode_inner(&metric, metric_oneof_last_wins, sizeof(metric_oneof_last_wins)) == OTLP_STATUS_OK);
  CHECK(metric.data_kind == OTLP_METRIC_DATA_SUM);
  otlp_metric_destroy(&metric);

  otlp_number_data_point_init(&number_point);
  CHECK(otlp_number_data_point_decode_inner(&number_point, number_point_oneof_last_wins, sizeof(number_point_oneof_last_wins)) == OTLP_STATUS_OK);
  CHECK(number_point.value_kind == OTLP_NUMBER_VALUE_INT);
  CHECK(number_point.as_int == 7);
  otlp_number_data_point_destroy(&number_point);

  otlp_exemplar_init(&exemplar);
  CHECK(otlp_exemplar_decode_inner(&exemplar, exemplar_oneof_last_wins, sizeof(exemplar_oneof_last_wins)) == OTLP_STATUS_OK);
  CHECK(exemplar.value_kind == OTLP_EXEMPLAR_VALUE_INT);
  CHECK(exemplar.as_int == 7);
  otlp_exemplar_destroy(&exemplar);

  otlp_histogram_data_point_init(&histogram_point);
  histogram_point.has_sum = 1;
  histogram_point.has_min = 1;
  histogram_point.has_max = 1;
  point_size = otlp_histogram_data_point_encoded_size_inner(&histogram_point);
  otlp__writer_init(&writer, encoded_histogram_point, sizeof(encoded_histogram_point));
  otlp_histogram_data_point_encode_field(&writer, 1, &histogram_point);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, point_size));
  CHECK(otlp_test_contains_bytes(encoded_histogram_point, writer.off, histogram_sum_zero, sizeof(histogram_sum_zero)));
  CHECK(otlp_test_contains_bytes(encoded_histogram_point, writer.off, histogram_min_zero, sizeof(histogram_min_zero)));
  CHECK(otlp_test_contains_bytes(encoded_histogram_point, writer.off, histogram_max_zero, sizeof(histogram_max_zero)));
  otlp_histogram_data_point_destroy(&histogram_point);

  otlp_exponential_histogram_data_point_init(&exponential_point);
  exponential_point.scale = -1;
  point_size = otlp_exponential_histogram_data_point_encoded_size_inner(&exponential_point);
  otlp__writer_init(&writer, encoded_exponential_point, sizeof(encoded_exponential_point));
  otlp_exponential_histogram_data_point_encode_field(&writer, 1, &exponential_point);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, point_size));
  CHECK(otlp_test_contains_bytes(encoded_exponential_point, writer.off, exponential_scale_minus_one, sizeof(exponential_scale_minus_one)));
  otlp_exponential_histogram_data_point_destroy(&exponential_point);

  otlp_exponential_histogram_data_point_init(&exponential_point);
  exponential_point.has_sum = 1;
  exponential_point.has_min = 1;
  exponential_point.has_max = 1;
  point_size = otlp_exponential_histogram_data_point_encoded_size_inner(&exponential_point);
  otlp__writer_init(&writer, encoded_exponential_point, sizeof(encoded_exponential_point));
  otlp_exponential_histogram_data_point_encode_field(&writer, 1, &exponential_point);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, point_size));
  CHECK(otlp_test_contains_bytes(encoded_exponential_point, writer.off, histogram_sum_zero, sizeof(histogram_sum_zero)));
  CHECK(otlp_test_contains_bytes(encoded_exponential_point, writer.off, exponential_min_zero, sizeof(exponential_min_zero)));
  CHECK(otlp_test_contains_bytes(encoded_exponential_point, writer.off, exponential_max_zero, sizeof(exponential_max_zero)));
  otlp_exponential_histogram_data_point_destroy(&exponential_point);

  otlp_export_metrics_partial_success_init(&partial_success);
  partial_success.rejected_data_points = -1;
  response_size = otlp_export_metrics_partial_success_encoded_size_inner(&partial_success);
  otlp__writer_init(&writer, encoded_response, sizeof(encoded_response));
  otlp_export_metrics_partial_success_encode_field(&writer, 1, &partial_success);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, response_size));
  CHECK(otlp_test_contains_bytes(encoded_response, writer.off, rejected_minus_one, sizeof(rejected_minus_one)));

  otlp_export_metrics_service_response_init(&response);
  CHECK(otlp_export_metrics_service_response_decode(&response, encoded_response, writer.off) == OTLP_STATUS_OK);
  CHECK(response.has_partial_success == 1);
  CHECK(response.partial_success.rejected_data_points == -1);
  otlp_export_metrics_service_response_destroy(&response);
  otlp_export_metrics_partial_success_destroy(&partial_success);
  otlp_test_ok("metrics proto wire compatibility");
}

static void test_metrics_round_trip(void) {
  otlp_metric metric;
  otlp_number_data_point point;
  otlp_gauge gauge;
  otlp_scope_metrics scope_metrics;
  otlp_resource_metrics resource_metrics;
  otlp_export_metrics_service_request request;
  otlp_export_metrics_service_request decoded;
  size_t encoded_size;
  uint8_t *encoded;

  otlp_number_data_point_init(&point);
  point.value_kind = OTLP_NUMBER_VALUE_DOUBLE;
  point.as_double = 42.5;
  point.time_unix_nano = 1234;

  otlp_gauge_init(&gauge);
  CHECK(otlp__append_primitive((void **) &gauge.data_points.ptr, &gauge.data_points.len, &gauge.data_points.cap, sizeof(point), &point) == OTLP_STATUS_OK);
  otlp_number_data_point_init(&point);

  otlp_metric_init(&metric);
  CHECK(otlp__set_string(&metric.name, "cpu.usage", 9) == OTLP_STATUS_OK);
  metric.data_kind = OTLP_METRIC_DATA_GAUGE;
  CHECK(otlp_gauge_copy(&metric.data.gauge, &gauge) == OTLP_STATUS_OK);

  otlp_scope_metrics_init(&scope_metrics);
  CHECK(otlp_scope_metrics_append_metric(&scope_metrics, &metric) == OTLP_STATUS_OK);
  otlp_resource_metrics_init(&resource_metrics);
  CHECK(otlp_resource_metrics_append_scope_metrics(&resource_metrics, &scope_metrics) == OTLP_STATUS_OK);
  otlp_export_metrics_service_request_init(&request);
  CHECK(otlp_export_metrics_service_request_append_resource_metrics(&request, &resource_metrics) == OTLP_STATUS_OK);

  encoded_size = otlp_export_metrics_service_request_encoded_size(&request);
  encoded = (uint8_t *) malloc(encoded_size);
  CHECK(encoded != NULL);
  CHECK(otlp_export_metrics_service_request_encode(&request, encoded, encoded_size) == OTLP_STATUS_OK);

  otlp_export_metrics_service_request_init(&decoded);
  CHECK(otlp_export_metrics_service_request_decode(&decoded, encoded, encoded_size) == OTLP_STATUS_OK);
  CHECK(decoded.resource_metrics.len == 1);
  CHECK(decoded.resource_metrics.ptr[0].scope_metrics.len == 1);
  CHECK(decoded.resource_metrics.ptr[0].scope_metrics.ptr[0].metrics.len == 1);
  CHECK(decoded.resource_metrics.ptr[0].scope_metrics.ptr[0].metrics.ptr[0].data_kind == OTLP_METRIC_DATA_GAUGE);
  CHECK(decoded.resource_metrics.ptr[0].scope_metrics.ptr[0].metrics.ptr[0].data.gauge.data_points.len == 1);

  otlp_export_metrics_service_request_destroy(&decoded);
  otlp_export_metrics_service_request_destroy(&request);
  otlp_resource_metrics_destroy(&resource_metrics);
  otlp_scope_metrics_destroy(&scope_metrics);
  otlp_metric_destroy(&metric);
  otlp_gauge_destroy(&gauge);
  otlp_number_data_point_destroy(&point);
  free(encoded);
  otlp_test_ok("metrics round trip");
}

static void test_metrics_all_variants_and_unknown(void) {
  otlp_metric metric;
  otlp_scope_metrics scope_metrics;
  otlp_resource_metrics resource_metrics;
  otlp_export_metrics_service_request request;
  otlp_export_metrics_service_request decoded;
  otlp_any_value attr_value;
  otlp_key_value metadata;
  size_t encoded_size;
  size_t extended_size;
  uint8_t *encoded;
  uint8_t *extended;

  otlp_scope_metrics_init(&scope_metrics);
  CHECK(otlp__set_string(&scope_metrics.schema_url, "metric-scope", 12) == OTLP_STATUS_OK);
  otlp_resource_metrics_init(&resource_metrics);
  CHECK(otlp__set_string(&resource_metrics.schema_url, "metric-resource", 15) == OTLP_STATUS_OK);
  otlp_export_metrics_service_request_init(&request);

  CHECK(otlp_any_value_init_string(&attr_value, "meta", 4) == OTLP_STATUS_OK);
  otlp_key_value_init(&metadata);
  CHECK(otlp__set_string(&metadata.key, "env", 3) == OTLP_STATUS_OK);
  CHECK(otlp_any_value_copy(&metadata.value, &attr_value) == OTLP_STATUS_OK);
  metadata.has_value = 1;

  otlp_metric_init(&metric);
  CHECK(otlp__set_string(&metric.name, "sum.metric", 10) == OTLP_STATUS_OK);
  CHECK(otlp__set_string(&metric.description, "sum desc", 8) == OTLP_STATUS_OK);
  CHECK(otlp__set_string(&metric.unit, "1", 1) == OTLP_STATUS_OK);
  metric.data_kind = OTLP_METRIC_DATA_SUM;
  otlp_sum_init(&metric.data.sum);
  metric.data.sum.aggregation_temporality = OTLP_AGGREGATION_TEMPORALITY_CUMULATIVE;
  metric.data.sum.is_monotonic = 1;
  {
    otlp_number_data_point point;
    otlp_exemplar exemplar;
    uint8_t trace_id[16] = { 1 };
    uint8_t span_id[8] = { 2 };
    otlp_number_data_point_init(&point);
    point.value_kind = OTLP_NUMBER_VALUE_INT;
    point.as_int = 42;
    point.flags = 5;
    otlp_exemplar_init(&exemplar);
    exemplar.value_kind = OTLP_EXEMPLAR_VALUE_INT;
    exemplar.as_int = 9;
    exemplar.time_unix_nano = 77;
    CHECK(otlp__set_bytes(&exemplar.trace_id, trace_id, sizeof(trace_id)) == OTLP_STATUS_OK);
    CHECK(otlp__set_bytes(&exemplar.span_id, span_id, sizeof(span_id)) == OTLP_STATUS_OK);
    CHECK(otlp__append_primitive((void **) &exemplar.filtered_attributes.ptr, &exemplar.filtered_attributes.len, &exemplar.filtered_attributes.cap, sizeof(metadata), &metadata) == OTLP_STATUS_OK);
    metadata.key.ptr = NULL;
    metadata.key.len = 0;
    otlp_any_value_init(&metadata.value);
    CHECK(otlp__append_primitive((void **) &point.exemplars.ptr, &point.exemplars.len, &point.exemplars.cap, sizeof(exemplar), &exemplar) == OTLP_STATUS_OK);
    otlp_exemplar_init(&exemplar);
    CHECK(otlp__append_primitive((void **) &metric.data.sum.data_points.ptr, &metric.data.sum.data_points.len, &metric.data.sum.data_points.cap, sizeof(point), &point) == OTLP_STATUS_OK);
    otlp_number_data_point_init(&point);
  }
  CHECK(otlp_scope_metrics_append_metric(&scope_metrics, &metric) == OTLP_STATUS_OK);
  otlp_metric_destroy(&metric);

  otlp_metric_init(&metric);
  CHECK(otlp__set_string(&metric.name, "hist.metric", 11) == OTLP_STATUS_OK);
  metric.data_kind = OTLP_METRIC_DATA_HISTOGRAM;
  otlp_histogram_init(&metric.data.histogram);
  metric.data.histogram.aggregation_temporality = OTLP_AGGREGATION_TEMPORALITY_DELTA;
  {
    otlp_histogram_data_point point;
    uint64_t bucket = 3;
    double bound = 1.5;
    otlp_histogram_data_point_init(&point);
    point.count = 4;
    point.has_sum = 1;
    point.sum = 9.5;
    point.has_min = 1;
    point.min = 1.0;
    point.has_max = 1;
    point.max = 3.0;
    CHECK(otlp__append_primitive((void **) &point.bucket_counts.ptr, &point.bucket_counts.len, &point.bucket_counts.cap, sizeof(bucket), &bucket) == OTLP_STATUS_OK);
    CHECK(otlp__append_primitive((void **) &point.explicit_bounds.ptr, &point.explicit_bounds.len, &point.explicit_bounds.cap, sizeof(bound), &bound) == OTLP_STATUS_OK);
    CHECK(otlp__append_primitive((void **) &metric.data.histogram.data_points.ptr, &metric.data.histogram.data_points.len, &metric.data.histogram.data_points.cap, sizeof(point), &point) == OTLP_STATUS_OK);
    otlp_histogram_data_point_init(&point);
  }
  CHECK(otlp_scope_metrics_append_metric(&scope_metrics, &metric) == OTLP_STATUS_OK);
  otlp_metric_destroy(&metric);

  otlp_metric_init(&metric);
  CHECK(otlp__set_string(&metric.name, "exp.metric", 10) == OTLP_STATUS_OK);
  metric.data_kind = OTLP_METRIC_DATA_EXPONENTIAL_HISTOGRAM;
  otlp_exponential_histogram_init(&metric.data.exponential_histogram);
  metric.data.exponential_histogram.aggregation_temporality = OTLP_AGGREGATION_TEMPORALITY_DELTA;
  {
    otlp_exponential_histogram_data_point point;
    uint64_t bucket = 7;
    otlp_exponential_histogram_data_point_init(&point);
    point.count = 8;
    point.scale = -1;
    point.zero_count = 2;
    point.zero_threshold = 0.25;
    point.has_sum = 1;
    point.sum = 12.0;
    point.has_positive = 1;
    point.positive.offset = 1;
    point.has_negative = 1;
    point.negative.offset = -1;
    point.has_min = 1;
    point.min = 0.1;
    point.has_max = 1;
    point.max = 5.0;
    CHECK(otlp__append_primitive((void **) &point.positive.bucket_counts.ptr, &point.positive.bucket_counts.len, &point.positive.bucket_counts.cap, sizeof(bucket), &bucket) == OTLP_STATUS_OK);
    CHECK(otlp__append_primitive((void **) &point.negative.bucket_counts.ptr, &point.negative.bucket_counts.len, &point.negative.bucket_counts.cap, sizeof(bucket), &bucket) == OTLP_STATUS_OK);
    CHECK(otlp__append_primitive((void **) &metric.data.exponential_histogram.data_points.ptr, &metric.data.exponential_histogram.data_points.len, &metric.data.exponential_histogram.data_points.cap, sizeof(point), &point) == OTLP_STATUS_OK);
    otlp_exponential_histogram_data_point_init(&point);
  }
  CHECK(otlp_scope_metrics_append_metric(&scope_metrics, &metric) == OTLP_STATUS_OK);
  otlp_metric_destroy(&metric);

  otlp_metric_init(&metric);
  CHECK(otlp__set_string(&metric.name, "summary.metric", 14) == OTLP_STATUS_OK);
  metric.data_kind = OTLP_METRIC_DATA_SUMMARY;
  otlp_summary_init(&metric.data.summary);
  {
    otlp_summary_data_point point;
    otlp_summary_data_point_value_at_quantile quantile;
    otlp_summary_data_point_init(&point);
    point.count = 5;
    point.sum = 6.0;
    point.flags = 9;
    otlp_summary_data_point_value_at_quantile_init(&quantile);
    quantile.quantile = 0.5;
    quantile.value = 2.0;
    CHECK(otlp__append_primitive((void **) &point.quantile_values.ptr, &point.quantile_values.len, &point.quantile_values.cap, sizeof(quantile), &quantile) == OTLP_STATUS_OK);
    quantile.quantile = 0.9;
    quantile.value = 4.0;
    CHECK(otlp__append_primitive((void **) &point.quantile_values.ptr, &point.quantile_values.len, &point.quantile_values.cap, sizeof(quantile), &quantile) == OTLP_STATUS_OK);
    CHECK(otlp__append_primitive((void **) &metric.data.summary.data_points.ptr, &metric.data.summary.data_points.len, &metric.data.summary.data_points.cap, sizeof(point), &point) == OTLP_STATUS_OK);
    otlp_summary_data_point_init(&point);
  }
  CHECK(otlp_scope_metrics_append_metric(&scope_metrics, &metric) == OTLP_STATUS_OK);
  otlp_metric_destroy(&metric);

  CHECK(otlp_resource_metrics_append_scope_metrics(&resource_metrics, &scope_metrics) == OTLP_STATUS_OK);
  CHECK(otlp_export_metrics_service_request_append_resource_metrics(&request, &resource_metrics) == OTLP_STATUS_OK);

  encoded_size = otlp_export_metrics_service_request_encoded_size(&request);
  encoded = (uint8_t *) malloc(encoded_size);
  CHECK(encoded != NULL);
  CHECK(otlp_export_metrics_service_request_encode(&request, encoded, encoded_size) == OTLP_STATUS_OK);
  CHECK(
    otlp_export_metrics_service_request_encode(&request, encoded, encoded_size - 1) ==
    OTLP_STATUS_BUFFER_TOO_SMALL
  );

  extended = otlp_test_append_unknown_varint_field(encoded, encoded_size, 66, 2, &extended_size);
  otlp_export_metrics_service_request_init(&decoded);
  CHECK(otlp_export_metrics_service_request_decode(&decoded, extended, extended_size) == OTLP_STATUS_OK);
  CHECK(decoded.resource_metrics.len == 1);
  CHECK(decoded.resource_metrics.ptr[0].schema_url.len == 15);
  CHECK(decoded.resource_metrics.ptr[0].scope_metrics.ptr[0].schema_url.len == 12);
  CHECK(decoded.resource_metrics.ptr[0].scope_metrics.ptr[0].metrics.len == 4);
  CHECK(decoded.resource_metrics.ptr[0].scope_metrics.ptr[0].metrics.ptr[0].data_kind == OTLP_METRIC_DATA_SUM);
  CHECK(decoded.resource_metrics.ptr[0].scope_metrics.ptr[0].metrics.ptr[0].data.sum.data_points.ptr[0].exemplars.len == 1);
  CHECK(decoded.resource_metrics.ptr[0].scope_metrics.ptr[0].metrics.ptr[1].data_kind == OTLP_METRIC_DATA_HISTOGRAM);
  CHECK(decoded.resource_metrics.ptr[0].scope_metrics.ptr[0].metrics.ptr[1].data.histogram.data_points.ptr[0].bucket_counts.len == 1);
  CHECK(decoded.resource_metrics.ptr[0].scope_metrics.ptr[0].metrics.ptr[2].data_kind == OTLP_METRIC_DATA_EXPONENTIAL_HISTOGRAM);
  CHECK(decoded.resource_metrics.ptr[0].scope_metrics.ptr[0].metrics.ptr[2].data.exponential_histogram.data_points.ptr[0].has_positive == 1);
  CHECK(decoded.resource_metrics.ptr[0].scope_metrics.ptr[0].metrics.ptr[3].data_kind == OTLP_METRIC_DATA_SUMMARY);
  CHECK(decoded.resource_metrics.ptr[0].scope_metrics.ptr[0].metrics.ptr[3].data.summary.data_points.ptr[0].quantile_values.len == 2);

  otlp_export_metrics_service_request_destroy(&decoded);
  free(extended);
  free(encoded);
  otlp_export_metrics_service_request_destroy(&request);
  otlp_resource_metrics_destroy(&resource_metrics);
  otlp_scope_metrics_destroy(&scope_metrics);
  otlp_key_value_destroy(&metadata);
  otlp_any_value_destroy(&attr_value);
  otlp_test_ok("metrics all variants and unknown");
}

static void test_metrics_data_and_response_round_trip(void) {
  otlp_metric metric;
  otlp_scope_metrics scope_metrics;
  otlp_resource_metrics resource_metrics;
  otlp_metrics_data data;
  otlp_metrics_data decoded_data;
  otlp_export_metrics_service_response response;
  otlp_export_metrics_service_response decoded_response;
  size_t data_size;
  size_t response_size;
  uint8_t *data_bytes;
  uint8_t *response_bytes;

  otlp_metric_init(&metric);
  CHECK(otlp__set_string(&metric.name, "g", 1) == OTLP_STATUS_OK);
  metric.data_kind = OTLP_METRIC_DATA_GAUGE;
  otlp_gauge_init(&metric.data.gauge);
  {
    otlp_number_data_point point;
    otlp_number_data_point_init(&point);
    point.value_kind = OTLP_NUMBER_VALUE_INT;
    point.as_int = 11;
    CHECK(otlp__append_primitive((void **)&metric.data.gauge.data_points.ptr,
                                  &metric.data.gauge.data_points.len,
                                  &metric.data.gauge.data_points.cap,
                                  sizeof(point), &point) == OTLP_STATUS_OK);
    otlp_number_data_point_init(&point);
  }

  otlp_scope_metrics_init(&scope_metrics);
  scope_metrics.has_scope = 1;
  CHECK(otlp__set_string(&scope_metrics.scope.name, "metrics", 7) == OTLP_STATUS_OK);
  CHECK(otlp_scope_metrics_append_metric(&scope_metrics, &metric) == OTLP_STATUS_OK);

  otlp_resource_metrics_init(&resource_metrics);
  resource_metrics.has_resource = 1;
  CHECK(otlp_resource_metrics_append_scope_metrics(&resource_metrics, &scope_metrics) == OTLP_STATUS_OK);

  otlp_metrics_data_init(&data);
  CHECK(otlp__append_primitive((void **)&data.resource_metrics.ptr,
                                &data.resource_metrics.len,
                                &data.resource_metrics.cap,
                                sizeof(resource_metrics),
                                &resource_metrics) == OTLP_STATUS_OK);
  otlp_resource_metrics_init(&resource_metrics);
  data_size = otlp_metrics_data_encoded_size(&data);
  data_bytes = (uint8_t *)malloc(data_size);
  CHECK(data_bytes != NULL);
  CHECK(otlp_metrics_data_encode(&data, data_bytes, data_size) == OTLP_STATUS_OK);
  otlp_metrics_data_init(&decoded_data);
  CHECK(otlp_metrics_data_decode(&decoded_data, data_bytes, data_size) == OTLP_STATUS_OK);
  CHECK(decoded_data.resource_metrics.len == 1);
  CHECK(decoded_data.resource_metrics.ptr[0].has_resource == 1);
  CHECK(decoded_data.resource_metrics.ptr[0].scope_metrics.ptr[0].has_scope == 1);

  otlp_export_metrics_service_response_init(&response);
  response.has_partial_success = 1;
  response.partial_success.rejected_data_points = 3;
  CHECK(otlp__set_string(&response.partial_success.error_message, "metric-partial", 14) == OTLP_STATUS_OK);
  response_size = otlp_export_metrics_service_response_encoded_size(&response);
  response_bytes = (uint8_t *)malloc(response_size);
  CHECK(response_bytes != NULL);
  CHECK(otlp_export_metrics_service_response_encode(&response, response_bytes, response_size) == OTLP_STATUS_OK);
  otlp_export_metrics_service_response_init(&decoded_response);
  CHECK(otlp_export_metrics_service_response_decode(&decoded_response, response_bytes, response_size) == OTLP_STATUS_OK);
  CHECK(decoded_response.has_partial_success == 1);
  CHECK(decoded_response.partial_success.rejected_data_points == 3);

  otlp_export_metrics_service_response_destroy(&decoded_response);
  otlp_export_metrics_service_response_destroy(&response);
  otlp_metrics_data_destroy(&decoded_data);
  otlp_metrics_data_destroy(&data);
  otlp_resource_metrics_destroy(&resource_metrics);
  otlp_scope_metrics_destroy(&scope_metrics);
  otlp_metric_destroy(&metric);
  free(data_bytes);
  free(response_bytes);
  otlp_test_ok("metrics data and response round trip");
}

OTLP_DEFINE_SUBTEST(test_metrics_proto_wire_compatibility)
OTLP_DEFINE_SUBTEST(test_metrics_round_trip)
OTLP_DEFINE_SUBTEST(test_metrics_all_variants_and_unknown)
OTLP_DEFINE_SUBTEST(test_metrics_data_and_response_round_trip)

#endif /* OTLP_FORMAT_ENABLE_METRICS */

int test_metrics_count(void) {
#if defined(OTLP_FORMAT_ENABLE_METRICS)
  return 4;
#else
  return 0;
#endif
}

void test_metrics_run(tap_t *t) {
#if defined(OTLP_FORMAT_ENABLE_METRICS)
  tap_test(t, "metrics proto wire compatibility", test_metrics_proto_wire_compatibility_subtest, NULL);
  tap_test(t, "metrics round trip", test_metrics_round_trip_subtest, NULL);
  tap_test(t, "metrics all variants and unknown", test_metrics_all_variants_and_unknown_subtest, NULL);
  tap_test(t, "metrics data and response round trip", test_metrics_data_and_response_round_trip_subtest, NULL);
#else
  (void)t;
#endif
}
