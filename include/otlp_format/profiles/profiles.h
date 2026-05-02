#ifndef OTLP_FORMAT_PROFILES_H
#define OTLP_FORMAT_PROFILES_H

#include "../common/common.h"

#if defined(OTLP_FORMAT_ENABLE_PROFILES)
typedef struct otlp_profiles_dev_value_type otlp_profiles_dev_value_type;
typedef struct otlp_profiles_dev_link otlp_profiles_dev_link;
typedef struct otlp_profiles_dev_line otlp_profiles_dev_line;
typedef struct otlp_profiles_dev_location otlp_profiles_dev_location;
typedef struct otlp_profiles_dev_mapping otlp_profiles_dev_mapping;
typedef struct otlp_profiles_dev_function otlp_profiles_dev_function;
typedef struct otlp_profiles_dev_key_value_and_unit otlp_profiles_dev_key_value_and_unit;
typedef struct otlp_profiles_dev_stack otlp_profiles_dev_stack;
typedef struct otlp_profiles_dev_sample otlp_profiles_dev_sample;
typedef struct otlp_profiles_dev_profile otlp_profiles_dev_profile;
typedef struct otlp_profiles_dev_scope_profiles otlp_profiles_dev_scope_profiles;
typedef struct otlp_profiles_dev_resource_profiles otlp_profiles_dev_resource_profiles;
typedef struct otlp_profiles_dev_profiles_dictionary otlp_profiles_dev_profiles_dictionary;
typedef struct otlp_profiles_dev_profiles_data otlp_profiles_dev_profiles_data;
typedef struct otlp_profiles_dev_export_profiles_service_request otlp_profiles_dev_export_profiles_service_request;
typedef struct otlp_profiles_dev_export_profiles_partial_success otlp_profiles_dev_export_profiles_partial_success;
typedef struct otlp_profiles_dev_export_profiles_service_response otlp_profiles_dev_export_profiles_service_response;
OTLP_DECLARE_ARRAY(otlp_profiles_dev_link_array, otlp_profiles_dev_link);
OTLP_DECLARE_ARRAY(otlp_profiles_dev_line_array, otlp_profiles_dev_line);
OTLP_DECLARE_ARRAY(otlp_profiles_dev_location_array, otlp_profiles_dev_location);
OTLP_DECLARE_ARRAY(otlp_profiles_dev_mapping_array, otlp_profiles_dev_mapping);
OTLP_DECLARE_ARRAY(otlp_profiles_dev_function_array, otlp_profiles_dev_function);
OTLP_DECLARE_ARRAY(otlp_profiles_dev_key_value_and_unit_array, otlp_profiles_dev_key_value_and_unit);
OTLP_DECLARE_ARRAY(otlp_profiles_dev_stack_array, otlp_profiles_dev_stack);
OTLP_DECLARE_ARRAY(otlp_profiles_dev_sample_array, otlp_profiles_dev_sample);
OTLP_DECLARE_ARRAY(otlp_profiles_dev_profile_array, otlp_profiles_dev_profile);
OTLP_DECLARE_ARRAY(otlp_profiles_dev_scope_profiles_array, otlp_profiles_dev_scope_profiles);
OTLP_DECLARE_ARRAY(otlp_profiles_dev_resource_profiles_array, otlp_profiles_dev_resource_profiles);

struct otlp_profiles_dev_value_type {
  int32_t type_strindex;
  int32_t unit_strindex;
};

struct otlp_profiles_dev_link {
  otlp_bytes trace_id;
  otlp_bytes span_id;
};

struct otlp_profiles_dev_line {
  int32_t function_index;
  int64_t line;
  int64_t column;
};

struct otlp_profiles_dev_location {
  int32_t mapping_index;
  uint64_t address;
  otlp_profiles_dev_line_array lines;
  otlp_int32_array attribute_indices;
};

struct otlp_profiles_dev_mapping {
  uint64_t memory_start;
  uint64_t memory_limit;
  uint64_t file_offset;
  int32_t filename_strindex;
  otlp_int32_array attribute_indices;
};

struct otlp_profiles_dev_function {
  int32_t name_strindex;
  int32_t system_name_strindex;
  int32_t filename_strindex;
  int64_t start_line;
};

struct otlp_profiles_dev_key_value_and_unit {
  int32_t key_strindex;
  otlp_any_value value;
  int32_t unit_strindex;
};

struct otlp_profiles_dev_stack {
  otlp_int32_array location_indices;
};

struct otlp_profiles_dev_sample {
  int32_t stack_index;
  otlp_int32_array attribute_indices;
  int32_t link_index;
  otlp_int64_array values;
  otlp_uint64_array timestamps_unix_nano;
};

struct otlp_profiles_dev_profile {
  int has_sample_type;
  otlp_profiles_dev_value_type sample_type;
  otlp_profiles_dev_sample_array samples;
  uint64_t time_unix_nano;
  uint64_t duration_nano;
  otlp_profiles_dev_value_type period_type;
  int64_t period;
  otlp_bytes profile_id;
  uint32_t dropped_attributes_count;
  otlp_string original_payload_format;
  otlp_bytes original_payload;
  otlp_int32_array attribute_indices;
};

static inline void otlp_profiles_dev_sample_init(otlp_profiles_dev_sample *sample);
static inline void otlp_profiles_dev_sample_destroy(otlp_profiles_dev_sample *sample);
static inline otlp_status otlp_profiles_dev_sample_append_value(otlp_profiles_dev_sample *sample, int64_t value);
static inline void otlp_profiles_dev_profile_init(otlp_profiles_dev_profile *profile);
static inline void otlp_profiles_dev_profile_destroy(otlp_profiles_dev_profile *profile);
static inline otlp_status otlp_profiles_dev_profile_append_sample(otlp_profiles_dev_profile *profile, const otlp_profiles_dev_sample *sample);

struct otlp_profiles_dev_resource_profiles {
  int has_resource;
  otlp_resource resource;
  otlp_profiles_dev_scope_profiles_array scope_profiles;
  otlp_string schema_url;
};

static inline void otlp_profiles_dev_resource_profiles_init(otlp_profiles_dev_resource_profiles *resource_profiles);
static inline void otlp_profiles_dev_resource_profiles_destroy(otlp_profiles_dev_resource_profiles *resource_profiles);
static inline otlp_status otlp_profiles_dev_resource_profiles_append_scope_profiles(otlp_profiles_dev_resource_profiles *resource_profiles, const otlp_profiles_dev_scope_profiles *scope_profiles);
static inline void otlp_profiles_dev_profiles_dictionary_init(otlp_profiles_dev_profiles_dictionary *dictionary);
static inline void otlp_profiles_dev_profiles_dictionary_destroy(otlp_profiles_dev_profiles_dictionary *dictionary);
static inline otlp_status otlp_profiles_dev_profiles_dictionary_init_default_zero_entries(otlp_profiles_dev_profiles_dictionary *dictionary);

struct otlp_profiles_dev_scope_profiles {
  int has_scope;
  otlp_instrumentation_scope scope;
  otlp_profiles_dev_profile_array profiles;
  otlp_string schema_url;
};

static inline void otlp_profiles_dev_scope_profiles_init(otlp_profiles_dev_scope_profiles *scope_profiles);
static inline void otlp_profiles_dev_scope_profiles_destroy(otlp_profiles_dev_scope_profiles *scope_profiles);
static inline otlp_status otlp_profiles_dev_scope_profiles_append_profile(otlp_profiles_dev_scope_profiles *scope_profiles, const otlp_profiles_dev_profile *profile);

struct otlp_profiles_dev_profiles_dictionary {
  otlp_profiles_dev_mapping_array mapping_table;
  otlp_profiles_dev_location_array location_table;
  otlp_profiles_dev_function_array function_table;
  otlp_profiles_dev_link_array link_table;
  otlp_string_array string_table;
  otlp_profiles_dev_key_value_and_unit_array attribute_table;
  otlp_profiles_dev_stack_array stack_table;
};

struct otlp_profiles_dev_export_profiles_service_request {
  otlp_profiles_dev_resource_profiles_array resource_profiles;
  otlp_profiles_dev_profiles_dictionary dictionary;
};

struct otlp_profiles_dev_profiles_data {
  otlp_profiles_dev_resource_profiles_array resource_profiles;
  otlp_profiles_dev_profiles_dictionary dictionary;
};

struct otlp_profiles_dev_export_profiles_partial_success {
  int64_t rejected_profiles;
  otlp_string error_message;
};

struct otlp_profiles_dev_export_profiles_service_response {
  int has_partial_success;
  otlp_profiles_dev_export_profiles_partial_success partial_success;
};

static inline void otlp_profiles_dev_export_profiles_service_request_init(otlp_profiles_dev_export_profiles_service_request *request);
static inline void otlp_profiles_dev_export_profiles_service_request_destroy(otlp_profiles_dev_export_profiles_service_request *request);
static inline otlp_status otlp_profiles_dev_export_profiles_service_request_append_resource_profiles(otlp_profiles_dev_export_profiles_service_request *request, const otlp_profiles_dev_resource_profiles *resource_profiles);
static inline otlp_status otlp_profiles_dev_profiles_dictionary_validate_zero_entries(const otlp_profiles_dev_profiles_dictionary *dictionary);
static inline size_t otlp_profiles_dev_export_profiles_service_request_encoded_size(const otlp_profiles_dev_export_profiles_service_request *request);
static inline otlp_status otlp_profiles_dev_export_profiles_service_request_encode(const otlp_profiles_dev_export_profiles_service_request *request, uint8_t *out, size_t len);
static inline otlp_status otlp_profiles_dev_export_profiles_service_request_decode(otlp_profiles_dev_export_profiles_service_request *request, const uint8_t *data, size_t len);
static inline void otlp_profiles_dev_profiles_data_init(otlp_profiles_dev_profiles_data *data);
static inline void otlp_profiles_dev_profiles_data_destroy(otlp_profiles_dev_profiles_data *data);
static inline size_t otlp_profiles_dev_profiles_data_encoded_size(const otlp_profiles_dev_profiles_data *data);
static inline otlp_status otlp_profiles_dev_profiles_data_encode(const otlp_profiles_dev_profiles_data *data, uint8_t *out, size_t len);
static inline otlp_status otlp_profiles_dev_profiles_data_decode(otlp_profiles_dev_profiles_data *data, const uint8_t *bytes, size_t len);
static inline void otlp_profiles_dev_export_profiles_service_response_init(otlp_profiles_dev_export_profiles_service_response *response);
static inline void otlp_profiles_dev_export_profiles_service_response_destroy(otlp_profiles_dev_export_profiles_service_response *response);
static inline size_t otlp_profiles_dev_export_profiles_service_response_encoded_size(const otlp_profiles_dev_export_profiles_service_response *response);
static inline otlp_status otlp_profiles_dev_export_profiles_service_response_encode(const otlp_profiles_dev_export_profiles_service_response *response, uint8_t *out, size_t len);
static inline otlp_status otlp_profiles_dev_export_profiles_service_response_decode(otlp_profiles_dev_export_profiles_service_response *response, const uint8_t *data, size_t len);

static inline size_t
otlp_profiles_dev_packed_int32_size(const otlp_int32_array *values) {
  size_t i;
  size_t size = 0;
  for (i = 0; i < values->len; ++i)
    size += otlp__varint_size((uint64_t)(int64_t)values->ptr[i]);
  return size;
}

static inline size_t
otlp_profiles_dev_packed_int64_size(const otlp_int64_array *values) {
  size_t i;
  size_t size = 0;
  for (i = 0; i < values->len; ++i)
    size += otlp__varint_size((uint64_t)values->ptr[i]);
  return size;
}

