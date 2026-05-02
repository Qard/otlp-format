#ifndef OTLP_FORMAT_COMMON_H
#define OTLP_FORMAT_COMMON_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if !defined(OTLP_FORMAT_ENABLE_LOGS) && \
    !defined(OTLP_FORMAT_ENABLE_PROFILES) && \
    !defined(OTLP_FORMAT_ENABLE_TRACES) && \
    !defined(OTLP_FORMAT_ENABLE_METRICS)
#define OTLP_FORMAT_ENABLE_LOGS 1
#define OTLP_FORMAT_ENABLE_PROFILES 1
#define OTLP_FORMAT_ENABLE_TRACES 1
#define OTLP_FORMAT_ENABLE_METRICS 1
#endif

typedef enum otlp_status_e {
  OTLP_STATUS_OK = 0,
  OTLP_STATUS_INVALID_ARGUMENT = 1,
  OTLP_STATUS_OUT_OF_MEMORY = 2,
  OTLP_STATUS_BUFFER_TOO_SMALL = 3,
  OTLP_STATUS_DECODE_ERROR = 4
} otlp_status;

typedef struct otlp_string_s {
  char *ptr;
  size_t len;
} otlp_string;

typedef struct otlp_bytes_s {
  uint8_t *ptr;
  size_t len;
} otlp_bytes;

typedef struct otlp_string_view_s {
  const char *ptr;
  size_t len;
} otlp_string_view;

typedef struct otlp_bytes_view_s {
  const uint8_t *ptr;
  size_t len;
} otlp_bytes_view;

typedef enum otlp_any_value_kind_e {
  OTLP_ANY_VALUE_EMPTY = 0,
  OTLP_ANY_VALUE_STRING = 1,
  OTLP_ANY_VALUE_BOOL = 2,
  OTLP_ANY_VALUE_INT = 3,
  OTLP_ANY_VALUE_DOUBLE = 4,
  OTLP_ANY_VALUE_ARRAY = 5,
  OTLP_ANY_VALUE_KVLIST = 6,
  OTLP_ANY_VALUE_BYTES = 7,
  OTLP_ANY_VALUE_STRING_STRINDEX = 8
} otlp_any_value_kind;

typedef struct otlp_any_value otlp_any_value;
typedef struct otlp_key_value otlp_key_value;
typedef struct otlp_array_value otlp_array_value;
typedef struct otlp_key_value_list otlp_key_value_list;
typedef struct otlp_instrumentation_scope otlp_instrumentation_scope;
typedef struct otlp_entity_ref otlp_entity_ref;
typedef struct otlp_resource otlp_resource;

#define OTLP_DECLARE_ARRAY(name, type) \
  typedef struct name##_s { \
    type *ptr; \
    size_t len; \
    size_t cap; \
  } name

OTLP_DECLARE_ARRAY(otlp_int32_array, int32_t);
OTLP_DECLARE_ARRAY(otlp_int64_array, int64_t);
OTLP_DECLARE_ARRAY(otlp_uint64_array, uint64_t);
OTLP_DECLARE_ARRAY(otlp_double_array, double);
OTLP_DECLARE_ARRAY(otlp_string_array, otlp_string);
OTLP_DECLARE_ARRAY(otlp_any_value_array, otlp_any_value);
OTLP_DECLARE_ARRAY(otlp_key_value_array, otlp_key_value);
OTLP_DECLARE_ARRAY(otlp_entity_ref_array, otlp_entity_ref);

struct otlp_any_value {
  otlp_any_value_kind kind;
  union {
    otlp_string string_value;
    int bool_value;
    int64_t int_value;
    double double_value;
    otlp_array_value *array_value;
    otlp_key_value_list *kvlist_value;
    otlp_bytes bytes_value;
    int32_t string_value_strindex;
  } value;
};

struct otlp_array_value {
  otlp_any_value_array values;
};

struct otlp_key_value_list {
  otlp_key_value_array values;
};

struct otlp_key_value {
  otlp_string key;
  otlp_any_value value;
  int has_value;
  int32_t key_strindex;
};

struct otlp_instrumentation_scope {
  otlp_string name;
  otlp_string version;
  otlp_key_value_array attributes;
  uint32_t dropped_attributes_count;
};

struct otlp_entity_ref {
  otlp_string schema_url;
  otlp_string type;
  otlp_string_array id_keys;
  otlp_string_array description_keys;
};

struct otlp_resource {
  otlp_key_value_array attributes;
  uint32_t dropped_attributes_count;
  otlp_entity_ref_array entity_refs;
};

static inline void otlp_any_value_init(otlp_any_value *value);
static inline void otlp_any_value_destroy(otlp_any_value *value);
static inline otlp_status otlp_any_value_init_string(otlp_any_value *value, const char *ptr, size_t len);
static inline void otlp_array_value_init(otlp_array_value *value);
static inline void otlp_array_value_destroy(otlp_array_value *value);
static inline otlp_status otlp_array_value_append_value(otlp_array_value *value, const otlp_any_value *item);
static inline void otlp_key_value_list_init(otlp_key_value_list *list);
static inline void otlp_key_value_list_destroy(otlp_key_value_list *list);
static inline otlp_status otlp_key_value_list_append_value(otlp_key_value_list *list, const otlp_key_value *item);
static inline void otlp_key_value_init(otlp_key_value *kv);
static inline void otlp_key_value_destroy(otlp_key_value *kv);
static inline otlp_status otlp_key_value_copy(otlp_key_value *dst, const otlp_key_value *src);
static inline void otlp_instrumentation_scope_init(otlp_instrumentation_scope *scope);
static inline void otlp_instrumentation_scope_destroy(otlp_instrumentation_scope *scope);
static inline void otlp_entity_ref_init(otlp_entity_ref *entity_ref);
static inline void otlp_entity_ref_destroy(otlp_entity_ref *entity_ref);
static inline void otlp_resource_init(otlp_resource *resource);
static inline void otlp_resource_destroy(otlp_resource *resource);

typedef struct otlp__writer_s {
  uint8_t *ptr;
  size_t len;
  size_t off;
  otlp_status status;
} otlp__writer;

typedef struct otlp__reader_s {
  const uint8_t *ptr;
  size_t len;
  size_t off;
  otlp_status status;
} otlp__reader;

