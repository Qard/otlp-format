#ifndef OTLP_FORMAT_METRICS_HPP
#define OTLP_FORMAT_METRICS_HPP

#include "../common/common.hpp"
#include "metrics.h"

#if defined(__cplusplus) &&                                                     \
    (__cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)) &&     \
    defined(OTLP_FORMAT_ENABLE_METRICS)
namespace otlp {

class Exemplar : private otlp_exemplar {
public:
  Exemplar() { otlp_exemplar_init(this); }

  Exemplar(const Exemplar &other) {
    otlp_exemplar_init(this);
    otlp_exemplar_copy(this, other.get());
  }

  Exemplar &operator=(const Exemplar &other) {
    if (this != &other) {
      otlp_exemplar_destroy(this);
      otlp_exemplar_init(this);
      otlp_exemplar_copy(this, other.get());
    }
    return *this;
  }

  ~Exemplar() { otlp_exemplar_destroy(this); }

  void set_time_unix_nano(uint64_t value) { this->time_unix_nano = value; }

  void set_as_double(double value) {
    this->value_kind = OTLP_EXEMPLAR_VALUE_DOUBLE;
    this->as_double = value;
  }

  void set_as_int(int64_t value) {
    this->value_kind = OTLP_EXEMPLAR_VALUE_INT;
    this->as_int = value;
  }

  void set_span_id(const uint8_t *ptr, size_t len) {
    otlp__set_bytes(&this->span_id, ptr, len);
  }

  void set_trace_id(const uint8_t *ptr, size_t len) {
    otlp__set_bytes(&this->trace_id, ptr, len);
  }

  void append_filtered_attribute(const KeyValue &attribute) {
    otlp_key_value copy;
    otlp_key_value_init(&copy);
    otlp_key_value_copy(&copy, attribute.get());
    otlp__append_primitive((void **)&this->filtered_attributes.ptr,
                           &this->filtered_attributes.len,
                           &this->filtered_attributes.cap, sizeof(copy),
                           &copy);
  }

  const otlp_exemplar *get() const { return this; }
};

class NumberDataPoint : private otlp_number_data_point {
public:
  NumberDataPoint() { otlp_number_data_point_init(this); }

  NumberDataPoint(const NumberDataPoint &other) {
    otlp_number_data_point_init(this);
    otlp_number_data_point_copy(this, other.get());
  }

  NumberDataPoint &operator=(const NumberDataPoint &other) {
    if (this != &other) {
      otlp_number_data_point_destroy(this);
      otlp_number_data_point_init(this);
      otlp_number_data_point_copy(this, other.get());
    }
    return *this;
  }

  ~NumberDataPoint() { otlp_number_data_point_destroy(this); }

  void append_attribute(const KeyValue &attribute) {
    otlp_key_value copy;
    otlp_key_value_init(&copy);
    otlp_key_value_copy(&copy, attribute.get());
    otlp__append_primitive((void **)&this->attributes.ptr,
                           &this->attributes.len, &this->attributes.cap,
                           sizeof(copy), &copy);
  }

  void set_start_time_unix_nano(uint64_t value) {
    this->start_time_unix_nano = value;
  }

  void set_time_unix_nano(uint64_t value) { this->time_unix_nano = value; }

  void set_as_double(double value) {
    this->value_kind = OTLP_NUMBER_VALUE_DOUBLE;
    this->as_double = value;
  }

  void set_as_int(int64_t value) {
    this->value_kind = OTLP_NUMBER_VALUE_INT;
    this->as_int = value;
  }

  void append_exemplar(const Exemplar &exemplar) {
    otlp_exemplar copy;
    otlp_exemplar_init(&copy);
    otlp_exemplar_copy(&copy, exemplar.get());
    otlp__append_primitive((void **)&this->exemplars.ptr,
                           &this->exemplars.len, &this->exemplars.cap,
                           sizeof(copy), &copy);
  }

  void set_flags(uint32_t value) { this->flags = value; }

  const otlp_number_data_point *get() const { return this; }
};

class HistogramDataPoint : private otlp_histogram_data_point {
public:
  HistogramDataPoint() { otlp_histogram_data_point_init(this); }

  HistogramDataPoint(const HistogramDataPoint &other) {
    otlp_histogram_data_point_init(this);
    otlp_histogram_data_point_copy(this, other.get());
  }