static inline void
otlp_profiles_dev_write_packed_int32_field(otlp__writer *writer,
                                           uint32_t field_number,
                                           const otlp_int32_array *values) {
  size_t i;
  size_t payload;
  if (values->len == 0)
    return;
  payload = otlp_profiles_dev_packed_int32_size(values);
  otlp__write_tag(writer, field_number, 2);
  otlp__write_varint(writer, (uint64_t)payload);
  for (i = 0; i < values->len; ++i)
    otlp__write_varint(writer, (uint64_t)(int64_t)values->ptr[i]);
}

static inline void
otlp_profiles_dev_write_packed_int64_field(otlp__writer *writer,
                                           uint32_t field_number,
                                           const otlp_int64_array *values) {
  size_t i;
  size_t payload;
  if (values->len == 0)
    return;
  payload = otlp_profiles_dev_packed_int64_size(values);
  otlp__write_tag(writer, field_number, 2);
  otlp__write_varint(writer, (uint64_t)payload);
  for (i = 0; i < values->len; ++i)
    otlp__write_varint(writer, (uint64_t)values->ptr[i]);
}

static inline size_t
otlp_profiles_dev_packed_fixed64_size(const otlp_uint64_array *values) {
  return values->len * 8U;
}

static inline void
otlp_profiles_dev_write_packed_fixed64_field(otlp__writer *writer,
                                             uint32_t field_number,
                                             const otlp_uint64_array *values) {
  size_t i;
  if (values->len == 0)
    return;
  otlp__write_tag(writer, field_number, 2);
  otlp__write_varint(writer, (uint64_t)otlp_profiles_dev_packed_fixed64_size(values));
  for (i = 0; i < values->len; ++i)
    otlp__write_fixed64(writer, values->ptr[i]);
}

static inline otlp_status
otlp_profiles_dev_append_packed_int32(otlp_int32_array *dst,
                                      const uint8_t *data, size_t len) {
  otlp__reader reader;
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    int32_t value = (int32_t)otlp__read_varint(&reader);
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if (otlp__append_primitive((void **)&dst->ptr, &dst->len, &dst->cap,
                               sizeof(value), &value) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  }
  return reader.status;
}

static inline otlp_status
otlp_profiles_dev_append_packed_int64(otlp_int64_array *dst,
                                      const uint8_t *data, size_t len) {
  otlp__reader reader;
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    int64_t value = otlp__int64_from_varint(otlp__read_varint(&reader));
    if (reader.status != OTLP_STATUS_OK)
      return reader.status;
    if (otlp__append_primitive((void **)&dst->ptr, &dst->len, &dst->cap,
                               sizeof(value), &value) != OTLP_STATUS_OK)
      return OTLP_STATUS_OUT_OF_MEMORY;
  }
  return reader.status;
}

static inline otlp_status
otlp_profiles_dev_append_packed_fixed64(otlp_uint64_array *dst,
                                        const uint8_t *data, size_t len) {
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

static inline void otlp_profiles_dev_link_init(otlp_profiles_dev_link *link) {
  otlp__zero(link, sizeof(*link));
}

static inline void
otlp_profiles_dev_link_destroy(otlp_profiles_dev_link *link) {
  free(link->trace_id.ptr);
  free(link->span_id.ptr);
  otlp_profiles_dev_link_init(link);
}

static inline otlp_status
otlp_profiles_dev_link_copy(otlp_profiles_dev_link *dst,
                            const otlp_profiles_dev_link *src) {
  otlp_profiles_dev_link_init(dst);

  if (otlp__set_bytes(&dst->trace_id, src->trace_id.ptr, src->trace_id.len) !=
      OTLP_STATUS_OK) {
    return OTLP_STATUS_OUT_OF_MEMORY;
  }

  return otlp__set_bytes(&dst->span_id, src->span_id.ptr, src->span_id.len);
}

static inline size_t
otlp_profiles_dev_link_encoded_size_inner(const otlp_profiles_dev_link *link) {
  size_t size = 0;

  if (link->trace_id.len) {
    size += otlp__bytes_field_size(1, link->trace_id.len);
  }

  if (link->span_id.len) {
    size += otlp__bytes_field_size(2, link->span_id.len);
  }

  return size;
}

static inline void
otlp_profiles_dev_link_encode_field(otlp__writer *writer, uint32_t field_number,
                                    const otlp_profiles_dev_link *link) {
  otlp__write_message_start(writer, field_number,
                            otlp_profiles_dev_link_encoded_size_inner(link));
  otlp__write_bytes_field(writer, 1, link->trace_id.ptr, link->trace_id.len);
  otlp__write_bytes_field(writer, 2, link->span_id.ptr, link->span_id.len);
}

static inline otlp_status
otlp_profiles_dev_link_decode_inner(otlp_profiles_dev_link *link,
                                    const uint8_t *data, size_t len) {
  otlp__reader reader;

  otlp_profiles_dev_link_destroy(link);
  otlp_profiles_dev_link_init(link);
  otlp__reader_init(&reader, data, len);

  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    const uint8_t *ptr;
    size_t size;

    if (field_number == 1 && wire_type == 2) {
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK) {
        return reader.status;
      }
      if (otlp__set_bytes(&link->trace_id, ptr, size) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 2 && wire_type == 2) {
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK) {
        return reader.status;
      }
      if (otlp__set_bytes(&link->span_id, ptr, size) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }

  return reader.status;
}

static inline void otlp_profiles_dev_line_init(otlp_profiles_dev_line *line) {
  otlp__zero(line, sizeof(*line));
}

static inline void
otlp_profiles_dev_line_destroy(otlp_profiles_dev_line *line) {
  otlp_profiles_dev_line_init(line);
}

static inline size_t
otlp_profiles_dev_line_encoded_size_inner(const otlp_profiles_dev_line *line) {
  size_t size = 0;

  if (line->function_index) {
    size += otlp__int32_field_size(1, line->function_index);
  }

  if (line->line) {
    size += otlp__varint_field_size(2, (uint64_t)line->line);
  }

  if (line->column) {
    size += otlp__varint_field_size(3, (uint64_t)line->column);
  }

  return size;
}

static inline void
otlp_profiles_dev_line_encode_field(otlp__writer *writer, uint32_t field_number,
                                    const otlp_profiles_dev_line *line) {
  size_t inner = otlp_profiles_dev_line_encoded_size_inner(line);

  otlp__write_message_start(writer, field_number, inner);

  if (line->function_index) {
    otlp__write_tag(writer, 1, 0);
    otlp__write_varint(writer, otlp__int32_varint(line->function_index));
  }

  if (line->line) {
    otlp__write_tag(writer, 2, 0);
    otlp__write_varint(writer, (uint64_t)line->line);
  }

  if (line->column) {
    otlp__write_tag(writer, 3, 0);
    otlp__write_varint(writer, (uint64_t)line->column);
  }
}

static inline otlp_status
otlp_profiles_dev_line_decode_inner(otlp_profiles_dev_line *line,
                                    const uint8_t *data, size_t len) {
  otlp__reader reader;

  otlp_profiles_dev_line_destroy(line);
  otlp_profiles_dev_line_init(line);
  otlp__reader_init(&reader, data, len);

  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);

    if (field_number == 1 && wire_type == 0) {
      line->function_index = (int32_t)otlp__read_varint(&reader);
    } else if (field_number == 2 && wire_type == 0) {
      line->line = (int64_t)otlp__read_varint(&reader);
    } else if (field_number == 3 && wire_type == 0) {
      line->column = (int64_t)otlp__read_varint(&reader);
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }

  return reader.status;
}

static inline void
otlp_profiles_dev_location_init(otlp_profiles_dev_location *location) {
  otlp__zero(location, sizeof(*location));
}

static inline void
otlp_profiles_dev_location_destroy(otlp_profiles_dev_location *location) {
  free(location->lines.ptr);
  free(location->attribute_indices.ptr);
  otlp_profiles_dev_location_init(location);
}

static inline otlp_status
otlp_profiles_dev_location_copy(otlp_profiles_dev_location *dst,
                                const otlp_profiles_dev_location *src) {
  size_t i;

  otlp_profiles_dev_location_init(dst);
  dst->mapping_index = src->mapping_index;
  dst->address = src->address;

  for (i = 0; i < src->lines.len; ++i) {
    if (otlp__append_primitive((void **)&dst->lines.ptr, &dst->lines.len,
                               &dst->lines.cap, sizeof(src->lines.ptr[i]),
                               &src->lines.ptr[i]) != OTLP_STATUS_OK) {
      return OTLP_STATUS_OUT_OF_MEMORY;
    }
  }

  for (i = 0; i < src->attribute_indices.len; ++i) {
    if (otlp__append_primitive(
            (void **)&dst->attribute_indices.ptr, &dst->attribute_indices.len,
            &dst->attribute_indices.cap, sizeof(src->attribute_indices.ptr[i]),
            &src->attribute_indices.ptr[i]) != OTLP_STATUS_OK) {
      return OTLP_STATUS_OUT_OF_MEMORY;
    }
  }

  return OTLP_STATUS_OK;
}

static inline size_t otlp_profiles_dev_location_encoded_size_inner(
    const otlp_profiles_dev_location *location) {
  size_t size = 0;
  size_t i;

  if (location->mapping_index) {
    size += otlp__int32_field_size(1, location->mapping_index);
  }

  if (location->address) {
    size += otlp__varint_field_size(2, location->address);
  }

  for (i = 0; i < location->lines.len; ++i) {
    size += otlp__message_field_size(
        3, otlp_profiles_dev_line_encoded_size_inner(&location->lines.ptr[i]));
  }

  if (location->attribute_indices.len) {
    size += otlp__bytes_field_size(
        4, otlp_profiles_dev_packed_int32_size(&location->attribute_indices));
  }

  return size;
}

static inline void otlp_profiles_dev_location_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_profiles_dev_location *location) {
  size_t i;
  size_t inner = otlp_profiles_dev_location_encoded_size_inner(location);

  otlp__write_message_start(writer, field_number, inner);

  if (location->mapping_index) {
    otlp__write_tag(writer, 1, 0);
    otlp__write_varint(writer, otlp__int32_varint(location->mapping_index));
  }

  if (location->address) {
    otlp__write_tag(writer, 2, 0);
    otlp__write_varint(writer, location->address);
  }

  for (i = 0; i < location->lines.len; ++i) {
    otlp_profiles_dev_line_encode_field(writer, 3, &location->lines.ptr[i]);
  }

  otlp_profiles_dev_write_packed_int32_field(writer, 4,
                                             &location->attribute_indices);
}

