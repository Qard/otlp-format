#ifndef OTLP_FORMAT_LOGS_H
#define OTLP_FORMAT_LOGS_H

#include "../common/common.h"

#if defined(OTLP_FORMAT_ENABLE_LOGS)
typedef enum otlp_severity_number_e {
  OTLP_SEVERITY_NUMBER_UNSPECIFIED = 0,
  OTLP_SEVERITY_NUMBER_TRACE = 1,
  OTLP_SEVERITY_NUMBER_TRACE2 = 2,
  OTLP_SEVERITY_NUMBER_TRACE3 = 3,
  OTLP_SEVERITY_NUMBER_TRACE4 = 4,
  OTLP_SEVERITY_NUMBER_DEBUG = 5,
  OTLP_SEVERITY_NUMBER_DEBUG2 = 6,
  OTLP_SEVERITY_NUMBER_DEBUG3 = 7,
  OTLP_SEVERITY_NUMBER_DEBUG4 = 8,
  OTLP_SEVERITY_NUMBER_INFO = 9,
  OTLP_SEVERITY_NUMBER_INFO2 = 10,
  OTLP_SEVERITY_NUMBER_INFO3 = 11,
  OTLP_SEVERITY_NUMBER_INFO4 = 12,
  OTLP_SEVERITY_NUMBER_WARN = 13,
  OTLP_SEVERITY_NUMBER_WARN2 = 14,
  OTLP_SEVERITY_NUMBER_WARN3 = 15,
  OTLP_SEVERITY_NUMBER_WARN4 = 16,
  OTLP_SEVERITY_NUMBER_ERROR = 17,
  OTLP_SEVERITY_NUMBER_ERROR2 = 18,
  OTLP_SEVERITY_NUMBER_ERROR3 = 19,
  OTLP_SEVERITY_NUMBER_ERROR4 = 20,
  OTLP_SEVERITY_NUMBER_FATAL = 21,
  OTLP_SEVERITY_NUMBER_FATAL2 = 22,
  OTLP_SEVERITY_NUMBER_FATAL3 = 23,
  OTLP_SEVERITY_NUMBER_FATAL4 = 24
} otlp_severity_number;
typedef struct otlp_log_record otlp_log_record;
typedef struct otlp_scope_logs otlp_scope_logs;
typedef struct otlp_resource_logs otlp_resource_logs;
typedef struct otlp_logs_data otlp_logs_data;
typedef struct otlp_export_logs_service_request otlp_export_logs_service_request;
typedef struct otlp_export_logs_partial_success otlp_export_logs_partial_success;
typedef struct otlp_export_logs_service_response otlp_export_logs_service_response;
OTLP_DECLARE_ARRAY(otlp_log_record_array, otlp_log_record);
OTLP_DECLARE_ARRAY(otlp_scope_logs_array, otlp_scope_logs);
OTLP_DECLARE_ARRAY(otlp_resource_logs_array, otlp_resource_logs);

#define OTLP_LOG_RECORD_FLAGS_DO_NOT_USE 0x00000000U
#define OTLP_LOG_RECORD_FLAGS_TRACE_FLAGS_MASK 0x000000FFU

struct otlp_log_record {
  uint64_t time_unix_nano;
  uint64_t observed_time_unix_nano;
  otlp_severity_number severity_number;
  otlp_string severity_text;
  otlp_string event_name;
  int has_body;
  otlp_any_value body;
  otlp_key_value_array attributes;
  uint32_t dropped_attributes_count;
  uint32_t flags;
  otlp_bytes trace_id;
  otlp_bytes span_id;
};

static inline void otlp_log_record_init(otlp_log_record *record);
static inline void otlp_log_record_destroy(otlp_log_record *record);
static inline void otlp_log_record_set_time_unix_nano(otlp_log_record *record, uint64_t value);
static inline void otlp_log_record_set_severity_number(otlp_log_record *record, otlp_severity_number value);
static inline otlp_status otlp_log_record_set_body(otlp_log_record *record, const otlp_any_value *value);