static inline void otlp__zero(void *ptr, size_t len) { memset(ptr, 0, len); }

static inline size_t otlp__tag(uint32_t field_number, uint32_t wire_type) {
  return ((size_t)field_number << 3U) | (size_t)wire_type;
}

static inline size_t otlp__varint_size(uint64_t value) {
  size_t size = 1;
  while (value >= 128U) {
    value >>= 7U;
    size++;
  }
  return size;
}

static inline int64_t otlp__int64_from_varint(uint64_t value) {
  union {
    uint64_t u;
    int64_t i;
  } bits;
  bits.u = value;
  return bits.i;
}

static inline size_t otlp__tag_size(uint32_t field_number, uint32_t wire_type) {
  return otlp__varint_size((uint64_t)otlp__tag(field_number, wire_type));
}

static inline size_t otlp__bytes_field_size(uint32_t field_number, size_t len) {
  return otlp__tag_size(field_number, 2) + otlp__varint_size((uint64_t)len) +
         len;
}

static inline size_t otlp__message_field_size(uint32_t field_number,
                                              size_t len) {
  return otlp__bytes_field_size(field_number, len);
}

static inline size_t otlp__fixed64_field_size(uint32_t field_number) {
  return otlp__tag_size(field_number, 1) + 8U;
}

static inline size_t otlp__fixed32_field_size(uint32_t field_number) {
  return otlp__tag_size(field_number, 5) + 4U;
}

static inline size_t otlp__varint_field_size(uint32_t field_number,
                                             uint64_t value) {
  return otlp__tag_size(field_number, 0) + otlp__varint_size(value);
}

static inline uint64_t otlp__int32_varint(int32_t value) {
  return (uint64_t)(int64_t)value;
}

static inline size_t otlp__int32_field_size(uint32_t field_number,
                                            int32_t value) {
  return otlp__varint_field_size(field_number, otlp__int32_varint(value));
}

static inline uint64_t otlp__zigzag64(int64_t value) {
  return ((uint64_t)value << 1U) ^ (uint64_t)-(value < 0);
}

static inline int64_t otlp__unzigzag64(uint64_t value) {
  return (int64_t)((value >> 1U) ^ (uint64_t)-(int64_t)(value & 1U));
}

static inline uint32_t otlp__zigzag32(int32_t value) {
  return ((uint32_t)value << 1U) ^ (uint32_t)-(value < 0);
}

static inline int32_t otlp__unzigzag32(uint32_t value) {
  return (int32_t)((value >> 1U) ^ (uint32_t)-(int32_t)(value & 1U));
}

static inline otlp_status otlp__grow(void **ptr, size_t *cap, size_t len,
                                     size_t elem_size) {
  size_t new_cap = *cap == 0 ? 4U : *cap * 2U;
  while (new_cap < len) {
    new_cap *= 2U;
  }
  void *next = realloc(*ptr, new_cap * elem_size);
  if (next == NULL) {
    return OTLP_STATUS_OUT_OF_MEMORY;
  }
  *ptr = next;
  *cap = new_cap;
  return OTLP_STATUS_OK;
}

static inline otlp_status otlp__copy_bytes(uint8_t **dst_ptr, size_t *dst_len,
                                           const uint8_t *src, size_t src_len) {
  *dst_ptr = NULL;
  *dst_len = 0;
  if (src_len == 0) {
    return OTLP_STATUS_OK;
  }
  *dst_ptr = (uint8_t *)malloc(src_len);
  if (*dst_ptr == NULL) {
    return OTLP_STATUS_OUT_OF_MEMORY;
  }
  memcpy(*dst_ptr, src, src_len);
  *dst_len = src_len;
  return OTLP_STATUS_OK;
}

static inline otlp_status otlp__set_string(otlp_string *dst, const char *src,
                                           size_t len) {
  free(dst->ptr);
  dst->ptr = NULL;
  dst->len = 0;
  if (len == 0) {
    return OTLP_STATUS_OK;
  }
  dst->ptr = (char *)malloc(len);
  if (dst->ptr == NULL) {
    return OTLP_STATUS_OUT_OF_MEMORY;
  }
  memcpy(dst->ptr, src, len);
  dst->len = len;
  return OTLP_STATUS_OK;
}

static inline otlp_status otlp__set_bytes(otlp_bytes *dst, const uint8_t *src,
                                          size_t len) {
  free(dst->ptr);
  dst->ptr = NULL;
  dst->len = 0;
  return otlp__copy_bytes(&dst->ptr, &dst->len, src, len);
}

static inline void otlp__writer_init(otlp__writer *writer, uint8_t *ptr,
                                     size_t len) {
  writer->ptr = ptr;
  writer->len = len;
  writer->off = 0;
  writer->status = OTLP_STATUS_OK;
}

static inline void otlp__write_byte(otlp__writer *writer, uint8_t value) {
  if (writer->status != OTLP_STATUS_OK)
    return;
  if (writer->off >= writer->len) {
    writer->status = OTLP_STATUS_BUFFER_TOO_SMALL;
    return;
  }
  writer->ptr[writer->off++] = value;
}

static inline void otlp__write_raw(otlp__writer *writer, const void *ptr,
                                   size_t len) {
  if (writer->status != OTLP_STATUS_OK)
    return;
  if (writer->off + len > writer->len) {
    writer->status = OTLP_STATUS_BUFFER_TOO_SMALL;
    return;
  }
  memcpy(writer->ptr + writer->off, ptr, len);
  writer->off += len;
}

static inline void otlp__write_varint(otlp__writer *writer, uint64_t value) {
  while (value >= 128U) {
    otlp__write_byte(writer, (uint8_t)((value & 0x7FU) | 0x80U));
    value >>= 7U;
  }
  otlp__write_byte(writer, (uint8_t)value);
}

static inline void otlp__write_tag(otlp__writer *writer, uint32_t field_number,
                                   uint32_t wire_type) {
  otlp__write_varint(writer, (uint64_t)otlp__tag(field_number, wire_type));
}

static inline void otlp__write_fixed32(otlp__writer *writer, uint32_t value) {
  otlp__write_byte(writer, (uint8_t)(value & 0xFFU));
  otlp__write_byte(writer, (uint8_t)((value >> 8U) & 0xFFU));
  otlp__write_byte(writer, (uint8_t)((value >> 16U) & 0xFFU));
  otlp__write_byte(writer, (uint8_t)((value >> 24U) & 0xFFU));
}