static inline otlp_status
otlp_profiles_dev_location_decode_inner(otlp_profiles_dev_location *location,
                                        const uint8_t *data, size_t len) {
  otlp__reader reader;

  otlp_profiles_dev_location_destroy(location);
  otlp_profiles_dev_location_init(location);
  otlp__reader_init(&reader, data, len);

  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    const uint8_t *ptr;
    size_t size;

    if (field_number == 1 && wire_type == 0) {
      location->mapping_index = (int32_t)otlp__read_varint(&reader);
    } else if (field_number == 2 && wire_type == 0) {
      location->address = otlp__read_varint(&reader);
    } else if (field_number == 3 && wire_type == 2) {
      otlp_profiles_dev_line line;
      otlp_profiles_dev_line_init(&line);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_line_decode_inner(&line, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&location->lines.ptr,
                                 &location->lines.len, &location->lines.cap,
                                 sizeof(line), &line) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (field_number == 4 && wire_type == 0) {
      int32_t value = (int32_t)otlp__read_varint(&reader);
      if (otlp__append_primitive((void **)&location->attribute_indices.ptr,
                                 &location->attribute_indices.len,
                                 &location->attribute_indices.cap,
                                 sizeof(value), &value) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (field_number == 4 && wire_type == 2) {
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_append_packed_int32(&location->attribute_indices,
                                                ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }

  return reader.status;
}

static inline void
otlp_profiles_dev_mapping_init(otlp_profiles_dev_mapping *mapping) {
  otlp__zero(mapping, sizeof(*mapping));
}

static inline void
otlp_profiles_dev_mapping_destroy(otlp_profiles_dev_mapping *mapping) {
  free(mapping->attribute_indices.ptr);
  otlp_profiles_dev_mapping_init(mapping);
}

static inline otlp_status
otlp_profiles_dev_mapping_copy(otlp_profiles_dev_mapping *dst,
                               const otlp_profiles_dev_mapping *src) {
  size_t i;

  otlp_profiles_dev_mapping_init(dst);
  *dst = *src;
  dst->attribute_indices.ptr = NULL;
  dst->attribute_indices.len = 0;
  dst->attribute_indices.cap = 0;

  for (i = 0; i < src->attribute_indices.len; ++i) {
    if (otlp__append_primitive(
            (void **)&dst->attribute_indices.ptr, &dst->attribute_indices.len,
            &dst->attribute_indices.cap, sizeof(src->attribute_indices.ptr[i]),
            &src->attribute_indices.ptr[i]) != OTLP_STATUS_OK) {
      return OTLP_STATUS_OUT_OF_MEMORY;
    }
  }

  return OTLP_STATUS_OK;
}

static inline size_t otlp_profiles_dev_mapping_encoded_size_inner(
    const otlp_profiles_dev_mapping *mapping) {
  size_t size = 0;

  if (mapping->memory_start) {
    size += otlp__varint_field_size(1, mapping->memory_start);
  }

  if (mapping->memory_limit) {
    size += otlp__varint_field_size(2, mapping->memory_limit);
  }

  if (mapping->file_offset) {
    size += otlp__varint_field_size(3, mapping->file_offset);
  }

  if (mapping->filename_strindex) {
    size += otlp__int32_field_size(4, mapping->filename_strindex);
  }

  if (mapping->attribute_indices.len) {
    size += otlp__bytes_field_size(
        5, otlp_profiles_dev_packed_int32_size(&mapping->attribute_indices));
  }

  return size;
}

static inline void otlp_profiles_dev_mapping_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_profiles_dev_mapping *mapping) {
  size_t inner = otlp_profiles_dev_mapping_encoded_size_inner(mapping);

  otlp__write_message_start(writer, field_number, inner);

  if (mapping->memory_start) {
    otlp__write_tag(writer, 1, 0);
    otlp__write_varint(writer, mapping->memory_start);
  }

  if (mapping->memory_limit) {
    otlp__write_tag(writer, 2, 0);
    otlp__write_varint(writer, mapping->memory_limit);
  }

  if (mapping->file_offset) {
    otlp__write_tag(writer, 3, 0);
    otlp__write_varint(writer, mapping->file_offset);
  }

  if (mapping->filename_strindex) {
    otlp__write_tag(writer, 4, 0);
    otlp__write_varint(writer, otlp__int32_varint(mapping->filename_strindex));
  }

  otlp_profiles_dev_write_packed_int32_field(writer, 5,
                                             &mapping->attribute_indices);
}

static inline otlp_status
otlp_profiles_dev_mapping_decode_inner(otlp_profiles_dev_mapping *mapping,
                                       const uint8_t *data, size_t len) {
  otlp__reader reader;

  otlp_profiles_dev_mapping_destroy(mapping);
  otlp_profiles_dev_mapping_init(mapping);
  otlp__reader_init(&reader, data, len);

  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);

    if (field_number == 1 && wire_type == 0) {
      mapping->memory_start = otlp__read_varint(&reader);
    } else if (field_number == 2 && wire_type == 0) {
      mapping->memory_limit = otlp__read_varint(&reader);
    } else if (field_number == 3 && wire_type == 0) {
      mapping->file_offset = otlp__read_varint(&reader);
    } else if (field_number == 4 && wire_type == 0) {
      mapping->filename_strindex = (int32_t)otlp__read_varint(&reader);
    } else if (field_number == 5 && wire_type == 0) {
      int32_t value = (int32_t)otlp__read_varint(&reader);
      if (otlp__append_primitive((void **)&mapping->attribute_indices.ptr,
                                 &mapping->attribute_indices.len,
                                 &mapping->attribute_indices.cap, sizeof(value),
                                 &value) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (field_number == 5 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_append_packed_int32(&mapping->attribute_indices,
                                                ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }

  return reader.status;
}

static inline void
otlp_profiles_dev_function_init(otlp_profiles_dev_function *function_value) {
  otlp__zero(function_value, sizeof(*function_value));
}

static inline void
otlp_profiles_dev_function_destroy(otlp_profiles_dev_function *function_value) {
  otlp_profiles_dev_function_init(function_value);
}

static inline otlp_status
otlp_profiles_dev_function_copy(otlp_profiles_dev_function *dst,
                                const otlp_profiles_dev_function *src) {
  *dst = *src;
  return OTLP_STATUS_OK;
}

static inline size_t otlp_profiles_dev_function_encoded_size_inner(
    const otlp_profiles_dev_function *function_value) {
  size_t size = 0;

  if (function_value->name_strindex) {
    size += otlp__int32_field_size(1, function_value->name_strindex);
  }

  if (function_value->system_name_strindex) {
    size += otlp__int32_field_size(2, function_value->system_name_strindex);
  }

  if (function_value->filename_strindex) {
    size += otlp__int32_field_size(3, function_value->filename_strindex);
  }

  if (function_value->start_line) {
    size += otlp__varint_field_size(4, (uint64_t)function_value->start_line);
  }

  return size;
}

static inline void otlp_profiles_dev_function_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_profiles_dev_function *function_value) {
  size_t inner = otlp_profiles_dev_function_encoded_size_inner(function_value);

  otlp__write_message_start(writer, field_number, inner);

  if (function_value->name_strindex) {
    otlp__write_tag(writer, 1, 0);
    otlp__write_varint(writer,
                       otlp__int32_varint(function_value->name_strindex));
  }

  if (function_value->system_name_strindex) {
    otlp__write_tag(writer, 2, 0);
    otlp__write_varint(
        writer, otlp__int32_varint(function_value->system_name_strindex));
  }

  if (function_value->filename_strindex) {
    otlp__write_tag(writer, 3, 0);
    otlp__write_varint(writer,
                       otlp__int32_varint(function_value->filename_strindex));
  }

  if (function_value->start_line) {
    otlp__write_tag(writer, 4, 0);
    otlp__write_varint(writer, (uint64_t)function_value->start_line);
  }
}

static inline otlp_status otlp_profiles_dev_function_decode_inner(
    otlp_profiles_dev_function *function_value, const uint8_t *data,
    size_t len) {
  otlp__reader reader;

  otlp_profiles_dev_function_destroy(function_value);
  otlp_profiles_dev_function_init(function_value);
  otlp__reader_init(&reader, data, len);

  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);

    if (field_number == 1 && wire_type == 0) {
      function_value->name_strindex = (int32_t)otlp__read_varint(&reader);
    } else if (field_number == 2 && wire_type == 0) {
      function_value->system_name_strindex =
          (int32_t)otlp__read_varint(&reader);
    } else if (field_number == 3 && wire_type == 0) {
      function_value->filename_strindex = (int32_t)otlp__read_varint(&reader);
    } else if (field_number == 4 && wire_type == 0) {
      function_value->start_line = (int64_t)otlp__read_varint(&reader);
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }

  return reader.status;
}

static inline void otlp_profiles_dev_key_value_and_unit_init(
    otlp_profiles_dev_key_value_and_unit *kv) {
  otlp__zero(kv, sizeof(*kv));
  otlp_any_value_init(&kv->value);
}

static inline void otlp_profiles_dev_key_value_and_unit_destroy(
    otlp_profiles_dev_key_value_and_unit *kv) {
  otlp_any_value_destroy(&kv->value);
  otlp_profiles_dev_key_value_and_unit_init(kv);
}

static inline otlp_status otlp_profiles_dev_key_value_and_unit_copy(
    otlp_profiles_dev_key_value_and_unit *dst,
    const otlp_profiles_dev_key_value_and_unit *src) {
  otlp_profiles_dev_key_value_and_unit_init(dst);
  dst->key_strindex = src->key_strindex;
  dst->unit_strindex = src->unit_strindex;
  return otlp_any_value_copy(&dst->value, &src->value);
}

static inline size_t otlp_profiles_dev_key_value_and_unit_encoded_size_inner(
    const otlp_profiles_dev_key_value_and_unit *kv) {
  size_t size = 0;

  if (kv->key_strindex) {
    size += otlp__int32_field_size(1, kv->key_strindex);
  }

  if (kv->value.kind != OTLP_ANY_VALUE_EMPTY) {
    size += otlp_any_value_encoded_size_field(2, &kv->value);
  }

  if (kv->unit_strindex) {
    size += otlp__int32_field_size(3, kv->unit_strindex);
  }

  return size;
}

static inline void otlp_profiles_dev_key_value_and_unit_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_profiles_dev_key_value_and_unit *kv) {
  size_t inner = otlp_profiles_dev_key_value_and_unit_encoded_size_inner(kv);

  otlp__write_message_start(writer, field_number, inner);

  if (kv->key_strindex) {
    otlp__write_tag(writer, 1, 0);
    otlp__write_varint(writer, otlp__int32_varint(kv->key_strindex));
  }

  if (kv->value.kind != OTLP_ANY_VALUE_EMPTY) {
    otlp_any_value_encode_field(writer, 2, &kv->value);
  }

  if (kv->unit_strindex) {
    otlp__write_tag(writer, 3, 0);
    otlp__write_varint(writer, otlp__int32_varint(kv->unit_strindex));
  }
}

static inline otlp_status otlp_profiles_dev_key_value_and_unit_decode_inner(
    otlp_profiles_dev_key_value_and_unit *kv, const uint8_t *data, size_t len) {
  otlp__reader reader;

  otlp_profiles_dev_key_value_and_unit_destroy(kv);
  otlp_profiles_dev_key_value_and_unit_init(kv);
  otlp__reader_init(&reader, data, len);

  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    const uint8_t *ptr;
    size_t size;

    if (field_number == 1 && wire_type == 0) {
      kv->key_strindex = (int32_t)otlp__read_varint(&reader);
    } else if (field_number == 2 && wire_type == 2) {
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_any_value_merge_inner(&kv->value, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
    } else if (field_number == 3 && wire_type == 0) {
      kv->unit_strindex = (int32_t)otlp__read_varint(&reader);
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }

  return reader.status;
}

static inline void
otlp_profiles_dev_stack_init(otlp_profiles_dev_stack *stack) {
  otlp__zero(stack, sizeof(*stack));
}

static inline void
otlp_profiles_dev_stack_destroy(otlp_profiles_dev_stack *stack) {
  free(stack->location_indices.ptr);
  otlp_profiles_dev_stack_init(stack);
}

static inline otlp_status
otlp_profiles_dev_stack_copy(otlp_profiles_dev_stack *dst,
                             const otlp_profiles_dev_stack *src) {
  size_t i;

  otlp_profiles_dev_stack_init(dst);

  for (i = 0; i < src->location_indices.len; ++i) {
    if (otlp__append_primitive(
            (void **)&dst->location_indices.ptr, &dst->location_indices.len,
            &dst->location_indices.cap, sizeof(src->location_indices.ptr[i]),
            &src->location_indices.ptr[i]) != OTLP_STATUS_OK) {
      return OTLP_STATUS_OUT_OF_MEMORY;
    }
  }

  return OTLP_STATUS_OK;
}

static inline size_t otlp_profiles_dev_stack_encoded_size_inner(
    const otlp_profiles_dev_stack *stack) {
  size_t size = 0;

  if (stack->location_indices.len) {
    size += otlp__bytes_field_size(
        1, otlp_profiles_dev_packed_int32_size(&stack->location_indices));
  }

  return size;
}

static inline void
otlp_profiles_dev_stack_encode_field(otlp__writer *writer,
                                     uint32_t field_number,
                                     const otlp_profiles_dev_stack *stack) {
  size_t inner = otlp_profiles_dev_stack_encoded_size_inner(stack);

  otlp__write_message_start(writer, field_number, inner);

  otlp_profiles_dev_write_packed_int32_field(writer, 1,
                                             &stack->location_indices);
}

static inline otlp_status
otlp_profiles_dev_stack_decode_inner(otlp_profiles_dev_stack *stack,
                                     const uint8_t *data, size_t len) {
  otlp__reader reader;

  otlp_profiles_dev_stack_destroy(stack);
  otlp_profiles_dev_stack_init(stack);
  otlp__reader_init(&reader, data, len);

  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);

    if (field_number == 1 && wire_type == 0) {
      int32_t value = (int32_t)otlp__read_varint(&reader);
      if (otlp__append_primitive((void **)&stack->location_indices.ptr,
                                 &stack->location_indices.len,
                                 &stack->location_indices.cap, sizeof(value),
                                 &value) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
    } else if (field_number == 1 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_append_packed_int32(&stack->location_indices,
                                                ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }

  return reader.status;
}

static inline void
otlp_profiles_dev_sample_init(otlp_profiles_dev_sample *sample) {
  otlp__zero(sample, sizeof(*sample));
}

static inline void
otlp_profiles_dev_sample_destroy(otlp_profiles_dev_sample *sample) {
  free(sample->attribute_indices.ptr);
  free(sample->values.ptr);
  free(sample->timestamps_unix_nano.ptr);
  otlp_profiles_dev_sample_init(sample);
}

static inline otlp_status
otlp_profiles_dev_sample_copy(otlp_profiles_dev_sample *dst,
                              const otlp_profiles_dev_sample *src) {
  size_t i;

  otlp_profiles_dev_sample_init(dst);
  dst->stack_index = src->stack_index;
  dst->link_index = src->link_index;

  for (i = 0; i < src->attribute_indices.len; ++i) {
    if (otlp__append_primitive(
            (void **)&dst->attribute_indices.ptr, &dst->attribute_indices.len,
            &dst->attribute_indices.cap, sizeof(src->attribute_indices.ptr[i]),
            &src->attribute_indices.ptr[i]) != OTLP_STATUS_OK) {
      return OTLP_STATUS_OUT_OF_MEMORY;
    }
  }

  for (i = 0; i < src->values.len; ++i) {
    if (otlp__append_primitive((void **)&dst->values.ptr, &dst->values.len,
                               &dst->values.cap, sizeof(src->values.ptr[i]),
                               &src->values.ptr[i]) != OTLP_STATUS_OK) {
      return OTLP_STATUS_OUT_OF_MEMORY;
    }
  }

  for (i = 0; i < src->timestamps_unix_nano.len; ++i) {
    if (otlp__append_primitive(
            (void **)&dst->timestamps_unix_nano.ptr,
            &dst->timestamps_unix_nano.len, &dst->timestamps_unix_nano.cap,
            sizeof(src->timestamps_unix_nano.ptr[i]),
            &src->timestamps_unix_nano.ptr[i]) != OTLP_STATUS_OK) {
      return OTLP_STATUS_OUT_OF_MEMORY;
    }
  }

  return OTLP_STATUS_OK;
}

static inline otlp_status
otlp_profiles_dev_sample_append_value(otlp_profiles_dev_sample *sample,
                                      int64_t value) {
  return otlp__append_primitive((void **)&sample->values.ptr,
                                &sample->values.len, &sample->values.cap,
                                sizeof(value), &value);
}

static inline size_t otlp_profiles_dev_sample_encoded_size_inner(
    const otlp_profiles_dev_sample *sample) {
  size_t size = 0;

  if (sample->stack_index) {
    size += otlp__int32_field_size(1, sample->stack_index);
  }

  if (sample->attribute_indices.len) {
    size += otlp__bytes_field_size(
        2, otlp_profiles_dev_packed_int32_size(&sample->attribute_indices));
  }

  if (sample->link_index) {
    size += otlp__int32_field_size(3, sample->link_index);
  }

  if (sample->values.len) {
    size += otlp__bytes_field_size(
        4, otlp_profiles_dev_packed_int64_size(&sample->values));
  }

  if (sample->timestamps_unix_nano.len) {
    size += otlp__bytes_field_size(
        5, otlp_profiles_dev_packed_fixed64_size(&sample->timestamps_unix_nano));
  }

  return size;
}

static inline void
otlp_profiles_dev_sample_encode_field(otlp__writer *writer,
                                      uint32_t field_number,
                                      const otlp_profiles_dev_sample *sample) {
  size_t inner = otlp_profiles_dev_sample_encoded_size_inner(sample);

  otlp__write_message_start(writer, field_number, inner);

  if (sample->stack_index) {
    otlp__write_tag(writer, 1, 0);
    otlp__write_varint(writer, otlp__int32_varint(sample->stack_index));
  }

  otlp_profiles_dev_write_packed_int32_field(writer, 2,
                                             &sample->attribute_indices);

  if (sample->link_index) {
    otlp__write_tag(writer, 3, 0);
    otlp__write_varint(writer, otlp__int32_varint(sample->link_index));
  }

  otlp_profiles_dev_write_packed_int64_field(writer, 4, &sample->values);

  otlp_profiles_dev_write_packed_fixed64_field(writer, 5,
                                               &sample->timestamps_unix_nano);
}

static inline size_t otlp_profiles_dev_value_type_encoded_size_inner(
    const otlp_profiles_dev_value_type *value_type) {
  size_t size = 0;

  if (value_type->type_strindex) {
    size += otlp__int32_field_size(1, value_type->type_strindex);
  }

  if (value_type->unit_strindex) {
    size += otlp__int32_field_size(2, value_type->unit_strindex);
  }

  return size;
}

static inline void otlp_profiles_dev_value_type_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_profiles_dev_value_type *value_type) {
  size_t inner = otlp_profiles_dev_value_type_encoded_size_inner(value_type);

  otlp__write_message_start(writer, field_number, inner);

  if (value_type->type_strindex) {
    otlp__write_tag(writer, 1, 0);
    otlp__write_varint(writer, otlp__int32_varint(value_type->type_strindex));
  }

  if (value_type->unit_strindex) {
    otlp__write_tag(writer, 2, 0);
    otlp__write_varint(writer, otlp__int32_varint(value_type->unit_strindex));
  }
}

static inline otlp_status otlp_profiles_dev_value_type_decode_inner(
    otlp_profiles_dev_value_type *value_type, const uint8_t *data,
    size_t len) {
  otlp__reader reader;
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (field_number == 1 && wire_type == 0) {
      value_type->type_strindex = (int32_t)otlp__read_varint(&reader);
    } else if (field_number == 2 && wire_type == 0) {
      value_type->unit_strindex = (int32_t)otlp__read_varint(&reader);
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }
  return reader.status;
}

static inline void
otlp_profiles_dev_profile_init(otlp_profiles_dev_profile *profile) {
  otlp__zero(profile, sizeof(*profile));
}

static inline void
otlp_profiles_dev_profile_destroy(otlp_profiles_dev_profile *profile) {
  size_t i;

  for (i = 0; i < profile->samples.len; ++i) {
    otlp_profiles_dev_sample_destroy(&profile->samples.ptr[i]);
  }

  free(profile->samples.ptr);
  free(profile->profile_id.ptr);
  free(profile->original_payload_format.ptr);
  free(profile->original_payload.ptr);
  free(profile->attribute_indices.ptr);
  otlp_profiles_dev_profile_init(profile);
}

static inline otlp_status
otlp_profiles_dev_profile_copy(otlp_profiles_dev_profile *dst,
                               const otlp_profiles_dev_profile *src) {
  size_t i;

  otlp_profiles_dev_profile_init(dst);
  dst->has_sample_type = src->has_sample_type;
  dst->sample_type = src->sample_type;
  dst->time_unix_nano = src->time_unix_nano;
  dst->duration_nano = src->duration_nano;
  dst->period_type = src->period_type;
  dst->period = src->period;
  dst->dropped_attributes_count = src->dropped_attributes_count;

  if (otlp__set_bytes(&dst->profile_id, src->profile_id.ptr,
                      src->profile_id.len) != OTLP_STATUS_OK) {
    return OTLP_STATUS_OUT_OF_MEMORY;
  }

  if (otlp__set_string(&dst->original_payload_format,
                       src->original_payload_format.ptr,
                       src->original_payload_format.len) != OTLP_STATUS_OK) {
    return OTLP_STATUS_OUT_OF_MEMORY;
  }

  if (otlp__set_bytes(&dst->original_payload, src->original_payload.ptr,
                      src->original_payload.len) != OTLP_STATUS_OK) {
    return OTLP_STATUS_OUT_OF_MEMORY;
  }

  for (i = 0; i < src->samples.len; ++i) {
    otlp_profiles_dev_sample copy;
    otlp_status status =
        otlp_profiles_dev_sample_copy(&copy, &src->samples.ptr[i]);

    if (status != OTLP_STATUS_OK) {
      return status;
    }

    status =
        otlp__append_primitive((void **)&dst->samples.ptr, &dst->samples.len,
                               &dst->samples.cap, sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK) {
      return status;
    }
  }

  for (i = 0; i < src->attribute_indices.len; ++i) {
    if (otlp__append_primitive(
            (void **)&dst->attribute_indices.ptr, &dst->attribute_indices.len,
            &dst->attribute_indices.cap, sizeof(src->attribute_indices.ptr[i]),
            &src->attribute_indices.ptr[i]) != OTLP_STATUS_OK) {
      return OTLP_STATUS_OUT_OF_MEMORY;
    }
  }

  return OTLP_STATUS_OK;
}

static inline otlp_status otlp_profiles_dev_profile_append_sample(
    otlp_profiles_dev_profile *profile,
    const otlp_profiles_dev_sample *sample) {
  otlp_profiles_dev_sample copy;
  otlp_status status = otlp_profiles_dev_sample_copy(&copy, sample);

  if (status != OTLP_STATUS_OK) {
    return status;
  }

  return otlp__append_primitive((void **)&profile->samples.ptr,
                                &profile->samples.len, &profile->samples.cap,
                                sizeof(copy), &copy);
}

static inline size_t otlp_profiles_dev_profile_encoded_size_inner(
    const otlp_profiles_dev_profile *profile) {
  size_t size = 0;
  size_t i;

  if (profile->has_sample_type || profile->sample_type.type_strindex ||
      profile->sample_type.unit_strindex) {
    size += otlp__message_field_size(
        1,
        otlp_profiles_dev_value_type_encoded_size_inner(&profile->sample_type));
  }

  for (i = 0; i < profile->samples.len; ++i) {
    size += otlp__message_field_size(
        2,
        otlp_profiles_dev_sample_encoded_size_inner(&profile->samples.ptr[i]));
  }

  if (profile->time_unix_nano) {
    size += otlp__fixed64_field_size(3);
  }

  if (profile->duration_nano) {
    size += otlp__varint_field_size(4, profile->duration_nano);
  }

  if (profile->period_type.type_strindex ||
      profile->period_type.unit_strindex) {
    size += otlp__message_field_size(
        5,
        otlp_profiles_dev_value_type_encoded_size_inner(&profile->period_type));
  }

  if (profile->period) {
    size += otlp__varint_field_size(6, (uint64_t)profile->period);
  }

  if (profile->profile_id.len) {
    size += otlp__bytes_field_size(7, profile->profile_id.len);
  }

  if (profile->dropped_attributes_count) {
    size += otlp__varint_field_size(8, profile->dropped_attributes_count);
  }

  if (profile->original_payload_format.len) {
    size += otlp__bytes_field_size(9, profile->original_payload_format.len);
  }

  if (profile->original_payload.len) {
    size += otlp__bytes_field_size(10, profile->original_payload.len);
  }

  if (profile->attribute_indices.len) {
    size += otlp__bytes_field_size(
        11, otlp_profiles_dev_packed_int32_size(&profile->attribute_indices));
  }

  return size;
}

static inline void otlp_profiles_dev_profile_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_profiles_dev_profile *profile) {
  size_t i;
  size_t inner = otlp_profiles_dev_profile_encoded_size_inner(profile);

  otlp__write_message_start(writer, field_number, inner);
  if (profile->has_sample_type || profile->sample_type.type_strindex ||
      profile->sample_type.unit_strindex) {
    otlp_profiles_dev_value_type_encode_field(writer, 1,
                                              &profile->sample_type);
  }

  for (i = 0; i < profile->samples.len; ++i) {
    otlp_profiles_dev_sample_encode_field(writer, 2, &profile->samples.ptr[i]);
  }

  if (profile->time_unix_nano) {
    otlp__write_tag(writer, 3, 1);
    otlp__write_fixed64(writer, profile->time_unix_nano);
  }

  if (profile->duration_nano) {
    otlp__write_tag(writer, 4, 0);
    otlp__write_varint(writer, profile->duration_nano);
  }

  if (profile->period_type.type_strindex ||
      profile->period_type.unit_strindex) {
    otlp_profiles_dev_value_type_encode_field(writer, 5, &profile->period_type);
  }

  if (profile->period) {
    otlp__write_tag(writer, 6, 0);
    otlp__write_varint(writer, (uint64_t)profile->period);
  }

  otlp__write_bytes_field(writer, 7, profile->profile_id.ptr,
                          profile->profile_id.len);

  if (profile->dropped_attributes_count) {
    otlp__write_tag(writer, 8, 0);
    otlp__write_varint(writer, profile->dropped_attributes_count);
  }

  otlp__write_string_field(writer, 9, &profile->original_payload_format);
  otlp__write_bytes_field(writer, 10, profile->original_payload.ptr,
                          profile->original_payload.len);

  otlp_profiles_dev_write_packed_int32_field(writer, 11,
                                             &profile->attribute_indices);
}

static inline void otlp_profiles_dev_scope_profiles_init(
    otlp_profiles_dev_scope_profiles *scope_profiles) {
  otlp__zero(scope_profiles, sizeof(*scope_profiles));
}

static inline void otlp_profiles_dev_scope_profiles_destroy(
    otlp_profiles_dev_scope_profiles *scope_profiles) {
  size_t i;
  if (scope_profiles->has_scope) {
    otlp_instrumentation_scope_destroy(&scope_profiles->scope);
  }

  for (i = 0; i < scope_profiles->profiles.len; ++i) {
    otlp_profiles_dev_profile_destroy(&scope_profiles->profiles.ptr[i]);
  }

  free(scope_profiles->profiles.ptr);
  free(scope_profiles->schema_url.ptr);
  otlp_profiles_dev_scope_profiles_init(scope_profiles);
}

static inline otlp_status otlp_profiles_dev_scope_profiles_append_profile(
    otlp_profiles_dev_scope_profiles *scope_profiles,
    const otlp_profiles_dev_profile *profile) {
  otlp_profiles_dev_profile copy;
  otlp_status status = otlp_profiles_dev_profile_copy(&copy, profile);

  if (status != OTLP_STATUS_OK) {
    return status;
  }

  return otlp__append_primitive(
      (void **)&scope_profiles->profiles.ptr, &scope_profiles->profiles.len,
      &scope_profiles->profiles.cap, sizeof(copy), &copy);
}

static inline size_t otlp_profiles_dev_scope_profiles_encoded_size_inner(
    const otlp_profiles_dev_scope_profiles *scope_profiles) {
  size_t size = 0;
  size_t i;
  if (scope_profiles->has_scope) {
    size += otlp__message_field_size(
        1, otlp_instrumentation_scope_encoded_size_inner(&scope_profiles->scope));
  }

  for (i = 0; i < scope_profiles->profiles.len; ++i) {
    size += otlp__message_field_size(
        2, otlp_profiles_dev_profile_encoded_size_inner(
               &scope_profiles->profiles.ptr[i]));
  }

  if (scope_profiles->schema_url.len) {
    size += otlp__bytes_field_size(3, scope_profiles->schema_url.len);
  }

  return size;
}

static inline void otlp_profiles_dev_scope_profiles_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_profiles_dev_scope_profiles *scope_profiles) {
  size_t i;
  size_t inner =
      otlp_profiles_dev_scope_profiles_encoded_size_inner(scope_profiles);

  otlp__write_message_start(writer, field_number, inner);
  if (scope_profiles->has_scope) {
    otlp_instrumentation_scope_encode_field(writer, 1, &scope_profiles->scope);
  }

  for (i = 0; i < scope_profiles->profiles.len; ++i) {
    otlp_profiles_dev_profile_encode_field(writer, 2,
                                           &scope_profiles->profiles.ptr[i]);
  }

  otlp__write_string_field(writer, 3, &scope_profiles->schema_url);
}

static inline otlp_status otlp_profiles_dev_scope_profiles_copy(
    otlp_profiles_dev_scope_profiles *dst,
    const otlp_profiles_dev_scope_profiles *src) {
  size_t i;
  otlp_profiles_dev_scope_profiles_init(dst);
  if (src->has_scope) {
    dst->has_scope = 1;
    if (otlp_instrumentation_scope_copy(&dst->scope, &src->scope) !=
        OTLP_STATUS_OK) {
      return OTLP_STATUS_OUT_OF_MEMORY;
    }
  }
  if (otlp__set_string(&dst->schema_url, src->schema_url.ptr,
                       src->schema_url.len) != OTLP_STATUS_OK) {
    return OTLP_STATUS_OUT_OF_MEMORY;
  }
  for (i = 0; i < src->profiles.len; ++i) {
    otlp_status status =
        otlp_profiles_dev_scope_profiles_append_profile(dst,
                                                        &src->profiles.ptr[i]);
    if (status != OTLP_STATUS_OK) {
      return status;
    }
  }
  return OTLP_STATUS_OK;
}

static inline void otlp_profiles_dev_resource_profiles_init(
    otlp_profiles_dev_resource_profiles *resource_profiles) {
  otlp__zero(resource_profiles, sizeof(*resource_profiles));
}

static inline void otlp_profiles_dev_resource_profiles_destroy(
    otlp_profiles_dev_resource_profiles *resource_profiles) {
  size_t i;
  if (resource_profiles->has_resource) {
    otlp_resource_destroy(&resource_profiles->resource);
  }

  for (i = 0; i < resource_profiles->scope_profiles.len; ++i) {
    otlp_profiles_dev_scope_profiles_destroy(
        &resource_profiles->scope_profiles.ptr[i]);
  }

  free(resource_profiles->scope_profiles.ptr);
  free(resource_profiles->schema_url.ptr);
  otlp_profiles_dev_resource_profiles_init(resource_profiles);
}

static inline otlp_status
otlp_profiles_dev_resource_profiles_append_scope_profiles(
    otlp_profiles_dev_resource_profiles *resource_profiles,
    const otlp_profiles_dev_scope_profiles *scope_profiles) {
  otlp_profiles_dev_scope_profiles copy;
  otlp_status status;

  otlp_profiles_dev_scope_profiles_init(&copy);
  status = otlp_profiles_dev_scope_profiles_copy(&copy, scope_profiles);
  if (status != OTLP_STATUS_OK) {
    return status;
  }

  return otlp__append_primitive((void **)&resource_profiles->scope_profiles.ptr,
                                &resource_profiles->scope_profiles.len,
                                &resource_profiles->scope_profiles.cap,
                                sizeof(copy), &copy);
}

static inline otlp_status otlp_profiles_dev_resource_profiles_copy(
    otlp_profiles_dev_resource_profiles *dst,
    const otlp_profiles_dev_resource_profiles *src) {
  size_t i;
  otlp_profiles_dev_resource_profiles_init(dst);
  if (src->has_resource) {
    dst->has_resource = 1;
    if (otlp_resource_copy(&dst->resource, &src->resource) !=
        OTLP_STATUS_OK) {
      return OTLP_STATUS_OUT_OF_MEMORY;
    }
  }
  if (otlp__set_string(&dst->schema_url, src->schema_url.ptr,
                       src->schema_url.len) != OTLP_STATUS_OK) {
    return OTLP_STATUS_OUT_OF_MEMORY;
  }
  for (i = 0; i < src->scope_profiles.len; ++i) {
    otlp_status status = otlp_profiles_dev_resource_profiles_append_scope_profiles(
        dst, &src->scope_profiles.ptr[i]);
    if (status != OTLP_STATUS_OK) {
      return status;
    }
  }
  return OTLP_STATUS_OK;
}

static inline size_t otlp_profiles_dev_resource_profiles_encoded_size_inner(
    const otlp_profiles_dev_resource_profiles *resource_profiles) {
  size_t size = 0;
  size_t i;
  if (resource_profiles->has_resource) {
    size += otlp__message_field_size(
        1, otlp_resource_encoded_size_inner(&resource_profiles->resource));
  }

  for (i = 0; i < resource_profiles->scope_profiles.len; ++i) {
    size += otlp__message_field_size(
        2, otlp_profiles_dev_scope_profiles_encoded_size_inner(
               &resource_profiles->scope_profiles.ptr[i]));
  }

  if (resource_profiles->schema_url.len) {
    size += otlp__bytes_field_size(3, resource_profiles->schema_url.len);
  }

  return size;
}

static inline void otlp_profiles_dev_resource_profiles_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_profiles_dev_resource_profiles *resource_profiles) {
  size_t i;
  size_t inner =
      otlp_profiles_dev_resource_profiles_encoded_size_inner(resource_profiles);

  otlp__write_message_start(writer, field_number, inner);
  if (resource_profiles->has_resource) {
    otlp_resource_encode_field(writer, 1, &resource_profiles->resource);
  }

  for (i = 0; i < resource_profiles->scope_profiles.len; ++i) {
    otlp_profiles_dev_scope_profiles_encode_field(
        writer, 2, &resource_profiles->scope_profiles.ptr[i]);
  }

  otlp__write_string_field(writer, 3, &resource_profiles->schema_url);
}