  HistogramDataPoint &operator=(const HistogramDataPoint &other) {
    if (this != &other) {
      otlp_histogram_data_point_destroy(this);
      otlp_histogram_data_point_init(this);
      otlp_histogram_data_point_copy(this, other.get());
    }
    return *this;
  }

  ~HistogramDataPoint() { otlp_histogram_data_point_destroy(this); }

  void append_attribute(const KeyValue &attribute) {
    otlp_key_value copy;
    otlp_key_value_init(&copy);
    otlp_key_value_copy(&copy, attribute.get());
    otlp__append_primitive((void **)&this->attributes.ptr,
                           &this->attributes.len, &this->attributes.cap,
                           sizeof(copy), &copy);
  }

  void set_start_time_unix_nano(uint64_t value) {
    this->start_time_unix_nano = value;
  }

  void set_time_unix_nano(uint64_t value) { this->time_unix_nano = value; }

  void set_count(uint64_t value) { this->count = value; }

  void set_sum(double value) {
    this->has_sum = 1;
    this->sum = value;
  }

  void append_bucket_count(uint64_t value) {
    otlp__append_primitive((void **)&this->bucket_counts.ptr,
                           &this->bucket_counts.len,
                           &this->bucket_counts.cap, sizeof(value), &value);
  }

  void append_explicit_bound(double value) {
    otlp__append_primitive((void **)&this->explicit_bounds.ptr,
                           &this->explicit_bounds.len,
                           &this->explicit_bounds.cap, sizeof(value), &value);
  }

  void append_exemplar(const Exemplar &exemplar) {
    otlp_exemplar copy;
    otlp_exemplar_init(&copy);
    otlp_exemplar_copy(&copy, exemplar.get());
    otlp__append_primitive((void **)&this->exemplars.ptr,
                           &this->exemplars.len, &this->exemplars.cap,
                           sizeof(copy), &copy);
  }

  void set_flags(uint32_t value) { this->flags = value; }

  void set_min(double value) {
    this->has_min = 1;
    this->min = value;
  }

  void set_max(double value) {
    this->has_max = 1;
    this->max = value;
  }

  const otlp_histogram_data_point *get() const { return this; }
};

class ExponentialHistogramBuckets : private otlp_exponential_histogram_buckets {
public:
  ExponentialHistogramBuckets() {
    otlp_exponential_histogram_buckets_init(this);
  }

  ExponentialHistogramBuckets(const ExponentialHistogramBuckets &other) {
    otlp_exponential_histogram_buckets_init(this);
    otlp_exponential_histogram_buckets_copy(this, other.get());
  }

  ExponentialHistogramBuckets &
  operator=(const ExponentialHistogramBuckets &other) {
    if (this != &other) {
      otlp_exponential_histogram_buckets_destroy(this);
      otlp_exponential_histogram_buckets_init(this);
      otlp_exponential_histogram_buckets_copy(this, other.get());
    }
    return *this;
  }

  ~ExponentialHistogramBuckets() {
    otlp_exponential_histogram_buckets_destroy(this);
  }

  void set_offset(int32_t value) { this->offset = value; }

  void append_bucket_count(uint64_t value) {
    otlp__append_primitive((void **)&this->bucket_counts.ptr,
                           &this->bucket_counts.len,
                           &this->bucket_counts.cap, sizeof(value), &value);
  }

