#ifndef OTLP_FORMAT_LOGS_HPP
#define OTLP_FORMAT_LOGS_HPP

#include "../common/common.hpp"
#include "logs.h"

#if defined(__cplusplus) &&                                                     \
    (__cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)) &&     \
    defined(OTLP_FORMAT_ENABLE_LOGS)
namespace otlp {

class LogRecord : private otlp_log_record {
public:
  LogRecord() { otlp_log_record_init(this); }

  LogRecord(const LogRecord &other) {
    otlp_log_record_init(this);
    otlp_log_record_copy(this, other.get());
  }

  LogRecord &operator=(const LogRecord &other) {
    if (this != &other) {
      otlp_log_record_destroy(this);
      otlp_log_record_init(this);
      otlp_log_record_copy(this, other.get());
    }
    return *this;
  }

  ~LogRecord() { otlp_log_record_destroy(this); }

  void set_time_unix_nano(uint64_t value) {
    otlp_log_record_set_time_unix_nano(this, value);
  }

  void set_observed_time_unix_nano(uint64_t value) {
    this->observed_time_unix_nano = value;
  }

  void set_severity_number(otlp_severity_number value) {
    otlp_log_record_set_severity_number(this, value);
  }

  void set_severity_text(const char *value) {
    otlp__set_string(&this->severity_text, value, std::strlen(value));
  }

  void set_body(const AnyValue &value) {
    otlp_log_record_set_body(this, value.get());
  }

  void append_attribute(const KeyValue &attribute) {
    otlp_key_value copy;
    otlp_key_value_init(&copy);
    otlp_key_value_copy(&copy, attribute.get());
    otlp__append_primitive((void **)&this->attributes.ptr,
                           &this->attributes.len, &this->attributes.cap,
                           sizeof(copy), &copy);
  }

  void set_dropped_attributes_count(uint32_t value) {
    this->dropped_attributes_count = value;
  }

  void set_flags(uint32_t value) { this->flags = value; }

  void set_trace_id(const uint8_t *ptr, size_t len) {
    otlp__set_bytes(&this->trace_id, ptr, len);
  }

  void set_span_id(const uint8_t *ptr, size_t len) {
    otlp__set_bytes(&this->span_id, ptr, len);
  }

  void set_event_name(const char *value) {
    otlp__set_string(&this->event_name, value, std::strlen(value));
  }

  const otlp_log_record *get() const { return this; }

  friend class ScopeLogs;
};

class ScopeLogs : private otlp_scope_logs {
public:
  ScopeLogs() { otlp_scope_logs_init(this); }

  ScopeLogs(const ScopeLogs &other) {
    otlp_scope_logs_init(this);
    otlp_scope_logs_copy(this, other.get());
  }

  ScopeLogs &operator=(const ScopeLogs &other) {
    if (this != &other) {
      otlp_scope_logs_destroy(this);
      otlp_scope_logs_init(this);
      otlp_scope_logs_copy(this, other.get());
    }
    return *this;
  }

  ~ScopeLogs() { otlp_scope_logs_destroy(this); }

  void append_log_record(const LogRecord &record) {
    otlp_scope_logs_append_log_record(this, record.get());
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

  const otlp_scope_logs *get() const { return this; }

  friend class ResourceLogs;
};

class ResourceLogs : private otlp_resource_logs {
public:
  ResourceLogs() { otlp_resource_logs_init(this); }

  ResourceLogs(const ResourceLogs &other) {
    otlp_resource_logs_init(this);
    otlp_resource_logs_copy(this, other.get());
  }

  ResourceLogs &operator=(const ResourceLogs &other) {
    if (this != &other) {
      otlp_resource_logs_destroy(this);
      otlp_resource_logs_init(this);
      otlp_resource_logs_copy(this, other.get());
    }
    return *this;
  }

  ~ResourceLogs() { otlp_resource_logs_destroy(this); }

  void append_scope_logs(const ScopeLogs &scope_logs) {
    otlp_resource_logs_append_scope_logs(this, scope_logs.get());
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

  const otlp_resource_logs *get() const { return this; }

  friend class LogsExportRequest;
};

class LogsExportRequest : private otlp_export_logs_service_request {
public:
  LogsExportRequest() { otlp_export_logs_service_request_init(this); }

  LogsExportRequest(const LogsExportRequest &other) {
    otlp_export_logs_service_request_init(this);
    for (size_t i = 0; i < other.get()->resource_logs.len; ++i) {
      otlp_export_logs_service_request_append_resource_logs(
          this, &other.get()->resource_logs.ptr[i]);
    }
  }

  LogsExportRequest &operator=(const LogsExportRequest &other) {
    if (this != &other) {
      otlp_export_logs_service_request_destroy(this);
      otlp_export_logs_service_request_init(this);
      for (size_t i = 0; i < other.get()->resource_logs.len; ++i) {
        otlp_export_logs_service_request_append_resource_logs(
            this, &other.get()->resource_logs.ptr[i]);
      }
    }
    return *this;
  }

