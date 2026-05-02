#ifndef OTLP_FORMAT_METRICS_H
#define OTLP_FORMAT_METRICS_H

#include "../common/common.h"

#if defined(OTLP_FORMAT_ENABLE_METRICS)
typedef struct otlp_exemplar otlp_exemplar;
typedef struct otlp_number_data_point otlp_number_data_point;
typedef struct otlp_histogram_data_point otlp_histogram_data_point;
typedef struct otlp_exponential_histogram_buckets otlp_exponential_histogram_buckets;
typedef struct otlp_exponential_histogram_data_point otlp_exponential_histogram_data_point;
typedef struct otlp_summary_data_point_value_at_quantile otlp_summary_data_point_value_at_quantile;
typedef struct otlp_summary_data_point otlp_summary_data_point;
typedef struct otlp_gauge otlp_gauge;
typedef struct otlp_sum otlp_sum;
typedef struct otlp_histogram otlp_histogram;
typedef struct otlp_exponential_histogram otlp_exponential_histogram;
typedef struct otlp_summary otlp_summary;
typedef struct otlp_metric otlp_metric;
typedef struct otlp_scope_metrics otlp_scope_metrics;
typedef struct otlp_resource_metrics otlp_resource_metrics;
typedef struct otlp_metrics_data otlp_metrics_data;
typedef struct otlp_export_metrics_service_request otlp_export_metrics_service_request;
typedef struct otlp_export_metrics_partial_success otlp_export_metrics_partial_success;
typedef struct otlp_export_metrics_service_response otlp_export_metrics_service_response;
OTLP_DECLARE_ARRAY(otlp_exemplar_array, otlp_exemplar);
OTLP_DECLARE_ARRAY(otlp_number_data_point_array, otlp_number_data_point);
OTLP_DECLARE_ARRAY(otlp_histogram_data_point_array, otlp_histogram_data_point);
OTLP_DECLARE_ARRAY(otlp_exponential_histogram_data_point_array, otlp_exponential_histogram_data_point);
OTLP_DECLARE_ARRAY(otlp_summary_data_point_value_at_quantile_array, otlp_summary_data_point_value_at_quantile);
OTLP_DECLARE_ARRAY(otlp_summary_data_point_array, otlp_summary_data_point);
OTLP_DECLARE_ARRAY(otlp_metric_array, otlp_metric);
OTLP_DECLARE_ARRAY(otlp_scope_metrics_array, otlp_scope_metrics);
OTLP_DECLARE_ARRAY(otlp_resource_metrics_array, otlp_resource_metrics);

#define OTLP_DATA_POINT_FLAGS_DO_NOT_USE 0x00000000U
#define OTLP_DATA_POINT_FLAGS_NO_RECORDED_VALUE_MASK 0x00000001U

typedef enum otlp_exemplar_value_kind_e {
  OTLP_EXEMPLAR_VALUE_NONE = 0,
  OTLP_EXEMPLAR_VALUE_DOUBLE = 1,
  OTLP_EXEMPLAR_VALUE_INT = 2
} otlp_exemplar_value_kind;

typedef enum otlp_number_value_kind_e {
  OTLP_NUMBER_VALUE_NONE = 0,
  OTLP_NUMBER_VALUE_DOUBLE = 1,
  OTLP_NUMBER_VALUE_INT = 2
} otlp_number_value_kind;

typedef enum otlp_aggregation_temporality_e {
  OTLP_AGGREGATION_TEMPORALITY_UNSPECIFIED = 0,
  OTLP_AGGREGATION_TEMPORALITY_DELTA = 1,
  OTLP_AGGREGATION_TEMPORALITY_CUMULATIVE = 2
} otlp_aggregation_temporality;

typedef enum otlp_metric_data_kind_e {
  OTLP_METRIC_DATA_NONE = 0,
  OTLP_METRIC_DATA_GAUGE = 1,
  OTLP_METRIC_DATA_SUM = 2,
  OTLP_METRIC_DATA_HISTOGRAM = 3,
  OTLP_METRIC_DATA_EXPONENTIAL_HISTOGRAM = 4,
  OTLP_METRIC_DATA_SUMMARY = 5
} otlp_metric_data_kind;

struct otlp_exemplar {
  uint64_t time_unix_nano;
  otlp_exemplar_value_kind value_kind;
  double as_double;
  int64_t as_int;
  otlp_bytes span_id;
  otlp_bytes trace_id;
  otlp_key_value_array filtered_attributes;
};

struct otlp_number_data_point {
  otlp_key_value_array attributes;
  uint64_t start_time_unix_nano;
  uint64_t time_unix_nano;
  otlp_number_value_kind value_kind;
  double as_double;
  int64_t as_int;
  otlp_exemplar_array exemplars;
  uint32_t flags;
};

struct otlp_histogram_data_point {
  otlp_key_value_array attributes;
  uint64_t start_time_unix_nano;
  uint64_t time_unix_nano;
  uint64_t count;
  int has_sum;
  double sum;
  otlp_uint64_array bucket_counts;
  otlp_double_array explicit_bounds;
  otlp_exemplar_array exemplars;
  uint32_t flags;
  int has_min;
  double min;
  int has_max;
  double max;
};

struct otlp_exponential_histogram_buckets {
  int32_t offset;
  otlp_uint64_array bucket_counts;
};

struct otlp_exponential_histogram_data_point {
  otlp_key_value_array attributes;
  uint64_t start_time_unix_nano;
  uint64_t time_unix_nano;
  uint64_t count;
  int has_sum;
  double sum;
  int32_t scale;
  uint64_t zero_count;
  double zero_threshold;
  int has_positive;
  otlp_exponential_histogram_buckets positive;
  int has_negative;
  otlp_exponential_histogram_buckets negative;
  otlp_exemplar_array exemplars;
  uint32_t flags;
  int has_min;
  double min;
  int has_max;
  double max;
};

struct otlp_summary_data_point_value_at_quantile {
  double quantile;
  double value;
};

struct otlp_summary_data_point {
  otlp_key_value_array attributes;
  uint64_t start_time_unix_nano;
  uint64_t time_unix_nano;
  uint64_t count;
  double sum;
  otlp_summary_data_point_value_at_quantile_array quantile_values;
  uint32_t flags;
};

struct otlp_gauge {
  otlp_number_data_point_array data_points;
};

struct otlp_sum {
  otlp_number_data_point_array data_points;
  otlp_aggregation_temporality aggregation_temporality;
  int is_monotonic;
};

struct otlp_histogram {
  otlp_histogram_data_point_array data_points;
  otlp_aggregation_temporality aggregation_temporality;
};

struct otlp_exponential_histogram {
  otlp_exponential_histogram_data_point_array data_points;
  otlp_aggregation_temporality aggregation_temporality;
};

struct otlp_summary {
  otlp_summary_data_point_array data_points;
};

struct otlp_metric {
  otlp_string name;
  otlp_string description;
  otlp_string unit;
  otlp_metric_data_kind data_kind;
  union {
    otlp_gauge gauge;
    otlp_sum sum;
    otlp_histogram histogram;
    otlp_exponential_histogram exponential_histogram;
    otlp_summary summary;
  } data;
  otlp_key_value_array metadata;
};

static inline void otlp_metric_init(otlp_metric *metric);
static inline void otlp_metric_destroy(otlp_metric *metric);
static inline otlp_status otlp_metric_copy(otlp_metric *dst, const otlp_metric *src);

struct otlp_resource_metrics {
  int has_resource;
  otlp_resource resource;
  otlp_scope_metrics_array scope_metrics;
  otlp_string schema_url;
};

struct otlp_metrics_data {
  otlp_resource_metrics_array resource_metrics;
};

struct otlp_export_metrics_service_request {
  otlp_resource_metrics_array resource_metrics;
};

struct otlp_export_metrics_partial_success {
  int64_t rejected_data_points;
  otlp_string error_message;
};

struct otlp_export_metrics_service_response {
  int has_partial_success;
  otlp_export_metrics_partial_success partial_success;
};

static inline void otlp_resource_metrics_init(otlp_resource_metrics *resource_metrics);
static inline void otlp_resource_metrics_destroy(otlp_resource_metrics *resource_metrics);
static inline otlp_status otlp_resource_metrics_append_scope_metrics(otlp_resource_metrics *resource_metrics, const otlp_scope_metrics *scope_metrics);

struct otlp_scope_metrics {
  int has_scope;
  otlp_instrumentation_scope scope;
  otlp_metric_array metrics;
  otlp_string schema_url;
};

static inline void otlp_scope_metrics_init(otlp_scope_metrics *scope_metrics);
static inline void otlp_scope_metrics_destroy(otlp_scope_metrics *scope_metrics);
static inline otlp_status otlp_scope_metrics_append_metric(otlp_scope_metrics *scope_metrics, const otlp_metric *metric);

static inline void otlp_export_metrics_service_request_init(otlp_export_metrics_service_request *request);
static inline void otlp_export_metrics_service_request_destroy(otlp_export_metrics_service_request *request);
static inline otlp_status otlp_export_metrics_service_request_append_resource_metrics(otlp_export_metrics_service_request *request, const otlp_resource_metrics *resource_metrics);
static inline size_t otlp_export_metrics_service_request_encoded_size(const otlp_export_metrics_service_request *request);
static inline otlp_status otlp_export_metrics_service_request_encode(const otlp_export_metrics_service_request *request, uint8_t *out, size_t len);
static inline otlp_status otlp_export_metrics_service_request_decode(otlp_export_metrics_service_request *request, const uint8_t *data, size_t len);
static inline void otlp_metrics_data_init(otlp_metrics_data *data);
static inline void otlp_metrics_data_destroy(otlp_metrics_data *data);
static inline size_t otlp_metrics_data_encoded_size(const otlp_metrics_data *data);
static inline otlp_status otlp_metrics_data_encode(const otlp_metrics_data *data, uint8_t *out, size_t len);
static inline otlp_status otlp_metrics_data_decode(otlp_metrics_data *data, const uint8_t *bytes, size_t len);
static inline void otlp_export_metrics_service_response_init(otlp_export_metrics_service_response *response);
static inline void otlp_export_metrics_service_response_destroy(otlp_export_metrics_service_response *response);
static inline size_t otlp_export_metrics_service_response_encoded_size(const otlp_export_metrics_service_response *response);
static inline otlp_status otlp_export_metrics_service_response_encode(const otlp_export_metrics_service_response *response, uint8_t *out, size_t len);
static inline otlp_status otlp_export_metrics_service_response_decode(otlp_export_metrics_service_response *response, const uint8_t *data, size_t len);

static inline size_t otlp__packed_fixed64_size(size_t len) {
  return len * 8U;
}

static inline void otlp__write_packed_uint64_fixed64_field(
    otlp__writer *writer, uint32_t field_number, const otlp_uint64_array *values) {
  size_t i;
  if (values->len == 0)
    return;
  otlp__write_tag(writer, field_number, 2);
  otlp__write_varint(writer, (uint64_t)otlp__packed_fixed64_size(values->len));
  for (i = 0; i < values->len; ++i)
    otlp__write_fixed64(writer, values->ptr[i]);
}

static inline void otlp__write_packed_double_fixed64_field(
    otlp__writer *writer, uint32_t field_number, const otlp_double_array *values) {
  size_t i;
  if (values->len == 0)
    return;
  otlp__write_tag(writer, field_number, 2);
  otlp__write_varint(writer, (uint64_t)otlp__packed_fixed64_size(values->len));
  for (i = 0; i < values->len; ++i) {
    union {
      double d;
      uint64_t u;
    } bits;
    bits.d = values->ptr[i];
    otlp__write_fixed64(writer, bits.u);
  }
}