static inline void otlp_profiles_dev_profiles_dictionary_init(
    otlp_profiles_dev_profiles_dictionary *dictionary) {
  otlp__zero(dictionary, sizeof(*dictionary));
}

static inline void otlp_profiles_dev_profiles_dictionary_destroy(
    otlp_profiles_dev_profiles_dictionary *dictionary) {
  size_t i;

  for (i = 0; i < dictionary->mapping_table.len; ++i) {
    otlp_profiles_dev_mapping_destroy(&dictionary->mapping_table.ptr[i]);
  }
  for (i = 0; i < dictionary->location_table.len; ++i) {
    otlp_profiles_dev_location_destroy(&dictionary->location_table.ptr[i]);
  }
  for (i = 0; i < dictionary->function_table.len; ++i) {
    otlp_profiles_dev_function_destroy(&dictionary->function_table.ptr[i]);
  }
  for (i = 0; i < dictionary->link_table.len; ++i) {
    otlp_profiles_dev_link_destroy(&dictionary->link_table.ptr[i]);
  }
  for (i = 0; i < dictionary->string_table.len; ++i) {
    free(dictionary->string_table.ptr[i].ptr);
  }
  for (i = 0; i < dictionary->attribute_table.len; ++i) {
    otlp_profiles_dev_key_value_and_unit_destroy(
        &dictionary->attribute_table.ptr[i]);
  }
  for (i = 0; i < dictionary->stack_table.len; ++i) {
    otlp_profiles_dev_stack_destroy(&dictionary->stack_table.ptr[i]);
  }

  free(dictionary->mapping_table.ptr);
  free(dictionary->location_table.ptr);
  free(dictionary->function_table.ptr);
  free(dictionary->link_table.ptr);
  free(dictionary->string_table.ptr);
  free(dictionary->attribute_table.ptr);
  free(dictionary->stack_table.ptr);
  otlp_profiles_dev_profiles_dictionary_init(dictionary);
}

