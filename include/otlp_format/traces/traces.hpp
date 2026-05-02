#ifndef OTLP_FORMAT_TRACES_HPP
#define OTLP_FORMAT_TRACES_HPP

#include "../common/common.hpp"
#include "traces.h"

#if defined(__cplusplus) &&                                                     \
    (__cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)) &&     \
    defined(OTLP_FORMAT_ENABLE_TRACES)
namespace otlp {

class TraceStatus : private otlp_trace_status {
public:
  TraceStatus() { otlp_trace_status_init(this); }

  TraceStatus(const TraceStatus &other) {
    otlp_trace_status_init(this);
    otlp_trace_status_copy(this, other.get());
  }

  TraceStatus &operator=(const TraceStatus &other) {
    if (this != &other) {
      otlp_trace_status_destroy(this);
      otlp_trace_status_init(this);
      otlp_trace_status_copy(this, other.get());
    }
    return *this;
  }

  ~TraceStatus() { otlp_trace_status_destroy(this); }

  void set_code(otlp_trace_status_code value) { this->code = value; }

  void set_message(const char *value) {
    otlp__set_string(&this->message, value, std::strlen(value));
  }

  const otlp_trace_status *get() const { return this; }
};

class SpanEvent : private otlp_span_event {
public:
  SpanEvent() { otlp_span_event_init(this); }

  SpanEvent(const SpanEvent &other) {
    otlp_span_event_init(this);
    otlp_span_event_copy(this, other.get());
  }

  SpanEvent &operator=(const SpanEvent &other) {
    if (this != &other) {
      otlp_span_event_destroy(this);
      otlp_span_event_init(this);
      otlp_span_event_copy(this, other.get());
    }
    return *this;
  }

  ~SpanEvent() { otlp_span_event_destroy(this); }

  void set_time_unix_nano(uint64_t value) { this->time_unix_nano = value; }

  void set_name(const char *value) {
    otlp__set_string(&this->name, value, std::strlen(value));
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

  const otlp_span_event *get() const { return this; }
};

class SpanLink : private otlp_span_link {
public:
  SpanLink() { otlp_span_link_init(this); }

  SpanLink(const SpanLink &other) {
    otlp_span_link_init(this);
    otlp_span_link_copy(this, other.get());
  }

  SpanLink &operator=(const SpanLink &other) {
    if (this != &other) {
      otlp_span_link_destroy(this);
      otlp_span_link_init(this);
      otlp_span_link_copy(this, other.get());
    }
    return *this;
  }

  ~SpanLink() { otlp_span_link_destroy(this); }

  void set_trace_id(const uint8_t *ptr, size_t len) {
    otlp__set_bytes(&this->trace_id, ptr, len);
  }

  void set_span_id(const uint8_t *ptr, size_t len) {
    otlp__set_bytes(&this->span_id, ptr, len);
  }

  void set_trace_state(const char *value) {
    otlp__set_string(&this->trace_state, value, std::strlen(value));
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

  const otlp_span_link *get() const { return this; }
};

class Span : private otlp_span {
public:
  Span() { otlp_span_init(this); }

  Span(const Span &other) {
    otlp_span_init(this);
    otlp_span_copy(this, other.get());
  }

  Span &operator=(const Span &other) {
    if (this != &other) {
      otlp_span_destroy(this);
      otlp_span_init(this);
      otlp_span_copy(this, other.get());
    }
    return *this;
  }

  ~Span() { otlp_span_destroy(this); }

  void set_name(const char *value) {
    otlp__set_string(&this->name, value, std::strlen(value));
  }

  void set_kind(otlp_span_kind value) { this->kind = value; }

  void set_trace_state(const char *value) {
    otlp__set_string(&this->trace_state, value, std::strlen(value));
  }

  void set_trace_id(const uint8_t *ptr, size_t len) {
    otlp__set_bytes(&this->trace_id, ptr, len);
  }

  void set_span_id(const uint8_t *ptr, size_t len) {
    otlp__set_bytes(&this->span_id, ptr, len);
  }

  void set_parent_span_id(const uint8_t *ptr, size_t len) {
    otlp__set_bytes(&this->parent_span_id, ptr, len);
  }

  void set_start_time_unix_nano(uint64_t value) {
    this->start_time_unix_nano = value;
  }

  void set_end_time_unix_nano(uint64_t value) {
    this->end_time_unix_nano = value;
  }