  const otlp_exponential_histogram_buckets *get() const { return this; }
};

class ExponentialHistogramDataPoint
    : private otlp_exponential_histogram_data_point {
public:
  ExponentialHistogramDataPoint() {
    otlp_exponential_histogram_data_point_init(this);
  }

  ExponentialHistogramDataPoint(const ExponentialHistogramDataPoint &other) {
    otlp_exponential_histogram_data_point_init(this);
    otlp_exponential_histogram_data_point_copy(this, other.get());
  }

  ExponentialHistogramDataPoint &
  operator=(const ExponentialHistogramDataPoint &other) {
    if (this != &other) {
      otlp_exponential_histogram_data_point_destroy(this);
      otlp_exponential_histogram_data_point_init(this);
      otlp_exponential_histogram_data_point_copy(this, other.get());
    }
    return *this;
  }

  ~ExponentialHistogramDataPoint() {
    otlp_exponential_histogram_data_point_destroy(this);
  }

  void append_attribute(const KeyValue &attribute) {
    otlp_key_value copy;
    otlp_key_value_init(&copy);
    otlp_key_value_copy(&copy, attribute.get());
    otlp__append_primitive((void **)&this->attributes.ptr,
                           &this->attributes.len, &this->attributes.cap,
                           sizeof(copy), &copy);
  }

  void set_start_time_unix_nano(uint64_t value) {
    this->start_time_unix_nano = value;
  }

  void set_time_unix_nano(uint64_t value) { this->time_unix_nano = value; }

  void set_count(uint64_t value) { this->count = value; }

  void set_sum(double value) {
    this->has_sum = 1;
    this->sum = value;
  }

  void set_scale(int32_t value) { this->scale = value; }

  void set_zero_count(uint64_t value) { this->zero_count = value; }

  void set_zero_threshold(double value) { this->zero_threshold = value; }

  void set_positive(const ExponentialHistogramBuckets &value) {
    if (this->has_positive) {
      otlp_exponential_histogram_buckets_destroy(&this->positive);
      otlp_exponential_histogram_buckets_init(&this->positive);
    }
    this->has_positive = 1;
    otlp_exponential_histogram_buckets_copy(&this->positive, value.get());
  }

  void set_negative(const ExponentialHistogramBuckets &value) {
    if (this->has_negative) {
      otlp_exponential_histogram_buckets_destroy(&this->negative);
      otlp_exponential_histogram_buckets_init(&this->negative);
    }
    this->has_negative = 1;
    otlp_exponential_histogram_buckets_copy(&this->negative, value.get());
  }

  void append_exemplar(const Exemplar &exemplar) {
    otlp_exemplar copy;
    otlp_exemplar_init(&copy);
    otlp_exemplar_copy(&copy, exemplar.get());
    otlp__append_primitive((void **)&this->exemplars.ptr,
                           &this->exemplars.len, &this->exemplars.cap,
                           sizeof(copy), &copy);
  }

  void set_flags(uint32_t value) { this->flags = value; }

  void set_min(double value) {
    this->has_min = 1;
    this->min = value;
  }

  void set_max(double value) {
    this->has_max = 1;
    this->max = value;
  }

  const otlp_exponential_histogram_data_point *get() const { return this; }
};

class SummaryDataPointValueAtQuantile
    : private otlp_summary_data_point_value_at_quantile {
public:
  SummaryDataPointValueAtQuantile() {
    otlp_summary_data_point_value_at_quantile_init(this);
  }

  void set_quantile(double value) { this->quantile = value; }

  void set_value(double value) { this->value = value; }

  const otlp_summary_data_point_value_at_quantile *get() const { return this; }
};

class SummaryDataPoint : private otlp_summary_data_point {
public:
  SummaryDataPoint() { otlp_summary_data_point_init(this); }

  SummaryDataPoint(const SummaryDataPoint &other) {
    otlp_summary_data_point_init(this);
    otlp_summary_data_point_copy(this, other.get());
  }

  SummaryDataPoint &operator=(const SummaryDataPoint &other) {
    if (this != &other) {
      otlp_summary_data_point_destroy(this);
      otlp_summary_data_point_init(this);
      otlp_summary_data_point_copy(this, other.get());
    }
    return *this;
  }

  ~SummaryDataPoint() { otlp_summary_data_point_destroy(this); }

  void append_attribute(const KeyValue &attribute) {
    otlp_key_value copy;
    otlp_key_value_init(&copy);
    otlp_key_value_copy(&copy, attribute.get());
    otlp__append_primitive((void **)&this->attributes.ptr,
                           &this->attributes.len, &this->attributes.cap,
                           sizeof(copy), &copy);
  }

  void set_start_time_unix_nano(uint64_t value) {
    this->start_time_unix_nano = value;
  }

  void set_time_unix_nano(uint64_t value) { this->time_unix_nano = value; }

  void set_count(uint64_t value) { this->count = value; }

  void set_sum(double value) {
    this->sum = value;
  }