static inline otlp_status
otlp_profiles_dev_profiles_dictionary_init_default_zero_entries(
    otlp_profiles_dev_profiles_dictionary *dictionary) {
  otlp_profiles_dev_mapping mapping;
  otlp_profiles_dev_location location;
  otlp_profiles_dev_function function_value;
  otlp_profiles_dev_link link;
  otlp_string string_value;
  otlp_profiles_dev_key_value_and_unit kv;
  otlp_profiles_dev_stack stack;

  otlp_profiles_dev_profiles_dictionary_destroy(dictionary);
  otlp_profiles_dev_profiles_dictionary_init(dictionary);
  otlp_profiles_dev_mapping_init(&mapping);
  otlp_profiles_dev_location_init(&location);
  otlp_profiles_dev_function_init(&function_value);
  otlp_profiles_dev_link_init(&link);
  otlp__zero(&string_value, sizeof(string_value));
  otlp_profiles_dev_key_value_and_unit_init(&kv);
  otlp_profiles_dev_stack_init(&stack);

  if (otlp__append_primitive((void **)&dictionary->mapping_table.ptr,
                             &dictionary->mapping_table.len,
                             &dictionary->mapping_table.cap, sizeof(mapping),
                             &mapping) != OTLP_STATUS_OK) {
    return OTLP_STATUS_OUT_OF_MEMORY;
  }

  if (otlp__append_primitive((void **)&dictionary->location_table.ptr,
                             &dictionary->location_table.len,
                             &dictionary->location_table.cap, sizeof(location),
                             &location) != OTLP_STATUS_OK) {
    return OTLP_STATUS_OUT_OF_MEMORY;
  }

  if (otlp__append_primitive(
          (void **)&dictionary->function_table.ptr,
          &dictionary->function_table.len, &dictionary->function_table.cap,
          sizeof(function_value), &function_value) != OTLP_STATUS_OK) {
    return OTLP_STATUS_OUT_OF_MEMORY;
  }

  if (otlp__append_primitive(
          (void **)&dictionary->link_table.ptr, &dictionary->link_table.len,
          &dictionary->link_table.cap, sizeof(link), &link) != OTLP_STATUS_OK) {
    return OTLP_STATUS_OUT_OF_MEMORY;
  }

  if (otlp__append_primitive(
          (void **)&dictionary->string_table.ptr, &dictionary->string_table.len,
          &dictionary->string_table.cap, sizeof(string_value),
          &string_value) != OTLP_STATUS_OK) {
    return OTLP_STATUS_OUT_OF_MEMORY;
  }

  if (otlp__append_primitive((void **)&dictionary->attribute_table.ptr,
                             &dictionary->attribute_table.len,
                             &dictionary->attribute_table.cap, sizeof(kv),
                             &kv) != OTLP_STATUS_OK) {
    return OTLP_STATUS_OUT_OF_MEMORY;
  }

  if (otlp__append_primitive((void **)&dictionary->stack_table.ptr,
                             &dictionary->stack_table.len,
                             &dictionary->stack_table.cap, sizeof(stack),
                             &stack) != OTLP_STATUS_OK) {
    return OTLP_STATUS_OUT_OF_MEMORY;
  }

  return OTLP_STATUS_OK;
}