static inline void otlp__write_fixed64(otlp__writer *writer, uint64_t value) {
  size_t i;
  for (i = 0; i < 8U; ++i) {
    otlp__write_byte(writer, (uint8_t)((value >> (i * 8U)) & 0xFFU));
  }
}

static inline void otlp__write_bytes_field(otlp__writer *writer,
                                           uint32_t field_number,
                                           const uint8_t *ptr, size_t len) {
  if (len == 0)
    return;
  otlp__write_tag(writer, field_number, 2);
  otlp__write_varint(writer, (uint64_t)len);
  otlp__write_raw(writer, ptr, len);
}

static inline void otlp__write_bytes_field_allow_empty(otlp__writer *writer,
                                                       uint32_t field_number,
                                                       const uint8_t *ptr,
                                                       size_t len) {
  otlp__write_tag(writer, field_number, 2);
  otlp__write_varint(writer, (uint64_t)len);
  if (len)
    otlp__write_raw(writer, ptr, len);
}

static inline void otlp__write_string_field(otlp__writer *writer,
                                            uint32_t field_number,
                                            const otlp_string *value) {
  otlp__write_bytes_field(writer, field_number, (const uint8_t *)value->ptr,
                          value->len);
}

static inline void otlp__write_message_start(otlp__writer *writer,
                                             uint32_t field_number,
                                             size_t message_size) {
  otlp__write_tag(writer, field_number, 2);
  otlp__write_varint(writer, (uint64_t)message_size);
}

static inline void otlp__reader_init(otlp__reader *reader, const uint8_t *ptr,
                                     size_t len) {
  reader->ptr = ptr;
  reader->len = len;
  reader->off = 0;
  reader->status = OTLP_STATUS_OK;
}

static inline int otlp__reader_has_remaining(const otlp__reader *reader) {
  return reader->off < reader->len;
}

static inline uint8_t otlp__read_byte(otlp__reader *reader) {
  if (reader->off >= reader->len) {
    reader->status = OTLP_STATUS_DECODE_ERROR;
    return 0;
  }
  return reader->ptr[reader->off++];
}

static inline uint64_t otlp__read_varint(otlp__reader *reader) {
  uint64_t value = 0;
  uint32_t shift = 0;
  while (shift < 64U) {
    uint8_t byte = otlp__read_byte(reader);
    if (reader->status != OTLP_STATUS_OK)
      return 0;
    value |= ((uint64_t)(byte & 0x7FU)) << shift;
    if ((byte & 0x80U) == 0)
      return value;
    shift += 7U;
  }
  reader->status = OTLP_STATUS_DECODE_ERROR;
  return 0;
}

static inline uint32_t otlp__read_fixed32(otlp__reader *reader) {
  uint32_t value = 0;
  size_t i;
  for (i = 0; i < 4U; ++i) {
    value |= ((uint32_t)otlp__read_byte(reader)) << (i * 8U);
  }
  return value;
}

static inline uint64_t otlp__read_fixed64(otlp__reader *reader) {
  uint64_t value = 0;
  size_t i;
  for (i = 0; i < 8U; ++i) {
    value |= ((uint64_t)otlp__read_byte(reader)) << (i * 8U);
  }
  return value;
}

static inline otlp_status otlp__read_length_delimited(otlp__reader *reader,
                                                      const uint8_t **ptr,
                                                      size_t *len) {
  uint64_t size = otlp__read_varint(reader);
  if (reader->status != OTLP_STATUS_OK)
    return reader->status;
  if (reader->off + (size_t)size > reader->len) {
    reader->status = OTLP_STATUS_DECODE_ERROR;
    return reader->status;
  }
  *ptr = reader->ptr + reader->off;
  *len = (size_t)size;
  reader->off += (size_t)size;
  return OTLP_STATUS_OK;
}

static inline otlp_status otlp__skip_field(otlp__reader *reader,
                                           uint32_t wire_type) {
  const uint8_t *ptr;
  size_t len;
  switch (wire_type) {
  case 0:
    (void)otlp__read_varint(reader);
    return reader->status;
  case 1:
    if (reader->off + 8U > reader->len) {
      reader->status = OTLP_STATUS_DECODE_ERROR;
      return reader->status;
    }
    reader->off += 8U;
    return OTLP_STATUS_OK;
  case 2:
    return otlp__read_length_delimited(reader, &ptr, &len);
  case 5:
    if (reader->off + 4U > reader->len) {
      reader->status = OTLP_STATUS_DECODE_ERROR;
      return reader->status;
    }
    reader->off += 4U;
    return OTLP_STATUS_OK;
  default:
    reader->status = OTLP_STATUS_DECODE_ERROR;
    return reader->status;
  }
}