struct otlp_resource_logs {
  int has_resource;
  otlp_resource resource;
  otlp_scope_logs_array scope_logs;
  otlp_string schema_url;
};

struct otlp_logs_data {
  otlp_resource_logs_array resource_logs;
};

struct otlp_export_logs_service_request {
  otlp_resource_logs_array resource_logs;
};

struct otlp_export_logs_partial_success {
  int64_t rejected_log_records;
  otlp_string error_message;
};

struct otlp_export_logs_service_response {
  int has_partial_success;
  otlp_export_logs_partial_success partial_success;
};

static inline void otlp_resource_logs_init(otlp_resource_logs *resource_logs);
static inline void otlp_resource_logs_destroy(otlp_resource_logs *resource_logs);
static inline otlp_status otlp_resource_logs_append_scope_logs(otlp_resource_logs *resource_logs, const otlp_scope_logs *scope_logs);

struct otlp_scope_logs {
  int has_scope;
  otlp_instrumentation_scope scope;
  otlp_log_record_array log_records;
  otlp_string schema_url;
};

static inline void otlp_scope_logs_init(otlp_scope_logs *scope_logs);
static inline void otlp_scope_logs_destroy(otlp_scope_logs *scope_logs);
static inline otlp_status otlp_scope_logs_append_log_record(otlp_scope_logs *scope_logs, const otlp_log_record *record);
static inline void otlp_export_logs_service_request_init(otlp_export_logs_service_request *request);
static inline void otlp_export_logs_service_request_destroy(otlp_export_logs_service_request *request);
static inline otlp_status otlp_export_logs_service_request_append_resource_logs(otlp_export_logs_service_request *request, const otlp_resource_logs *resource_logs);
static inline size_t otlp_export_logs_service_request_encoded_size(const otlp_export_logs_service_request *request);
static inline otlp_status otlp_export_logs_service_request_encode(const otlp_export_logs_service_request *request, uint8_t *out, size_t len);
static inline otlp_status otlp_export_logs_service_request_decode(otlp_export_logs_service_request *request, const uint8_t *data, size_t len);
static inline void otlp_logs_data_init(otlp_logs_data *data);
static inline void otlp_logs_data_destroy(otlp_logs_data *data);
static inline size_t otlp_logs_data_encoded_size(const otlp_logs_data *data);
static inline otlp_status otlp_logs_data_encode(const otlp_logs_data *data, uint8_t *out, size_t len);
static inline otlp_status otlp_logs_data_decode(otlp_logs_data *data, const uint8_t *bytes, size_t len);
static inline void otlp_export_logs_service_response_init(otlp_export_logs_service_response *response);
static inline void otlp_export_logs_service_response_destroy(otlp_export_logs_service_response *response);
static inline size_t otlp_export_logs_service_response_encoded_size(const otlp_export_logs_service_response *response);
static inline otlp_status otlp_export_logs_service_response_encode(const otlp_export_logs_service_response *response, uint8_t *out, size_t len);
static inline otlp_status otlp_export_logs_service_response_decode(otlp_export_logs_service_response *response, const uint8_t *data, size_t len);

static inline void otlp_log_record_init(otlp_log_record *record) {
  otlp__zero(record, sizeof(*record));
  otlp_any_value_init(&record->body);
}

static inline void otlp_log_record_destroy(otlp_log_record *record) {
  size_t i;
  free(record->severity_text.ptr);
  free(record->event_name.ptr);
  for (i = 0; i < record->attributes.len; ++i)
    otlp_key_value_destroy(&record->attributes.ptr[i]);
  free(record->attributes.ptr);
  free(record->trace_id.ptr);
  free(record->span_id.ptr);
  if (record->has_body)
    otlp_any_value_destroy(&record->body);
  otlp_log_record_init(record);
}