  void append_quantile_value(const SummaryDataPointValueAtQuantile &value) {
    otlp_summary_data_point_value_at_quantile copy;
    otlp_summary_data_point_value_at_quantile_init(&copy);
    copy = *value.get();
    otlp__append_primitive((void **)&this->quantile_values.ptr,
                           &this->quantile_values.len,
                           &this->quantile_values.cap, sizeof(copy), &copy);
  }

  void set_flags(uint32_t value) { this->flags = value; }

  const otlp_summary_data_point *get() const { return this; }
};

class Gauge : private otlp_gauge {
public:
  Gauge() { otlp_gauge_init(this); }

  Gauge(const Gauge &other) {
    otlp_gauge_init(this);
    otlp_gauge_copy(this, other.get());
  }

  Gauge &operator=(const Gauge &other) {
    if (this != &other) {
      otlp_gauge_destroy(this);
      otlp_gauge_init(this);
      otlp_gauge_copy(this, other.get());
    }
    return *this;
  }

  ~Gauge() { otlp_gauge_destroy(this); }

  void append_data_point(const NumberDataPoint &point) {
    otlp_number_data_point copy;
    otlp_number_data_point_init(&copy);
    otlp_number_data_point_copy(&copy, point.get());
    otlp__append_primitive((void **)&this->data_points.ptr,
                           &this->data_points.len, &this->data_points.cap,
                           sizeof(copy), &copy);
  }

  const otlp_gauge *get() const { return this; }
};

class Sum : private otlp_sum {
public:
  Sum() { otlp_sum_init(this); }

  Sum(const Sum &other) {
    otlp_sum_init(this);
    otlp_sum_copy(this, other.get());
  }

  Sum &operator=(const Sum &other) {
    if (this != &other) {
      otlp_sum_destroy(this);
      otlp_sum_init(this);
      otlp_sum_copy(this, other.get());
    }
    return *this;
  }

  ~Sum() { otlp_sum_destroy(this); }

  void append_data_point(const NumberDataPoint &point) {
    otlp_number_data_point copy;
    otlp_number_data_point_init(&copy);
    otlp_number_data_point_copy(&copy, point.get());
    otlp__append_primitive((void **)&this->data_points.ptr,
                           &this->data_points.len, &this->data_points.cap,
                           sizeof(copy), &copy);
  }

  void set_aggregation_temporality(otlp_aggregation_temporality value) {
    this->aggregation_temporality = value;
  }

  void set_is_monotonic(bool value) { this->is_monotonic = value ? 1 : 0; }

  const otlp_sum *get() const { return this; }
};

class Histogram : private otlp_histogram {
public:
  Histogram() { otlp_histogram_init(this); }

  Histogram(const Histogram &other) {
    otlp_histogram_init(this);
    otlp_histogram_copy(this, other.get());
  }

  Histogram &operator=(const Histogram &other) {
    if (this != &other) {
      otlp_histogram_destroy(this);
      otlp_histogram_init(this);
      otlp_histogram_copy(this, other.get());
    }
    return *this;
  }

  ~Histogram() { otlp_histogram_destroy(this); }

  void append_data_point(const HistogramDataPoint &point) {
    otlp_histogram_data_point copy;
    otlp_histogram_data_point_init(&copy);
    otlp_histogram_data_point_copy(&copy, point.get());
    otlp__append_primitive((void **)&this->data_points.ptr,
                           &this->data_points.len, &this->data_points.cap,
                           sizeof(copy), &copy);
  }

  void set_aggregation_temporality(otlp_aggregation_temporality value) {
    this->aggregation_temporality = value;
  }

  const otlp_histogram *get() const { return this; }
};

class ExponentialHistogram : private otlp_exponential_histogram {
public:
  ExponentialHistogram() { otlp_exponential_histogram_init(this); }

  ExponentialHistogram(const ExponentialHistogram &other) {
    otlp_exponential_histogram_init(this);
    otlp_exponential_histogram_copy(this, other.get());
  }

  ExponentialHistogram &operator=(const ExponentialHistogram &other) {
    if (this != &other) {
      otlp_exponential_histogram_destroy(this);
      otlp_exponential_histogram_init(this);
      otlp_exponential_histogram_copy(this, other.get());
    }
    return *this;
  }

  ~ExponentialHistogram() { otlp_exponential_histogram_destroy(this); }