  void append_attribute(const KeyValue &attribute) {
    otlp_key_value copy;
    otlp_key_value_init(&copy);
    otlp_key_value_copy(&copy, attribute.get());
    otlp__append_primitive((void **)&this->attributes.ptr,
                           &this->attributes.len, &this->attributes.cap,
                           sizeof(copy), &copy);
  }

  void append_event(const SpanEvent &event) {
    otlp_span_event copy;
    otlp_span_event_init(&copy);
    otlp_span_event_copy(&copy, event.get());
    otlp__append_primitive((void **)&this->events.ptr, &this->events.len,
                           &this->events.cap, sizeof(copy), &copy);
  }

  void append_link(const SpanLink &link) {
    otlp_span_link copy;
    otlp_span_link_init(&copy);
    otlp_span_link_copy(&copy, link.get());
    otlp__append_primitive((void **)&this->links.ptr, &this->links.len,
                           &this->links.cap, sizeof(copy), &copy);
  }

  void set_status(const TraceStatus &status) {
    if (this->has_status) {
      otlp_trace_status_destroy(&this->status);
      otlp_trace_status_init(&this->status);
    }
    this->has_status = 1;
    otlp_trace_status_copy(&this->status, status.get());
  }

  void set_dropped_attributes_count(uint32_t value) {
    this->dropped_attributes_count = value;
  }

  void set_dropped_events_count(uint32_t value) {
    this->dropped_events_count = value;
  }

  void set_dropped_links_count(uint32_t value) {
    this->dropped_links_count = value;
  }

  void set_flags(uint32_t value) { this->flags = value; }

  const otlp_span *get() const { return this; }
};

class ScopeSpans : private otlp_scope_spans {
public:
  ScopeSpans() { otlp_scope_spans_init(this); }

  ScopeSpans(const ScopeSpans &other) {
    otlp_scope_spans_init(this);
    otlp_scope_spans_copy(this, other.get());
  }

  ScopeSpans &operator=(const ScopeSpans &other) {
    if (this != &other) {
      otlp_scope_spans_destroy(this);
      otlp_scope_spans_init(this);
      otlp_scope_spans_copy(this, other.get());
    }
    return *this;
  }

  ~ScopeSpans() { otlp_scope_spans_destroy(this); }

  void append_span(const Span &span) {
    otlp_scope_spans_append_span(this, span.get());
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

  const otlp_scope_spans *get() const { return this; }
};

class ResourceSpans : private otlp_resource_spans {
public:
  ResourceSpans() { otlp_resource_spans_init(this); }

  ResourceSpans(const ResourceSpans &other) {
    otlp_resource_spans_init(this);
    otlp_resource_spans_copy(this, other.get());
  }

  ResourceSpans &operator=(const ResourceSpans &other) {
    if (this != &other) {
      otlp_resource_spans_destroy(this);
      otlp_resource_spans_init(this);
      otlp_resource_spans_copy(this, other.get());
    }
    return *this;
  }

  ~ResourceSpans() { otlp_resource_spans_destroy(this); }

  void append_scope_spans(const ScopeSpans &scope_spans) {
    otlp_resource_spans_append_scope_spans(this, scope_spans.get());
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

  const otlp_resource_spans *get() const { return this; }
};

class TracesExportRequest : private otlp_export_trace_service_request {
public:
  TracesExportRequest() { otlp_export_trace_service_request_init(this); }

  TracesExportRequest(const TracesExportRequest &other) {
    otlp_export_trace_service_request_init(this);
    for (size_t i = 0; i < other.get()->resource_spans.len; ++i) {
      otlp_export_trace_service_request_append_resource_spans(
          this, &other.get()->resource_spans.ptr[i]);
    }
  }

  TracesExportRequest &operator=(const TracesExportRequest &other) {
    if (this != &other) {
      otlp_export_trace_service_request_destroy(this);
      otlp_export_trace_service_request_init(this);
      for (size_t i = 0; i < other.get()->resource_spans.len; ++i) {
        otlp_export_trace_service_request_append_resource_spans(
            this, &other.get()->resource_spans.ptr[i]);
      }
    }
    return *this;
  }

  ~TracesExportRequest() { otlp_export_trace_service_request_destroy(this); }

  void append_resource_spans(const ResourceSpans &resource_spans) {
    otlp_export_trace_service_request_append_resource_spans(
        this, resource_spans.get());
  }

  size_t encoded_size() const {
    return otlp_export_trace_service_request_encoded_size(this);
  }