static inline otlp_status otlp__append_packed_uint64_fixed64(
    otlp_uint64_array *dst, const uint8_t *data, size_t len) {
  otlp__reader reader;
  if ((len % 8U) != 0)
    return OTLP_STATUS_DECODE_ERROR;
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t value = otlp__read_fixed64(&reader);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if (otlp__append_primitive((void **)&dst->ptr, &dst->len, &dst->cap,
                               sizeof(value), &value) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  }
  return reader.status;
}

static inline otlp_status otlp__append_packed_double_fixed64(
    otlp_double_array *dst, const uint8_t *data, size_t len) {
  otlp__reader reader;
  if ((len % 8U) != 0)
    return OTLP_STATUS_DECODE_ERROR;
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    union {
      double d;
      uint64_t u;
    } bits;
    bits.u = otlp__read_fixed64(&reader);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if (otlp__append_primitive((void **)&dst->ptr, &dst->len, &dst->cap,
                               sizeof(bits.d), &bits.d) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  }
  return reader.status;
}

static inline void otlp_exemplar_init(otlp_exemplar *exemplar) {
  otlp__zero(exemplar, sizeof(*exemplar));
}

static inline void otlp_exemplar_destroy(otlp_exemplar *exemplar) {
  size_t i;
  free(exemplar->span_id.ptr);
  free(exemplar->trace_id.ptr);
  for (i = 0; i < exemplar->filtered_attributes.len; ++i)
    otlp_key_value_destroy(&exemplar->filtered_attributes.ptr[i]);
  free(exemplar->filtered_attributes.ptr);
  otlp_exemplar_init(exemplar);
}