static inline otlp_status otlp_log_record_copy(otlp_log_record *dst,
                                               const otlp_log_record *src) {
  size_t i;
  otlp_log_record_init(dst);
  dst->time_unix_nano = src->time_unix_nano;
  dst->observed_time_unix_nano = src->observed_time_unix_nano;
  dst->severity_number = src->severity_number;
  dst->dropped_attributes_count = src->dropped_attributes_count;
  dst->flags = src->flags;
  if (otlp__set_string(&dst->severity_text, src->severity_text.ptr,
                       src->severity_text.len) != OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  if (otlp__set_string(&dst->event_name, src->event_name.ptr,
                       src->event_name.len) != OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  if (src->has_body) {
    dst->has_body = 1;
    if (otlp_any_value_copy(&dst->body, &src->body) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  }
  if (otlp__set_bytes(&dst->trace_id, src->trace_id.ptr, src->trace_id.len) !=
      OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  if (otlp__set_bytes(&dst->span_id, src->span_id.ptr, src->span_id.len) !=
      OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  for (i = 0; i < src->attributes.len; ++i) {
    otlp_key_value copy;
    otlp_status status;
    status = otlp_key_value_copy(&copy, &src->attributes.ptr[i]);
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

static inline void otlp_log_record_set_time_unix_nano(otlp_log_record *record,
                                                      uint64_t value) {
  record->time_unix_nano = value;
}

static inline void
otlp_log_record_set_severity_number(otlp_log_record *record,
                                    otlp_severity_number value) {
  record->severity_number = value;
}

static inline otlp_status
otlp_log_record_set_body(otlp_log_record *record, const otlp_any_value *value) {
  if (record->has_body)
    otlp_any_value_destroy(&record->body);
  record->has_body = 1;
  return otlp_any_value_copy(&record->body, value);
}

static inline size_t
otlp_log_record_encoded_size_inner(const otlp_log_record *record) {
  size_t size = 0;
  size_t i;
  if (record->time_unix_nano)
    size += otlp__fixed64_field_size(1);
  if (record->observed_time_unix_nano)
    size += otlp__fixed64_field_size(11);
  if (record->severity_number != OTLP_SEVERITY_NUMBER_UNSPECIFIED)
    size += otlp__varint_field_size(2, (uint64_t)record->severity_number);
  if (record->severity_text.len)
    size += otlp__bytes_field_size(3, record->severity_text.len);
  if (record->event_name.len)
    size += otlp__bytes_field_size(12, record->event_name.len);
  if (record->has_body)
    size += otlp_any_value_encoded_size_field(5, &record->body);
  for (i = 0; i < record->attributes.len; ++i)
    size += otlp__message_field_size(
        6, otlp_key_value_encoded_size_inner(&record->attributes.ptr[i]));
  if (record->dropped_attributes_count)
    size += otlp__varint_field_size(7, record->dropped_attributes_count);
  if (record->flags)
    size += otlp__fixed32_field_size(8);
  if (record->trace_id.len)
    size += otlp__bytes_field_size(9, record->trace_id.len);
  if (record->span_id.len)
    size += otlp__bytes_field_size(10, record->span_id.len);
  return size;
}

static inline void otlp_log_record_encode_field(otlp__writer *writer,
                                                uint32_t field_number,
                                                const otlp_log_record *record) {
  size_t i;
  size_t inner = otlp_log_record_encoded_size_inner(record);
  otlp__write_message_start(writer, field_number, inner);
  if (record->time_unix_nano) {
    otlp__write_tag(writer, 1, 1);
    otlp__write_fixed64(writer, record->time_unix_nano);
  }
  if (record->observed_time_unix_nano) {
    otlp__write_tag(writer, 11, 1);
    otlp__write_fixed64(writer, record->observed_time_unix_nano);
  }
  if (record->severity_number != OTLP_SEVERITY_NUMBER_UNSPECIFIED) {
    otlp__write_tag(writer, 2, 0);
    otlp__write_varint(writer, (uint64_t)record->severity_number);
  }
  otlp__write_string_field(writer, 3, &record->severity_text);
  otlp__write_string_field(writer, 12, &record->event_name);
  if (record->has_body)
    otlp_any_value_encode_field(writer, 5, &record->body);
  for (i = 0; i < record->attributes.len; ++i)
    otlp_key_value_encode_field(writer, 6, &record->attributes.ptr[i]);
  if (record->dropped_attributes_count) {
    otlp__write_tag(writer, 7, 0);
    otlp__write_varint(writer, record->dropped_attributes_count);
  }
  if (record->flags) {
    otlp__write_tag(writer, 8, 5);
    otlp__write_fixed32(writer, record->flags);
  }
  otlp__write_bytes_field(writer, 9, record->trace_id.ptr,
                          record->trace_id.len);
  otlp__write_bytes_field(writer, 10, record->span_id.ptr, record->span_id.len);
}

static inline otlp_status otlp_log_record_decode_inner(otlp_log_record *record,
                                                       const uint8_t *data,
                                                       size_t len) {
  otlp__reader reader;
  otlp_log_record_destroy(record);
  otlp_log_record_init(record);
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
      record->time_unix_nano = otlp__read_fixed64(&reader);
      break;
    case 11:
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      record->observed_time_unix_nano = otlp__read_fixed64(&reader);
      break;
    case 2:
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      record->severity_number =
          (otlp_severity_number)otlp__read_varint(&reader);
      break;
    case 3: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&record->severity_text, (const char *)ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 12: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&record->event_name, (const char *)ptr, size) !=
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
      record->has_body = 1;
      if (otlp_any_value_merge_inner(&record->body, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      break;
    }
    case 6: {
      const uint8_t *ptr;
      size_t size;
      otlp_key_value attribute;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      otlp_key_value_init(&attribute);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_key_value_decode_inner(&attribute, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&record->attributes.ptr,
                                 &record->attributes.len,
                                 &record->attributes.cap, sizeof(attribute),
                                 &attribute) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 7:
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      record->dropped_attributes_count = (uint32_t)otlp__read_varint(&reader);
      break;
    case 8:
      if (wire_type != 5)
        return OTLP_STATUS_DECODE_ERROR;
      record->flags = otlp__read_fixed32(&reader);
      break;
    case 9: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_bytes(&record->trace_id, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 10: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_bytes(&record->span_id, ptr, size) != OTLP_STATUS_OK)
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

static inline void otlp_scope_logs_init(otlp_scope_logs *scope_logs) {
  otlp__zero(scope_logs, sizeof(*scope_logs));
}

static inline void otlp_scope_logs_destroy(otlp_scope_logs *scope_logs) {
  size_t i;
  if (scope_logs->has_scope)
    otlp_instrumentation_scope_destroy(&scope_logs->scope);
  for (i = 0; i < scope_logs->log_records.len; ++i)
    otlp_log_record_destroy(&scope_logs->log_records.ptr[i]);
  free(scope_logs->log_records.ptr);
  free(scope_logs->schema_url.ptr);
  otlp_scope_logs_init(scope_logs);
}

static inline otlp_status
otlp_scope_logs_append_log_record(otlp_scope_logs *scope_logs,
                                  const otlp_log_record *record) {
  otlp_log_record copy;
  otlp_status status = otlp_log_record_copy(&copy, record);
  if (status != OTLP_STATUS_OK)
    return status;
  return otlp__append_primitive(
      (void **)&scope_logs->log_records.ptr, &scope_logs->log_records.len,
      &scope_logs->log_records.cap, sizeof(copy), &copy);
}

static inline size_t
otlp_scope_logs_encoded_size_inner(const otlp_scope_logs *scope_logs) {
  size_t size = 0;
  size_t i;
  if (scope_logs->has_scope)
    size += otlp__message_field_size(
        1, otlp_instrumentation_scope_encoded_size_inner(&scope_logs->scope));
  for (i = 0; i < scope_logs->log_records.len; ++i)
    size += otlp__message_field_size(
        2, otlp_log_record_encoded_size_inner(&scope_logs->log_records.ptr[i]));
  if (scope_logs->schema_url.len)
    size += otlp__bytes_field_size(3, scope_logs->schema_url.len);
  return size;
}

static inline void
otlp_scope_logs_encode_field(otlp__writer *writer, uint32_t field_number,
                             const otlp_scope_logs *scope_logs) {
  size_t i;
  size_t inner = otlp_scope_logs_encoded_size_inner(scope_logs);
  otlp__write_message_start(writer, field_number, inner);
  if (scope_logs->has_scope)
    otlp_instrumentation_scope_encode_field(writer, 1, &scope_logs->scope);
  for (i = 0; i < scope_logs->log_records.len; ++i)
    otlp_log_record_encode_field(writer, 2, &scope_logs->log_records.ptr[i]);
  otlp__write_string_field(writer, 3, &scope_logs->schema_url);
}

static inline otlp_status otlp_scope_logs_copy(otlp_scope_logs *dst,
                                               const otlp_scope_logs *src) {
  size_t i;
  otlp_scope_logs_init(dst);
  if (src->has_scope) {
    dst->has_scope = 1;
    if (otlp_instrumentation_scope_copy(&dst->scope, &src->scope) !=
        OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  }
  if (otlp__set_string(&dst->schema_url, src->schema_url.ptr,
                       src->schema_url.len) != OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  for (i = 0; i < src->log_records.len; ++i) {
    otlp_status status =
        otlp_scope_logs_append_log_record(dst, &src->log_records.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}

static inline otlp_status
otlp_scope_logs_decode_inner(otlp_scope_logs *scope_logs, const uint8_t *data,
                             size_t len) {
  otlp__reader reader;
  otlp_scope_logs_destroy(scope_logs);
  otlp_scope_logs_init(scope_logs);
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
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      scope_logs->has_scope = 1;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_instrumentation_scope_decode_inner(&scope_logs->scope, ptr,
                                                  size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      break;
    }
    case 2: {
      const uint8_t *ptr;
      size_t size;
      otlp_log_record record;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      otlp_log_record_init(&record);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_log_record_decode_inner(&record, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&scope_logs->log_records.ptr,
                                 &scope_logs->log_records.len,
                                 &scope_logs->log_records.cap, sizeof(record),
                                 &record) != OTLP_STATUS_OK)
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
      if (otlp__set_string(&scope_logs->schema_url, (const char *)ptr, size) !=
          OTLP_STATUS_OK)
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

static inline void otlp_resource_logs_init(otlp_resource_logs *resource_logs) {
  otlp__zero(resource_logs, sizeof(*resource_logs));
}

static inline void
otlp_resource_logs_destroy(otlp_resource_logs *resource_logs) {
  size_t i;
  if (resource_logs->has_resource)
    otlp_resource_destroy(&resource_logs->resource);
  for (i = 0; i < resource_logs->scope_logs.len; ++i)
    otlp_scope_logs_destroy(&resource_logs->scope_logs.ptr[i]);
  free(resource_logs->scope_logs.ptr);
  free(resource_logs->schema_url.ptr);
  otlp_resource_logs_init(resource_logs);
}

static inline otlp_status
otlp_resource_logs_append_scope_logs(otlp_resource_logs *resource_logs,
                                     const otlp_scope_logs *scope_logs) {
  otlp_scope_logs copy;
  otlp_status status;
  otlp_scope_logs_init(&copy);
  status = otlp_scope_logs_copy(&copy, scope_logs);
  if (status != OTLP_STATUS_OK)
    return status;
  return otlp__append_primitive(
      (void **)&resource_logs->scope_logs.ptr, &resource_logs->scope_logs.len,
      &resource_logs->scope_logs.cap, sizeof(copy), &copy);
}

static inline otlp_status otlp_resource_logs_copy(otlp_resource_logs *dst,
                                                  const otlp_resource_logs *src) {
  size_t i;
  otlp_resource_logs_init(dst);
  if (src->has_resource) {
    dst->has_resource = 1;
    if (otlp_resource_copy(&dst->resource, &src->resource) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  }
  if (otlp__set_string(&dst->schema_url, src->schema_url.ptr,
                       src->schema_url.len) != OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  for (i = 0; i < src->scope_logs.len; ++i) {
    otlp_status status =
        otlp_resource_logs_append_scope_logs(dst, &src->scope_logs.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}

static inline size_t
otlp_resource_logs_encoded_size_inner(const otlp_resource_logs *resource_logs) {
  size_t size = 0;
  size_t i;
  if (resource_logs->has_resource)
    size += otlp__message_field_size(
        1, otlp_resource_encoded_size_inner(&resource_logs->resource));
  for (i = 0; i < resource_logs->scope_logs.len; ++i)
    size += otlp__message_field_size(2, otlp_scope_logs_encoded_size_inner(
                                            &resource_logs->scope_logs.ptr[i]));
  if (resource_logs->schema_url.len)
    size += otlp__bytes_field_size(3, resource_logs->schema_url.len);
  return size;
}

static inline void
otlp_resource_logs_encode_field(otlp__writer *writer, uint32_t field_number,
                                const otlp_resource_logs *resource_logs) {
  size_t i;
  size_t inner = otlp_resource_logs_encoded_size_inner(resource_logs);
  otlp__write_message_start(writer, field_number, inner);
  if (resource_logs->has_resource)
    otlp_resource_encode_field(writer, 1, &resource_logs->resource);
  for (i = 0; i < resource_logs->scope_logs.len; ++i)
    otlp_scope_logs_encode_field(writer, 2, &resource_logs->scope_logs.ptr[i]);
  otlp__write_string_field(writer, 3, &resource_logs->schema_url);
}

static inline otlp_status
otlp_resource_logs_decode_inner(otlp_resource_logs *resource_logs,
                                const uint8_t *data, size_t len) {
  otlp__reader reader;
  otlp_resource_logs_destroy(resource_logs);
  otlp_resource_logs_init(resource_logs);
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
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      resource_logs->has_resource = 1;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_resource_decode_inner(&resource_logs->resource, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      break;
    }
    case 2: {
      const uint8_t *ptr;
      size_t size;
      otlp_scope_logs scope_logs;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      otlp_scope_logs_init(&scope_logs);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_scope_logs_decode_inner(&scope_logs, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive(
              (void **)&resource_logs->scope_logs.ptr,
              &resource_logs->scope_logs.len, &resource_logs->scope_logs.cap,
              sizeof(scope_logs), &scope_logs) != OTLP_STATUS_OK)
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
      if (otlp__set_string(&resource_logs->schema_url, (const char *)ptr,
                           size) != OTLP_STATUS_OK)
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

static inline void otlp_export_logs_service_request_init(
    otlp_export_logs_service_request *request) {
  otlp__zero(request, sizeof(*request));
}

static inline void otlp_export_logs_service_request_destroy(
    otlp_export_logs_service_request *request) {
  size_t i;
  for (i = 0; i < request->resource_logs.len; ++i)
    otlp_resource_logs_destroy(&request->resource_logs.ptr[i]);
  free(request->resource_logs.ptr);
  otlp_export_logs_service_request_init(request);
}

static inline otlp_status otlp_export_logs_service_request_append_resource_logs(
    otlp_export_logs_service_request *request,
    const otlp_resource_logs *resource_logs) {
  otlp_resource_logs copy;
  otlp_status status;
  otlp_resource_logs_init(&copy);
  status = otlp_resource_logs_copy(&copy, resource_logs);
  if (status != OTLP_STATUS_OK)
    return status;
  return otlp__append_primitive(
      (void **)&request->resource_logs.ptr, &request->resource_logs.len,
      &request->resource_logs.cap, sizeof(copy), &copy);
}

static inline size_t otlp_export_logs_service_request_encoded_size(
    const otlp_export_logs_service_request *request) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < request->resource_logs.len; ++i)
    size += otlp__message_field_size(1, otlp_resource_logs_encoded_size_inner(
                                            &request->resource_logs.ptr[i]));
  return size;
}

static inline otlp_status otlp_export_logs_service_request_encode(
    const otlp_export_logs_service_request *request, uint8_t *out, size_t len) {
  otlp__writer writer;
  size_t i;
  otlp__writer_init(&writer, out, len);
  for (i = 0; i < request->resource_logs.len; ++i)
    otlp_resource_logs_encode_field(&writer, 1, &request->resource_logs.ptr[i]);
  if (writer.status != OTLP_STATUS_OK)
    return writer.status;
  return writer.off == len ? OTLP_STATUS_OK : OTLP_STATUS_INVALID_ARGUMENT;
}

static inline otlp_status otlp_export_logs_service_request_decode(
    otlp_export_logs_service_request *request, const uint8_t *data,
    size_t len) {
  otlp__reader reader;
  otlp_export_logs_service_request_destroy(request);
  otlp_export_logs_service_request_init(request);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    const uint8_t *ptr;
    size_t size;
    otlp_resource_logs resource_logs;
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if (field_number != 1 || wire_type != 2) {
      if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
        return reader.status;
      continue;
    }
    otlp_resource_logs_init(&resource_logs);
    if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
      return reader.status;
    if (otlp_resource_logs_decode_inner(&resource_logs, ptr, size) !=
        OTLP_STATUS_OK)
      return OTLP_STATUS_DECODE_ERROR;
    if (otlp__append_primitive(
            (void **)&request->resource_logs.ptr, &request->resource_logs.len,
            &request->resource_logs.cap, sizeof(resource_logs),
            &resource_logs) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  }
  return reader.status;
}

static inline void otlp_logs_data_init(otlp_logs_data *data) {
  otlp__zero(data, sizeof(*data));
}

static inline void otlp_logs_data_destroy(otlp_logs_data *data) {
  size_t i;
  for (i = 0; i < data->resource_logs.len; ++i)
    otlp_resource_logs_destroy(&data->resource_logs.ptr[i]);
  free(data->resource_logs.ptr);
  otlp_logs_data_init(data);
}

static inline size_t otlp_logs_data_encoded_size(const otlp_logs_data *data) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < data->resource_logs.len; ++i)
    size += otlp__message_field_size(1, otlp_resource_logs_encoded_size_inner(
                                            &data->resource_logs.ptr[i]));
  return size;
}

static inline otlp_status otlp_logs_data_encode(const otlp_logs_data *data,
                                                uint8_t *out, size_t len) {
  otlp__writer writer;
  size_t i;
  otlp__writer_init(&writer, out, len);
  for (i = 0; i < data->resource_logs.len; ++i)
    otlp_resource_logs_encode_field(&writer, 1, &data->resource_logs.ptr[i]);
  if (writer.status != OTLP_STATUS_OK)
    return writer.status;
  return writer.off == len ? OTLP_STATUS_OK : OTLP_STATUS_INVALID_ARGUMENT;
}

static inline otlp_status otlp_logs_data_decode(otlp_logs_data *data,
                                                const uint8_t *bytes,
                                                size_t len) {
  otlp_export_logs_service_request request;
  otlp_export_logs_service_request_init(&request);
  if (otlp_export_logs_service_request_decode(&request, bytes, len) !=
      OTLP_STATUS_OK) {
    otlp_export_logs_service_request_destroy(&request);
    return OTLP_STATUS_DECODE_ERROR;
  }
  otlp_logs_data_destroy(data);
  data->resource_logs = request.resource_logs;
  return OTLP_STATUS_OK;
}

static inline void otlp_export_logs_partial_success_init(
    otlp_export_logs_partial_success *partial_success) {
  otlp__zero(partial_success, sizeof(*partial_success));
}

static inline void otlp_export_logs_partial_success_destroy(
    otlp_export_logs_partial_success *partial_success) {
  free(partial_success->error_message.ptr);
  otlp_export_logs_partial_success_init(partial_success);
}

static inline size_t otlp_export_logs_partial_success_encoded_size_inner(
    const otlp_export_logs_partial_success *partial_success) {
  size_t size = 0;
  if (partial_success->rejected_log_records)
    size += otlp__varint_field_size(
        1, (uint64_t)partial_success->rejected_log_records);
  if (partial_success->error_message.len)
    size += otlp__bytes_field_size(2, partial_success->error_message.len);
  return size;
}

static inline void otlp_export_logs_partial_success_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_export_logs_partial_success *partial_success) {
  size_t inner =
      otlp_export_logs_partial_success_encoded_size_inner(partial_success);
  otlp__write_message_start(writer, field_number, inner);
  if (partial_success->rejected_log_records) {
    otlp__write_tag(writer, 1, 0);
    otlp__write_varint(writer, (uint64_t)partial_success->rejected_log_records);
  }
  otlp__write_string_field(writer, 2, &partial_success->error_message);
}

static inline otlp_status otlp_export_logs_partial_success_decode_inner(
    otlp_export_logs_partial_success *partial_success, const uint8_t *data,
    size_t len) {
  otlp__reader reader;
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    switch (field_number) {
    case 1: {
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      partial_success->rejected_log_records =
          (int64_t)otlp__read_varint(&reader);
      break;
    }
    case 2: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&partial_success->error_message, (const char *)ptr,
                           size) != OTLP_STATUS_OK)
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

static inline void otlp_export_logs_service_response_init(
    otlp_export_logs_service_response *response) {
  otlp__zero(response, sizeof(*response));
}

static inline void otlp_export_logs_service_response_destroy(
    otlp_export_logs_service_response *response) {
  if (response->has_partial_success)
    otlp_export_logs_partial_success_destroy(&response->partial_success);
  otlp_export_logs_service_response_init(response);
}

static inline size_t otlp_export_logs_service_response_encoded_size(
    const otlp_export_logs_service_response *response) {
  size_t size = 0;
  if (response->has_partial_success)
    size += otlp__message_field_size(
        1, otlp_export_logs_partial_success_encoded_size_inner(
               &response->partial_success));
  return size;
}

static inline otlp_status otlp_export_logs_service_response_encode(
    const otlp_export_logs_service_response *response, uint8_t *out,
    size_t len) {
  otlp__writer writer;
  otlp__writer_init(&writer, out, len);
  if (response->has_partial_success)
    otlp_export_logs_partial_success_encode_field(
        &writer, 1, &response->partial_success);
  if (writer.status != OTLP_STATUS_OK)
    return writer.status;
  return writer.off == len ? OTLP_STATUS_OK : OTLP_STATUS_INVALID_ARGUMENT;
}

static inline otlp_status otlp_export_logs_service_response_decode(
    otlp_export_logs_service_response *response, const uint8_t *data,
    size_t len) {
  otlp__reader reader;
  otlp_export_logs_service_response_destroy(response);
  otlp_export_logs_service_response_init(response);
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
      if (otlp_export_logs_partial_success_decode_inner(
              &response->partial_success, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      continue;
    }
    if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
      return reader.status;
  }
  return reader.status;
}
#endif

#endif /* OTLP_FORMAT_LOGS_H */