  otlp_status encode(uint8_t *out, size_t len) const {
    return otlp_export_trace_service_request_encode(this, out, len);
  }

  otlp_status decode(const uint8_t *data, size_t len) {
    return otlp_export_trace_service_request_decode(this, data, len);
  }

  const otlp_export_trace_service_request *get() const { return this; }
};

class TracesData : private otlp_traces_data {
public:
  TracesData() { otlp_traces_data_init(this); }

  TracesData(const TracesData &other) {
    otlp_traces_data_init(this);
    for (size_t i = 0; i < other.get()->resource_spans.len; ++i) {
      otlp_resource_spans copy;
      otlp_resource_spans_init(&copy);
      otlp_resource_spans_copy(&copy, &other.get()->resource_spans.ptr[i]);
      otlp__append_primitive((void **)&this->resource_spans.ptr,
                             &this->resource_spans.len,
                             &this->resource_spans.cap, sizeof(copy), &copy);
    }
  }

  TracesData &operator=(const TracesData &other) {
    if (this != &other) {
      otlp_traces_data_destroy(this);
      otlp_traces_data_init(this);
      for (size_t i = 0; i < other.get()->resource_spans.len; ++i) {
        otlp_resource_spans copy;
        otlp_resource_spans_init(&copy);
        otlp_resource_spans_copy(&copy, &other.get()->resource_spans.ptr[i]);
        otlp__append_primitive((void **)&this->resource_spans.ptr,
                               &this->resource_spans.len,
                               &this->resource_spans.cap, sizeof(copy), &copy);
      }
    }
    return *this;
  }

  ~TracesData() { otlp_traces_data_destroy(this); }

  void append_resource_spans(const ResourceSpans &resource_spans) {
    otlp_resource_spans copy;
    otlp_resource_spans_init(&copy);
    otlp_resource_spans_copy(&copy, resource_spans.get());
    otlp__append_primitive((void **)&this->resource_spans.ptr,
                           &this->resource_spans.len,
                           &this->resource_spans.cap, sizeof(copy), &copy);
  }

  size_t encoded_size() const { return otlp_traces_data_encoded_size(this); }

  otlp_status encode(uint8_t *out, size_t len) const {
    return otlp_traces_data_encode(this, out, len);
  }

  otlp_status decode(const uint8_t *data, size_t len) {
    return otlp_traces_data_decode(this, data, len);
  }

  const otlp_traces_data *get() const { return this; }
};

class TracesExportResponse : private otlp_export_trace_service_response {
public:
  TracesExportResponse() { otlp_export_trace_service_response_init(this); }

  TracesExportResponse(const TracesExportResponse &other) {
    otlp_export_trace_service_response_init(this);
    if (other.get()->has_partial_success) {
      this->has_partial_success = 1;
      this->partial_success.rejected_spans =
          other.get()->partial_success.rejected_spans;
      otlp__set_string(&this->partial_success.error_message,
                       other.get()->partial_success.error_message.ptr,
                       other.get()->partial_success.error_message.len);
    }
  }

  TracesExportResponse &operator=(const TracesExportResponse &other) {
    if (this != &other) {
      otlp_export_trace_service_response_destroy(this);
      otlp_export_trace_service_response_init(this);
      if (other.get()->has_partial_success) {
        this->has_partial_success = 1;
        this->partial_success.rejected_spans =
            other.get()->partial_success.rejected_spans;
        otlp__set_string(&this->partial_success.error_message,
                         other.get()->partial_success.error_message.ptr,
                         other.get()->partial_success.error_message.len);
      }
    }
    return *this;
  }

  ~TracesExportResponse() { otlp_export_trace_service_response_destroy(this); }

  void set_partial_success(int64_t rejected_spans, const char *error_message) {
    this->has_partial_success = 1;
    this->partial_success.rejected_spans = rejected_spans;
    otlp__set_string(&this->partial_success.error_message, error_message,
                     std::strlen(error_message));
  }

  size_t encoded_size() const {
    return otlp_export_trace_service_response_encoded_size(this);
  }

  otlp_status encode(uint8_t *out, size_t len) const {
    return otlp_export_trace_service_response_encode(this, out, len);
  }

  otlp_status decode(const uint8_t *data, size_t len) {
    return otlp_export_trace_service_response_decode(this, data, len);
  }

  const otlp_export_trace_service_response *get() const { return this; }
};

} /* namespace otlp */
#endif

#endif /* OTLP_FORMAT_TRACES_HPP */
