#ifndef OTLP_FORMAT_TRACES_H
#define OTLP_FORMAT_TRACES_H

#include "../common/common.h"

#if defined(OTLP_FORMAT_ENABLE_TRACES)
typedef struct otlp_trace_status otlp_trace_status;
typedef struct otlp_span_event otlp_span_event;
typedef struct otlp_span_link otlp_span_link;
typedef struct otlp_span otlp_span;
typedef struct otlp_scope_spans otlp_scope_spans;
typedef struct otlp_resource_spans otlp_resource_spans;
typedef struct otlp_traces_data otlp_traces_data;
typedef struct otlp_export_trace_service_request otlp_export_trace_service_request;
typedef struct otlp_export_trace_partial_success otlp_export_trace_partial_success;
typedef struct otlp_export_trace_service_response otlp_export_trace_service_response;
OTLP_DECLARE_ARRAY(otlp_span_event_array, otlp_span_event);
OTLP_DECLARE_ARRAY(otlp_span_link_array, otlp_span_link);
OTLP_DECLARE_ARRAY(otlp_span_array, otlp_span);
OTLP_DECLARE_ARRAY(otlp_scope_spans_array, otlp_scope_spans);
OTLP_DECLARE_ARRAY(otlp_resource_spans_array, otlp_resource_spans);

typedef enum otlp_span_kind_e {
  OTLP_SPAN_KIND_UNSPECIFIED = 0,
  OTLP_SPAN_KIND_INTERNAL = 1,
  OTLP_SPAN_KIND_SERVER = 2,
  OTLP_SPAN_KIND_CLIENT = 3,
  OTLP_SPAN_KIND_PRODUCER = 4,
  OTLP_SPAN_KIND_CONSUMER = 5
} otlp_span_kind;

typedef enum otlp_trace_status_code_e {
  OTLP_TRACE_STATUS_CODE_UNSET = 0,
  OTLP_TRACE_STATUS_CODE_OK = 1,
  OTLP_TRACE_STATUS_CODE_ERROR = 2
} otlp_trace_status_code;

#define OTLP_SPAN_FLAGS_DO_NOT_USE 0x00000000U
#define OTLP_SPAN_FLAGS_TRACE_FLAGS_MASK 0x000000FFU
#define OTLP_SPAN_FLAGS_CONTEXT_HAS_IS_REMOTE_MASK 0x00000100U
#define OTLP_SPAN_FLAGS_CONTEXT_IS_REMOTE_MASK 0x00000200U

struct otlp_trace_status {
  otlp_trace_status_code code;
  otlp_string message;
};

struct otlp_span_event {
  uint64_t time_unix_nano;
  otlp_string name;
  otlp_key_value_array attributes;
  uint32_t dropped_attributes_count;
};

struct otlp_span_link {
  otlp_bytes trace_id;
  otlp_bytes span_id;
  otlp_string trace_state;
  otlp_key_value_array attributes;
  uint32_t dropped_attributes_count;
  uint32_t flags;
};

struct otlp_span {
  otlp_bytes trace_id;
  otlp_bytes span_id;
  otlp_string trace_state;
  otlp_bytes parent_span_id;
  uint32_t flags;
  otlp_string name;
  otlp_span_kind kind;
  uint64_t start_time_unix_nano;
  uint64_t end_time_unix_nano;
  otlp_key_value_array attributes;
  uint32_t dropped_attributes_count;
  otlp_span_event_array events;
  uint32_t dropped_events_count;
  otlp_span_link_array links;
  uint32_t dropped_links_count;
  int has_status;
  otlp_trace_status status;
};

static inline void otlp_span_init(otlp_span *span);
static inline void otlp_span_destroy(otlp_span *span);
static inline otlp_status otlp_span_copy(otlp_span *dst, const otlp_span *src);

struct otlp_resource_spans {
  int has_resource;
  otlp_resource resource;
  otlp_scope_spans_array scope_spans;
  otlp_string schema_url;
};

struct otlp_traces_data {
  otlp_resource_spans_array resource_spans;
};

struct otlp_export_trace_service_request {
  otlp_resource_spans_array resource_spans;
};

struct otlp_export_trace_partial_success {
  int64_t rejected_spans;
  otlp_string error_message;
};

struct otlp_export_trace_service_response {
  int has_partial_success;
  otlp_export_trace_partial_success partial_success;
};

static inline void otlp_resource_spans_init(otlp_resource_spans *resource_spans);
static inline void otlp_resource_spans_destroy(otlp_resource_spans *resource_spans);
static inline otlp_status otlp_resource_spans_append_scope_spans(otlp_resource_spans *resource_spans, const otlp_scope_spans *scope_spans);

struct otlp_scope_spans {
  int has_scope;
  otlp_instrumentation_scope scope;
  otlp_span_array spans;
  otlp_string schema_url;
};

static inline void otlp_scope_spans_init(otlp_scope_spans *scope_spans);
static inline void otlp_scope_spans_destroy(otlp_scope_spans *scope_spans);
static inline otlp_status otlp_scope_spans_append_span(otlp_scope_spans *scope_spans, const otlp_span *span);