static inline otlp_status
otlp_profiles_dev_profiles_dictionary_validate_zero_entries(
    const otlp_profiles_dev_profiles_dictionary *dictionary) {
  const otlp_profiles_dev_mapping *mapping;
  const otlp_profiles_dev_location *location;
  const otlp_profiles_dev_function *function_value;
  const otlp_profiles_dev_link *link;
  const otlp_profiles_dev_key_value_and_unit *attribute;
  const otlp_profiles_dev_stack *stack;

  if (dictionary->mapping_table.len == 0 || dictionary->location_table.len == 0 ||
      dictionary->function_table.len == 0 || dictionary->link_table.len == 0 ||
      dictionary->string_table.len == 0 || dictionary->attribute_table.len == 0 ||
      dictionary->stack_table.len == 0) {
    return OTLP_STATUS_INVALID_ARGUMENT;
  }

  mapping = &dictionary->mapping_table.ptr[0];
  location = &dictionary->location_table.ptr[0];
  function_value = &dictionary->function_table.ptr[0];
  link = &dictionary->link_table.ptr[0];
  attribute = &dictionary->attribute_table.ptr[0];
  stack = &dictionary->stack_table.ptr[0];

  if (mapping->memory_start != 0 || mapping->memory_limit != 0 ||
      mapping->file_offset != 0 || mapping->filename_strindex != 0 ||
      mapping->attribute_indices.len != 0) {
    return OTLP_STATUS_INVALID_ARGUMENT;
  }

  if (location->mapping_index != 0 || location->address != 0 ||
      location->lines.len != 0 || location->attribute_indices.len != 0) {
    return OTLP_STATUS_INVALID_ARGUMENT;
  }

  if (function_value->name_strindex != 0 ||
      function_value->system_name_strindex != 0 ||
      function_value->filename_strindex != 0 ||
      function_value->start_line != 0) {
    return OTLP_STATUS_INVALID_ARGUMENT;
  }

  if (link->trace_id.len != 0 || link->span_id.len != 0 ||
      dictionary->string_table.ptr[0].len != 0 ||
      attribute->key_strindex != 0 ||
      attribute->value.kind != OTLP_ANY_VALUE_EMPTY ||
      attribute->unit_strindex != 0 || stack->location_indices.len != 0) {
    return OTLP_STATUS_INVALID_ARGUMENT;
  }

  return OTLP_STATUS_OK;
}

static inline size_t otlp_profiles_dev_profiles_dictionary_encoded_size_inner(
    const otlp_profiles_dev_profiles_dictionary *dictionary) {
  size_t size = 0;
  size_t i;

  for (i = 0; i < dictionary->mapping_table.len; ++i) {
    size += otlp__message_field_size(
        1, otlp_profiles_dev_mapping_encoded_size_inner(
               &dictionary->mapping_table.ptr[i]));
  }
  for (i = 0; i < dictionary->location_table.len; ++i) {
    size += otlp__message_field_size(
        2, otlp_profiles_dev_location_encoded_size_inner(
               &dictionary->location_table.ptr[i]));
  }
  for (i = 0; i < dictionary->function_table.len; ++i) {
    size += otlp__message_field_size(
        3, otlp_profiles_dev_function_encoded_size_inner(
               &dictionary->function_table.ptr[i]));
  }
  for (i = 0; i < dictionary->link_table.len; ++i) {
    size +=
        otlp__message_field_size(4, otlp_profiles_dev_link_encoded_size_inner(
                                        &dictionary->link_table.ptr[i]));
  }
  for (i = 0; i < dictionary->string_table.len; ++i) {
    size += otlp__bytes_field_size(5, dictionary->string_table.ptr[i].len);
  }
  for (i = 0; i < dictionary->attribute_table.len; ++i) {
    size += otlp__message_field_size(
        6, otlp_profiles_dev_key_value_and_unit_encoded_size_inner(
               &dictionary->attribute_table.ptr[i]));
  }
  for (i = 0; i < dictionary->stack_table.len; ++i) {
    size +=
        otlp__message_field_size(7, otlp_profiles_dev_stack_encoded_size_inner(
                                        &dictionary->stack_table.ptr[i]));
  }

  return size;
}

static inline void otlp_profiles_dev_profiles_dictionary_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_profiles_dev_profiles_dictionary *dictionary) {
  size_t i;
  size_t inner =
      otlp_profiles_dev_profiles_dictionary_encoded_size_inner(dictionary);

  otlp__write_message_start(writer, field_number, inner);

  for (i = 0; i < dictionary->mapping_table.len; ++i) {
    otlp_profiles_dev_mapping_encode_field(writer, 1,
                                           &dictionary->mapping_table.ptr[i]);
  }
  for (i = 0; i < dictionary->location_table.len; ++i) {
    otlp_profiles_dev_location_encode_field(writer, 2,
                                            &dictionary->location_table.ptr[i]);
  }
  for (i = 0; i < dictionary->function_table.len; ++i) {
    otlp_profiles_dev_function_encode_field(writer, 3,
                                            &dictionary->function_table.ptr[i]);
  }
  for (i = 0; i < dictionary->link_table.len; ++i) {
    otlp_profiles_dev_link_encode_field(writer, 4,
                                        &dictionary->link_table.ptr[i]);
  }
  for (i = 0; i < dictionary->string_table.len; ++i) {
    otlp__write_bytes_field_allow_empty(
        writer, 5, (const uint8_t *)dictionary->string_table.ptr[i].ptr,
        dictionary->string_table.ptr[i].len);
  }
  for (i = 0; i < dictionary->attribute_table.len; ++i) {
    otlp_profiles_dev_key_value_and_unit_encode_field(
        writer, 6, &dictionary->attribute_table.ptr[i]);
  }
  for (i = 0; i < dictionary->stack_table.len; ++i) {
    otlp_profiles_dev_stack_encode_field(writer, 7,
                                         &dictionary->stack_table.ptr[i]);
  }
}

static inline void otlp_profiles_dev_export_profiles_service_request_init(
    otlp_profiles_dev_export_profiles_service_request *request) {
  otlp__zero(request, sizeof(*request));
}

static inline void otlp_profiles_dev_export_profiles_service_request_destroy(
    otlp_profiles_dev_export_profiles_service_request *request) {
  size_t i;

  for (i = 0; i < request->resource_profiles.len; ++i) {
    otlp_profiles_dev_resource_profiles_destroy(
        &request->resource_profiles.ptr[i]);
  }

  free(request->resource_profiles.ptr);
  otlp_profiles_dev_profiles_dictionary_destroy(&request->dictionary);
  otlp_profiles_dev_export_profiles_service_request_init(request);
}

