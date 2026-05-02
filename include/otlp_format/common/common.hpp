#ifndef OTLP_FORMAT_COMMON_HPP
#define OTLP_FORMAT_COMMON_HPP

#include "common.h"

#if defined(__cplusplus) &&                                                     \
    (__cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900))
#include <cstdlib>
#include <cstring>
#include <string>

namespace otlp {

class ArrayValue;
class KeyValue;
class KeyValueList;
class InstrumentationScope;
class EntityRef;
class Resource;

class AnyValue : private otlp_any_value {
public:
  AnyValue() { otlp_any_value_init(this); }

  AnyValue(const AnyValue &other) {
    otlp_any_value_init(this);
    otlp_any_value_copy(this, other.get());
  }

  AnyValue &operator=(const AnyValue &other) {
    if (this != &other) {
      otlp_any_value_destroy(this);
      otlp_any_value_init(this);
      otlp_any_value_copy(this, other.get());
    }
    return *this;
  }

  ~AnyValue() { otlp_any_value_destroy(this); }

  static AnyValue string(const char *value) {
    AnyValue out;
    otlp_any_value_init_string(&out, value, std::strlen(value));
    return out;
  }

  static AnyValue bool_value(bool value) {
    AnyValue out;
    out.set_bool_value(value);
    return out;
  }

  static AnyValue int_value(int64_t value) {
    AnyValue out;
    out.set_int_value(value);
    return out;
  }

  static AnyValue double_value(double value) {
    AnyValue out;
    out.set_double_value(value);
    return out;
  }

  static AnyValue bytes_value(const uint8_t *ptr, size_t len) {
    AnyValue out;
    out.set_bytes_value(ptr, len);
    return out;
  }

  static AnyValue string_value_strindex(int32_t value) {
    AnyValue out;
    out.set_string_value_strindex(value);
    return out;
  }

  void set_string(const char *value) {
    otlp_any_value_destroy(this);
    otlp_any_value_init(this);
    otlp_any_value_init_string(this, value, std::strlen(value));
  }

  void set_bool_value(bool value) {
    otlp_any_value_destroy(this);
    otlp_any_value_init(this);
    this->kind = OTLP_ANY_VALUE_BOOL;
    this->value.bool_value = value ? 1 : 0;
  }

  void set_int_value(int64_t value) {
    otlp_any_value_destroy(this);
    otlp_any_value_init(this);
    this->kind = OTLP_ANY_VALUE_INT;
    this->value.int_value = value;
  }

  void set_double_value(double value) {
    otlp_any_value_destroy(this);
    otlp_any_value_init(this);
    this->kind = OTLP_ANY_VALUE_DOUBLE;
    this->value.double_value = value;
  }

  void set_bytes_value(const uint8_t *ptr, size_t len) {
    otlp_any_value_destroy(this);
    otlp_any_value_init(this);
    this->kind = OTLP_ANY_VALUE_BYTES;
    otlp__set_bytes(&this->value.bytes_value, ptr, len);
  }

  void set_string_value_strindex(int32_t value) {
    otlp_any_value_destroy(this);
    otlp_any_value_init(this);
    this->kind = OTLP_ANY_VALUE_STRING_STRINDEX;
    this->value.string_value_strindex = value;
  }

  void set_array_value(const ArrayValue &value);

  void set_kvlist_value(const KeyValueList &value);

  const otlp_any_value *get() const { return this; }

  friend class LogRecord;
  friend class KeyValue;
};

class ArrayValue : private otlp_array_value {
public:
  ArrayValue() { otlp_array_value_init(this); }

  ArrayValue(const ArrayValue &other) {
    otlp_array_value_init(this);
    otlp_array_value_copy(this, other.get());
  }

  ArrayValue &operator=(const ArrayValue &other) {
    if (this != &other) {
      otlp_array_value_destroy(this);
      otlp_array_value_init(this);
      otlp_array_value_copy(this, other.get());
    }
    return *this;
  }

  ~ArrayValue() { otlp_array_value_destroy(this); }

  void append_value(const AnyValue &value) {
    otlp_array_value_append_value(this, value.get());
  }

  const otlp_array_value *get() const { return this; }
};

class KeyValue : private otlp_key_value {
public:
  KeyValue() { otlp_key_value_init(this); }

  KeyValue(const KeyValue &other) {
    otlp_key_value_init(this);
    otlp_key_value_copy(this, other.get());
  }

  KeyValue &operator=(const KeyValue &other) {
    if (this != &other) {
      otlp_key_value_destroy(this);
      otlp_key_value_init(this);
      otlp_key_value_copy(this, other.get());
    }
    return *this;
  }

  ~KeyValue() { otlp_key_value_destroy(this); }

  void set_key(const char *value) {
    otlp__set_string(&this->key, value, std::strlen(value));
    this->key_strindex = 0;
  }

  void set_value(const AnyValue &value) {
    otlp_any_value_copy(&this->value, value.get());
    this->has_value = 1;
  }

  void set_key_strindex(int32_t value) {
    free(this->key.ptr);
    this->key.ptr = NULL;
    this->key.len = 0;
    this->key_strindex = value;
  }

  const otlp_key_value *get() const { return this; }
};

class KeyValueList : private otlp_key_value_list {
public:
  KeyValueList() { otlp_key_value_list_init(this); }

  KeyValueList(const KeyValueList &other) {
    otlp_key_value_list_init(this);
    otlp_key_value_list_copy(this, other.get());
  }

  KeyValueList &operator=(const KeyValueList &other) {
    if (this != &other) {
      otlp_key_value_list_destroy(this);
      otlp_key_value_list_init(this);
      otlp_key_value_list_copy(this, other.get());
    }
    return *this;
  }