static inline void otlp_export_trace_service_request_init(otlp_export_trace_service_request *request);
static inline void otlp_export_trace_service_request_destroy(otlp_export_trace_service_request *request);
static inline otlp_status otlp_export_trace_service_request_append_resource_spans(otlp_export_trace_service_request *request, const otlp_resource_spans *resource_spans);
static inline size_t otlp_export_trace_service_request_encoded_size(const otlp_export_trace_service_request *request);
static inline otlp_status otlp_export_trace_service_request_encode(const otlp_export_trace_service_request *request, uint8_t *out, size_t len);
static inline otlp_status otlp_export_trace_service_request_decode(otlp_export_trace_service_request *request, const uint8_t *data, size_t len);
static inline void otlp_traces_data_init(otlp_traces_data *data);
static inline void otlp_traces_data_destroy(otlp_traces_data *data);
static inline size_t otlp_traces_data_encoded_size(const otlp_traces_data *data);
static inline otlp_status otlp_traces_data_encode(const otlp_traces_data *data, uint8_t *out, size_t len);
static inline otlp_status otlp_traces_data_decode(otlp_traces_data *data, const uint8_t *bytes, size_t len);
static inline void otlp_export_trace_service_response_init(otlp_export_trace_service_response *response);
static inline void otlp_export_trace_service_response_destroy(otlp_export_trace_service_response *response);
static inline size_t otlp_export_trace_service_response_encoded_size(const otlp_export_trace_service_response *response);
static inline otlp_status otlp_export_trace_service_response_encode(const otlp_export_trace_service_response *response, uint8_t *out, size_t len);
static inline otlp_status otlp_export_trace_service_response_decode(otlp_export_trace_service_response *response, const uint8_t *data, size_t len);

static inline void otlp_trace_status_init(otlp_trace_status *status) {
  otlp__zero(status, sizeof(*status));
}

static inline void otlp_trace_status_destroy(otlp_trace_status *status) {
  free(status->message.ptr);
  otlp_trace_status_init(status);
}

static inline otlp_status otlp_trace_status_copy(otlp_trace_status *dst,
                                                 const otlp_trace_status *src) {
  otlp_trace_status_init(dst);
  dst->code = src->code;
  return otlp__set_string(&dst->message, src->message.ptr, src->message.len);
}

static inline size_t
otlp_trace_status_encoded_size_inner(const otlp_trace_status *status) {
  size_t size = 0;
  if (status->message.len)
    size += otlp__bytes_field_size(2, status->message.len);
  if (status->code != OTLP_TRACE_STATUS_CODE_UNSET)
    size += otlp__varint_field_size(3, (uint64_t)status->code);
  return size;
}

static inline void
otlp_trace_status_encode_field(otlp__writer *writer, uint32_t field_number,
                               const otlp_trace_status *status) {
  size_t inner = otlp_trace_status_encoded_size_inner(status);
  otlp__write_message_start(writer, field_number, inner);
  otlp__write_string_field(writer, 2, &status->message);
  if (status->code != OTLP_TRACE_STATUS_CODE_UNSET) {
    otlp__write_tag(writer, 3, 0);
    otlp__write_varint(writer, (uint64_t)status->code);
  }
}