  ~LogsExportRequest() { otlp_export_logs_service_request_destroy(this); }

  void append_resource_logs(const ResourceLogs &resource_logs) {
    otlp_export_logs_service_request_append_resource_logs(this,
                                                          resource_logs.get());
  }

  size_t encoded_size() const {
    return otlp_export_logs_service_request_encoded_size(this);
  }

  otlp_status encode(uint8_t *out, size_t len) const {
    return otlp_export_logs_service_request_encode(this, out, len);
  }

  otlp_status decode(const uint8_t *data, size_t len) {
    return otlp_export_logs_service_request_decode(this, data, len);
  }

  const otlp_export_logs_service_request *get() const { return this; }
};

class LogsData : private otlp_logs_data {
public:
  LogsData() { otlp_logs_data_init(this); }

  LogsData(const LogsData &other) {
    otlp_logs_data_init(this);
    for (size_t i = 0; i < other.get()->resource_logs.len; ++i) {
      otlp_resource_logs copy;
      otlp_resource_logs_init(&copy);
      otlp_resource_logs_copy(&copy, &other.get()->resource_logs.ptr[i]);
      otlp__append_primitive((void **)&this->resource_logs.ptr,
                             &this->resource_logs.len,
                             &this->resource_logs.cap, sizeof(copy), &copy);
    }
  }

  LogsData &operator=(const LogsData &other) {
    if (this != &other) {
      otlp_logs_data_destroy(this);
      otlp_logs_data_init(this);
      for (size_t i = 0; i < other.get()->resource_logs.len; ++i) {
        otlp_resource_logs copy;
        otlp_resource_logs_init(&copy);
        otlp_resource_logs_copy(&copy, &other.get()->resource_logs.ptr[i]);
        otlp__append_primitive((void **)&this->resource_logs.ptr,
                               &this->resource_logs.len,
                               &this->resource_logs.cap, sizeof(copy), &copy);
      }
    }
    return *this;
  }

  ~LogsData() { otlp_logs_data_destroy(this); }

  void append_resource_logs(const ResourceLogs &resource_logs) {
    otlp_resource_logs copy;
    otlp_resource_logs_init(&copy);
    otlp_resource_logs_copy(&copy, resource_logs.get());
    otlp__append_primitive((void **)&this->resource_logs.ptr,
                           &this->resource_logs.len,
                           &this->resource_logs.cap, sizeof(copy), &copy);
  }

  size_t encoded_size() const { return otlp_logs_data_encoded_size(this); }

  otlp_status encode(uint8_t *out, size_t len) const {
    return otlp_logs_data_encode(this, out, len);
  }

  otlp_status decode(const uint8_t *data, size_t len) {
    return otlp_logs_data_decode(this, data, len);
  }

  const otlp_logs_data *get() const { return this; }
};

class LogsExportResponse : private otlp_export_logs_service_response {
public:
  LogsExportResponse() { otlp_export_logs_service_response_init(this); }

  LogsExportResponse(const LogsExportResponse &other) {
    otlp_export_logs_service_response_init(this);
    if (other.get()->has_partial_success) {
      this->has_partial_success = 1;
      this->partial_success.rejected_log_records =
          other.get()->partial_success.rejected_log_records;
      otlp__set_string(&this->partial_success.error_message,
                       other.get()->partial_success.error_message.ptr,
                       other.get()->partial_success.error_message.len);
    }
  }

  LogsExportResponse &operator=(const LogsExportResponse &other) {
    if (this != &other) {
      otlp_export_logs_service_response_destroy(this);
      otlp_export_logs_service_response_init(this);
      if (other.get()->has_partial_success) {
        this->has_partial_success = 1;
        this->partial_success.rejected_log_records =
            other.get()->partial_success.rejected_log_records;
        otlp__set_string(&this->partial_success.error_message,
                         other.get()->partial_success.error_message.ptr,
                         other.get()->partial_success.error_message.len);
      }
    }
    return *this;
  }

  ~LogsExportResponse() { otlp_export_logs_service_response_destroy(this); }

  void set_partial_success(int64_t rejected_log_records,
                           const char *error_message) {
    this->has_partial_success = 1;
    this->partial_success.rejected_log_records = rejected_log_records;
    otlp__set_string(&this->partial_success.error_message, error_message,
                     std::strlen(error_message));
  }

  size_t encoded_size() const {
    return otlp_export_logs_service_response_encoded_size(this);
  }

  otlp_status encode(uint8_t *out, size_t len) const {
    return otlp_export_logs_service_response_encode(this, out, len);
  }

  otlp_status decode(const uint8_t *data, size_t len) {
    return otlp_export_logs_service_response_decode(this, data, len);
  }

  const otlp_export_logs_service_response *get() const { return this; }
};

} /* namespace otlp */
#endif

#endif /* OTLP_FORMAT_LOGS_HPP */