static inline otlp_status
otlp_profiles_dev_export_profiles_service_request_append_resource_profiles(
    otlp_profiles_dev_export_profiles_service_request *request,
    const otlp_profiles_dev_resource_profiles *resource_profiles) {
  otlp_profiles_dev_resource_profiles copy;
  otlp_status status;

  otlp_profiles_dev_resource_profiles_init(&copy);
  status = otlp_profiles_dev_resource_profiles_copy(&copy, resource_profiles);
  if (status != OTLP_STATUS_OK) {
    return status;
  }

  return otlp__append_primitive(
      (void **)&request->resource_profiles.ptr, &request->resource_profiles.len,
      &request->resource_profiles.cap, sizeof(copy), &copy);
}

static inline size_t
otlp_profiles_dev_export_profiles_service_request_encoded_size(
    const otlp_profiles_dev_export_profiles_service_request *request) {
  size_t size = 0;
  size_t i;

  for (i = 0; i < request->resource_profiles.len; ++i) {
    size += otlp__message_field_size(
        1, otlp_profiles_dev_resource_profiles_encoded_size_inner(
               &request->resource_profiles.ptr[i]));
  }

  size += otlp__message_field_size(
      2, otlp_profiles_dev_profiles_dictionary_encoded_size_inner(
             &request->dictionary));

  return size;
}

static inline otlp_status
otlp_profiles_dev_export_profiles_service_request_encode(
    const otlp_profiles_dev_export_profiles_service_request *request,
    uint8_t *out, size_t len) {
  otlp__writer writer;
  size_t i;

  if (otlp_profiles_dev_profiles_dictionary_validate_zero_entries(
          &request->dictionary) != OTLP_STATUS_OK) {
    return OTLP_STATUS_INVALID_ARGUMENT;
  }

  otlp__writer_init(&writer, out, len);

  for (i = 0; i < request->resource_profiles.len; ++i) {
    otlp_profiles_dev_resource_profiles_encode_field(
        &writer, 1, &request->resource_profiles.ptr[i]);
  }
  otlp_profiles_dev_profiles_dictionary_encode_field(&writer, 2,
                                                     &request->dictionary);

  if (writer.status != OTLP_STATUS_OK) {
    return writer.status;
  }

  return writer.off == len ? OTLP_STATUS_OK : OTLP_STATUS_INVALID_ARGUMENT;
}