static inline otlp_status
otlp_trace_status_decode_inner(otlp_trace_status *status, const uint8_t *data,
                               size_t len) {
  otlp__reader reader;
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if (field_number == 2 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&status->message, (const char *)ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (field_number == 3 && wire_type == 0) {
      status->code = (otlp_trace_status_code)otlp__read_varint(&reader);
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }
  return reader.status;
}

static inline void otlp_span_event_init(otlp_span_event *event) {
  otlp__zero(event, sizeof(*event));
}

static inline void otlp_span_event_destroy(otlp_span_event *event) {
  size_t i;
  free(event->name.ptr);
  for (i = 0; i < event->attributes.len; ++i)
    otlp_key_value_destroy(&event->attributes.ptr[i]);
  free(event->attributes.ptr);
  otlp_span_event_init(event);
}

static inline otlp_status otlp_span_event_copy(otlp_span_event *dst,
                                               const otlp_span_event *src) {
  size_t i;
  otlp_span_event_init(dst);
  dst->time_unix_nano = src->time_unix_nano;
  dst->dropped_attributes_count = src->dropped_attributes_count;
  if (otlp__set_string(&dst->name, src->name.ptr, src->name.len) !=
      OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
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
  return OTLP_STATUS_OK;
}

static inline size_t
otlp_span_event_encoded_size_inner(const otlp_span_event *event) {
  size_t size = 0;
  size_t i;
  if (event->time_unix_nano)
    size += otlp__fixed64_field_size(1);
  if (event->name.len)
    size += otlp__bytes_field_size(2, event->name.len);
  for (i = 0; i < event->attributes.len; ++i)
    size += otlp__message_field_size(
        3, otlp_key_value_encoded_size_inner(&event->attributes.ptr[i]));
  if (event->dropped_attributes_count)
    size += otlp__varint_field_size(4, event->dropped_attributes_count);
  return size;
}

static inline void otlp_span_event_encode_field(otlp__writer *writer,
                                                uint32_t field_number,
                                                const otlp_span_event *event) {
  size_t i;
  size_t inner = otlp_span_event_encoded_size_inner(event);
  otlp__write_message_start(writer, field_number, inner);
  if (event->time_unix_nano) {
    otlp__write_tag(writer, 1, 1);
    otlp__write_fixed64(writer, event->time_unix_nano);
  }
  otlp__write_string_field(writer, 2, &event->name);
  for (i = 0; i < event->attributes.len; ++i)
    otlp_key_value_encode_field(writer, 3, &event->attributes.ptr[i]);
  if (event->dropped_attributes_count) {
    otlp__write_tag(writer, 4, 0);
    otlp__write_varint(writer, event->dropped_attributes_count);
  }
}

static inline otlp_status otlp_span_event_decode_inner(otlp_span_event *event,
                                                       const uint8_t *data,
                                                       size_t len) {
  otlp__reader reader;
  otlp_span_event_destroy(event);
  otlp_span_event_init(event);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    switch (field_number) {
    case 1:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      event->time_unix_nano = otlp__read_fixed64(&reader);
      break;
    case 2: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&event->name, (const char *)ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 3: {
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
      if (otlp__append_primitive((void **)&event->attributes.ptr,
                                 &event->attributes.len, &event->attributes.cap,
                                 sizeof(kv), &kv) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 4:
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      event->dropped_attributes_count = (uint32_t)otlp__read_varint(&reader);
      break;
    default:
      if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
        return reader.status;
      break;
    }
  }
  return reader.status;
}

static inline void otlp_span_link_init(otlp_span_link *link) {
  otlp__zero(link, sizeof(*link));
}

static inline void otlp_span_link_destroy(otlp_span_link *link) {
  size_t i;
  free(link->trace_id.ptr);
  free(link->span_id.ptr);
  free(link->trace_state.ptr);
  for (i = 0; i < link->attributes.len; ++i)
    otlp_key_value_destroy(&link->attributes.ptr[i]);
  free(link->attributes.ptr);
  otlp_span_link_init(link);
}

static inline otlp_status otlp_span_link_copy(otlp_span_link *dst,
                                              const otlp_span_link *src) {
  size_t i;
  otlp_span_link_init(dst);
  dst->dropped_attributes_count = src->dropped_attributes_count;
  dst->flags = src->flags;
  if (otlp__set_bytes(&dst->trace_id, src->trace_id.ptr, src->trace_id.len) !=
      OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  if (otlp__set_bytes(&dst->span_id, src->span_id.ptr, src->span_id.len) !=
      OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  if (otlp__set_string(&dst->trace_state, src->trace_state.ptr,
                       src->trace_state.len) != OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
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
  return OTLP_STATUS_OK;
}

static inline size_t
otlp_span_link_encoded_size_inner(const otlp_span_link *link) {
  size_t size = 0;
  size_t i;
  if (link->trace_id.len)
    size += otlp__bytes_field_size(1, link->trace_id.len);
  if (link->span_id.len)
    size += otlp__bytes_field_size(2, link->span_id.len);
  if (link->trace_state.len)
    size += otlp__bytes_field_size(3, link->trace_state.len);
  for (i = 0; i < link->attributes.len; ++i)
    size += otlp__message_field_size(
        4, otlp_key_value_encoded_size_inner(&link->attributes.ptr[i]));
  if (link->dropped_attributes_count)
    size += otlp__varint_field_size(5, link->dropped_attributes_count);
  if (link->flags)
    size += otlp__fixed32_field_size(6);
  return size;
}

static inline void otlp_span_link_encode_field(otlp__writer *writer,
                                               uint32_t field_number,
                                               const otlp_span_link *link) {
  size_t i;
  size_t inner = otlp_span_link_encoded_size_inner(link);
  otlp__write_message_start(writer, field_number, inner);
  otlp__write_bytes_field(writer, 1, link->trace_id.ptr, link->trace_id.len);
  otlp__write_bytes_field(writer, 2, link->span_id.ptr, link->span_id.len);
  otlp__write_string_field(writer, 3, &link->trace_state);
  for (i = 0; i < link->attributes.len; ++i)
    otlp_key_value_encode_field(writer, 4, &link->attributes.ptr[i]);
  if (link->dropped_attributes_count) {
    otlp__write_tag(writer, 5, 0);
    otlp__write_varint(writer, link->dropped_attributes_count);
  }
  if (link->flags) {
    otlp__write_tag(writer, 6, 5);
    otlp__write_fixed32(writer, link->flags);
  }
}

static inline otlp_status otlp_span_link_decode_inner(otlp_span_link *link,
                                                      const uint8_t *data,
                                                      size_t len) {
  otlp__reader reader;
  otlp_span_link_destroy(link);
  otlp_span_link_init(link);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    switch (field_number) {
    case 1:
    case 2: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if ((field_number == 1
               ? otlp__set_bytes(&link->trace_id, ptr, size)
               : otlp__set_bytes(&link->span_id, ptr, size)) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 3: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&link->trace_state, (const char *)ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 4: {
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
      if (otlp__append_primitive((void **)&link->attributes.ptr,
                                 &link->attributes.len, &link->attributes.cap,
                                 sizeof(kv), &kv) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 5:
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      link->dropped_attributes_count = (uint32_t)otlp__read_varint(&reader);
      break;
    case 6:
      if (wire_type != 5)
        return OTLP_STATUS_DECODE_ERROR;
      link->flags = otlp__read_fixed32(&reader);
      break;
    default:
      if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
        return reader.status;
      break;
    }
  }
  return reader.status;
}

static inline void otlp_span_init(otlp_span *span) {
  otlp__zero(span, sizeof(*span));
  otlp_trace_status_init(&span->status);
}

static inline void otlp_span_destroy(otlp_span *span) {
  size_t i;
  free(span->trace_id.ptr);
  free(span->span_id.ptr);
  free(span->trace_state.ptr);
  free(span->parent_span_id.ptr);
  free(span->name.ptr);
  for (i = 0; i < span->attributes.len; ++i)
    otlp_key_value_destroy(&span->attributes.ptr[i]);
  free(span->attributes.ptr);
  for (i = 0; i < span->events.len; ++i)
    otlp_span_event_destroy(&span->events.ptr[i]);
  free(span->events.ptr);
  for (i = 0; i < span->links.len; ++i)
    otlp_span_link_destroy(&span->links.ptr[i]);
  free(span->links.ptr);
  if (span->has_status)
    otlp_trace_status_destroy(&span->status);
  otlp_span_init(span);
}

static inline otlp_status otlp_span_copy(otlp_span *dst, const otlp_span *src) {
  size_t i;
  otlp_span_init(dst);
  dst->flags = src->flags;
  dst->kind = src->kind;
  dst->start_time_unix_nano = src->start_time_unix_nano;
  dst->end_time_unix_nano = src->end_time_unix_nano;
  dst->dropped_attributes_count = src->dropped_attributes_count;
  dst->dropped_events_count = src->dropped_events_count;
  dst->dropped_links_count = src->dropped_links_count;
  if (otlp__set_bytes(&dst->trace_id, src->trace_id.ptr, src->trace_id.len) !=
      OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  if (otlp__set_bytes(&dst->span_id, src->span_id.ptr, src->span_id.len) !=
      OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  if (otlp__set_string(&dst->trace_state, src->trace_state.ptr,
                       src->trace_state.len) != OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  if (otlp__set_bytes(&dst->parent_span_id, src->parent_span_id.ptr,
                      src->parent_span_id.len) != OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  if (otlp__set_string(&dst->name, src->name.ptr, src->name.len) !=
      OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
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
  for (i = 0; i < src->events.len; ++i) {
    otlp_span_event copy;
    otlp_status status = otlp_span_event_copy(&copy, &src->events.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->events.ptr, &dst->events.len,
                                    &dst->events.cap, sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  for (i = 0; i < src->links.len; ++i) {
    otlp_span_link copy;
    otlp_status status = otlp_span_link_copy(&copy, &src->links.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->links.ptr, &dst->links.len,
                                    &dst->links.cap, sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  if (src->has_status) {
    dst->has_status = 1;
    if (otlp_trace_status_copy(&dst->status, &src->status) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  }
  return OTLP_STATUS_OK;
}

static inline size_t otlp_span_encoded_size_inner(const otlp_span *span) {
  size_t size = 0;
  size_t i;
  if (span->trace_id.len)
    size += otlp__bytes_field_size(1, span->trace_id.len);
  if (span->span_id.len)
    size += otlp__bytes_field_size(2, span->span_id.len);
  if (span->trace_state.len)
    size += otlp__bytes_field_size(3, span->trace_state.len);
  if (span->parent_span_id.len)
    size += otlp__bytes_field_size(4, span->parent_span_id.len);
  if (span->name.len)
    size += otlp__bytes_field_size(5, span->name.len);
  if (span->kind != OTLP_SPAN_KIND_UNSPECIFIED)
    size += otlp__varint_field_size(6, (uint64_t)span->kind);
  if (span->start_time_unix_nano)
    size += otlp__fixed64_field_size(7);
  if (span->end_time_unix_nano)
    size += otlp__fixed64_field_size(8);
  for (i = 0; i < span->attributes.len; ++i)
    size += otlp__message_field_size(
        9, otlp_key_value_encoded_size_inner(&span->attributes.ptr[i]));
  if (span->dropped_attributes_count)
    size += otlp__varint_field_size(10, span->dropped_attributes_count);
  for (i = 0; i < span->events.len; ++i)
    size += otlp__message_field_size(
        11, otlp_span_event_encoded_size_inner(&span->events.ptr[i]));
  if (span->dropped_events_count)
    size += otlp__varint_field_size(12, span->dropped_events_count);
  for (i = 0; i < span->links.len; ++i)
    size += otlp__message_field_size(
        13, otlp_span_link_encoded_size_inner(&span->links.ptr[i]));
  if (span->dropped_links_count)
    size += otlp__varint_field_size(14, span->dropped_links_count);
  if (span->has_status)
    size += otlp__message_field_size(
        15, otlp_trace_status_encoded_size_inner(&span->status));
  if (span->flags)
    size += otlp__fixed32_field_size(16);
  return size;
}

static inline void otlp_span_encode_field(otlp__writer *writer,
                                          uint32_t field_number,
                                          const otlp_span *span) {
  size_t i;
  size_t inner = otlp_span_encoded_size_inner(span);
  otlp__write_message_start(writer, field_number, inner);
  otlp__write_bytes_field(writer, 1, span->trace_id.ptr, span->trace_id.len);
  otlp__write_bytes_field(writer, 2, span->span_id.ptr, span->span_id.len);
  otlp__write_string_field(writer, 3, &span->trace_state);
  otlp__write_bytes_field(writer, 4, span->parent_span_id.ptr,
                          span->parent_span_id.len);
  otlp__write_string_field(writer, 5, &span->name);
  if (span->kind != OTLP_SPAN_KIND_UNSPECIFIED) {
    otlp__write_tag(writer, 6, 0);
    otlp__write_varint(writer, (uint64_t)span->kind);
  }
  if (span->start_time_unix_nano) {
    otlp__write_tag(writer, 7, 1);
    otlp__write_fixed64(writer, span->start_time_unix_nano);
  }
  if (span->end_time_unix_nano) {
    otlp__write_tag(writer, 8, 1);
    otlp__write_fixed64(writer, span->end_time_unix_nano);
  }
  for (i = 0; i < span->attributes.len; ++i)
    otlp_key_value_encode_field(writer, 9, &span->attributes.ptr[i]);
  if (span->dropped_attributes_count) {
    otlp__write_tag(writer, 10, 0);
    otlp__write_varint(writer, span->dropped_attributes_count);
  }
  for (i = 0; i < span->events.len; ++i)
    otlp_span_event_encode_field(writer, 11, &span->events.ptr[i]);
  if (span->dropped_events_count) {
    otlp__write_tag(writer, 12, 0);
    otlp__write_varint(writer, span->dropped_events_count);
  }
  for (i = 0; i < span->links.len; ++i)
    otlp_span_link_encode_field(writer, 13, &span->links.ptr[i]);
  if (span->dropped_links_count) {
    otlp__write_tag(writer, 14, 0);
    otlp__write_varint(writer, span->dropped_links_count);
  }
  if (span->has_status)
    otlp_trace_status_encode_field(writer, 15, &span->status);
  if (span->flags) {
    otlp__write_tag(writer, 16, 5);
    otlp__write_fixed32(writer, span->flags);
  }
}

static inline otlp_status
otlp_span_decode_inner(otlp_span *span, const uint8_t *data, size_t len) {
  otlp__reader reader;
  otlp_span_destroy(span);
  otlp_span_init(span);
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
    case 4: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if ((field_number == 1   ? otlp__set_bytes(&span->trace_id, ptr, size)
           : field_number == 2 ? otlp__set_bytes(&span->span_id, ptr, size)
                               : otlp__set_bytes(&span->parent_span_id, ptr,
                                                 size)) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 3:
    case 5: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if ((field_number == 3
               ? otlp__set_string(&span->trace_state, (const char *)ptr, size)
               : otlp__set_string(&span->name, (const char *)ptr, size)) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 6:
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      span->kind = (otlp_span_kind)otlp__read_varint(&reader);
      break;
    case 7:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      span->start_time_unix_nano = otlp__read_fixed64(&reader);
      break;
    case 8:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      span->end_time_unix_nano = otlp__read_fixed64(&reader);
      break;
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
      if (otlp__append_primitive((void **)&span->attributes.ptr,
                                 &span->attributes.len, &span->attributes.cap,
                                 sizeof(kv), &kv) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 10:
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      span->dropped_attributes_count = (uint32_t)otlp__read_varint(&reader);
      break;
    case 11: {
      const uint8_t *ptr;
      size_t size;
      otlp_span_event event;
      otlp_span_event_init(&event);
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_span_event_decode_inner(&event, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&span->events.ptr, &span->events.len,
                                 &span->events.cap, sizeof(event),
                                 &event) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 12:
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      span->dropped_events_count = (uint32_t)otlp__read_varint(&reader);
      break;
    case 13: {
      const uint8_t *ptr;
      size_t size;
      otlp_span_link link;
      otlp_span_link_init(&link);
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_span_link_decode_inner(&link, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&span->links.ptr, &span->links.len,
                                 &span->links.cap, sizeof(link),
                                 &link) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 14:
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      span->dropped_links_count = (uint32_t)otlp__read_varint(&reader);
      break;
    case 15: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      span->has_status = 1;
      if (otlp_trace_status_decode_inner(&span->status, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      break;
    }
    case 16:
      if (wire_type != 5)
        return OTLP_STATUS_DECODE_ERROR;
      span->flags = otlp__read_fixed32(&reader);
      break;
    default:
      if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
        return reader.status;
      break;
    }
  }
  return reader.status;
}

static inline void otlp_scope_spans_init(otlp_scope_spans *scope_spans) {
  otlp__zero(scope_spans, sizeof(*scope_spans));
}

static inline void otlp_scope_spans_destroy(otlp_scope_spans *scope_spans) {
  size_t i;
  if (scope_spans->has_scope)
    otlp_instrumentation_scope_destroy(&scope_spans->scope);
  for (i = 0; i < scope_spans->spans.len; ++i)
    otlp_span_destroy(&scope_spans->spans.ptr[i]);
  free(scope_spans->spans.ptr);
  free(scope_spans->schema_url.ptr);
  otlp_scope_spans_init(scope_spans);
}

static inline otlp_status
otlp_scope_spans_append_span(otlp_scope_spans *scope_spans,
                             const otlp_span *span) {
  otlp_span copy;
  otlp_status status = otlp_span_copy(&copy, span);
  if (status != OTLP_STATUS_OK)
    return status;
  return otlp__append_primitive((void **)&scope_spans->spans.ptr,
                                &scope_spans->spans.len,
                                &scope_spans->spans.cap, sizeof(copy), &copy);
}

static inline size_t
otlp_scope_spans_encoded_size_inner(const otlp_scope_spans *scope_spans) {
  size_t size = 0;
  size_t i;
  if (scope_spans->has_scope)
    size += otlp__message_field_size(
        1, otlp_instrumentation_scope_encoded_size_inner(&scope_spans->scope));
  for (i = 0; i < scope_spans->spans.len; ++i)
    size += otlp__message_field_size(
        2, otlp_span_encoded_size_inner(&scope_spans->spans.ptr[i]));
  if (scope_spans->schema_url.len)
    size += otlp__bytes_field_size(3, scope_spans->schema_url.len);
  return size;
}

static inline void
otlp_scope_spans_encode_field(otlp__writer *writer, uint32_t field_number,
                              const otlp_scope_spans *scope_spans) {
  size_t i;
  size_t inner = otlp_scope_spans_encoded_size_inner(scope_spans);
  otlp__write_message_start(writer, field_number, inner);
  if (scope_spans->has_scope)
    otlp_instrumentation_scope_encode_field(writer, 1, &scope_spans->scope);
  for (i = 0; i < scope_spans->spans.len; ++i)
    otlp_span_encode_field(writer, 2, &scope_spans->spans.ptr[i]);
  otlp__write_string_field(writer, 3, &scope_spans->schema_url);
}

static inline otlp_status otlp_scope_spans_copy(otlp_scope_spans *dst,
                                                const otlp_scope_spans *src) {
  size_t i;
  otlp_scope_spans_init(dst);
  if (src->has_scope) {
    dst->has_scope = 1;
    if (otlp_instrumentation_scope_copy(&dst->scope, &src->scope) !=
        OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  }
  if (otlp__set_string(&dst->schema_url, src->schema_url.ptr,
                       src->schema_url.len) != OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  for (i = 0; i < src->spans.len; ++i) {
    otlp_status status =
        otlp_scope_spans_append_span(dst, &src->spans.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}

static inline otlp_status
otlp_scope_spans_decode_inner(otlp_scope_spans *scope_spans,
                              const uint8_t *data, size_t len) {
  otlp__reader reader;
  otlp_scope_spans_destroy(scope_spans);
  otlp_scope_spans_init(scope_spans);
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
      scope_spans->has_scope = 1;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_instrumentation_scope_decode_inner(&scope_spans->scope, ptr,
                                                  size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
    } else if (field_number == 2 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      otlp_span span;
      otlp_span_init(&span);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_span_decode_inner(&span, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive(
              (void **)&scope_spans->spans.ptr, &scope_spans->spans.len,
              &scope_spans->spans.cap, sizeof(span), &span) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (field_number == 3 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&scope_spans->schema_url, (const char *)ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }
  return reader.status;
}

static inline void
otlp_resource_spans_init(otlp_resource_spans *resource_spans) {
  otlp__zero(resource_spans, sizeof(*resource_spans));
}

static inline void
otlp_resource_spans_destroy(otlp_resource_spans *resource_spans) {
  size_t i;
  if (resource_spans->has_resource)
    otlp_resource_destroy(&resource_spans->resource);
  for (i = 0; i < resource_spans->scope_spans.len; ++i)
    otlp_scope_spans_destroy(&resource_spans->scope_spans.ptr[i]);
  free(resource_spans->scope_spans.ptr);
  free(resource_spans->schema_url.ptr);
  otlp_resource_spans_init(resource_spans);
}

static inline otlp_status
otlp_resource_spans_append_scope_spans(otlp_resource_spans *resource_spans,
                                       const otlp_scope_spans *scope_spans) {
  otlp_scope_spans copy;
  otlp_scope_spans_init(&copy);
  {
    otlp_status status = otlp_scope_spans_copy(&copy, scope_spans);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return otlp__append_primitive((void **)&resource_spans->scope_spans.ptr,
                                &resource_spans->scope_spans.len,
                                &resource_spans->scope_spans.cap, sizeof(copy),
                                &copy);
}

static inline otlp_status otlp_resource_spans_copy(otlp_resource_spans *dst,
                                                   const otlp_resource_spans *src) {
  size_t i;
  otlp_resource_spans_init(dst);
  if (src->has_resource) {
    dst->has_resource = 1;
    if (otlp_resource_copy(&dst->resource, &src->resource) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  }
  if (otlp__set_string(&dst->schema_url, src->schema_url.ptr,
                       src->schema_url.len) != OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  for (i = 0; i < src->scope_spans.len; ++i) {
    otlp_status status =
        otlp_resource_spans_append_scope_spans(dst, &src->scope_spans.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}

static inline size_t otlp_resource_spans_encoded_size_inner(
    const otlp_resource_spans *resource_spans) {
  size_t size = 0;
  size_t i;
  if (resource_spans->has_resource)
    size += otlp__message_field_size(
        1, otlp_resource_encoded_size_inner(&resource_spans->resource));
  for (i = 0; i < resource_spans->scope_spans.len; ++i)
    size +=
        otlp__message_field_size(2, otlp_scope_spans_encoded_size_inner(
                                        &resource_spans->scope_spans.ptr[i]));
  if (resource_spans->schema_url.len)
    size += otlp__bytes_field_size(3, resource_spans->schema_url.len);
  return size;
}

static inline void
otlp_resource_spans_encode_field(otlp__writer *writer, uint32_t field_number,
                                 const otlp_resource_spans *resource_spans) {
  size_t i;
  size_t inner = otlp_resource_spans_encoded_size_inner(resource_spans);
  otlp__write_message_start(writer, field_number, inner);
  if (resource_spans->has_resource)
    otlp_resource_encode_field(writer, 1, &resource_spans->resource);
  for (i = 0; i < resource_spans->scope_spans.len; ++i)
    otlp_scope_spans_encode_field(writer, 2,
                                  &resource_spans->scope_spans.ptr[i]);
  otlp__write_string_field(writer, 3, &resource_spans->schema_url);
}

static inline otlp_status
otlp_resource_spans_decode_inner(otlp_resource_spans *resource_spans,
                                 const uint8_t *data, size_t len) {
  otlp__reader reader;
  otlp_resource_spans_destroy(resource_spans);
  otlp_resource_spans_init(resource_spans);
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
      resource_spans->has_resource = 1;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_resource_decode_inner(&resource_spans->resource, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
    } else if (field_number == 2 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      otlp_scope_spans scope_spans;
      otlp_scope_spans_init(&scope_spans);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_scope_spans_decode_inner(&scope_spans, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&resource_spans->scope_spans.ptr,
                                 &resource_spans->scope_spans.len,
                                 &resource_spans->scope_spans.cap,
                                 sizeof(scope_spans),
                                 &scope_spans) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (field_number == 3 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&resource_spans->schema_url, (const char *)ptr,
                           size) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }
  return reader.status;
}

static inline void otlp_export_trace_service_request_init(
    otlp_export_trace_service_request *request) {
  otlp__zero(request, sizeof(*request));
}

static inline void otlp_export_trace_service_request_destroy(
    otlp_export_trace_service_request *request) {
  size_t i;
  for (i = 0; i < request->resource_spans.len; ++i)
    otlp_resource_spans_destroy(&request->resource_spans.ptr[i]);
  free(request->resource_spans.ptr);
  otlp_export_trace_service_request_init(request);
}

static inline otlp_status
otlp_export_trace_service_request_append_resource_spans(
    otlp_export_trace_service_request *request,
    const otlp_resource_spans *resource_spans) {
  otlp_resource_spans copy;
  otlp_resource_spans_init(&copy);
  {
    otlp_status status = otlp_resource_spans_copy(&copy, resource_spans);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return otlp__append_primitive(
      (void **)&request->resource_spans.ptr, &request->resource_spans.len,
      &request->resource_spans.cap, sizeof(copy), &copy);
}

static inline size_t otlp_export_trace_service_request_encoded_size(
    const otlp_export_trace_service_request *request) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < request->resource_spans.len; ++i)
    size += otlp__message_field_size(1, otlp_resource_spans_encoded_size_inner(
                                            &request->resource_spans.ptr[i]));
  return size;
}

static inline otlp_status otlp_export_trace_service_request_encode(
    const otlp_export_trace_service_request *request, uint8_t *out,
    size_t len) {
  otlp__writer writer;
  size_t i;
  otlp__writer_init(&writer, out, len);
  for (i = 0; i < request->resource_spans.len; ++i)
    otlp_resource_spans_encode_field(&writer, 1,
                                     &request->resource_spans.ptr[i]);
  if (writer.status != OTLP_STATUS_OK)
    return writer.status;
  return writer.off == len ? OTLP_STATUS_OK : OTLP_STATUS_INVALID_ARGUMENT;
}

static inline otlp_status otlp_export_trace_service_request_decode(
    otlp_export_trace_service_request *request, const uint8_t *data,
    size_t len) {
  otlp__reader reader;
  otlp_export_trace_service_request_destroy(request);
  otlp_export_trace_service_request_init(request);
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
      otlp_resource_spans resource_spans;
      otlp_resource_spans_init(&resource_spans);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_resource_spans_decode_inner(&resource_spans, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive(
              (void **)&request->resource_spans.ptr,
              &request->resource_spans.len, &request->resource_spans.cap,
              sizeof(resource_spans), &resource_spans) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }
  return reader.status;
}

static inline void otlp_traces_data_init(otlp_traces_data *data) {
  otlp__zero(data, sizeof(*data));
}

static inline void otlp_traces_data_destroy(otlp_traces_data *data) {
  size_t i;
  for (i = 0; i < data->resource_spans.len; ++i)
    otlp_resource_spans_destroy(&data->resource_spans.ptr[i]);
  free(data->resource_spans.ptr);
  otlp_traces_data_init(data);
}

static inline size_t otlp_traces_data_encoded_size(const otlp_traces_data *data) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < data->resource_spans.len; ++i)
    size += otlp__message_field_size(1, otlp_resource_spans_encoded_size_inner(
                                            &data->resource_spans.ptr[i]));
  return size;
}

static inline otlp_status otlp_traces_data_encode(const otlp_traces_data *data,
                                                  uint8_t *out, size_t len) {
  otlp__writer writer;
  size_t i;
  otlp__writer_init(&writer, out, len);
  for (i = 0; i < data->resource_spans.len; ++i)
    otlp_resource_spans_encode_field(&writer, 1, &data->resource_spans.ptr[i]);
  if (writer.status != OTLP_STATUS_OK)
    return writer.status;
  return writer.off == len ? OTLP_STATUS_OK : OTLP_STATUS_INVALID_ARGUMENT;
}

static inline otlp_status otlp_traces_data_decode(otlp_traces_data *data,
                                                  const uint8_t *bytes,
                                                  size_t len) {
  otlp_export_trace_service_request request;
  otlp_export_trace_service_request_init(&request);
  if (otlp_export_trace_service_request_decode(&request, bytes, len) !=
      OTLP_STATUS_OK) {
    otlp_export_trace_service_request_destroy(&request);
    return OTLP_STATUS_DECODE_ERROR;
  }
  otlp_traces_data_destroy(data);
  data->resource_spans = request.resource_spans;
  return OTLP_STATUS_OK;
}

static inline void otlp_export_trace_partial_success_init(
    otlp_export_trace_partial_success *partial_success) {
  otlp__zero(partial_success, sizeof(*partial_success));
}

static inline void otlp_export_trace_partial_success_destroy(
    otlp_export_trace_partial_success *partial_success) {
  free(partial_success->error_message.ptr);
  otlp_export_trace_partial_success_init(partial_success);
}

static inline size_t otlp_export_trace_partial_success_encoded_size_inner(
    const otlp_export_trace_partial_success *partial_success) {
  size_t size = 0;
  if (partial_success->rejected_spans)
    size += otlp__varint_field_size(
        1, (uint64_t)partial_success->rejected_spans);
  if (partial_success->error_message.len)
    size += otlp__bytes_field_size(2, partial_success->error_message.len);
  return size;
}

static inline void otlp_export_trace_partial_success_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_export_trace_partial_success *partial_success) {
  size_t inner =
      otlp_export_trace_partial_success_encoded_size_inner(partial_success);
  otlp__write_message_start(writer, field_number, inner);
  if (partial_success->rejected_spans) {
    otlp__write_tag(writer, 1, 0);
    otlp__write_varint(writer, (uint64_t)partial_success->rejected_spans);
  }
  otlp__write_string_field(writer, 2, &partial_success->error_message);
}

static inline otlp_status otlp_export_trace_partial_success_decode_inner(
    otlp_export_trace_partial_success *partial_success, const uint8_t *data,
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
      partial_success->rejected_spans =
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

static inline void otlp_export_trace_service_response_init(
    otlp_export_trace_service_response *response) {
  otlp__zero(response, sizeof(*response));
}

static inline void otlp_export_trace_service_response_destroy(
    otlp_export_trace_service_response *response) {
  if (response->has_partial_success)
    otlp_export_trace_partial_success_destroy(&response->partial_success);
  otlp_export_trace_service_response_init(response);
}

static inline size_t otlp_export_trace_service_response_encoded_size(
    const otlp_export_trace_service_response *response) {
  size_t size = 0;
  if (response->has_partial_success)
    size += otlp__message_field_size(
        1, otlp_export_trace_partial_success_encoded_size_inner(
               &response->partial_success));
  return size;
}

static inline otlp_status otlp_export_trace_service_response_encode(
    const otlp_export_trace_service_response *response, uint8_t *out,
    size_t len) {
  otlp__writer writer;
  otlp__writer_init(&writer, out, len);
  if (response->has_partial_success)
    otlp_export_trace_partial_success_encode_field(
        &writer, 1, &response->partial_success);
  if (writer.status != OTLP_STATUS_OK)
    return writer.status;
  return writer.off == len ? OTLP_STATUS_OK : OTLP_STATUS_INVALID_ARGUMENT;
}

static inline otlp_status otlp_export_trace_service_response_decode(
    otlp_export_trace_service_response *response, const uint8_t *data,
    size_t len) {
  otlp__reader reader;
  otlp_export_trace_service_response_destroy(response);
  otlp_export_trace_service_response_init(response);
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
      if (otlp_export_trace_partial_success_decode_inner(
              &response->partial_success, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }
  return reader.status;
}
#endif

#endif /* OTLP_FORMAT_TRACES_H */