  void append_data_point(const ExponentialHistogramDataPoint &point) {
    otlp_exponential_histogram_data_point copy;
    otlp_exponential_histogram_data_point_init(&copy);
    otlp_exponential_histogram_data_point_copy(&copy, point.get());
    otlp__append_primitive((void **)&this->data_points.ptr,
                           &this->data_points.len, &this->data_points.cap,
                           sizeof(copy), &copy);
  }

  void set_aggregation_temporality(otlp_aggregation_temporality value) {
    this->aggregation_temporality = value;
  }

  const otlp_exponential_histogram *get() const { return this; }
};

class Summary : private otlp_summary {
public:
  Summary() { otlp_summary_init(this); }

  Summary(const Summary &other) {
    otlp_summary_init(this);
    otlp_summary_copy(this, other.get());
  }

  Summary &operator=(const Summary &other) {
    if (this != &other) {
      otlp_summary_destroy(this);
      otlp_summary_init(this);
      otlp_summary_copy(this, other.get());
    }
    return *this;
  }

  ~Summary() { otlp_summary_destroy(this); }

  void append_data_point(const SummaryDataPoint &point) {
    otlp_summary_data_point copy;
    otlp_summary_data_point_init(&copy);
    otlp_summary_data_point_copy(&copy, point.get());
    otlp__append_primitive((void **)&this->data_points.ptr,
                           &this->data_points.len, &this->data_points.cap,
                           sizeof(copy), &copy);
  }

  const otlp_summary *get() const { return this; }
};

class Metric : private otlp_metric {
public:
  Metric() { otlp_metric_init(this); }

  Metric(const Metric &other) {
    otlp_metric_init(this);
    otlp_metric_copy(this, other.get());
  }

  Metric &operator=(const Metric &other) {
    if (this != &other) {
      otlp_metric_destroy(this);
      otlp_metric_init(this);
      otlp_metric_copy(this, other.get());
    }
    return *this;
  }

  ~Metric() { otlp_metric_destroy(this); }

  void set_name(const char *value) {
    otlp__set_string(&this->name, value, std::strlen(value));
  }

  void set_description(const char *value) {
    otlp__set_string(&this->description, value, std::strlen(value));
  }

  void set_unit(const char *value) {
    otlp__set_string(&this->unit, value, std::strlen(value));
  }

  void append_metadata(const KeyValue &metadata) {
    otlp_key_value copy;
    otlp_key_value_init(&copy);
    otlp_key_value_copy(&copy, metadata.get());
    otlp__append_primitive((void **)&this->metadata.ptr,
                           &this->metadata.len, &this->metadata.cap,
                           sizeof(copy), &copy);
  }

  void set_gauge(const otlp_gauge &value) {
    otlp_metric_data_destroy(this);
    this->data_kind = OTLP_METRIC_DATA_GAUGE;
    otlp_gauge_copy(&this->data.gauge, &value);
  }

  void set_gauge(const Gauge &value) { set_gauge(*value.get()); }

  void set_sum(const otlp_sum &value) {
    otlp_metric_data_destroy(this);
    this->data_kind = OTLP_METRIC_DATA_SUM;
    otlp_sum_copy(&this->data.sum, &value);
  }

  void set_sum(const Sum &value) { set_sum(*value.get()); }

  void set_histogram(const otlp_histogram &value) {
    otlp_metric_data_destroy(this);
    this->data_kind = OTLP_METRIC_DATA_HISTOGRAM;
    otlp_histogram_copy(&this->data.histogram, &value);
  }

  void set_histogram(const Histogram &value) { set_histogram(*value.get()); }

  void set_exponential_histogram(const otlp_exponential_histogram &value) {
    otlp_metric_data_destroy(this);
    this->data_kind = OTLP_METRIC_DATA_EXPONENTIAL_HISTOGRAM;
    otlp_exponential_histogram_copy(&this->data.exponential_histogram, &value);
  }

  void set_exponential_histogram(const ExponentialHistogram &value) {
    set_exponential_histogram(*value.get());
  }

  void set_summary(const otlp_summary &value) {
    otlp_metric_data_destroy(this);
    this->data_kind = OTLP_METRIC_DATA_SUMMARY;
    otlp_summary_copy(&this->data.summary, &value);
  }

  void set_summary(const Summary &value) { set_summary(*value.get()); }

  otlp_metric *get() { return this; }