  ~KeyValueList() { otlp_key_value_list_destroy(this); }

  void append_value(const KeyValue &value) {
    otlp_key_value_list_append_value(this, value.get());
  }

  const otlp_key_value_list *get() const { return this; }
};

class InstrumentationScope : private otlp_instrumentation_scope {
public:
  InstrumentationScope() { otlp_instrumentation_scope_init(this); }

  InstrumentationScope(const InstrumentationScope &other) {
    otlp_instrumentation_scope_init(this);
    otlp_instrumentation_scope_copy(this, other.get());
  }

  InstrumentationScope &operator=(const InstrumentationScope &other) {
    if (this != &other) {
      otlp_instrumentation_scope_destroy(this);
      otlp_instrumentation_scope_init(this);
      otlp_instrumentation_scope_copy(this, other.get());
    }
    return *this;
  }

  ~InstrumentationScope() { otlp_instrumentation_scope_destroy(this); }

  void set_name(const char *value) {
    otlp__set_string(&this->name, value, std::strlen(value));
  }

  void set_version(const char *value) {
    otlp__set_string(&this->version, value, std::strlen(value));
  }

  void append_attribute(const KeyValue &value) {
    otlp_key_value copy;
    otlp_key_value_init(&copy);
    otlp_key_value_copy(&copy, value.get());
    otlp__append_primitive((void **)&this->attributes.ptr, &this->attributes.len,
                           &this->attributes.cap, sizeof(copy), &copy);
  }

  void set_dropped_attributes_count(uint32_t value) {
    this->dropped_attributes_count = value;
  }

  const otlp_instrumentation_scope *get() const { return this; }
};

class EntityRef : private otlp_entity_ref {
public:
  EntityRef() { otlp_entity_ref_init(this); }

  EntityRef(const EntityRef &other) {
    otlp_entity_ref_init(this);
    otlp_entity_ref_copy(this, other.get());
  }

  EntityRef &operator=(const EntityRef &other) {
    if (this != &other) {
      otlp_entity_ref_destroy(this);
      otlp_entity_ref_init(this);
      otlp_entity_ref_copy(this, other.get());
    }
    return *this;
  }

  ~EntityRef() { otlp_entity_ref_destroy(this); }

  void set_schema_url(const char *value) {
    otlp__set_string(&this->schema_url, value, std::strlen(value));
  }

  void set_type(const char *value) {
    otlp__set_string(&this->type, value, std::strlen(value));
  }

  void append_id_key(const char *value) {
    otlp_string copy;
    otlp__zero(&copy, sizeof(copy));
    otlp__set_string(&copy, value, std::strlen(value));
    otlp__append_primitive((void **)&this->id_keys.ptr, &this->id_keys.len,
                           &this->id_keys.cap, sizeof(copy), &copy);
  }

  void append_description_key(const char *value) {
    otlp_string copy;
    otlp__zero(&copy, sizeof(copy));
    otlp__set_string(&copy, value, std::strlen(value));
    otlp__append_primitive((void **)&this->description_keys.ptr,
                           &this->description_keys.len,
                           &this->description_keys.cap, sizeof(copy), &copy);
  }

  const otlp_entity_ref *get() const { return this; }
};

class Resource : private otlp_resource {
public:
  Resource() { otlp_resource_init(this); }

  Resource(const Resource &other) {
    otlp_resource_init(this);
    otlp_resource_copy(this, other.get());
  }

  Resource &operator=(const Resource &other) {
    if (this != &other) {
      otlp_resource_destroy(this);
      otlp_resource_init(this);
      otlp_resource_copy(this, other.get());
    }
    return *this;
  }

  ~Resource() { otlp_resource_destroy(this); }

  void append_attribute(const KeyValue &value) {
    otlp_key_value copy;
    otlp_key_value_init(&copy);
    otlp_key_value_copy(&copy, value.get());
    otlp__append_primitive((void **)&this->attributes.ptr, &this->attributes.len,
                           &this->attributes.cap, sizeof(copy), &copy);
  }

  void append_entity_ref(const EntityRef &value) {
    otlp_entity_ref copy;
    otlp_entity_ref_init(&copy);
    otlp_entity_ref_copy(&copy, value.get());
    otlp__append_primitive((void **)&this->entity_refs.ptr, &this->entity_refs.len,
                           &this->entity_refs.cap, sizeof(copy), &copy);
  }

  void set_dropped_attributes_count(uint32_t value) {
    this->dropped_attributes_count = value;
  }

  const otlp_resource *get() const { return this; }
};

inline void AnyValue::set_array_value(const ArrayValue &value) {
  otlp_any_value_destroy(this);
  otlp_any_value_init(this);
  this->kind = OTLP_ANY_VALUE_ARRAY;
  this->value.array_value =
      static_cast<otlp_array_value *>(std::malloc(sizeof(otlp_array_value)));
  if (this->value.array_value != NULL) {
    otlp_array_value_init(this->value.array_value);
    otlp_array_value_copy(this->value.array_value, value.get());
  }
}

inline void AnyValue::set_kvlist_value(const KeyValueList &value) {
  otlp_any_value_destroy(this);
  otlp_any_value_init(this);
  this->kind = OTLP_ANY_VALUE_KVLIST;
  this->value.kvlist_value =
      static_cast<otlp_key_value_list *>(std::malloc(sizeof(otlp_key_value_list)));
  if (this->value.kvlist_value != NULL) {
    otlp_key_value_list_init(this->value.kvlist_value);
    otlp_key_value_list_copy(this->value.kvlist_value, value.get());
  }
}

} /* namespace otlp */
#endif

#endif /* OTLP_FORMAT_COMMON_HPP */