static inline otlp_status otlp__append_primitive(void **ptr, size_t *len,
                                                 size_t *cap, size_t elem_size,
                                                 const void *value) {
  otlp_status status;
  if (*len == *cap) {
    status = otlp__grow(ptr, cap, *len + 1U, elem_size);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  memcpy(((uint8_t *)*ptr) + (*len * elem_size), value, elem_size);
  *len += 1U;
  return OTLP_STATUS_OK;
}

static inline otlp_status otlp_any_value_copy(otlp_any_value *dst,
                                              const otlp_any_value *src);
static inline size_t
otlp_any_value_encoded_size_field(uint32_t field_number,
                                  const otlp_any_value *value);
static inline void otlp_any_value_encode_field(otlp__writer *writer,
                                               uint32_t field_number,
                                               const otlp_any_value *value);
static inline otlp_status otlp_any_value_decode_inner(otlp_any_value *value,
                                                      const uint8_t *data,
                                                      size_t len);
static inline otlp_status otlp_any_value_merge_inner(otlp_any_value *value,
                                                     const uint8_t *data,
                                                     size_t len);
static inline otlp_status otlp_key_value_copy(otlp_key_value *dst,
                                              const otlp_key_value *src);
static inline size_t
otlp_key_value_encoded_size_inner(const otlp_key_value *kv);
static inline void otlp_key_value_encode_field(otlp__writer *writer,
                                               uint32_t field_number,
                                               const otlp_key_value *kv);
static inline otlp_status otlp_key_value_decode_inner(otlp_key_value *kv,
                                                      const uint8_t *data,
                                                      size_t len);
static inline otlp_status otlp_key_value_merge_inner(otlp_key_value *kv,
                                                     const uint8_t *data,
                                                     size_t len);
static inline otlp_status otlp_array_value_merge_inner(otlp_array_value *value,
                                                       const uint8_t *data,
                                                       size_t len);
static inline otlp_status
otlp_key_value_list_merge_inner(otlp_key_value_list *list,
                                const uint8_t *data, size_t len);

static inline void otlp_array_value_init(otlp_array_value *value) {
  otlp__zero(value, sizeof(*value));
}

static inline void otlp_array_value_destroy(otlp_array_value *value) {
  size_t i;
  for (i = 0; i < value->values.len; ++i)
    otlp_any_value_destroy(&value->values.ptr[i]);
  free(value->values.ptr);
  otlp_array_value_init(value);
}

static inline otlp_status
otlp_array_value_copy(otlp_array_value *dst, const otlp_array_value *src) {
  size_t i;
  otlp_array_value_init(dst);
  for (i = 0; i < src->values.len; ++i) {
    otlp_status status =
        otlp_array_value_append_value(dst, &src->values.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}

static inline otlp_status
otlp_array_value_append_value(otlp_array_value *value,
                              const otlp_any_value *item) {
  otlp_any_value copy;
  otlp_status status;
  otlp_any_value_init(&copy);
  status = otlp_any_value_copy(&copy, item);
  if (status != OTLP_STATUS_OK)
    return status;
  return otlp__append_primitive((void **)&value->values.ptr, &value->values.len,
                                &value->values.cap, sizeof(copy), &copy);
}

static inline size_t
otlp_array_value_encoded_size_inner(const otlp_array_value *value) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < value->values.len; ++i)
    size += otlp_any_value_encoded_size_field(1, &value->values.ptr[i]);
  return size;
}

static inline void otlp_array_value_encode_field(otlp__writer *writer,
                                                 uint32_t field_number,
                                                 const otlp_array_value *value) {
  size_t i;
  size_t inner = otlp_array_value_encoded_size_inner(value);
  otlp__write_message_start(writer, field_number, inner);
  for (i = 0; i < value->values.len; ++i)
    otlp_any_value_encode_field(writer, 1, &value->values.ptr[i]);
}

static inline otlp_status otlp_array_value_merge_inner(otlp_array_value *value,
                                                       const uint8_t *data,
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
      const uint8_t *ptr;
      size_t size;
      otlp_any_value item;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      otlp_any_value_init(&item);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_any_value_decode_inner(&item, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&value->values.ptr, &value->values.len,
                                 &value->values.cap, sizeof(item), &item) !=
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

static inline otlp_status otlp_array_value_decode_inner(otlp_array_value *value,
                                                        const uint8_t *data,
                                                        size_t len) {
  otlp_array_value_destroy(value);
  otlp_array_value_init(value);
  return otlp_array_value_merge_inner(value, data, len);
}

static inline void otlp_key_value_list_init(otlp_key_value_list *list) {
  otlp__zero(list, sizeof(*list));
}

static inline void otlp_key_value_list_destroy(otlp_key_value_list *list) {
  size_t i;
  for (i = 0; i < list->values.len; ++i)
    otlp_key_value_destroy(&list->values.ptr[i]);
  free(list->values.ptr);
  otlp_key_value_list_init(list);
}

static inline otlp_status
otlp_key_value_list_copy(otlp_key_value_list *dst,
                         const otlp_key_value_list *src) {
  size_t i;
  otlp_key_value_list_init(dst);
  for (i = 0; i < src->values.len; ++i) {
    otlp_status status =
        otlp_key_value_list_append_value(dst, &src->values.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}

static inline otlp_status
otlp_key_value_list_append_value(otlp_key_value_list *list,
                                 const otlp_key_value *item) {
  otlp_key_value copy;
  otlp_status status;
  otlp_key_value_init(&copy);
  status = otlp_key_value_copy(&copy, item);
  if (status != OTLP_STATUS_OK)
    return status;
  return otlp__append_primitive((void **)&list->values.ptr, &list->values.len,
                                &list->values.cap, sizeof(copy), &copy);
}

static inline size_t
otlp_key_value_list_encoded_size_inner(const otlp_key_value_list *list) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < list->values.len; ++i)
    size += otlp__message_field_size(1, otlp_key_value_encoded_size_inner(
                                            &list->values.ptr[i]));
  return size;
}

static inline void otlp_key_value_list_encode_field(otlp__writer *writer,
                                                    uint32_t field_number,
                                                    const otlp_key_value_list *list) {
  size_t i;
  size_t inner = otlp_key_value_list_encoded_size_inner(list);
  otlp__write_message_start(writer, field_number, inner);
  for (i = 0; i < list->values.len; ++i)
    otlp_key_value_encode_field(writer, 1, &list->values.ptr[i]);
}

static inline otlp_status
otlp_key_value_list_decode_inner(otlp_key_value_list *list,
                                 const uint8_t *data, size_t len) {
  otlp_key_value_list_destroy(list);
  otlp_key_value_list_init(list);
  return otlp_key_value_list_merge_inner(list, data, len);
}

static inline otlp_status
otlp_key_value_list_merge_inner(otlp_key_value_list *list,
                                const uint8_t *data, size_t len) {
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
      const uint8_t *ptr;
      size_t size;
      otlp_key_value item;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      otlp_key_value_init(&item);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_key_value_decode_inner(&item, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&list->values.ptr, &list->values.len,
                                 &list->values.cap, sizeof(item), &item) !=
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

static inline void otlp_any_value_init(otlp_any_value *value) {
  otlp__zero(value, sizeof(*value));
}

static inline void otlp_any_value_destroy(otlp_any_value *value) {
  if (value->kind == OTLP_ANY_VALUE_STRING)
    free(value->value.string_value.ptr);
  if (value->kind == OTLP_ANY_VALUE_ARRAY) {
    otlp_array_value_destroy(value->value.array_value);
    free(value->value.array_value);
  }
  if (value->kind == OTLP_ANY_VALUE_KVLIST) {
    otlp_key_value_list_destroy(value->value.kvlist_value);
    free(value->value.kvlist_value);
  }
  if (value->kind == OTLP_ANY_VALUE_BYTES)
    free(value->value.bytes_value.ptr);
  otlp_any_value_init(value);
}

static inline otlp_status otlp_any_value_copy(otlp_any_value *dst,
                                              const otlp_any_value *src) {
  otlp_any_value_init(dst);
  dst->kind = src->kind;
  switch (src->kind) {
  case OTLP_ANY_VALUE_STRING:
    return otlp__set_string(&dst->value.string_value,
                            src->value.string_value.ptr,
                            src->value.string_value.len);
  case OTLP_ANY_VALUE_BOOL:
    dst->value.bool_value = src->value.bool_value;
    return OTLP_STATUS_OK;
  case OTLP_ANY_VALUE_INT:
    dst->value.int_value = src->value.int_value;
    return OTLP_STATUS_OK;
  case OTLP_ANY_VALUE_DOUBLE:
    dst->value.double_value = src->value.double_value;
    return OTLP_STATUS_OK;
  case OTLP_ANY_VALUE_ARRAY:
    dst->value.array_value = (otlp_array_value *)malloc(sizeof(*dst->value.array_value));
    if (dst->value.array_value == NULL)
      return OTLP_STATUS_OUT_OF_MEMORY;
    return otlp_array_value_copy(dst->value.array_value, src->value.array_value);
  case OTLP_ANY_VALUE_KVLIST:
    dst->value.kvlist_value =
        (otlp_key_value_list *)malloc(sizeof(*dst->value.kvlist_value));
    if (dst->value.kvlist_value == NULL)
      return OTLP_STATUS_OUT_OF_MEMORY;
    return otlp_key_value_list_copy(dst->value.kvlist_value,
                                    src->value.kvlist_value);
  case OTLP_ANY_VALUE_BYTES:
    return otlp__set_bytes(&dst->value.bytes_value, src->value.bytes_value.ptr,
                           src->value.bytes_value.len);
  case OTLP_ANY_VALUE_STRING_STRINDEX:
    dst->value.string_value_strindex = src->value.string_value_strindex;
    return OTLP_STATUS_OK;
  default:
    return OTLP_STATUS_OK;
  }
}

static inline otlp_status
otlp_any_value_init_string(otlp_any_value *value, const char *ptr, size_t len) {
  otlp_any_value_init(value);
  value->kind = OTLP_ANY_VALUE_STRING;
  return otlp__set_string(&value->value.string_value, ptr, len);
}

static inline size_t
otlp_any_value_encoded_size_inner(const otlp_any_value *value) {
  switch (value->kind) {
  case OTLP_ANY_VALUE_STRING:
    return otlp__bytes_field_size(1, value->value.string_value.len);
  case OTLP_ANY_VALUE_BOOL:
    return otlp__varint_field_size(2, value->value.bool_value ? 1U : 0U);
  case OTLP_ANY_VALUE_INT:
    return otlp__varint_field_size(3, (uint64_t)value->value.int_value);
  case OTLP_ANY_VALUE_DOUBLE:
    return otlp__fixed64_field_size(4);
  case OTLP_ANY_VALUE_ARRAY:
    return otlp__message_field_size(
        5, otlp_array_value_encoded_size_inner(value->value.array_value));
  case OTLP_ANY_VALUE_KVLIST:
    return otlp__message_field_size(
        6, otlp_key_value_list_encoded_size_inner(value->value.kvlist_value));
  case OTLP_ANY_VALUE_BYTES:
    return otlp__bytes_field_size(7, value->value.bytes_value.len);
  case OTLP_ANY_VALUE_STRING_STRINDEX:
    return otlp__varint_field_size(
        8, otlp__int32_varint(value->value.string_value_strindex));
  default:
    return 0;
  }
}

static inline size_t
otlp_any_value_encoded_size_field(uint32_t field_number,
                                  const otlp_any_value *value) {
  size_t inner = otlp_any_value_encoded_size_inner(value);
  if (value->kind == OTLP_ANY_VALUE_EMPTY)
    return otlp__message_field_size(field_number, 0);
  return inner == 0 ? 0 : otlp__message_field_size(field_number, inner);
}

static inline void otlp_any_value_encode_inner(otlp__writer *writer,
                                               const otlp_any_value *value) {
  switch (value->kind) {
  case OTLP_ANY_VALUE_STRING:
    otlp__write_bytes_field_allow_empty(writer, 1,
                                        (const uint8_t *)value->value.string_value.ptr,
                                        value->value.string_value.len);
    break;
  case OTLP_ANY_VALUE_BOOL:
    otlp__write_tag(writer, 2, 0);
    otlp__write_varint(writer, value->value.bool_value ? 1U : 0U);
    break;
  case OTLP_ANY_VALUE_INT:
    otlp__write_tag(writer, 3, 0);
    otlp__write_varint(writer, (uint64_t)value->value.int_value);
    break;
  case OTLP_ANY_VALUE_DOUBLE: {
    union {
      double d;
      uint64_t u;
    } bits;
    bits.d = value->value.double_value;
    otlp__write_tag(writer, 4, 1);
    otlp__write_fixed64(writer, bits.u);
    break;
  }
  case OTLP_ANY_VALUE_ARRAY:
    otlp_array_value_encode_field(writer, 5, value->value.array_value);
    break;
  case OTLP_ANY_VALUE_KVLIST:
    otlp_key_value_list_encode_field(writer, 6, value->value.kvlist_value);
    break;
  case OTLP_ANY_VALUE_BYTES:
    otlp__write_bytes_field_allow_empty(writer, 7, value->value.bytes_value.ptr,
                                        value->value.bytes_value.len);
    break;
  case OTLP_ANY_VALUE_STRING_STRINDEX:
    otlp__write_tag(writer, 8, 0);
    otlp__write_varint(writer,
                       otlp__int32_varint(value->value.string_value_strindex));
    break;
  default:
    break;
  }
}

static inline void otlp_any_value_encode_field(otlp__writer *writer,
                                               uint32_t field_number,
                                               const otlp_any_value *value) {
  size_t inner = otlp_any_value_encoded_size_inner(value);
  if (inner == 0 && value->kind != OTLP_ANY_VALUE_EMPTY)
    return;
  otlp__write_message_start(writer, field_number, inner);
  otlp_any_value_encode_inner(writer, value);
}

static inline otlp_status otlp_any_value_decode_inner(otlp_any_value *value,
                                                      const uint8_t *data,
                                                      size_t len) {
  otlp_any_value_destroy(value);
  otlp_any_value_init(value);
  return otlp_any_value_merge_inner(value, data, len);
}

static inline otlp_status otlp_any_value_merge_inner(otlp_any_value *value,
                                                     const uint8_t *data,
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
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      otlp_any_value_destroy(value);
      value->kind = OTLP_ANY_VALUE_STRING;
      if (otlp__set_string(&value->value.string_value, (const char *)ptr,
                           size) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 2:
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      otlp_any_value_destroy(value);
      value->kind = OTLP_ANY_VALUE_BOOL;
      value->value.bool_value = (int)otlp__read_varint(&reader);
      break;
    case 3: {
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      otlp_any_value_destroy(value);
      value->kind = OTLP_ANY_VALUE_INT;
      value->value.int_value = (int64_t)otlp__read_varint(&reader);
      break;
    }
    case 4: {
      union {
        double d;
        uint64_t u;
      } bits;
      if (wire_type != 1)
        return OTLP_STATUS_DECODE_ERROR;
      otlp_any_value_destroy(value);
      value->kind = OTLP_ANY_VALUE_DOUBLE;
      bits.u = otlp__read_fixed64(&reader);
      value->value.double_value = bits.d;
    break;
  }
  case 5: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (value->kind != OTLP_ANY_VALUE_ARRAY) {
        otlp_any_value_destroy(value);
        value->kind = OTLP_ANY_VALUE_ARRAY;
        value->value.array_value =
            (otlp_array_value *)malloc(sizeof(*value->value.array_value));
        if (value->value.array_value == NULL)
          return OTLP_STATUS_OUT_OF_MEMORY;
        otlp_array_value_init(value->value.array_value);
      }
      if (otlp_array_value_merge_inner(value->value.array_value, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      break;
    }
    case 6: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (value->kind != OTLP_ANY_VALUE_KVLIST) {
        otlp_any_value_destroy(value);
        value->kind = OTLP_ANY_VALUE_KVLIST;
        value->value.kvlist_value =
            (otlp_key_value_list *)malloc(sizeof(*value->value.kvlist_value));
        if (value->value.kvlist_value == NULL)
          return OTLP_STATUS_OUT_OF_MEMORY;
        otlp_key_value_list_init(value->value.kvlist_value);
      }
      if (otlp_key_value_list_merge_inner(value->value.kvlist_value, ptr,
                                          size) != OTLP_STATUS_OK)
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
      otlp_any_value_destroy(value);
      value->kind = OTLP_ANY_VALUE_BYTES;
      if (otlp__set_bytes(&value->value.bytes_value, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 8:
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      otlp_any_value_destroy(value);
      value->kind = OTLP_ANY_VALUE_STRING_STRINDEX;
      value->value.string_value_strindex = (int32_t)otlp__read_varint(&reader);
      break;
    default:
      if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
        return reader.status;
      break;
    }
  }
  return reader.status;
}

static inline void otlp_key_value_init(otlp_key_value *kv) {
  otlp__zero(kv, sizeof(*kv));
  otlp_any_value_init(&kv->value);
}

static inline void otlp_key_value_destroy(otlp_key_value *kv) {
  free(kv->key.ptr);
  otlp_any_value_destroy(&kv->value);
  otlp_key_value_init(kv);
}

static inline otlp_status otlp_key_value_copy(otlp_key_value *dst,
                                              const otlp_key_value *src) {
  otlp_key_value_init(dst);
  if (otlp__set_string(&dst->key, src->key.ptr, src->key.len) != OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  dst->key_strindex = src->key_strindex;
  dst->has_value = src->has_value;
  return otlp_any_value_copy(&dst->value, &src->value);
}

static inline size_t
otlp_key_value_encoded_size_inner(const otlp_key_value *kv) {
  return (kv->key.len ? otlp__bytes_field_size(1, kv->key.len) : 0) +
         (kv->has_value ? otlp_any_value_encoded_size_field(2, &kv->value)
                        : 0) +
         (kv->key_strindex
              ? otlp__int32_field_size(3, kv->key_strindex)
              : 0);
}

static inline void otlp_key_value_encode_field(otlp__writer *writer,
                                               uint32_t field_number,
                                               const otlp_key_value *kv) {
  size_t inner = otlp_key_value_encoded_size_inner(kv);
  if (kv->key.len && kv->key_strindex) {
    writer->status = OTLP_STATUS_INVALID_ARGUMENT;
    return;
  }
  otlp__write_message_start(writer, field_number, inner);
  otlp__write_string_field(writer, 1, &kv->key);
  if (kv->has_value)
    otlp_any_value_encode_field(writer, 2, &kv->value);
  if (kv->key_strindex) {
    otlp__write_tag(writer, 3, 0);
    otlp__write_varint(writer, otlp__int32_varint(kv->key_strindex));
  }
}

static inline otlp_status otlp_key_value_decode_inner(otlp_key_value *kv,
                                                      const uint8_t *data,
                                                      size_t len) {
  otlp_key_value_destroy(kv);
  otlp_key_value_init(kv);
  return otlp_key_value_merge_inner(kv, data, len);
}

static inline otlp_status otlp_key_value_merge_inner(otlp_key_value *kv,
                                                     const uint8_t *data,
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
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&kv->key, (const char *)ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 2: {
      const uint8_t *ptr;
      size_t size;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_any_value_merge_inner(&kv->value, ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      kv->has_value = 1;
      break;
    }
    case 3:
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      kv->key_strindex = (int32_t)otlp__read_varint(&reader);
      break;
    default:
      if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
        return reader.status;
      break;
    }
  }
  return reader.status;
}

static inline void otlp_instrumentation_scope_init(
    otlp_instrumentation_scope *scope) {
  otlp__zero(scope, sizeof(*scope));
}

static inline void otlp_instrumentation_scope_destroy(
    otlp_instrumentation_scope *scope) {
  size_t i;
  free(scope->name.ptr);
  free(scope->version.ptr);
  for (i = 0; i < scope->attributes.len; ++i)
    otlp_key_value_destroy(&scope->attributes.ptr[i]);
  free(scope->attributes.ptr);
  otlp_instrumentation_scope_init(scope);
}

static inline otlp_status
otlp_instrumentation_scope_copy(otlp_instrumentation_scope *dst,
                                const otlp_instrumentation_scope *src) {
  size_t i;
  otlp_instrumentation_scope_init(dst);
  dst->dropped_attributes_count = src->dropped_attributes_count;
  if (otlp__set_string(&dst->name, src->name.ptr, src->name.len) !=
          OTLP_STATUS_OK ||
      otlp__set_string(&dst->version, src->version.ptr, src->version.len) !=
          OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  for (i = 0; i < src->attributes.len; ++i) {
    otlp_key_value copy;
    otlp_status status;
    otlp_key_value_init(&copy);
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

static inline size_t otlp_instrumentation_scope_encoded_size_inner(
    const otlp_instrumentation_scope *scope) {
  size_t size = 0;
  size_t i;
  if (scope->name.len)
    size += otlp__bytes_field_size(1, scope->name.len);
  if (scope->version.len)
    size += otlp__bytes_field_size(2, scope->version.len);
  for (i = 0; i < scope->attributes.len; ++i)
    size += otlp__message_field_size(
        3, otlp_key_value_encoded_size_inner(&scope->attributes.ptr[i]));
  if (scope->dropped_attributes_count)
    size += otlp__varint_field_size(4, scope->dropped_attributes_count);
  return size;
}

static inline void
otlp_instrumentation_scope_encode_field(otlp__writer *writer,
                                        uint32_t field_number,
                                        const otlp_instrumentation_scope *scope) {
  size_t i;
  size_t inner = otlp_instrumentation_scope_encoded_size_inner(scope);
  otlp__write_message_start(writer, field_number, inner);
  otlp__write_string_field(writer, 1, &scope->name);
  otlp__write_string_field(writer, 2, &scope->version);
  for (i = 0; i < scope->attributes.len; ++i)
    otlp_key_value_encode_field(writer, 3, &scope->attributes.ptr[i]);
  if (scope->dropped_attributes_count) {
    otlp__write_tag(writer, 4, 0);
    otlp__write_varint(writer, scope->dropped_attributes_count);
  }
}

static inline otlp_status otlp_instrumentation_scope_decode_inner(
    otlp_instrumentation_scope *scope, const uint8_t *data, size_t len) {
  otlp__reader reader;
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
      otlp_string *dst = field_number == 1 ? &scope->name : &scope->version;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(dst, (const char *)ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 3: {
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
      if (otlp__append_primitive((void **)&scope->attributes.ptr,
                                 &scope->attributes.len,
                                 &scope->attributes.cap, sizeof(attribute),
                                 &attribute) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 4:
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      scope->dropped_attributes_count = (uint32_t)otlp__read_varint(&reader);
      break;
    default:
      if (otlp__skip_field(&reader, wire_type) != OTLP_STATUS_OK)
        return reader.status;
      break;
    }
  }
  return reader.status;
}

static inline void otlp_entity_ref_init(otlp_entity_ref *entity_ref) {
  otlp__zero(entity_ref, sizeof(*entity_ref));
}

static inline void otlp_entity_ref_destroy(otlp_entity_ref *entity_ref) {
  size_t i;
  free(entity_ref->schema_url.ptr);
  free(entity_ref->type.ptr);
  for (i = 0; i < entity_ref->id_keys.len; ++i)
    free(entity_ref->id_keys.ptr[i].ptr);
  free(entity_ref->id_keys.ptr);
  for (i = 0; i < entity_ref->description_keys.len; ++i)
    free(entity_ref->description_keys.ptr[i].ptr);
  free(entity_ref->description_keys.ptr);
  otlp_entity_ref_init(entity_ref);
}

static inline otlp_status otlp_entity_ref_copy(otlp_entity_ref *dst,
                                               const otlp_entity_ref *src) {
  size_t i;
  otlp_entity_ref_init(dst);
  if (otlp__set_string(&dst->schema_url, src->schema_url.ptr,
                       src->schema_url.len) != OTLP_STATUS_OK ||
      otlp__set_string(&dst->type, src->type.ptr, src->type.len) !=
          OTLP_STATUS_OK)
    return OTLP_STATUS_OUT_OF_MEMORY;
  for (i = 0; i < src->id_keys.len; ++i) {
    otlp_string copy;
    otlp__zero(&copy, sizeof(copy));
    otlp_status status =
        otlp__set_string(&copy, src->id_keys.ptr[i].ptr, src->id_keys.ptr[i].len);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->id_keys.ptr, &dst->id_keys.len,
                                    &dst->id_keys.cap, sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  for (i = 0; i < src->description_keys.len; ++i) {
    otlp_string copy;
    otlp__zero(&copy, sizeof(copy));
    otlp_status status = otlp__set_string(&copy, src->description_keys.ptr[i].ptr,
                                          src->description_keys.ptr[i].len);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->description_keys.ptr,
                                    &dst->description_keys.len,
                                    &dst->description_keys.cap, sizeof(copy),
                                    &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}

static inline size_t
otlp_entity_ref_encoded_size_inner(const otlp_entity_ref *entity_ref) {
  size_t size = 0;
  size_t i;
  if (entity_ref->schema_url.len)
    size += otlp__bytes_field_size(1, entity_ref->schema_url.len);
  if (entity_ref->type.len)
    size += otlp__bytes_field_size(2, entity_ref->type.len);
  for (i = 0; i < entity_ref->id_keys.len; ++i)
    size += otlp__bytes_field_size(3, entity_ref->id_keys.ptr[i].len);
  for (i = 0; i < entity_ref->description_keys.len; ++i)
    size += otlp__bytes_field_size(4, entity_ref->description_keys.ptr[i].len);
  return size;
}

static inline void otlp_entity_ref_encode_field(otlp__writer *writer,
                                                uint32_t field_number,
                                                const otlp_entity_ref *entity_ref) {
  size_t i;
  size_t inner = otlp_entity_ref_encoded_size_inner(entity_ref);
  otlp__write_message_start(writer, field_number, inner);
  otlp__write_string_field(writer, 1, &entity_ref->schema_url);
  otlp__write_string_field(writer, 2, &entity_ref->type);
  for (i = 0; i < entity_ref->id_keys.len; ++i)
    otlp__write_bytes_field_allow_empty(
        writer, 3, (const uint8_t *)entity_ref->id_keys.ptr[i].ptr,
        entity_ref->id_keys.ptr[i].len);
  for (i = 0; i < entity_ref->description_keys.len; ++i)
    otlp__write_bytes_field_allow_empty(
        writer, 4, (const uint8_t *)entity_ref->description_keys.ptr[i].ptr,
        entity_ref->description_keys.ptr[i].len);
}

static inline otlp_status otlp_entity_ref_decode_inner(otlp_entity_ref *entity_ref,
                                                       const uint8_t *data,
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
    case 1:
    case 2: {
      const uint8_t *ptr;
      size_t size;
      otlp_string *dst = field_number == 1 ? &entity_ref->schema_url
                                            : &entity_ref->type;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(dst, (const char *)ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 3:
    case 4: {
      const uint8_t *ptr;
      size_t size;
      otlp_string copy;
      otlp_string_array *dst = field_number == 3 ? &entity_ref->id_keys
                                                 : &entity_ref->description_keys;
      otlp__zero(&copy, sizeof(copy));
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp__set_string(&copy, (const char *)ptr, size) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      if (otlp__append_primitive((void **)&dst->ptr, &dst->len, &dst->cap,
                                 sizeof(copy), &copy) != OTLP_STATUS_OK)
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

static inline void otlp_resource_init(otlp_resource *resource) {
  otlp__zero(resource, sizeof(*resource));
}

static inline void otlp_resource_destroy(otlp_resource *resource) {
  size_t i;
  for (i = 0; i < resource->attributes.len; ++i)
    otlp_key_value_destroy(&resource->attributes.ptr[i]);
  free(resource->attributes.ptr);
  for (i = 0; i < resource->entity_refs.len; ++i)
    otlp_entity_ref_destroy(&resource->entity_refs.ptr[i]);
  free(resource->entity_refs.ptr);
  otlp_resource_init(resource);
}

static inline otlp_status otlp_resource_copy(otlp_resource *dst,
                                             const otlp_resource *src) {
  size_t i;
  otlp_resource_init(dst);
  dst->dropped_attributes_count = src->dropped_attributes_count;
  for (i = 0; i < src->attributes.len; ++i) {
    otlp_key_value copy;
    otlp_status status;
    otlp_key_value_init(&copy);
    status = otlp_key_value_copy(&copy, &src->attributes.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->attributes.ptr,
                                    &dst->attributes.len, &dst->attributes.cap,
                                    sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  for (i = 0; i < src->entity_refs.len; ++i) {
    otlp_entity_ref copy;
    otlp_status status;
    otlp_entity_ref_init(&copy);
    status = otlp_entity_ref_copy(&copy, &src->entity_refs.ptr[i]);
    if (status != OTLP_STATUS_OK)
      return status;
    status = otlp__append_primitive((void **)&dst->entity_refs.ptr,
                                    &dst->entity_refs.len,
                                    &dst->entity_refs.cap, sizeof(copy), &copy);
    if (status != OTLP_STATUS_OK)
      return status;
  }
  return OTLP_STATUS_OK;
}

static inline size_t otlp_resource_encoded_size_inner(
    const otlp_resource *resource) {
  size_t size = 0;
  size_t i;
  for (i = 0; i < resource->attributes.len; ++i)
    size += otlp__message_field_size(
        1, otlp_key_value_encoded_size_inner(&resource->attributes.ptr[i]));
  if (resource->dropped_attributes_count)
    size += otlp__varint_field_size(2, resource->dropped_attributes_count);
  for (i = 0; i < resource->entity_refs.len; ++i)
    size += otlp__message_field_size(
        3, otlp_entity_ref_encoded_size_inner(&resource->entity_refs.ptr[i]));
  return size;
}

static inline void otlp_resource_encode_field(otlp__writer *writer,
                                              uint32_t field_number,
                                              const otlp_resource *resource) {
  size_t i;
  size_t inner = otlp_resource_encoded_size_inner(resource);
  otlp__write_message_start(writer, field_number, inner);
  for (i = 0; i < resource->attributes.len; ++i)
    otlp_key_value_encode_field(writer, 1, &resource->attributes.ptr[i]);
  if (resource->dropped_attributes_count) {
    otlp__write_tag(writer, 2, 0);
    otlp__write_varint(writer, resource->dropped_attributes_count);
  }
  for (i = 0; i < resource->entity_refs.len; ++i)
    otlp_entity_ref_encode_field(writer, 3, &resource->entity_refs.ptr[i]);
}

static inline otlp_status otlp_resource_decode_inner(otlp_resource *resource,
                                                     const uint8_t *data,
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
      if (otlp__append_primitive((void **)&resource->attributes.ptr,
                                 &resource->attributes.len,
                                 &resource->attributes.cap, sizeof(attribute),
                                 &attribute) != OTLP_STATUS_OK)
        return OTLP_STATUS_OUT_OF_MEMORY;
      break;
    }
    case 2:
      if (wire_type != 0)
        return OTLP_STATUS_DECODE_ERROR;
      resource->dropped_attributes_count = (uint32_t)otlp__read_varint(&reader);
      break;
    case 3: {
      const uint8_t *ptr;
      size_t size;
      otlp_entity_ref entity_ref;
      if (wire_type != 2)
        return OTLP_STATUS_DECODE_ERROR;
      otlp_entity_ref_init(&entity_ref);
      if (otlp__read_length_delimited(&reader, &ptr, &size) != OTLP_STATUS_OK)
        return reader.status;
      if (otlp_entity_ref_decode_inner(&entity_ref, ptr, size) !=
          OTLP_STATUS_OK)
        return OTLP_STATUS_DECODE_ERROR;
      if (otlp__append_primitive((void **)&resource->entity_refs.ptr,
                                 &resource->entity_refs.len,
                                 &resource->entity_refs.cap, sizeof(entity_ref),
                                 &entity_ref) != OTLP_STATUS_OK)
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

#endif /* OTLP_FORMAT_COMMON_H */