  const otlp_metric *get() const { return this; }
};

class ScopeMetrics : private otlp_scope_metrics {
public:
  ScopeMetrics() { otlp_scope_metrics_init(this); }

  ScopeMetrics(const ScopeMetrics &other) {
    otlp_scope_metrics_init(this);
    otlp_scope_metrics_copy(this, other.get());
  }

  ScopeMetrics &operator=(const ScopeMetrics &other) {
    if (this != &other) {
      otlp_scope_metrics_destroy(this);
      otlp_scope_metrics_init(this);
      otlp_scope_metrics_copy(this, other.get());
    }
    return *this;
  }

  ~ScopeMetrics() { otlp_scope_metrics_destroy(this); }

  void append_metric(const Metric &metric) {
    otlp_scope_metrics_append_metric(this, metric.get());
  }

  void set_scope(const InstrumentationScope &scope) {
    if (this->has_scope) {
      otlp_instrumentation_scope_destroy(&this->scope);
      otlp_instrumentation_scope_init(&this->scope);
    }
    this->has_scope = 1;
    otlp_instrumentation_scope_copy(&this->scope, scope.get());
  }

  void set_schema_url(const char *value) {
    otlp__set_string(&this->schema_url, value, std::strlen(value));
  }

  const otlp_scope_metrics *get() const { return this; }
};

class ResourceMetrics : private otlp_resource_metrics {
public:
  ResourceMetrics() { otlp_resource_metrics_init(this); }

  ResourceMetrics(const ResourceMetrics &other) {
    otlp_resource_metrics_init(this);
    otlp_resource_metrics_copy(this, other.get());
  }

  ResourceMetrics &operator=(const ResourceMetrics &other) {
    if (this != &other) {
      otlp_resource_metrics_destroy(this);
      otlp_resource_metrics_init(this);
      otlp_resource_metrics_copy(this, other.get());
    }
    return *this;
  }

  ~ResourceMetrics() { otlp_resource_metrics_destroy(this); }

  void append_scope_metrics(const ScopeMetrics &scope_metrics) {
    otlp_resource_metrics_append_scope_metrics(this, scope_metrics.get());
  }

  void set_resource(const Resource &resource) {
    if (this->has_resource) {
      otlp_resource_destroy(&this->resource);
      otlp_resource_init(&this->resource);
    }
    this->has_resource = 1;
    otlp_resource_copy(&this->resource, resource.get());
  }

  void set_schema_url(const char *value) {
    otlp__set_string(&this->schema_url, value, std::strlen(value));
  }

  const otlp_resource_metrics *get() const { return this; }
};

class MetricsExportRequest : private otlp_export_metrics_service_request {
public:
  MetricsExportRequest() { otlp_export_metrics_service_request_init(this); }

  MetricsExportRequest(const MetricsExportRequest &other) {
    otlp_export_metrics_service_request_init(this);
    for (size_t i = 0; i < other.get()->resource_metrics.len; ++i) {
      otlp_export_metrics_service_request_append_resource_metrics(
          this, &other.get()->resource_metrics.ptr[i]);
    }
  }

  MetricsExportRequest &operator=(const MetricsExportRequest &other) {
    if (this != &other) {
      otlp_export_metrics_service_request_destroy(this);
      otlp_export_metrics_service_request_init(this);
      for (size_t i = 0; i < other.get()->resource_metrics.len; ++i) {
        otlp_export_metrics_service_request_append_resource_metrics(
            this, &other.get()->resource_metrics.ptr[i]);
      }
    }
    return *this;
  }

  ~MetricsExportRequest() { otlp_export_metrics_service_request_destroy(this); }

  void append_resource_metrics(const ResourceMetrics &resource_metrics) {
    otlp_export_metrics_service_request_append_resource_metrics(
        this, resource_metrics.get());
  }

  size_t encoded_size() const {
    return otlp_export_metrics_service_request_encoded_size(this);
  }

  otlp_status encode(uint8_t *out, size_t len) const {
    return otlp_export_metrics_service_request_encode(this, out, len);
  }

  otlp_status decode(const uint8_t *data, size_t len) {
    return otlp_export_metrics_service_request_decode(this, data, len);
  }

  const otlp_export_metrics_service_request *get() const { return this; }
};

class MetricsData : private otlp_metrics_data {
public:
  MetricsData() { otlp_metrics_data_init(this); }