static inline otlp_status
otlp_profiles_dev_sample_decode_inner(otlp_profiles_dev_sample *sample,
                                      const uint8_t *data, size_t len) {
  otlp__reader reader;

  otlp_profiles_dev_sample_destroy(sample);
  otlp_profiles_dev_sample_init(sample);
  otlp__reader_init(&reader, data, len);

  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    const uint8_t *ptr;
    size_t size;

    if (field_number == 1 && wire_type == 0) {
      sample->stack_index = (int32_t)otlp__read_varint(&reader);
    } else if (field_number == 2 && wire_type == 0) {
      int32_t value = (int32_t)otlp__read_varint(&reader);
      if (otlp__append_primitive((void **)&sample->attribute_indices.ptr,
                                 &sample->attribute_indices.len,
                                 &sample->attribute_indices.cap, sizeof(value),
                                 &value) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 2 && wire_type == 2) {
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_append_packed_int32(&sample->attribute_indices,
                                                ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
    } else if (field_number == 3 && wire_type == 0) {
      sample->link_index = (int32_t)otlp__read_varint(&reader);
    } else if (field_number == 4 && wire_type == 0) {
      int64_t value = otlp__int64_from_varint(otlp__read_varint(&reader));
      if (otlp__append_primitive((void **)&sample->values.ptr,
                                 &sample->values.len, &sample->values.cap,
                                 sizeof(value), &value) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 4 && wire_type == 2) {
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_append_packed_int64(&sample->values, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
    } else if (field_number == 5 && wire_type == 1) {
      uint64_t value = otlp__read_fixed64(&reader);
      if (otlp__append_primitive((void **)&sample->timestamps_unix_nano.ptr,
                                 &sample->timestamps_unix_nano.len,
                                 &sample->timestamps_unix_nano.cap,
                                 sizeof(value), &value) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 5 && wire_type == 2) {
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_append_packed_fixed64(&sample->timestamps_unix_nano,
                                                  ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }

  return reader.status;
}

static inline otlp_status
otlp_profiles_dev_profile_decode_inner(otlp_profiles_dev_profile *profile,
                                       const uint8_t *data, size_t len) {
  otlp__reader reader;

  otlp_profiles_dev_profile_destroy(profile);
  otlp_profiles_dev_profile_init(profile);
  otlp__reader_init(&reader, data, len);

  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    const uint8_t *ptr;
    size_t size;

    if (field_number == 1 && wire_type == 2) {
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      profile->has_sample_type = 1;
      if (otlp_profiles_dev_value_type_decode_inner(&profile->sample_type, ptr,
                                                    size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
    } else if (field_number == 2 && wire_type == 2) {
      otlp_profiles_dev_sample sample;
      otlp_profiles_dev_sample_init(&sample);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_sample_decode_inner(&sample, ptr, size) !=
          OTLP_STATUS_OK) {
        return OTLP_STATUS_DECODE_ERROR;
      }
      if (otlp__append_primitive((void **)&profile->samples.ptr,
                                 &profile->samples.len, &profile->samples.cap,
                                 sizeof(sample), &sample) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 3 && wire_type == 1) {
      profile->time_unix_nano = otlp__read_fixed64(&reader);
    } else if (field_number == 4 && wire_type == 0) {
      profile->duration_nano = otlp__read_varint(&reader);
    } else if (field_number == 5 && wire_type == 2) {
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_value_type_decode_inner(&profile->period_type, ptr,
                                                    size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
    } else if (field_number == 6 && wire_type == 0) {
      profile->period = otlp__int64_from_varint(otlp__read_varint(&reader));
    } else if (field_number == 7 && wire_type == 2) {
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_bytes(&profile->profile_id, ptr, size) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 8 && wire_type == 0) {
      profile->dropped_attributes_count = (uint32_t)otlp__read_varint(&reader);
    } else if (field_number == 9 && wire_type == 2) {
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&profile->original_payload_format, (const char *)ptr,
                           size) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 10 && wire_type == 2) {
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_bytes(&profile->original_payload, ptr, size) !=
          OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 11 && wire_type == 0) {
      int32_t value = (int32_t)otlp__read_varint(&reader);
      if (otlp__append_primitive((void **)&profile->attribute_indices.ptr,
                                 &profile->attribute_indices.len,
                                 &profile->attribute_indices.cap, sizeof(value),
                                 &value) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 11 && wire_type == 2) {
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_append_packed_int32(&profile->attribute_indices,
                                                ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }

  return reader.status;
}

static inline otlp_status otlp_profiles_dev_scope_profiles_decode_inner(
    otlp_profiles_dev_scope_profiles *scope_profiles, const uint8_t *data,
    size_t len) {
  otlp__reader reader;

  otlp_profiles_dev_scope_profiles_destroy(scope_profiles);
  otlp_profiles_dev_scope_profiles_init(scope_profiles);
  otlp__reader_init(&reader, data, len);

  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    const uint8_t *ptr;
    size_t size;

    if (field_number == 1 && wire_type == 2) {
      scope_profiles->has_scope = 1;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_instrumentation_scope_decode_inner(&scope_profiles->scope, ptr,
                                                  size) != OTLP_STATUS_OK) {
        return OTLP_STATUS_DECODE_ERROR;
      }
    } else if (field_number == 2 && wire_type == 2) {
      otlp_profiles_dev_profile profile;
      otlp_profiles_dev_profile_init(&profile);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_profile_decode_inner(&profile, ptr, size) !=
          OTLP_STATUS_OK) {
        return OTLP_STATUS_DECODE_ERROR;
      }
      if (otlp__append_primitive((void **)&scope_profiles->profiles.ptr,
                                 &scope_profiles->profiles.len,
                                 &scope_profiles->profiles.cap, sizeof(profile),
                                 &profile) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 3 && wire_type == 2) {
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&scope_profiles->schema_url, (const char *)ptr,
                           size) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }

  return reader.status;
}

static inline otlp_status otlp_profiles_dev_resource_profiles_decode_inner(
    otlp_profiles_dev_resource_profiles *resource_profiles, const uint8_t *data,
    size_t len) {
  otlp__reader reader;

  otlp_profiles_dev_resource_profiles_destroy(resource_profiles);
  otlp_profiles_dev_resource_profiles_init(resource_profiles);
  otlp__reader_init(&reader, data, len);

  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    const uint8_t *ptr;
    size_t size;

    if (field_number == 1 && wire_type == 2) {
      resource_profiles->has_resource = 1;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_resource_decode_inner(&resource_profiles->resource, ptr, size) !=
          OTLP_STATUS_OK) {
        return OTLP_STATUS_DECODE_ERROR;
      }
    } else if (field_number == 2 && wire_type == 2) {
      otlp_profiles_dev_scope_profiles scope_profiles;
      otlp_profiles_dev_scope_profiles_init(&scope_profiles);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_scope_profiles_decode_inner(
              &scope_profiles, ptr, size) != OTLP_STATUS_OK) {
        return OTLP_STATUS_DECODE_ERROR;
      }
      if (otlp__append_primitive(
              (void **)&resource_profiles->scope_profiles.ptr,
              &resource_profiles->scope_profiles.len,
              &resource_profiles->scope_profiles.cap, sizeof(scope_profiles),
              &scope_profiles) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 3 && wire_type == 2) {
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&resource_profiles->schema_url, (const char *)ptr,
                           size) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }

  return reader.status;
}

static inline otlp_status otlp_profiles_dev_profiles_dictionary_merge_inner(
    otlp_profiles_dev_profiles_dictionary *dictionary, const uint8_t *data,
    size_t len) {
  otlp__reader reader;

  otlp__reader_init(&reader, data, len);

  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    const uint8_t *ptr;
    size_t size;

    if (field_number == 1 && wire_type == 2) {
      otlp_profiles_dev_mapping value;
      otlp_profiles_dev_mapping_init(&value);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_mapping_decode_inner(&value, ptr, size) !=
          OTLP_STATUS_OK) {
        return OTLP_STATUS_DECODE_ERROR;
      }
      if (otlp__append_primitive((void **)&dictionary->mapping_table.ptr,
                                 &dictionary->mapping_table.len,
                                 &dictionary->mapping_table.cap, sizeof(value),
                                 &value) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 2 && wire_type == 2) {
      otlp_profiles_dev_location value;
      otlp_profiles_dev_location_init(&value);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_location_decode_inner(&value, ptr, size) !=
          OTLP_STATUS_OK) {
        return OTLP_STATUS_DECODE_ERROR;
      }
      if (otlp__append_primitive((void **)&dictionary->location_table.ptr,
                                 &dictionary->location_table.len,
                                 &dictionary->location_table.cap, sizeof(value),
                                 &value) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 3 && wire_type == 2) {
      otlp_profiles_dev_function value;
      otlp_profiles_dev_function_init(&value);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_function_decode_inner(&value, ptr, size) !=
          OTLP_STATUS_OK) {
        return OTLP_STATUS_DECODE_ERROR;
      }
      if (otlp__append_primitive((void **)&dictionary->function_table.ptr,
                                 &dictionary->function_table.len,
                                 &dictionary->function_table.cap, sizeof(value),
                                 &value) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 4 && wire_type == 2) {
      otlp_profiles_dev_link value;
      otlp_profiles_dev_link_init(&value);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_link_decode_inner(&value, ptr, size) !=
          OTLP_STATUS_OK) {
        return OTLP_STATUS_DECODE_ERROR;
      }
      if (otlp__append_primitive((void **)&dictionary->link_table.ptr,
                                 &dictionary->link_table.len,
                                 &dictionary->link_table.cap, sizeof(value),
                                 &value) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 5 && wire_type == 2) {
      otlp_string value;
      otlp__zero(&value, sizeof(value));
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&value, (const char *)ptr, size) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
      if (otlp__append_primitive((void **)&dictionary->string_table.ptr,
                                 &dictionary->string_table.len,
                                 &dictionary->string_table.cap, sizeof(value),
                                 &value) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 6 && wire_type == 2) {
      otlp_profiles_dev_key_value_and_unit value;
      otlp_profiles_dev_key_value_and_unit_init(&value);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_key_value_and_unit_decode_inner(
              &value, ptr, size) != OTLP_STATUS_OK) {
        return OTLP_STATUS_DECODE_ERROR;
      }
      if (otlp__append_primitive((void **)&dictionary->attribute_table.ptr,
                                 &dictionary->attribute_table.len,
                                 &dictionary->attribute_table.cap,
                                 sizeof(value), &value) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 7 && wire_type == 2) {
      otlp_profiles_dev_stack value;
      otlp_profiles_dev_stack_init(&value);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_stack_decode_inner(&value, ptr, size) !=
          OTLP_STATUS_OK) {
        return OTLP_STATUS_DECODE_ERROR;
      }
      if (otlp__append_primitive((void **)&dictionary->stack_table.ptr,
                                 &dictionary->stack_table.len,
                                 &dictionary->stack_table.cap, sizeof(value),
                                 &value) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }

  return reader.status;
}

static inline otlp_status otlp_profiles_dev_profiles_dictionary_decode_inner(
    otlp_profiles_dev_profiles_dictionary *dictionary, const uint8_t *data,
    size_t len) {
  otlp_status status;

  otlp_profiles_dev_profiles_dictionary_destroy(dictionary);
  otlp_profiles_dev_profiles_dictionary_init(dictionary);

  status = otlp_profiles_dev_profiles_dictionary_merge_inner(dictionary, data,
                                                             len);
  if (status != OTLP_STATUS_OK) {
    return status;
  }

  if (otlp_profiles_dev_profiles_dictionary_validate_zero_entries(dictionary) !=
      OTLP_STATUS_OK) {
    return OTLP_STATUS_DECODE_ERROR;
  }

  return OTLP_STATUS_OK;
}

static inline otlp_status
otlp_profiles_dev_export_profiles_service_request_decode(
    otlp_profiles_dev_export_profiles_service_request *request,
    const uint8_t *data, size_t len) {
  otlp__reader reader;

  otlp_profiles_dev_export_profiles_service_request_destroy(request);
  otlp_profiles_dev_export_profiles_service_request_init(request);
  otlp__reader_init(&reader, data, len);

  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    const uint8_t *ptr;
    size_t size;

    if (field_number == 1 && wire_type == 2) {
      otlp_profiles_dev_resource_profiles value;
      otlp_profiles_dev_resource_profiles_init(&value);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_resource_profiles_decode_inner(&value, ptr, size) !=
          OTLP_STATUS_OK) {
        return OTLP_STATUS_DECODE_ERROR;
      }
      if (otlp__append_primitive((void **)&request->resource_profiles.ptr,
                                 &request->resource_profiles.len,
                                 &request->resource_profiles.cap, sizeof(value),
                                 &value) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (field_number == 2 && wire_type == 2) {
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_profiles_dictionary_merge_inner(
              &request->dictionary, ptr, size) != OTLP_STATUS_OK) {
        return OTLP_STATUS_DECODE_ERROR;
      }
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }

  if (reader.status != OTLP_STATUS_OK) {
    return reader.status;
  }

  if (otlp_profiles_dev_profiles_dictionary_validate_zero_entries(
          &request->dictionary) != OTLP_STATUS_OK) {
    return OTLP_STATUS_DECODE_ERROR;
  }

  return OTLP_STATUS_OK;
}

static inline void otlp_profiles_dev_profiles_data_init(
    otlp_profiles_dev_profiles_data *data) {
  otlp__zero(data, sizeof(*data));
}

static inline void otlp_profiles_dev_profiles_data_destroy(
    otlp_profiles_dev_profiles_data *data) {
  size_t i;
  for (i = 0; i < data->resource_profiles.len; ++i) {
    otlp_profiles_dev_resource_profiles_destroy(&data->resource_profiles.ptr[i]);
  }
  free(data->resource_profiles.ptr);
  otlp_profiles_dev_profiles_dictionary_destroy(&data->dictionary);
  otlp_profiles_dev_profiles_data_init(data);
}

static inline size_t otlp_profiles_dev_profiles_data_encoded_size(
    const otlp_profiles_dev_profiles_data *data) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < data->resource_profiles.len; ++i) {
    size += otlp__message_field_size(
        1, otlp_profiles_dev_resource_profiles_encoded_size_inner(
               &data->resource_profiles.ptr[i]));
  }
  size += otlp__message_field_size(
      2, otlp_profiles_dev_profiles_dictionary_encoded_size_inner(
             &data->dictionary));
  return size;
}

static inline otlp_status otlp_profiles_dev_profiles_data_encode(
    const otlp_profiles_dev_profiles_data *data, uint8_t *out, size_t len) {
  otlp__writer writer;
  size_t i;
  if (otlp_profiles_dev_profiles_dictionary_validate_zero_entries(
          &data->dictionary) != OTLP_STATUS_OK) {
    return OTLP_STATUS_INVALID_ARGUMENT;
  }

  otlp__writer_init(&writer, out, len);
  for (i = 0; i < data->resource_profiles.len; ++i) {
    otlp_profiles_dev_resource_profiles_encode_field(
        &writer, 1, &data->resource_profiles.ptr[i]);
  }
  otlp_profiles_dev_profiles_dictionary_encode_field(&writer, 2,
                                                     &data->dictionary);
  if (writer.status != OTLP_STATUS_OK) {
    return writer.status;
  }
  return writer.off == len ? OTLP_STATUS_OK : OTLP_STATUS_INVALID_ARGUMENT;
}

static inline otlp_status otlp_profiles_dev_profiles_data_decode(
    otlp_profiles_dev_profiles_data *data, const uint8_t *bytes, size_t len) {
  otlp_profiles_dev_export_profiles_service_request request;
  otlp_profiles_dev_export_profiles_service_request_init(&request);
  if (otlp_profiles_dev_export_profiles_service_request_decode(
          &request, bytes, len) != OTLP_STATUS_OK) {
    otlp_profiles_dev_export_profiles_service_request_destroy(&request);
    return OTLP_STATUS_DECODE_ERROR;
  }
  otlp_profiles_dev_profiles_data_destroy(data);
  data->resource_profiles = request.resource_profiles;
  data->dictionary = request.dictionary;
  return OTLP_STATUS_OK;
}

static inline void otlp_profiles_dev_export_profiles_partial_success_init(
    otlp_profiles_dev_export_profiles_partial_success *partial_success) {
  otlp__zero(partial_success, sizeof(*partial_success));
}

static inline void otlp_profiles_dev_export_profiles_partial_success_destroy(
    otlp_profiles_dev_export_profiles_partial_success *partial_success) {
  free(partial_success->error_message.ptr);
  otlp_profiles_dev_export_profiles_partial_success_init(partial_success);
}

static inline size_t
otlp_profiles_dev_export_profiles_partial_success_encoded_size_inner(
    const otlp_profiles_dev_export_profiles_partial_success *partial_success) {
  size_t size = 0;
  if (partial_success->rejected_profiles) {
    size += otlp__varint_field_size(
        1, (uint64_t)partial_success->rejected_profiles);
  }
  if (partial_success->error_message.len) {
    size += otlp__bytes_field_size(2, partial_success->error_message.len);
  }
  return size;
}

static inline void otlp_profiles_dev_export_profiles_partial_success_encode_field(
    otlp__writer *writer, uint32_t field_number,
    const otlp_profiles_dev_export_profiles_partial_success *partial_success) {
  size_t inner =
      otlp_profiles_dev_export_profiles_partial_success_encoded_size_inner(
          partial_success);
  otlp__write_message_start(writer, field_number, inner);
  if (partial_success->rejected_profiles) {
    otlp__write_tag(writer, 1, 0);
    otlp__write_varint(writer, (uint64_t)partial_success->rejected_profiles);
  }
  otlp__write_string_field(writer, 2, &partial_success->error_message);
}

static inline otlp_status
otlp_profiles_dev_export_profiles_partial_success_decode_inner(
    otlp_profiles_dev_export_profiles_partial_success *partial_success,
    const uint8_t *data, size_t len) {
  otlp__reader reader;
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK) {
      return reader.status;
    }
    if (field_number == 1 && wire_type == 0) {
      partial_success->rejected_profiles =
          (int64_t)otlp__read_varint(&reader);
    } else if (field_number == 2 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&partial_success->error_message, (const char *)ptr,
                           size) != OTLP_STATUS_OK) {
        return OTLP_STATUS_OUT_OF_MEMORY;
      }
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }
  return reader.status;
}

static inline void otlp_profiles_dev_export_profiles_service_response_init(
    otlp_profiles_dev_export_profiles_service_response *response) {
  otlp__zero(response, sizeof(*response));
}

static inline void otlp_profiles_dev_export_profiles_service_response_destroy(
    otlp_profiles_dev_export_profiles_service_response *response) {
  if (response->has_partial_success) {
    otlp_profiles_dev_export_profiles_partial_success_destroy(
        &response->partial_success);
  }
  otlp_profiles_dev_export_profiles_service_response_init(response);
}

static inline size_t
otlp_profiles_dev_export_profiles_service_response_encoded_size(
    const otlp_profiles_dev_export_profiles_service_response *response) {
  size_t size = 0;
  if (response->has_partial_success) {
    size += otlp__message_field_size(
        1, otlp_profiles_dev_export_profiles_partial_success_encoded_size_inner(
               &response->partial_success));
  }
  return size;
}

static inline otlp_status
otlp_profiles_dev_export_profiles_service_response_encode(
    const otlp_profiles_dev_export_profiles_service_response *response,
    uint8_t *out, size_t len) {
  otlp__writer writer;
  otlp__writer_init(&writer, out, len);
  if (response->has_partial_success) {
    otlp_profiles_dev_export_profiles_partial_success_encode_field(
        &writer, 1, &response->partial_success);
  }
  if (writer.status != OTLP_STATUS_OK) {
    return writer.status;
  }
  return writer.off == len ? OTLP_STATUS_OK : OTLP_STATUS_INVALID_ARGUMENT;
}

static inline otlp_status
otlp_profiles_dev_export_profiles_service_response_decode(
    otlp_profiles_dev_export_profiles_service_response *response,
    const uint8_t *data, size_t len) {
  otlp__reader reader;
  otlp_profiles_dev_export_profiles_service_response_destroy(response);
  otlp_profiles_dev_export_profiles_service_response_init(response);
  otlp__reader_init(&reader, data, len);
  while (otlp__reader_has_remaining(&reader)) {
    uint64_t tag = otlp__read_varint(&reader);
    uint32_t field_number = (uint32_t)(tag >> 3U);
    uint32_t wire_type = (uint32_t)(tag & 0x07U);
    if (reader.status != OTLP_STATUS_OK) {
      return reader.status;
    }
    if (field_number == 1 && wire_type == 2) {
      const uint8_t *ptr;
      size_t size;
      response->has_partial_success = 1;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_profiles_dev_export_profiles_partial_success_decode_inner(
              &response->partial_success, ptr, size) != OTLP_STATUS_OK) {
        return OTLP_STATUS_DECODE_ERROR;
      }
    } else if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK) {
      return reader.status;
    }
  }
  return reader.status;
}
#endif

#endif /* OTLP_FORMAT_PROFILES_H */