static inline otlp_status otlp_exemplar_copy(otlp_exemplar *dst,
                                             const otlp_exemplar *src) {
  size_t i;
  otlp_exemplar_init(dst);
  dst->time_unix_nano = src->time_unix_nano;
  dst->value_kind = src->value_kind;
  dst->as_double = src->as_double;
  dst->as_int = src->as_int;
  if (otlp__set_bytes(&dst->span_id, src->span_id.ptr, src->span_id.len) !=
      OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  if (otlp__set_bytes(&dst->trace_id, src->trace_id.ptr, src->trace_id.len) !=
      OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  for (i = 0; i < src->filtered_attributes.len; ++i) {
    otlp_key_value copy;
    otlp_status status =
        otlp_key_value_copy(&copy, &src->filtered_attributes.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive(
        (void **)&dst->filtered_attributes.ptr, &dst->filtered_attributes.len,
        &dst->filtered_attributes.cap, sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}

static inline size_t
otlp_exemplar_encoded_size_inner(const otlp_exemplar *exemplar) {
  size_t size = 0;
  size_t i;
  if (exemplar->time_unix_nano)
    size += otlp__fixed64_field_size(2);
  if (exemplar->value_kind == OTLP_EXEMPLAR_VALUE_DOUBLE)
    size += otlp__fixed64_field_size(3);
  if (exemplar->span_id.len)
    size += otlp__bytes_field_size(4, exemplar->span_id.len);
  if (exemplar->trace_id.len)
    size += otlp__bytes_field_size(5, exemplar->trace_id.len);
  if (exemplar->value_kind == OTLP_EXEMPLAR_VALUE_INT)
    size += otlp__fixed64_field_size(6);
  for (i = 0; i < exemplar->filtered_attributes.len; ++i)
    size +=
        otlp__message_field_size(7, otlp_key_value_encoded_size_inner(
                                        &exemplar->filtered_attributes.ptr[i]));
  return size;
}

static inline void otlp_exemplar_encode_field(otlp__writer *writer,
                                              uint32_t field_number,
                                              const otlp_exemplar *exemplar) {
  size_t i;
  size_t inner = otlp_exemplar_encoded_size_inner(exemplar);
  otlp__write_message_start(writer, field_number, inner);
  if (exemplar->time_unix_nano) {
    otlp__write_tag(writer, 2, 1);
    otlp__write_fixed64(writer, exemplar->time_unix_nano);
  }
  if (exemplar->value_kind == OTLP_EXEMPLAR_VALUE_DOUBLE) {
    union {
      double d;
      uint64_t u;
    } bits;
    bits.d = exemplar->as_double;
    otlp__write_tag(writer, 3, 1);
    otlp__write_fixed64(writer, bits.u);
  }
  otlp__write_bytes_field(writer, 4, exemplar->span_id.ptr,
                          exemplar->span_id.len);
  otlp__write_bytes_field(writer, 5, exemplar->trace_id.ptr,
                          exemplar->trace_id.len);
  if (exemplar->value_kind == OTLP_EXEMPLAR_VALUE_INT) {
    otlp__write_tag(writer, 6, 1);
    otlp__write_fixed64(writer, (uint64_t)exemplar->as_int);
  }
  for (i = 0; i < exemplar->filtered_attributes.len; ++i)
    otlp_key_value_encode_field(writer, 7,
                                &exemplar->filtered_attributes.ptr[i]);
}

static inline otlp_status otlp_exemplar_decode_inner(otlp_exemplar *exemplar,
                                                     const uint8_t *data,
                                                     size_t len) {
  otlp__reader reader;
  otlp_exemplar_destroy(exemplar);
  otlp_exemplar_init(exemplar);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    switch (field_number) {
    case 2:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      exemplar->time_unix_nano = otlp__read_fixed64(&reader);
      break;
    case 3: {
      union {
        double d;
        uint64_t u;
      } bits;
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      exemplar->value_kind = OTLP_EXEMPLAR_VALUE_DOUBLE;
      bits.u = otlp__read_fixed64(&reader);
      exemplar->as_double = bits.d;
      break;
    }
    case 4:
    case 5: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if ((field_number == 4 ? otlp__set_bytes(&exemplar->span_id, ptr, size)
                             : otlp__set_bytes(&exemplar->trace_id, ptr,
                                               size)) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 6:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      exemplar->value_kind = OTLP_EXEMPLAR_VALUE_INT;
      exemplar->as_int = (int64_t)otlp__read_fixed64(&reader);
      break;
    case 7: {
      const uint8_t *ptr;
      size_t size;
      otlp_key_value kv;
      otlp_key_value_init(&kv);
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_key_value_decode_inner(&kv, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&exemplar->filtered_attributes.ptr,
                                 &exemplar->filtered_attributes.len,
                                 &exemplar->filtered_attributes.cap, sizeof(kv),
                                 &kv) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    default:
      if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
        return reader.status;
      break;
    }
  }
  return reader.status;
}

static inline void otlp_number_data_point_init(otlp_number_data_point *point) {
  otlp__zero(point, sizeof(*point));
}

static inline void
otlp_number_data_point_destroy(otlp_number_data_point *point) {
  size_t i;
  for (i = 0; i < point->attributes.len; ++i)
    otlp_key_value_destroy(&point->attributes.ptr[i]);
  free(point->attributes.ptr);
  for (i = 0; i < point->exemplars.len; ++i)
    otlp_exemplar_destroy(&point->exemplars.ptr[i]);
  free(point->exemplars.ptr);
  otlp_number_data_point_init(point);
}

static inline otlp_status
otlp_number_data_point_copy(otlp_number_data_point *dst,
                            const otlp_number_data_point *src) {
  size_t i;
  otlp_number_data_point_init(dst);
  dst->start_time_unix_nano = src->start_time_unix_nano;
  dst->time_unix_nano = src->time_unix_nano;
  dst->value_kind = src->value_kind;
  dst->as_double = src->as_double;
  dst->as_int = src->as_int;
  dst->flags = src->flags;
  for (i = 0; i < src->attributes.len; ++i) {
    otlp_key_value copy;
    otlp_status status = otlp_key_value_copy(&copy, &src->attributes.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->attributes.ptr,
                                    &dst->attributes.len, &dst->attributes.cap,
                                    sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  for (i = 0; i < src->exemplars.len; ++i) {
    otlp_exemplar copy;
    otlp_status status = otlp_exemplar_copy(&copy, &src->exemplars.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->exemplars.ptr,
                                    &dst->exemplars.len, &dst->exemplars.cap,
                                    sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}

static inline size_t
otlp_number_data_point_encoded_size_inner(const otlp_number_data_point *point) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < point->attributes.len; ++i)
    size += otlp__message_field_size(
        7, otlp_key_value_encoded_size_inner(&point->attributes.ptr[i]));
  if (point->start_time_unix_nano)
    size += otlp__fixed64_field_size(2);
  if (point->time_unix_nano)
    size += otlp__fixed64_field_size(3);
  if (point->value_kind == OTLP_NUMBER_VALUE_DOUBLE)
    size += otlp__fixed64_field_size(4);
  for (i = 0; i < point->exemplars.len; ++i)
    size += otlp__message_field_size(
        5, otlp_exemplar_encoded_size_inner(&point->exemplars.ptr[i]));
  if (point->value_kind == OTLP_NUMBER_VALUE_INT)
    size += otlp__fixed64_field_size(6);
  if (point->flags)
    size += otlp__varint_field_size(8, point->flags);
  return size;
}

static inline void
otlp_number_data_point_encode_field(otlp__writer *writer, uint32_t field_number,
                                    const otlp_number_data_point *point) {
  size_t i;
  size_t inner = otlp_number_data_point_encoded_size_inner(point);
  otlp__write_message_start(writer, field_number, inner);
  for (i = 0; i < point->attributes.len; ++i)
    otlp_key_value_encode_field(writer, 7, &point->attributes.ptr[i]);
  if (point->start_time_unix_nano) {
    otlp__write_tag(writer, 2, 1);
    otlp__write_fixed64(writer, point->start_time_unix_nano);
  }
  if (point->time_unix_nano) {
    otlp__write_tag(writer, 3, 1);
    otlp__write_fixed64(writer, point->time_unix_nano);
  }
  if (point->value_kind == OTLP_NUMBER_VALUE_DOUBLE) {
    union {
      double d;
      uint64_t u;
    } bits;
    bits.d = point->as_double;
    otlp__write_tag(writer, 4, 1);
    otlp__write_fixed64(writer, bits.u);
  }
  for (i = 0; i < point->exemplars.len; ++i)
    otlp_exemplar_encode_field(writer, 5, &point->exemplars.ptr[i]);
  if (point->value_kind == OTLP_NUMBER_VALUE_INT) {
    otlp__write_tag(writer, 6, 1);
    otlp__write_fixed64(writer, (uint64_t)point->as_int);
  }
  if (point->flags) {
    otlp__write_tag(writer, 8, 0);
    otlp__write_varint(writer, point->flags);
  }
}

static inline otlp_status
otlp_number_data_point_decode_inner(otlp_number_data_point *point,
                                    const uint8_t *data, size_t len) {
  otlp__reader reader;
  otlp_number_data_point_destroy(point);
  otlp_number_data_point_init(point);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    switch (field_number) {
    case 7: {
      const uint8_t *ptr;
      size_t size;
      otlp_key_value kv;
      otlp_key_value_init(&kv);
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_key_value_decode_inner(&kv, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&point->attributes.ptr,
                                 &point->attributes.len, &point->attributes.cap,
                                 sizeof(kv), &kv) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 2:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->start_time_unix_nano = otlp__read_fixed64(&reader);
      break;
    case 3:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->time_unix_nano = otlp__read_fixed64(&reader);
      break;
    case 4: {
      union {
        double d;
        uint64_t u;
      } bits;
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->value_kind = OTLP_NUMBER_VALUE_DOUBLE;
      bits.u = otlp__read_fixed64(&reader);
      point->as_double = bits.d;
      break;
    }
    case 5: {
      const uint8_t *ptr;
      size_t size;
      otlp_exemplar exemplar;
      otlp_exemplar_init(&exemplar);
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_exemplar_decode_inner(&exemplar, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&point->exemplars.ptr,
                                 &point->exemplars.len, &point->exemplars.cap,
                                 sizeof(exemplar), &exemplar) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 6:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->value_kind = OTLP_NUMBER_VALUE_INT;
      point->as_int = (int64_t)otlp__read_fixed64(&reader);
      break;
    case 8:
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      point->flags = (uint32_t)otlp__read_varint(&reader);
      break;
    default:
      if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
        return reader.status;
      break;
    }
  }
  return reader.status;
}

static inline void
otlp_histogram_data_point_init(otlp_histogram_data_point *point) {
  otlp__zero(point, sizeof(*point));
}

static inline void
otlp_histogram_data_point_destroy(otlp_histogram_data_point *point) {
  size_t i;
  for (i = 0; i < point->attributes.len; ++i)
    otlp_key_value_destroy(&point->attributes.ptr[i]);
  free(point->attributes.ptr);
  free(point->bucket_counts.ptr);
  free(point->explicit_bounds.ptr);
  for (i = 0; i < point->exemplars.len; ++i)
    otlp_exemplar_destroy(&point->exemplars.ptr[i]);
  free(point->exemplars.ptr);
  otlp_histogram_data_point_init(point);
}

static inline otlp_status
otlp_histogram_data_point_copy(otlp_histogram_data_point *dst,
                               const otlp_histogram_data_point *src) {
  size_t i;
  otlp_histogram_data_point_init(dst);
  *dst = *src;
  dst->attributes.ptr = NULL;
  dst->attributes.len = 0;
  dst->attributes.cap = 0;
  dst->bucket_counts.ptr = NULL;
  dst->bucket_counts.len = 0;
  dst->bucket_counts.cap = 0;
  dst->explicit_bounds.ptr = NULL;
  dst->explicit_bounds.len = 0;
  dst->explicit_bounds.cap = 0;
  dst->exemplars.ptr = NULL;
  dst->exemplars.len = 0;
  dst->exemplars.cap = 0;
  for (i = 0; i < src->attributes.len; ++i) {
    otlp_key_value copy;
    otlp_status status = otlp_key_value_copy(&copy, &src->attributes.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->attributes.ptr,
                                    &dst->attributes.len, &dst->attributes.cap,
                                    sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  for (i = 0; i < src->bucket_counts.len; ++i)
    if (otlp__append_primitive((void **)&dst->bucket_counts.ptr,
                               &dst->bucket_counts.len, &dst->bucket_counts.cap,
                               sizeof(src->bucket_counts.ptr[i]),
                               &src->bucket_counts.ptr[i]) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  for (i = 0; i < src->explicit_bounds.len; ++i)
    if (otlp__append_primitive(
            (void **)&dst->explicit_bounds.ptr, &dst->explicit_bounds.len,
            &dst->explicit_bounds.cap, sizeof(src->explicit_bounds.ptr[i]),
            &src->explicit_bounds.ptr[i]) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  for (i = 0; i < src->exemplars.len; ++i) {
    otlp_exemplar copy;
    otlp_status status = otlp_exemplar_copy(&copy, &src->exemplars.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->exemplars.ptr,
                                    &dst->exemplars.len, &dst->exemplars.cap,
                                    sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}

static inline size_t otlp_histogram_data_point_encoded_size_inner(
    const otlp_histogram_data_point *point) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < point->attributes.len; ++i)
    size += otlp__message_field_size(
        9, otlp_key_value_encoded_size_inner(&point->attributes.ptr[i]));
  if (point->start_time_unix_nano)
    size += otlp__fixed64_field_size(2);
  if (point->time_unix_nano)
    size += otlp__fixed64_field_size(3);
  size += otlp__fixed64_field_size(4);
  if (point->has_sum)
    size += otlp__fixed64_field_size(5);
  if (point->bucket_counts.len)
    size += otlp__bytes_field_size(
        6, otlp__packed_fixed64_size(point->bucket_counts.len));
  if (point->explicit_bounds.len)
    size += otlp__bytes_field_size(
        7, otlp__packed_fixed64_size(point->explicit_bounds.len));
  for (i = 0; i < point->exemplars.len; ++i)
    size += otlp__message_field_size(
        8, otlp_exemplar_encoded_size_inner(&point->exemplars.ptr[i]));
  if (point->flags)
    size += otlp__varint_field_size(10, point->flags);
  if (point->has_min)
    size += otlp__fixed64_field_size(11);
  if (point->has_max)
    size += otlp__fixed64_field_size(12);
  return size;
}

static inline void
otlp_histogram_data_point_encode_field(otlp__writer *writer,
                                       uint32_t field_number,
                                       const otlp_histogram_data_point *point) {
  size_t i;
  size_t inner = otlp_histogram_data_point_encoded_size_inner(point);
  otlp__write_message_start(writer, field_number, inner);
  for (i = 0; i < point->attributes.len; ++i)
    otlp_key_value_encode_field(writer, 9, &point->attributes.ptr[i]);
  if (point->start_time_unix_nano) {
    otlp__write_tag(writer, 2, 1);
    otlp__write_fixed64(writer, point->start_time_unix_nano);
  }
  if (point->time_unix_nano) {
    otlp__write_tag(writer, 3, 1);
    otlp__write_fixed64(writer, point->time_unix_nano);
  }
  otlp__write_tag(writer, 4, 1);
  otlp__write_fixed64(writer, point->count);
  if (point->has_sum) {
    union {
      double d;
      uint64_t u;
    } bits;
    bits.d = point->sum;
    otlp__write_tag(writer, 5, 1);
    otlp__write_fixed64(writer, bits.u);
  }
  otlp__write_packed_uint64_fixed64_field(writer, 6, &point->bucket_counts);
  otlp__write_packed_double_fixed64_field(writer, 7, &point->explicit_bounds);
  for (i = 0; i < point->exemplars.len; ++i)
    otlp_exemplar_encode_field(writer, 8, &point->exemplars.ptr[i]);
  if (point->flags) {
    otlp__write_tag(writer, 10, 0);
    otlp__write_varint(writer, point->flags);
  }
  if (point->has_min) {
    union {
      double d;
      uint64_t u;
    } bits;
    bits.d = point->min;
    otlp__write_tag(writer, 11, 1);
    otlp__write_fixed64(writer, bits.u);
  }
  if (point->has_max) {
    union {
      double d;
      uint64_t u;
    } bits;
    bits.d = point->max;
    otlp__write_tag(writer, 12, 1);
    otlp__write_fixed64(writer, bits.u);
  }
}

static inline otlp_status
otlp_histogram_data_point_decode_inner(otlp_histogram_data_point *point,
                                       const uint8_t *data, size_t len) {
  otlp__reader reader;
  otlp_histogram_data_point_destroy(point);
  otlp_histogram_data_point_init(point);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    switch (field_number) {
    case 9: {
      const uint8_t *ptr;
      size_t size;
      otlp_key_value kv;
      otlp_key_value_init(&kv);
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_key_value_decode_inner(&kv, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&point->attributes.ptr,
                                 &point->attributes.len, &point->attributes.cap,
                                 sizeof(kv), &kv) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 2:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->start_time_unix_nano = otlp__read_fixed64(&reader);
      break;
    case 3:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->time_unix_nano = otlp__read_fixed64(&reader);
      break;
    case 4:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->count = otlp__read_fixed64(&reader);
      break;
    case 5: {
      union {
        double d;
        uint64_t u;
      } bits;
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->has_sum = 1;
      bits.u = otlp__read_fixed64(&reader);
      point->sum = bits.d;
      break;
    }
    case 6: {
      uint64_t value;
      if (wire_type == 2) {
        const uint8_t *ptr;
        size_t size;
        if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
          return reader.status;
        if (otlp__append_packed_uint64_fixed64(&point->bucket_counts, ptr,
                                               size) != OTLP_STATUS_OK)
          return OTLP_STATUS_DECODE_ERROR;
        break;
      }
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      value = otlp__read_fixed64(&reader);
      if (otlp__append_primitive((void **)&point->bucket_counts.ptr,
                                 &point->bucket_counts.len,
                                 &point->bucket_counts.cap, sizeof(value),
                                 &value) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 7: {
      union {
        double d;
        uint64_t u;
      } bits;
      if (wire_type == 2) {
        const uint8_t *ptr;
        size_t size;
        if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
          return reader.status;
        if (otlp__append_packed_double_fixed64(&point->explicit_bounds, ptr,
                                               size) != OTLP_STATUS_OK)
          return OTLP_STATUS_DECODE_ERROR;
        break;
      }
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      bits.u = otlp__read_fixed64(&reader);
      if (otlp__append_primitive((void **)&point->explicit_bounds.ptr,
                                 &point->explicit_bounds.len,
                                 &point->explicit_bounds.cap, sizeof(bits.d),
                                 &bits.d) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 8: {
      const uint8_t *ptr;
      size_t size;
      otlp_exemplar exemplar;
      otlp_exemplar_init(&exemplar);
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_exemplar_decode_inner(&exemplar, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&point->exemplars.ptr,
                                 &point->exemplars.len, &point->exemplars.cap,
                                 sizeof(exemplar), &exemplar) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 10:
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      point->flags = (uint32_t)otlp__read_varint(&reader);
      break;
    case 11: {
      union {
        double d;
        uint64_t u;
      } bits;
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->has_min = 1;
      bits.u = otlp__read_fixed64(&reader);
      point->min = bits.d;
      break;
    }
    case 12: {
      union {
        double d;
        uint64_t u;
      } bits;
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->has_max = 1;
      bits.u = otlp__read_fixed64(&reader);
      point->max = bits.d;
      break;
    }
    default:
      if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
        return reader.status;
      break;
    }
  }
  return reader.status;
}

static inline void otlp_exponential_histogram_buckets_init(
    otlp_exponential_histogram_buckets *buckets) {
  otlp__zero(buckets, sizeof(*buckets));
}
static inline void otlp_exponential_histogram_buckets_destroy(
    otlp_exponential_histogram_buckets *buckets) {
  free(buckets->bucket_counts.ptr);
  otlp_exponential_histogram_buckets_init(buckets);
}
static inline otlp_status otlp_exponential_histogram_buckets_copy(
    otlp_exponential_histogram_buckets *dst,
    const otlp_exponential_histogram_buckets *src) {
  size_t i;
  otlp_exponential_histogram_buckets_init(dst);
  dst->offset = src->offset;
  for (i = 0; i < src->bucket_counts.len; ++i)
    if (otlp__append_primitive((void **)&dst->bucket_counts.ptr,
                               &dst->bucket_counts.len, &dst->bucket_counts.cap,
                               sizeof(src->bucket_counts.ptr[i]),
                               &src->bucket_counts.ptr[i]) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  return OTLP_STATUS_OK;
}
static inline size_t otlp_exponential_histogram_buckets_encoded_size_inner(
    const otlp_exponential_histogram_buckets *buckets) {
  size_t size = 0;
  size_t i;
  if (buckets->offset) {
    size += otlp__varint_field_size(1, (uint64_t)otlp__zigzag32(buckets->offset));
  }
  if (buckets->bucket_counts.len) {
    size_t payload = 0;
    for (i = 0; i < buckets->bucket_counts.len; ++i)
      payload += otlp__varint_size(buckets->bucket_counts.ptr[i]);
    size += otlp__bytes_field_size(2, payload);
  }
  return size;
}
static inline void otlp_exponential_histogram_buckets_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_exponential_histogram_buckets *buckets) {
  size_t i;
  size_t inner = otlp_exponential_histogram_buckets_encoded_size_inner(buckets);
  otlp__write_message_start(writer, field_number, inner);
  if (buckets->offset) {
    otlp__write_tag(writer, 1, 0);
    otlp__write_varint(writer, (uint64_t)otlp__zigzag32(buckets->offset));
  }
  if (buckets->bucket_counts.len) {
    size_t payload = 0;
    for (i = 0; i < buckets->bucket_counts.len; ++i)
      payload += otlp__varint_size(buckets->bucket_counts.ptr[i]);
    otlp__write_tag(writer, 2, 2);
    otlp__write_varint(writer, (uint64_t)payload);
    for (i = 0; i < buckets->bucket_counts.len; ++i)
      otlp__write_varint(writer, buckets->bucket_counts.ptr[i]);
  }
}
static inline otlp_status otlp_exponential_histogram_buckets_decode_inner(
    otlp_exponential_histogram_buckets *buckets, const uint8_t *data,
    size_t len) {
  otlp__reader reader;
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if (field_number == 1 && wire_type == 0) {
      buckets->offset = otlp__unzigzag32((uint32_t)otlp__read_varint(&reader));
    } else if (field_number == 2 && wire_type == 0) {
      uint64_t value = otlp__read_varint(&reader);
      if (otlp__append_primitive((void **)&buckets->bucket_counts.ptr,
                                 &buckets->bucket_counts.len,
                                 &buckets->bucket_counts.cap, sizeof(value),
                                 &value) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (field_number == 2 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      otlp__reader inner_reader;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      otlp__reader_init(&inner_reader, ptr, size);
      while (otlp__reader_has_remaining(&inner_reader)) {
        uint64_t value = otlp__read_varint(&inner_reader);
        if (inner_reader.status != OTLP_STATUS_OK)
          return inner_reader.status;
        if (otlp__append_primitive((void **)&buckets->bucket_counts.ptr,
                                   &buckets->bucket_counts.len,
                                   &buckets->bucket_counts.cap, sizeof(value),
                                   &value) != OTLP_STATUS_OK)
          return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
      return reader.status;
  }
  return reader.status;
}

static inline void otlp_exponential_histogram_data_point_init(
    otlp_exponential_histogram_data_point *point) {
  otlp__zero(point, sizeof(*point));
}
static inline void otlp_exponential_histogram_data_point_destroy(
    otlp_exponential_histogram_data_point *point) {
  size_t i;
  for (i = 0; i < point->attributes.len; ++i)
    otlp_key_value_destroy(&point->attributes.ptr[i]);
  free(point->attributes.ptr);
  if (point->has_positive)
    otlp_exponential_histogram_buckets_destroy(&point->positive);
  if (point->has_negative)
    otlp_exponential_histogram_buckets_destroy(&point->negative);
  for (i = 0; i < point->exemplars.len; ++i)
    otlp_exemplar_destroy(&point->exemplars.ptr[i]);
  free(point->exemplars.ptr);
  otlp_exponential_histogram_data_point_init(point);
}
static inline otlp_status otlp_exponential_histogram_data_point_copy(
    otlp_exponential_histogram_data_point *dst,
    const otlp_exponential_histogram_data_point *src) {
  size_t i;
  otlp_exponential_histogram_data_point_init(dst);
  *dst = *src;
  dst->attributes.ptr = NULL;
  dst->attributes.len = 0;
  dst->attributes.cap = 0;
  dst->exemplars.ptr = NULL;
  dst->exemplars.len = 0;
  dst->exemplars.cap = 0;
  dst->has_positive = 0;
  dst->has_negative = 0;
  for (i = 0; i < src->attributes.len; ++i) {
    otlp_key_value copy;
    otlp_status status = otlp_key_value_copy(&copy, &src->attributes.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->attributes.ptr,
                                    &dst->attributes.len, &dst->attributes.cap,
                                    sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  if (src->has_positive) {
    dst->has_positive = 1;
    if (otlp_exponential_histogram_buckets_copy(
            &dst->positive, &src->positive) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  }
  if (src->has_negative) {
    dst->has_negative = 1;
    if (otlp_exponential_histogram_buckets_copy(
            &dst->negative, &src->negative) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  }
  for (i = 0; i < src->exemplars.len; ++i) {
    otlp_exemplar copy;
    otlp_status status = otlp_exemplar_copy(&copy, &src->exemplars.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->exemplars.ptr,
                                    &dst->exemplars.len, &dst->exemplars.cap,
                                    sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}
static inline size_t otlp_exponential_histogram_data_point_encoded_size_inner(
    const otlp_exponential_histogram_data_point *point) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < point->attributes.len; ++i)
    size += otlp__message_field_size(
        1, otlp_key_value_encoded_size_inner(&point->attributes.ptr[i]));
  if (point->start_time_unix_nano)
    size += otlp__fixed64_field_size(2);
  if (point->time_unix_nano)
    size += otlp__fixed64_field_size(3);
  size += otlp__fixed64_field_size(4);
  if (point->has_sum)
    size += otlp__fixed64_field_size(5);
  if (point->scale)
    size += otlp__varint_field_size(6, (uint64_t)otlp__zigzag32(point->scale));
  if (point->zero_count)
    size += otlp__fixed64_field_size(7);
  if (point->has_positive)
    size += otlp__message_field_size(
        8, otlp_exponential_histogram_buckets_encoded_size_inner(
               &point->positive));
  if (point->has_negative)
    size += otlp__message_field_size(
        9, otlp_exponential_histogram_buckets_encoded_size_inner(
               &point->negative));
  if (point->flags)
    size += otlp__varint_field_size(10, point->flags);
  for (i = 0; i < point->exemplars.len; ++i)
    size += otlp__message_field_size(
        11, otlp_exemplar_encoded_size_inner(&point->exemplars.ptr[i]));
  if (point->has_min)
    size += otlp__fixed64_field_size(12);
  if (point->has_max)
    size += otlp__fixed64_field_size(13);
  if (point->zero_threshold != 0.0)
    size += otlp__fixed64_field_size(14);
  return size;
}
static inline void otlp_exponential_histogram_data_point_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_exponential_histogram_data_point *point) {
  size_t i;
  size_t inner =
      otlp_exponential_histogram_data_point_encoded_size_inner(point);
  otlp__write_message_start(writer, field_number, inner);
  for (i = 0; i < point->attributes.len; ++i)
    otlp_key_value_encode_field(writer, 1, &point->attributes.ptr[i]);
  if (point->start_time_unix_nano) {
    otlp__write_tag(writer, 2, 1);
    otlp__write_fixed64(writer, point->start_time_unix_nano);
  }
  if (point->time_unix_nano) {
    otlp__write_tag(writer, 3, 1);
    otlp__write_fixed64(writer, point->time_unix_nano);
  }
  otlp__write_tag(writer, 4, 1);
  otlp__write_fixed64(writer, point->count);
  if (point->has_sum) {
    union {
      double d;
      uint64_t u;
    } bits;
    bits.d = point->sum;
    otlp__write_tag(writer, 5, 1);
    otlp__write_fixed64(writer, bits.u);
  }
  if (point->scale) {
    otlp__write_tag(writer, 6, 0);
    otlp__write_varint(writer, (uint64_t)otlp__zigzag32(point->scale));
  }
  if (point->zero_count) {
    otlp__write_tag(writer, 7, 1);
    otlp__write_fixed64(writer, point->zero_count);
  }
  if (point->has_positive)
    otlp_exponential_histogram_buckets_encode_field(writer, 8,
                                                    &point->positive);
  if (point->has_negative)
    otlp_exponential_histogram_buckets_encode_field(writer, 9,
                                                    &point->negative);
  if (point->flags) {
    otlp__write_tag(writer, 10, 0);
    otlp__write_varint(writer, point->flags);
  }
  for (i = 0; i < point->exemplars.len; ++i)
    otlp_exemplar_encode_field(writer, 11, &point->exemplars.ptr[i]);
  if (point->has_min) {
    union {
      double d;
      uint64_t u;
    } bits;
    bits.d = point->min;
    otlp__write_tag(writer, 12, 1);
    otlp__write_fixed64(writer, bits.u);
  }
  if (point->has_max) {
    union {
      double d;
      uint64_t u;
    } bits;
    bits.d = point->max;
    otlp__write_tag(writer, 13, 1);
    otlp__write_fixed64(writer, bits.u);
  }
  if (point->zero_threshold != 0.0) {
    union {
      double d;
      uint64_t u;
    } bits;
    bits.d = point->zero_threshold;
    otlp__write_tag(writer, 14, 1);
    otlp__write_fixed64(writer, bits.u);
  }
}
static inline otlp_status otlp_exponential_histogram_data_point_decode_inner(
    otlp_exponential_histogram_data_point *point, const uint8_t *data,
    size_t len) {
  otlp__reader reader;
  otlp_exponential_histogram_data_point_destroy(point);
  otlp_exponential_histogram_data_point_init(point);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    switch (field_number) {
    case 1: {
      const uint8_t *ptr;
      size_t size;
      otlp_key_value kv;
      otlp_key_value_init(&kv);
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_key_value_decode_inner(&kv, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&point->attributes.ptr,
                                 &point->attributes.len, &point->attributes.cap,
                                 sizeof(kv), &kv) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 2:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->start_time_unix_nano = otlp__read_fixed64(&reader);
      break;
    case 3:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->time_unix_nano = otlp__read_fixed64(&reader);
      break;
    case 4:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->count = otlp__read_fixed64(&reader);
      break;
    case 5: {
      union {
        double d;
        uint64_t u;
      } bits;
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->has_sum = 1;
      bits.u = otlp__read_fixed64(&reader);
      point->sum = bits.d;
      break;
    }
    case 6: {
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      point->scale = otlp__unzigzag32((uint32_t)otlp__read_varint(&reader));
      break;
    }
    case 7:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->zero_count = otlp__read_fixed64(&reader);
      break;
    case 8: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      point->has_positive = 1;
      if (otlp_exponential_histogram_buckets_decode_inner(
              &point->positive, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      break;
    }
    case 9: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      point->has_negative = 1;
      if (otlp_exponential_histogram_buckets_decode_inner(
              &point->negative, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      break;
    }
    case 10:
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      point->flags = (uint32_t)otlp__read_varint(&reader);
      break;
    case 11: {
      const uint8_t *ptr;
      size_t size;
      otlp_exemplar exemplar;
      otlp_exemplar_init(&exemplar);
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_exemplar_decode_inner(&exemplar, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&point->exemplars.ptr,
                                 &point->exemplars.len, &point->exemplars.cap,
                                 sizeof(exemplar), &exemplar) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 12: {
      union {
        double d;
        uint64_t u;
      } bits;
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->has_min = 1;
      bits.u = otlp__read_fixed64(&reader);
      point->min = bits.d;
      break;
    }
    case 13: {
      union {
        double d;
        uint64_t u;
      } bits;
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->has_max = 1;
      bits.u = otlp__read_fixed64(&reader);
      point->max = bits.d;
      break;
    }
    case 14: {
      union {
        double d;
        uint64_t u;
      } bits;
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      bits.u = otlp__read_fixed64(&reader);
      point->zero_threshold = bits.d;
      break;
    }
    default:
      if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
        return reader.status;
      break;
    }
  }
  return reader.status;
}

static inline void otlp_summary_data_point_value_at_quantile_init(
    otlp_summary_data_point_value_at_quantile *value) {
  otlp__zero(value, sizeof(*value));
}
static inline void otlp_summary_data_point_value_at_quantile_destroy(
    otlp_summary_data_point_value_at_quantile *value) {
  otlp_summary_data_point_value_at_quantile_init(value);
}
static inline size_t
otlp_summary_data_point_value_at_quantile_encoded_size_inner(
    const otlp_summary_data_point_value_at_quantile *value) {
  (void)value;
  return otlp__fixed64_field_size(1) + otlp__fixed64_field_size(2);
}
static inline void otlp_summary_data_point_value_at_quantile_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_summary_data_point_value_at_quantile *value) {
  union {
    double d;
    uint64_t u;
  } q;
  union {
    double d;
    uint64_t u;
  } v;
  q.d = value->quantile;
  v.d = value->value;
  otlp__write_message_start(
      writer, field_number,
      otlp_summary_data_point_value_at_quantile_encoded_size_inner(value));
  otlp__write_tag(writer, 1, 1);
  otlp__write_fixed64(writer, q.u);
  otlp__write_tag(writer, 2, 1);
  otlp__write_fixed64(writer, v.u);
}
static inline otlp_status
otlp_summary_data_point_value_at_quantile_decode_inner(
    otlp_summary_data_point_value_at_quantile *value, const uint8_t *data,
    size_t len) {
  otlp__reader reader;
  otlp_summary_data_point_value_at_quantile_destroy(value);
  otlp_summary_data_point_value_at_quantile_init(value);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if ((field_number == 1 || field_number == 2) && wire_type == 1) {
      union {
        double d;
        uint64_t u;
      } bits;
      bits.u = otlp__read_fixed64(&reader);
      if (field_number == 1)
        value->quantile = bits.d;
      else
        value->value = bits.d;
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
      return reader.status;
  }
  return reader.status;
}

static inline void
otlp_summary_data_point_init(otlp_summary_data_point *point) {
  otlp__zero(point, sizeof(*point));
}
static inline void
otlp_summary_data_point_destroy(otlp_summary_data_point *point) {
  size_t i;
  for (i = 0; i < point->attributes.len; ++i)
    otlp_key_value_destroy(&point->attributes.ptr[i]);
  free(point->attributes.ptr);
  free(point->quantile_values.ptr);
  otlp_summary_data_point_init(point);
}
static inline otlp_status
otlp_summary_data_point_copy(otlp_summary_data_point *dst,
                             const otlp_summary_data_point *src) {
  size_t i;
  otlp_summary_data_point_init(dst);
  *dst = *src;
  dst->attributes.ptr = NULL;
  dst->attributes.len = 0;
  dst->attributes.cap = 0;
  dst->quantile_values.ptr = NULL;
  dst->quantile_values.len = 0;
  dst->quantile_values.cap = 0;
  for (i = 0; i < src->attributes.len; ++i) {
    otlp_key_value copy;
    otlp_status status = otlp_key_value_copy(&copy, &src->attributes.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->attributes.ptr,
                                    &dst->attributes.len, &dst->attributes.cap,
                                    sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  for (i = 0; i < src->quantile_values.len; ++i)
    if (otlp__append_primitive(
            (void **)&dst->quantile_values.ptr, &dst->quantile_values.len,
            &dst->quantile_values.cap, sizeof(src->quantile_values.ptr[i]),
            &src->quantile_values.ptr[i]) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  return OTLP_STATUS_OK;
}
static inline size_t otlp_summary_data_point_encoded_size_inner(
    const otlp_summary_data_point *point) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < point->attributes.len; ++i)
    size += otlp__message_field_size(
        7, otlp_key_value_encoded_size_inner(&point->attributes.ptr[i]));
  if (point->start_time_unix_nano)
    size += otlp__fixed64_field_size(2);
  if (point->time_unix_nano)
    size += otlp__fixed64_field_size(3);
  size += otlp__fixed64_field_size(4);
  if (point->sum != 0.0)
    size += otlp__fixed64_field_size(5);
  for (i = 0; i < point->quantile_values.len; ++i)
    size += otlp__message_field_size(
        6, otlp_summary_data_point_value_at_quantile_encoded_size_inner(
               &point->quantile_values.ptr[i]));
  if (point->flags)
    size += otlp__varint_field_size(8, point->flags);
  return size;
}
static inline void
otlp_summary_data_point_encode_field(otlp__writer *writer,
                                     uint32_t field_number,
                                     const otlp_summary_data_point *point) {
  size_t i;
  size_t inner = otlp_summary_data_point_encoded_size_inner(point);
  otlp__write_message_start(writer, field_number, inner);
  for (i = 0; i < point->attributes.len; ++i)
    otlp_key_value_encode_field(writer, 7, &point->attributes.ptr[i]);
  if (point->start_time_unix_nano) {
    otlp__write_tag(writer, 2, 1);
    otlp__write_fixed64(writer, point->start_time_unix_nano);
  }
  if (point->time_unix_nano) {
    otlp__write_tag(writer, 3, 1);
    otlp__write_fixed64(writer, point->time_unix_nano);
  }
  otlp__write_tag(writer, 4, 1);
  otlp__write_fixed64(writer, point->count);
  if (point->sum != 0.0) {
    union {
      double d;
      uint64_t u;
    } bits;
    bits.d = point->sum;
    otlp__write_tag(writer, 5, 1);
    otlp__write_fixed64(writer, bits.u);
  }
  for (i = 0; i < point->quantile_values.len; ++i)
    otlp_summary_data_point_value_at_quantile_encode_field(
        writer, 6, &point->quantile_values.ptr[i]);
  if (point->flags) {
    otlp__write_tag(writer, 8, 0);
    otlp__write_varint(writer, point->flags);
  }
}
static inline otlp_status
otlp_summary_data_point_decode_inner(otlp_summary_data_point *point,
                                     const uint8_t *data, size_t len) {
  otlp__reader reader;
  otlp_summary_data_point_destroy(point);
  otlp_summary_data_point_init(point);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    switch (field_number) {
    case 7: {
      const uint8_t *ptr;
      size_t size;
      otlp_key_value kv;
      otlp_key_value_init(&kv);
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_key_value_decode_inner(&kv, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&point->attributes.ptr,
                                 &point->attributes.len, &point->attributes.cap,
                                 sizeof(kv), &kv) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 2:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->start_time_unix_nano = otlp__read_fixed64(&reader);
      break;
    case 3:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->time_unix_nano = otlp__read_fixed64(&reader);
      break;
    case 4:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      point->count = otlp__read_fixed64(&reader);
      break;
    case 5: {
      union {
        double d;
        uint64_t u;
      } bits;
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      bits.u = otlp__read_fixed64(&reader);
      point->sum = bits.d;
      break;
    }
    case 6: {
      const uint8_t *ptr;
      size_t size;
      otlp_summary_data_point_value_at_quantile value;
      otlp_summary_data_point_value_at_quantile_init(&value);
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_summary_data_point_value_at_quantile_decode_inner(
              &value, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&point->quantile_values.ptr,
                                 &point->quantile_values.len,
                                 &point->quantile_values.cap, sizeof(value),
                                 &value) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 8:
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      point->flags = (uint32_t)otlp__read_varint(&reader);
      break;
    default:
      if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
        return reader.status;
      break;
    }
  }
  return reader.status;
}

static inline void otlp_gauge_init(otlp_gauge *gauge) {
  otlp__zero(gauge, sizeof(*gauge));
}
static inline void otlp_gauge_destroy(otlp_gauge *gauge) {
  size_t i;
  for (i = 0; i < gauge->data_points.len; ++i)
    otlp_number_data_point_destroy(&gauge->data_points.ptr[i]);
  free(gauge->data_points.ptr);
  otlp_gauge_init(gauge);
}
static inline otlp_status otlp_gauge_copy(otlp_gauge *dst,
                                          const otlp_gauge *src) {
  size_t i;
  otlp_gauge_init(dst);
  for (i = 0; i < src->data_points.len; ++i) {
    otlp_number_data_point copy;
    otlp_status status =
        otlp_number_data_point_copy(&copy, &src->data_points.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->data_points.ptr,
                                    &dst->data_points.len,
                                    &dst->data_points.cap, sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}
static inline size_t otlp_gauge_encoded_size_inner(const otlp_gauge *gauge) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < gauge->data_points.len; ++i)
    size += otlp__message_field_size(
        1,
        otlp_number_data_point_encoded_size_inner(&gauge->data_points.ptr[i]));
  return size;
}
static inline void otlp_gauge_encode_field(otlp__writer *writer,
                                           uint32_t field_number,
                                           const otlp_gauge *gauge) {
  size_t i;
  size_t inner = otlp_gauge_encoded_size_inner(gauge);
  otlp__write_message_start(writer, field_number, inner);
  for (i = 0; i < gauge->data_points.len; ++i)
    otlp_number_data_point_encode_field(writer, 1, &gauge->data_points.ptr[i]);
}
static inline otlp_status
otlp_gauge_decode_inner(otlp_gauge *gauge, const uint8_t *data, size_t len) {
  otlp__reader reader;
  otlp_gauge_destroy(gauge);
  otlp_gauge_init(gauge);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if (field_number == 1 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      otlp_number_data_point point;
      otlp_number_data_point_init(&point);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_number_data_point_decode_inner(&point, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive(
              (void **)&gauge->data_points.ptr, &gauge->data_points.len,
              &gauge->data_points.cap, sizeof(point), &point) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
      return reader.status;
  }
  return reader.status;
}

static inline void otlp_sum_init(otlp_sum *sum) {
  otlp__zero(sum, sizeof(*sum));
}
static inline void otlp_sum_destroy(otlp_sum *sum) {
  size_t i;
  for (i = 0; i < sum->data_points.len; ++i)
    otlp_number_data_point_destroy(&sum->data_points.ptr[i]);
  free(sum->data_points.ptr);
  otlp_sum_init(sum);
}
static inline otlp_status otlp_sum_copy(otlp_sum *dst, const otlp_sum *src) {
  size_t i;
  otlp_sum_init(dst);
  dst->aggregation_temporality = src->aggregation_temporality;
  dst->is_monotonic = src->is_monotonic;
  for (i = 0; i < src->data_points.len; ++i) {
    otlp_number_data_point copy;
    otlp_status status =
        otlp_number_data_point_copy(&copy, &src->data_points.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->data_points.ptr,
                                    &dst->data_points.len,
                                    &dst->data_points.cap, sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}
static inline size_t otlp_sum_encoded_size_inner(const otlp_sum *sum) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < sum->data_points.len; ++i)
    size += otlp__message_field_size(
        1, otlp_number_data_point_encoded_size_inner(&sum->data_points.ptr[i]));
  size += otlp__varint_field_size(2, (uint64_t)sum->aggregation_temporality);
  if (sum->is_monotonic)
    size += otlp__varint_field_size(3, 1);
  return size;
}
static inline void otlp_sum_encode_field(otlp__writer *writer,
                                         uint32_t field_number,
                                         const otlp_sum *sum) {
  size_t i;
  size_t inner = otlp_sum_encoded_size_inner(sum);
  otlp__write_message_start(writer, field_number, inner);
  for (i = 0; i < sum->data_points.len; ++i)
    otlp_number_data_point_encode_field(writer, 1, &sum->data_points.ptr[i]);
  otlp__write_tag(writer, 2, 0);
  otlp__write_varint(writer, (uint64_t)sum->aggregation_temporality);
  if (sum->is_monotonic) {
    otlp__write_tag(writer, 3, 0);
    otlp__write_varint(writer, 1);
  }
}
static inline otlp_status
otlp_sum_decode_inner(otlp_sum *sum, const uint8_t *data, size_t len) {
  otlp__reader reader;
  otlp_sum_destroy(sum);
  otlp_sum_init(sum);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if (field_number == 1 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      otlp_number_data_point point;
      otlp_number_data_point_init(&point);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_number_data_point_decode_inner(&point, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&sum->data_points.ptr,
                                 &sum->data_points.len, &sum->data_points.cap,
                                 sizeof(point), &point) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (field_number == 2 && wire_type == 0)
      sum->aggregation_temporality =
          (otlp_aggregation_temporality)otlp__read_varint(&reader);
    else if (field_number == 3 && wire_type == 0)
      sum->is_monotonic = (int)otlp__read_varint(&reader);
    else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
      return reader.status;
  }
  return reader.status;
}

static inline void otlp_histogram_init(otlp_histogram *histogram) {
  otlp__zero(histogram, sizeof(*histogram));
}
static inline void otlp_histogram_destroy(otlp_histogram *histogram) {
  size_t i;
  for (i = 0; i < histogram->data_points.len; ++i)
    otlp_histogram_data_point_destroy(&histogram->data_points.ptr[i]);
  free(histogram->data_points.ptr);
  otlp_histogram_init(histogram);
}
static inline otlp_status otlp_histogram_copy(otlp_histogram *dst,
                                              const otlp_histogram *src) {
  size_t i;
  otlp_histogram_init(dst);
  dst->aggregation_temporality = src->aggregation_temporality;
  for (i = 0; i < src->data_points.len; ++i) {
    otlp_histogram_data_point copy;
    otlp_status status =
        otlp_histogram_data_point_copy(&copy, &src->data_points.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->data_points.ptr,
                                    &dst->data_points.len,
                                    &dst->data_points.cap, sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}
static inline size_t
otlp_histogram_encoded_size_inner(const otlp_histogram *histogram) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < histogram->data_points.len; ++i)
    size += otlp__message_field_size(
        1, otlp_histogram_data_point_encoded_size_inner(
               &histogram->data_points.ptr[i]));
  size +=
      otlp__varint_field_size(2, (uint64_t)histogram->aggregation_temporality);
  return size;
}
static inline void
otlp_histogram_encode_field(otlp__writer *writer, uint32_t field_number,
                            const otlp_histogram *histogram) {
  size_t i;
  size_t inner = otlp_histogram_encoded_size_inner(histogram);
  otlp__write_message_start(writer, field_number, inner);
  for (i = 0; i < histogram->data_points.len; ++i)
    otlp_histogram_data_point_encode_field(writer, 1,
                                           &histogram->data_points.ptr[i]);
  otlp__write_tag(writer, 2, 0);
  otlp__write_varint(writer, (uint64_t)histogram->aggregation_temporality);
}
static inline otlp_status otlp_histogram_decode_inner(otlp_histogram *histogram,
                                                      const uint8_t *data,
                                                      size_t len) {
  otlp__reader reader;
  otlp_histogram_destroy(histogram);
  otlp_histogram_init(histogram);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if (field_number == 1 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      otlp_histogram_data_point point;
      otlp_histogram_data_point_init(&point);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_histogram_data_point_decode_inner(&point, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&histogram->data_points.ptr,
                                 &histogram->data_points.len,
                                 &histogram->data_points.cap, sizeof(point),
                                 &point) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (field_number == 2 && wire_type == 0)
      histogram->aggregation_temporality =
          (otlp_aggregation_temporality)otlp__read_varint(&reader);
    else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
      return reader.status;
  }
  return reader.status;
}

static inline void
otlp_exponential_histogram_init(otlp_exponential_histogram *histogram) {
  otlp__zero(histogram, sizeof(*histogram));
}
static inline void
otlp_exponential_histogram_destroy(otlp_exponential_histogram *histogram) {
  size_t i;
  for (i = 0; i < histogram->data_points.len; ++i)
    otlp_exponential_histogram_data_point_destroy(
        &histogram->data_points.ptr[i]);
  free(histogram->data_points.ptr);
  otlp_exponential_histogram_init(histogram);
}
static inline otlp_status
otlp_exponential_histogram_copy(otlp_exponential_histogram *dst,
                                const otlp_exponential_histogram *src) {
  size_t i;
  otlp_exponential_histogram_init(dst);
  dst->aggregation_temporality = src->aggregation_temporality;
  for (i = 0; i < src->data_points.len; ++i) {
    otlp_exponential_histogram_data_point copy;
    otlp_status status = otlp_exponential_histogram_data_point_copy(
        &copy, &src->data_points.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->data_points.ptr,
                                    &dst->data_points.len,
                                    &dst->data_points.cap, sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}
static inline size_t otlp_exponential_histogram_encoded_size_inner(
    const otlp_exponential_histogram *histogram) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < histogram->data_points.len; ++i)
    size += otlp__message_field_size(
        1, otlp_exponential_histogram_data_point_encoded_size_inner(
               &histogram->data_points.ptr[i]));
  size +=
      otlp__varint_field_size(2, (uint64_t)histogram->aggregation_temporality);
  return size;
}
static inline void otlp_exponential_histogram_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_exponential_histogram *histogram) {
  size_t i;
  size_t inner = otlp_exponential_histogram_encoded_size_inner(histogram);
  otlp__write_message_start(writer, field_number, inner);
  for (i = 0; i < histogram->data_points.len; ++i)
    otlp_exponential_histogram_data_point_encode_field(
        writer, 1, &histogram->data_points.ptr[i]);
  otlp__write_tag(writer, 2, 0);
  otlp__write_varint(writer, (uint64_t)histogram->aggregation_temporality);
}
static inline otlp_status
otlp_exponential_histogram_decode_inner(otlp_exponential_histogram *histogram,
                                        const uint8_t *data, size_t len) {
  otlp__reader reader;
  otlp_exponential_histogram_destroy(histogram);
  otlp_exponential_histogram_init(histogram);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if (field_number == 1 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      otlp_exponential_histogram_data_point point;
      otlp_exponential_histogram_data_point_init(&point);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_exponential_histogram_data_point_decode_inner(
              &point, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&histogram->data_points.ptr,
                                 &histogram->data_points.len,
                                 &histogram->data_points.cap, sizeof(point),
                                 &point) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (field_number == 2 && wire_type == 0)
      histogram->aggregation_temporality =
          (otlp_aggregation_temporality)otlp__read_varint(&reader);
    else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
      return reader.status;
  }
  return reader.status;
}

static inline void otlp_summary_init(otlp_summary *summary) {
  otlp__zero(summary, sizeof(*summary));
}
static inline void otlp_summary_destroy(otlp_summary *summary) {
  size_t i;
  for (i = 0; i < summary->data_points.len; ++i)
    otlp_summary_data_point_destroy(&summary->data_points.ptr[i]);
  free(summary->data_points.ptr);
  otlp_summary_init(summary);
}
static inline otlp_status otlp_summary_copy(otlp_summary *dst,
                                            const otlp_summary *src) {
  size_t i;
  otlp_summary_init(dst);
  for (i = 0; i < src->data_points.len; ++i) {
    otlp_summary_data_point copy;
    otlp_status status =
        otlp_summary_data_point_copy(&copy, &src->data_points.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->data_points.ptr,
                                    &dst->data_points.len,
                                    &dst->data_points.cap, sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}
static inline size_t
otlp_summary_encoded_size_inner(const otlp_summary *summary) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < summary->data_points.len; ++i)
    size +=
        otlp__message_field_size(1, otlp_summary_data_point_encoded_size_inner(
                                        &summary->data_points.ptr[i]));
  return size;
}
static inline void otlp_summary_encode_field(otlp__writer *writer,
                                             uint32_t field_number,
                                             const otlp_summary *summary) {
  size_t i;
  size_t inner = otlp_summary_encoded_size_inner(summary);
  otlp__write_message_start(writer, field_number, inner);
  for (i = 0; i < summary->data_points.len; ++i)
    otlp_summary_data_point_encode_field(writer, 1,
                                         &summary->data_points.ptr[i]);
}
static inline otlp_status otlp_summary_decode_inner(otlp_summary *summary,
                                                    const uint8_t *data,
                                                    size_t len) {
  otlp__reader reader;
  otlp_summary_destroy(summary);
  otlp_summary_init(summary);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if (field_number == 1 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      otlp_summary_data_point point;
      otlp_summary_data_point_init(&point);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_summary_data_point_decode_inner(&point, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&summary->data_points.ptr,
                                 &summary->data_points.len,
                                 &summary->data_points.cap, sizeof(point),
                                 &point) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
      return reader.status;
  }
  return reader.status;
}

static inline void otlp_metric_init(otlp_metric *metric) {
  otlp__zero(metric, sizeof(*metric));
}
static inline void otlp_metric_destroy(otlp_metric *metric) {
  size_t i;
  free(metric->name.ptr);
  free(metric->description.ptr);
  free(metric->unit.ptr);
  switch (metric->data_kind) {
  case OTLP_METRIC_DATA_GAUGE:
    otlp_gauge_destroy(&metric->data.gauge);
    break;
  case OTLP_METRIC_DATA_SUM:
    otlp_sum_destroy(&metric->data.sum);
    break;
  case OTLP_METRIC_DATA_HISTOGRAM:
    otlp_histogram_destroy(&metric->data.histogram);
    break;
  case OTLP_METRIC_DATA_EXPONENTIAL_HISTOGRAM:
    otlp_exponential_histogram_destroy(&metric->data.exponential_histogram);
    break;
  case OTLP_METRIC_DATA_SUMMARY:
    otlp_summary_destroy(&metric->data.summary);
    break;
  default:
    break;
  }
  for (i = 0; i < metric->metadata.len; ++i)
    otlp_key_value_destroy(&metric->metadata.ptr[i]);
  free(metric->metadata.ptr);
  otlp_metric_init(metric);
}

static inline void otlp_metric_data_destroy(otlp_metric *metric) {
  switch (metric->data_kind) {
  case OTLP_METRIC_DATA_GAUGE:
    otlp_gauge_destroy(&metric->data.gauge);
    break;
  case OTLP_METRIC_DATA_SUM:
    otlp_sum_destroy(&metric->data.sum);
    break;
  case OTLP_METRIC_DATA_HISTOGRAM:
    otlp_histogram_destroy(&metric->data.histogram);
    break;
  case OTLP_METRIC_DATA_EXPONENTIAL_HISTOGRAM:
    otlp_exponential_histogram_destroy(&metric->data.exponential_histogram);
    break;
  case OTLP_METRIC_DATA_SUMMARY:
    otlp_summary_destroy(&metric->data.summary);
    break;
  default:
    break;
  }
  metric->data_kind = OTLP_METRIC_DATA_NONE;
}

static inline otlp_status otlp_metric_copy(otlp_metric *dst,
                                           const otlp_metric *src) {
  size_t i;
  otlp_metric_init(dst);
  dst->data_kind = src->data_kind;
  if (otlp__set_string(&dst->name, src->name.ptr, src->name.len) !=
      OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  if (otlp__set_string(&dst->description, src->description.ptr,
                       src->description.len) != OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  if (otlp__set_string(&dst->unit, src->unit.ptr, src->unit.len) !=
      OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  switch (src->data_kind) {
  case OTLP_METRIC_DATA_GAUGE:
    if (otlp_gauge_copy(&dst->data.gauge, &src->data.gauge) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
    break;
  case OTLP_METRIC_DATA_SUM:
    if (otlp_sum_copy(&dst->data.sum, &src->data.sum) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
    break;
  case OTLP_METRIC_DATA_HISTOGRAM:
    if (otlp_histogram_copy(&dst->data.histogram, &src->data.histogram) !=
        OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
    break;
  case OTLP_METRIC_DATA_EXPONENTIAL_HISTOGRAM:
    if (otlp_exponential_histogram_copy(&dst->data.exponential_histogram,
                                        &src->data.exponential_histogram) !=
        OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
    break;
  case OTLP_METRIC_DATA_SUMMARY:
    if (otlp_summary_copy(&dst->data.summary, &src->data.summary) !=
        OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
    break;
  default:
    break;
  }
  for (i = 0; i < src->metadata.len; ++i) {
    otlp_key_value copy;
    otlp_status status = otlp_key_value_copy(&copy, &src->metadata.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status =
        otlp__append_primitive((void **)&dst->metadata.ptr, &dst->metadata.len,
                               &dst->metadata.cap, sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}
static inline size_t otlp_metric_encoded_size_inner(const otlp_metric *metric) {
  size_t size = metric->name.len ? otlp__bytes_field_size(1, metric->name.len) : 0;
  size_t i;
  if (metric->description.len)
    size += otlp__bytes_field_size(2, metric->description.len);
  if (metric->unit.len)
    size += otlp__bytes_field_size(3, metric->unit.len);
  switch (metric->data_kind) {
  case OTLP_METRIC_DATA_GAUGE:
    size += otlp__message_field_size(
        5, otlp_gauge_encoded_size_inner(&metric->data.gauge));
    break;
  case OTLP_METRIC_DATA_SUM:
    size += otlp__message_field_size(
        7, otlp_sum_encoded_size_inner(&metric->data.sum));
    break;
  case OTLP_METRIC_DATA_HISTOGRAM:
    size += otlp__message_field_size(
        9, otlp_histogram_encoded_size_inner(&metric->data.histogram));
    break;
  case OTLP_METRIC_DATA_EXPONENTIAL_HISTOGRAM:
    size += otlp__message_field_size(
        10, otlp_exponential_histogram_encoded_size_inner(
                &metric->data.exponential_histogram));
    break;
  case OTLP_METRIC_DATA_SUMMARY:
    size += otlp__message_field_size(
        11, otlp_summary_encoded_size_inner(&metric->data.summary));
    break;
  default:
    break;
  }
  for (i = 0; i < metric->metadata.len; ++i)
    size += otlp__message_field_size(
        12, otlp_key_value_encoded_size_inner(&metric->metadata.ptr[i]));
  return size;
}
static inline void otlp_metric_encode_field(otlp__writer *writer,
                                            uint32_t field_number,
                                            const otlp_metric *metric) {
  size_t i;
  size_t inner = otlp_metric_encoded_size_inner(metric);
  otlp__write_message_start(writer, field_number, inner);
  otlp__write_string_field(writer, 1, &metric->name);
  otlp__write_string_field(writer, 2, &metric->description);
  otlp__write_string_field(writer, 3, &metric->unit);
  switch (metric->data_kind) {
  case OTLP_METRIC_DATA_GAUGE:
    otlp_gauge_encode_field(writer, 5, &metric->data.gauge);
    break;
  case OTLP_METRIC_DATA_SUM:
    otlp_sum_encode_field(writer, 7, &metric->data.sum);
    break;
  case OTLP_METRIC_DATA_HISTOGRAM:
    otlp_histogram_encode_field(writer, 9, &metric->data.histogram);
    break;
  case OTLP_METRIC_DATA_EXPONENTIAL_HISTOGRAM:
    otlp_exponential_histogram_encode_field(
        writer, 10, &metric->data.exponential_histogram);
    break;
  case OTLP_METRIC_DATA_SUMMARY:
    otlp_summary_encode_field(writer, 11, &metric->data.summary);
    break;
  default:
    break;
  }
  for (i = 0; i < metric->metadata.len; ++i)
    otlp_key_value_encode_field(writer, 12, &metric->metadata.ptr[i]);
}
static inline otlp_status
otlp_metric_decode_inner(otlp_metric *metric, const uint8_t *data, size_t len) {
  otlp__reader reader;
  otlp_metric_destroy(metric);
  otlp_metric_init(metric);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    switch (field_number) {
    case 1:
    case 2:
    case 3: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if ((field_number == 1
               ? otlp__set_string(&metric->name, (const char *)ptr, size)
           : field_number == 2
               ? otlp__set_string(&metric->description, (const char *)ptr, size)
               : otlp__set_string(&metric->unit, (const char *)ptr, size)) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 5: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      otlp_metric_data_destroy(metric);
      metric->data_kind = OTLP_METRIC_DATA_GAUGE;
      if (otlp_gauge_decode_inner(&metric->data.gauge, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      break;
    }
    case 7: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      otlp_metric_data_destroy(metric);
      metric->data_kind = OTLP_METRIC_DATA_SUM;
      if (otlp_sum_decode_inner(&metric->data.sum, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      break;
    }
    case 9: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      otlp_metric_data_destroy(metric);
      metric->data_kind = OTLP_METRIC_DATA_HISTOGRAM;
      if (otlp_histogram_decode_inner(&metric->data.histogram, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      break;
    }
    case 10: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      otlp_metric_data_destroy(metric);
      metric->data_kind = OTLP_METRIC_DATA_EXPONENTIAL_HISTOGRAM;
      if (otlp_exponential_histogram_decode_inner(
              &metric->data.exponential_histogram, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      break;
    }
    case 11: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      otlp_metric_data_destroy(metric);
      metric->data_kind = OTLP_METRIC_DATA_SUMMARY;
      if (otlp_summary_decode_inner(&metric->data.summary, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      break;
    }
    case 12: {
      const uint8_t *ptr;
      size_t size;
      otlp_key_value kv;
      otlp_key_value_init(&kv);
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_key_value_decode_inner(&kv, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&metric->metadata.ptr,
                                 &metric->metadata.len, &metric->metadata.cap,
                                 sizeof(kv), &kv) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    default:
      if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
        return reader.status;
      break;
    }
  }
  return reader.status;
}

static inline void otlp_scope_metrics_init(otlp_scope_metrics *scope_metrics) {
  otlp__zero(scope_metrics, sizeof(*scope_metrics));
}
static inline void
otlp_scope_metrics_destroy(otlp_scope_metrics *scope_metrics) {
  size_t i;
  if (scope_metrics->has_scope)
    otlp_instrumentation_scope_destroy(&scope_metrics->scope);
  for (i = 0; i < scope_metrics->metrics.len; ++i)
    otlp_metric_destroy(&scope_metrics->metrics.ptr[i]);
  free(scope_metrics->metrics.ptr);
  free(scope_metrics->schema_url.ptr);
  otlp_scope_metrics_init(scope_metrics);
}

static inline otlp_status
otlp_scope_metrics_append_metric(otlp_scope_metrics *scope_metrics,
                                 const otlp_metric *metric) {
  otlp_metric copy;
  otlp_status status = otlp_metric_copy(&copy, metric);
  if (status != OTLP_STATUS_OK)
    return status;
  return otlp__append_primitive(
      (void **)&scope_metrics->metrics.ptr, &scope_metrics->metrics.len,
      &scope_metrics->metrics.cap, sizeof(copy), &copy);
}
static inline size_t
otlp_scope_metrics_encoded_size_inner(const otlp_scope_metrics *scope_metrics) {
  size_t size = 0;
  size_t i;
  if (scope_metrics->has_scope)
    size += otlp__message_field_size(
        1, otlp_instrumentation_scope_encoded_size_inner(&scope_metrics->scope));
  for (i = 0; i < scope_metrics->metrics.len; ++i)
    size += otlp__message_field_size(
        2, otlp_metric_encoded_size_inner(&scope_metrics->metrics.ptr[i]));
  if (scope_metrics->schema_url.len)
    size += otlp__bytes_field_size(3, scope_metrics->schema_url.len);
  return size;
}
static inline void
otlp_scope_metrics_encode_field(otlp__writer *writer, uint32_t field_number,
                                const otlp_scope_metrics *scope_metrics) {
  size_t i;
  size_t inner = otlp_scope_metrics_encoded_size_inner(scope_metrics);
  otlp__write_message_start(writer, field_number, inner);
  if (scope_metrics->has_scope)
    otlp_instrumentation_scope_encode_field(writer, 1, &scope_metrics->scope);
  for (i = 0; i < scope_metrics->metrics.len; ++i)
    otlp_metric_encode_field(writer, 2, &scope_metrics->metrics.ptr[i]);
  otlp__write_string_field(writer, 3, &scope_metrics->schema_url);
}

static inline otlp_status otlp_scope_metrics_copy(otlp_scope_metrics *dst,
                                                  const otlp_scope_metrics *src) {
  size_t i;
  otlp_scope_metrics_init(dst);
  if (src->has_scope) {
    dst->has_scope = 1;
    if (otlp_instrumentation_scope_copy(&dst->scope, &src->scope) !=
        OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  }
  if (otlp__set_string(&dst->schema_url, src->schema_url.ptr,
                       src->schema_url.len) != OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  for (i = 0; i < src->metrics.len; ++i) {
    otlp_status status =
        otlp_scope_metrics_append_metric(dst, &src->metrics.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}
static inline otlp_status
otlp_scope_metrics_decode_inner(otlp_scope_metrics *scope_metrics,
                                const uint8_t *data, size_t len) {
  otlp__reader reader;
  otlp_scope_metrics_destroy(scope_metrics);
  otlp_scope_metrics_init(scope_metrics);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if (field_number == 1 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      scope_metrics->has_scope = 1;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_instrumentation_scope_decode_inner(&scope_metrics->scope, ptr,
                                                  size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
    } else if (field_number == 2 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      otlp_metric metric;
      otlp_metric_init(&metric);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_metric_decode_inner(&metric, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&scope_metrics->metrics.ptr,
                                 &scope_metrics->metrics.len,
                                 &scope_metrics->metrics.cap, sizeof(metric),
                                 &metric) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (field_number == 3 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&scope_metrics->schema_url, (const char *)ptr,
                           size) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
      return reader.status;
  }
  return reader.status;
}

static inline void
otlp_resource_metrics_init(otlp_resource_metrics *resource_metrics) {
  otlp__zero(resource_metrics, sizeof(*resource_metrics));
}
static inline void
otlp_resource_metrics_destroy(otlp_resource_metrics *resource_metrics) {
  size_t i;
  if (resource_metrics->has_resource)
    otlp_resource_destroy(&resource_metrics->resource);
  for (i = 0; i < resource_metrics->scope_metrics.len; ++i)
    otlp_scope_metrics_destroy(&resource_metrics->scope_metrics.ptr[i]);
  free(resource_metrics->scope_metrics.ptr);
  free(resource_metrics->schema_url.ptr);
  otlp_resource_metrics_init(resource_metrics);
}
static inline otlp_status otlp_resource_metrics_append_scope_metrics(
    otlp_resource_metrics *resource_metrics,
    const otlp_scope_metrics *scope_metrics) {
  otlp_scope_metrics copy;
  otlp_scope_metrics_init(&copy);
  {
    otlp_status status = otlp_scope_metrics_copy(&copy, scope_metrics);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return otlp__append_primitive((void **)&resource_metrics->scope_metrics.ptr,
                                &resource_metrics->scope_metrics.len,
                                &resource_metrics->scope_metrics.cap,
                                sizeof(copy), &copy);
}

static inline otlp_status
otlp_resource_metrics_copy(otlp_resource_metrics *dst,
                           const otlp_resource_metrics *src) {
  size_t i;
  otlp_resource_metrics_init(dst);
  if (src->has_resource) {
    dst->has_resource = 1;
    if (otlp_resource_copy(&dst->resource, &src->resource) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  }
  if (otlp__set_string(&dst->schema_url, src->schema_url.ptr,
                       src->schema_url.len) != OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  for (i = 0; i < src->scope_metrics.len; ++i) {
    otlp_status status = otlp_resource_metrics_append_scope_metrics(
        dst, &src->scope_metrics.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}
static inline size_t otlp_resource_metrics_encoded_size_inner(
    const otlp_resource_metrics *resource_metrics) {
  size_t size = 0;
  size_t i;
  if (resource_metrics->has_resource)
    size += otlp__message_field_size(
        1, otlp_resource_encoded_size_inner(&resource_metrics->resource));
  for (i = 0; i < resource_metrics->scope_metrics.len; ++i)
    size += otlp__message_field_size(
        2, otlp_scope_metrics_encoded_size_inner(
               &resource_metrics->scope_metrics.ptr[i]));
  if (resource_metrics->schema_url.len)
    size += otlp__bytes_field_size(3, resource_metrics->schema_url.len);
  return size;
}

static inline void otlp_resource_metrics_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_resource_metrics *resource_metrics) {
  size_t i;
  size_t inner = otlp_resource_metrics_encoded_size_inner(resource_metrics);
  otlp__write_message_start(writer, field_number, inner);
  if (resource_metrics->has_resource)
    otlp_resource_encode_field(writer, 1, &resource_metrics->resource);
  for (i = 0; i < resource_metrics->scope_metrics.len; ++i)
    otlp_scope_metrics_encode_field(writer, 2,
                                    &resource_metrics->scope_metrics.ptr[i]);
  otlp__write_string_field(writer, 3, &resource_metrics->schema_url);
}
static inline otlp_status
otlp_resource_metrics_decode_inner(otlp_resource_metrics *resource_metrics,
                                   const uint8_t *data, size_t len) {
  otlp__reader reader;
  otlp_resource_metrics_destroy(resource_metrics);
  otlp_resource_metrics_init(resource_metrics);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if (field_number == 1 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      resource_metrics->has_resource = 1;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_resource_decode_inner(&resource_metrics->resource, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
    } else if (field_number == 2 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      otlp_scope_metrics scope_metrics;
      otlp_scope_metrics_init(&scope_metrics);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_scope_metrics_decode_inner(&scope_metrics, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&resource_metrics->scope_metrics.ptr,
                                 &resource_metrics->scope_metrics.len,
                                 &resource_metrics->scope_metrics.cap,
                                 sizeof(scope_metrics),
                                 &scope_metrics) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (field_number == 3 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&resource_metrics->schema_url, (const char *)ptr,
                           size) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
      return reader.status;
  }
  return reader.status;
}

static inline void otlp_export_metrics_service_request_init(
    otlp_export_metrics_service_request *request) {
  otlp__zero(request, sizeof(*request));
}
static inline void otlp_export_metrics_service_request_destroy(
    otlp_export_metrics_service_request *request) {
  size_t i;
  for (i = 0; i < request->resource_metrics.len; ++i)
    otlp_resource_metrics_destroy(&request->resource_metrics.ptr[i]);
  free(request->resource_metrics.ptr);
  otlp_export_metrics_service_request_init(request);
}
static inline otlp_status
otlp_export_metrics_service_request_append_resource_metrics(
    otlp_export_metrics_service_request *request,
    const otlp_resource_metrics *resource_metrics) {
  otlp_resource_metrics copy;
  otlp_resource_metrics_init(&copy);
  {
    otlp_status status = otlp_resource_metrics_copy(&copy, resource_metrics);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return otlp__append_primitive(
      (void **)&request->resource_metrics.ptr, &request->resource_metrics.len,
      &request->resource_metrics.cap, sizeof(copy), &copy);
}
static inline size_t otlp_export_metrics_service_request_encoded_size(
    const otlp_export_metrics_service_request *request) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < request->resource_metrics.len; ++i)
    size +=
        otlp__message_field_size(1, otlp_resource_metrics_encoded_size_inner(
                                        &request->resource_metrics.ptr[i]));
  return size;
}
static inline otlp_status otlp_export_metrics_service_request_encode(
    const otlp_export_metrics_service_request *request, uint8_t *out,
    size_t len) {
  otlp__writer writer;
  size_t i;
  otlp__writer_init(&writer, out, len);
  for (i = 0; i < request->resource_metrics.len; ++i)
    otlp_resource_metrics_encode_field(&writer, 1,
                                       &request->resource_metrics.ptr[i]);
  if (writer.status != OTLP_STATUS_OK)
    return writer.status;
  return writer.off == len ? OTLP_STATUS_OK : OTLP_STATUS_INVALID_ARGUMENT;
}
static inline otlp_status otlp_export_metrics_service_request_decode(
    otlp_export_metrics_service_request *request, const uint8_t *data,
    size_t len) {
  otlp__reader reader;
  otlp_export_metrics_service_request_destroy(request);
  otlp_export_metrics_service_request_init(request);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if (field_number == 1 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      otlp_resource_metrics resource_metrics;
      otlp_resource_metrics_init(&resource_metrics);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_resource_metrics_decode_inner(&resource_metrics, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive(
              (void **)&request->resource_metrics.ptr,
              &request->resource_metrics.len, &request->resource_metrics.cap,
              sizeof(resource_metrics), &resource_metrics) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
      return reader.status;
  }
  return reader.status;
}

static inline void otlp_metrics_data_init(otlp_metrics_data *data) {
  otlp__zero(data, sizeof(*data));
}

static inline void otlp_metrics_data_destroy(otlp_metrics_data *data) {
  size_t i;
  for (i = 0; i < data->resource_metrics.len; ++i)
    otlp_resource_metrics_destroy(&data->resource_metrics.ptr[i]);
  free(data->resource_metrics.ptr);
  otlp_metrics_data_init(data);
}

static inline size_t otlp_metrics_data_encoded_size(
    const otlp_metrics_data *data) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < data->resource_metrics.len; ++i)
    size += otlp__message_field_size(
        1, otlp_resource_metrics_encoded_size_inner(&data->resource_metrics.ptr[i]));
  return size;
}

static inline otlp_status otlp_metrics_data_encode(const otlp_metrics_data *data,
                                                   uint8_t *out, size_t len) {
  otlp__writer writer;
  size_t i;
  otlp__writer_init(&writer, out, len);
  for (i = 0; i < data->resource_metrics.len; ++i)
    otlp_resource_metrics_encode_field(&writer, 1,
                                       &data->resource_metrics.ptr[i]);
  if (writer.status != OTLP_STATUS_OK)
    return writer.status;
  return writer.off == len ? OTLP_STATUS_OK : OTLP_STATUS_INVALID_ARGUMENT;
}

static inline otlp_status otlp_metrics_data_decode(otlp_metrics_data *data,
                                                   const uint8_t *bytes,
                                                   size_t len) {
  otlp_export_metrics_service_request request;
  otlp_export_metrics_service_request_init(&request);
  if (otlp_export_metrics_service_request_decode(&request, bytes, len) !=
      OTLP_STATUS_OK) {
    otlp_export_metrics_service_request_destroy(&request);
    return OTLP_STATUS_DECODE_ERROR;
  }
  otlp_metrics_data_destroy(data);
  data->resource_metrics = request.resource_metrics;
  return OTLP_STATUS_OK;
}

static inline void otlp_export_metrics_partial_success_init(
    otlp_export_metrics_partial_success *partial_success) {
  otlp__zero(partial_success, sizeof(*partial_success));
}

static inline void otlp_export_metrics_partial_success_destroy(
    otlp_export_metrics_partial_success *partial_success) {
  free(partial_success->error_message.ptr);
  otlp_export_metrics_partial_success_init(partial_success);
}

static inline size_t otlp_export_metrics_partial_success_encoded_size_inner(
    const otlp_export_metrics_partial_success *partial_success) {
  size_t size = 0;
  if (partial_success->rejected_data_points)
    size += otlp__varint_field_size(
        1, (uint64_t)partial_success->rejected_data_points);
  if (partial_success->error_message.len)
    size += otlp__bytes_field_size(2, partial_success->error_message.len);
  return size;
}

static inline void otlp_export_metrics_partial_success_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_export_metrics_partial_success *partial_success) {
  size_t inner =
      otlp_export_metrics_partial_success_encoded_size_inner(partial_success);
  otlp__write_message_start(writer, field_number, inner);
  if (partial_success->rejected_data_points) {
    otlp__write_tag(writer, 1, 0);
    otlp__write_varint(writer, (uint64_t)partial_success->rejected_data_points);
  }
  otlp__write_string_field(writer, 2, &partial_success->error_message);
}

static inline otlp_status otlp_export_metrics_partial_success_decode_inner(
    otlp_export_metrics_partial_success *partial_success, const uint8_t *data,
    size_t len) {
  otlp__reader reader;
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if (field_number == 1 && wire_type == 0) {
      partial_success->rejected_data_points =
          (int64_t)otlp__read_varint(&reader);
    } else if (field_number == 2 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&partial_success->error_message, (const char *)ptr,
                           size) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }
  return reader.status;
}

static inline void otlp_export_metrics_service_response_init(
    otlp_export_metrics_service_response *response) {
  otlp__zero(response, sizeof(*response));
}

static inline void otlp_export_metrics_service_response_destroy(
    otlp_export_metrics_service_response *response) {
  if (response->has_partial_success)
    otlp_export_metrics_partial_success_destroy(&response->partial_success);
  otlp_export_metrics_service_response_init(response);
}

static inline size_t otlp_export_metrics_service_response_encoded_size(
    const otlp_export_metrics_service_response *response) {
  size_t size = 0;
  if (response->has_partial_success)
    size += otlp__message_field_size(
        1, otlp_export_metrics_partial_success_encoded_size_inner(
               &response->partial_success));
  return size;
}

static inline otlp_status otlp_export_metrics_service_response_encode(
    const otlp_export_metrics_service_response *response, uint8_t *out,
    size_t len) {
  otlp__writer writer;
  otlp__writer_init(&writer, out, len);
  if (response->has_partial_success)
    otlp_export_metrics_partial_success_encode_field(
        &writer, 1, &response->partial_success);
  if (writer.status != OTLP_STATUS_OK)
    return writer.status;
  return writer.off == len ? OTLP_STATUS_OK : OTLP_STATUS_INVALID_ARGUMENT;
}

static inline otlp_status otlp_export_metrics_service_response_decode(
    otlp_export_metrics_service_response *response, const uint8_t *data,
    size_t len) {
  otlp__reader reader;
  otlp_export_metrics_service_response_destroy(response);
  otlp_export_metrics_service_response_init(response);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if (field_number == 1 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      response->has_partial_success = 1;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_export_metrics_partial_success_decode_inner(
              &response->partial_success, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }
  return reader.status;
}
#endif

#endif /* OTLP_FORMAT_METRICS_H */