  MetricsData(const MetricsData &other) {
    otlp_metrics_data_init(this);
    for (size_t i = 0; i < other.get()->resource_metrics.len; ++i) {
      otlp_resource_metrics copy;
      otlp_resource_metrics_init(&copy);
      otlp_resource_metrics_copy(&copy, &other.get()->resource_metrics.ptr[i]);
      otlp__append_primitive((void **)&this->resource_metrics.ptr,
                             &this->resource_metrics.len,
                             &this->resource_metrics.cap, sizeof(copy), &copy);
    }
  }

  MetricsData &operator=(const MetricsData &other) {
    if (this != &other) {
      otlp_metrics_data_destroy(this);
      otlp_metrics_data_init(this);
      for (size_t i = 0; i < other.get()->resource_metrics.len; ++i) {
        otlp_resource_metrics copy;
        otlp_resource_metrics_init(&copy);
        otlp_resource_metrics_copy(&copy, &other.get()->resource_metrics.ptr[i]);
        otlp__append_primitive((void **)&this->resource_metrics.ptr,
                               &this->resource_metrics.len,
                               &this->resource_metrics.cap, sizeof(copy), &copy);
      }
    }
    return *this;
  }

  ~MetricsData() { otlp_metrics_data_destroy(this); }

  void append_resource_metrics(const ResourceMetrics &resource_metrics) {
    otlp_resource_metrics copy;
    otlp_resource_metrics_init(&copy);
    otlp_resource_metrics_copy(&copy, resource_metrics.get());
    otlp__append_primitive((void **)&this->resource_metrics.ptr,
                           &this->resource_metrics.len,
                           &this->resource_metrics.cap, sizeof(copy), &copy);
  }

  size_t encoded_size() const { return otlp_metrics_data_encoded_size(this); }

  otlp_status encode(uint8_t *out, size_t len) const {
    return otlp_metrics_data_encode(this, out, len);
  }

  otlp_status decode(const uint8_t *data, size_t len) {
    return otlp_metrics_data_decode(this, data, len);
  }

  const otlp_metrics_data *get() const { return this; }
};

class MetricsExportResponse : private otlp_export_metrics_service_response {
public:
  MetricsExportResponse() { otlp_export_metrics_service_response_init(this); }

  MetricsExportResponse(const MetricsExportResponse &other) {
    otlp_export_metrics_service_response_init(this);
    if (other.get()->has_partial_success) {
      this->has_partial_success = 1;
      this->partial_success.rejected_data_points =
          other.get()->partial_success.rejected_data_points;
      otlp__set_string(&this->partial_success.error_message,
                       other.get()->partial_success.error_message.ptr,
                       other.get()->partial_success.error_message.len);
    }
  }

  MetricsExportResponse &operator=(const MetricsExportResponse &other) {
    if (this != &other) {
      otlp_export_metrics_service_response_destroy(this);
      otlp_export_metrics_service_response_init(this);
      if (other.get()->has_partial_success) {
        this->has_partial_success = 1;
        this->partial_success.rejected_data_points =
            other.get()->partial_success.rejected_data_points;
        otlp__set_string(&this->partial_success.error_message,
                         other.get()->partial_success.error_message.ptr,
                         other.get()->partial_success.error_message.len);
      }
    }
    return *this;
  }

  ~MetricsExportResponse() {
    otlp_export_metrics_service_response_destroy(this);
  }

  void set_partial_success(int64_t rejected_data_points,
                           const char *error_message) {
    this->has_partial_success = 1;
    this->partial_success.rejected_data_points = rejected_data_points;
    otlp__set_string(&this->partial_success.error_message, error_message,
                     std::strlen(error_message));
  }

  size_t encoded_size() const {
    return otlp_export_metrics_service_response_encoded_size(this);
  }

  otlp_status encode(uint8_t *out, size_t len) const {
    return otlp_export_metrics_service_response_encode(this, out, len);
  }

  otlp_status decode(const uint8_t *data, size_t len) {
    return otlp_export_metrics_service_response_decode(this, data, len);
  }

  const otlp_export_metrics_service_response *get() const { return this; }
};

} /* namespace otlp */
#endif

#endif /* OTLP_FORMAT_METRICS_HPP */
