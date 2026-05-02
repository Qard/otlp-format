#ifndef OTLP_FORMAT_PROFILES_HPP
#define OTLP_FORMAT_PROFILES_HPP

#include "../common/common.hpp"
#include "profiles.h"

#if defined(__cplusplus) &&                                                     \
    (__cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)) &&     \
    defined(OTLP_FORMAT_ENABLE_PROFILES)
namespace otlp {

class ProfilesValueType : private otlp_profiles_dev_value_type {
public:
  ProfilesValueType() { otlp__zero(this, sizeof(*this)); }

  void set_type_strindex(int32_t value) { this->type_strindex = value; }

  void set_unit_strindex(int32_t value) { this->unit_strindex = value; }

  const otlp_profiles_dev_value_type *get() const { return this; }
};

class ProfilesLink : private otlp_profiles_dev_link {
public:
  ProfilesLink() { otlp_profiles_dev_link_init(this); }

  ProfilesLink(const ProfilesLink &other) {
    otlp_profiles_dev_link_init(this);
    otlp_profiles_dev_link_copy(this, other.get());
  }

  ProfilesLink &operator=(const ProfilesLink &other) {
    if (this != &other) {
      otlp_profiles_dev_link_destroy(this);
      otlp_profiles_dev_link_init(this);
      otlp_profiles_dev_link_copy(this, other.get());
    }
    return *this;
  }

  ~ProfilesLink() { otlp_profiles_dev_link_destroy(this); }

  void set_trace_id(const uint8_t *ptr, size_t len) {
    otlp__set_bytes(&this->trace_id, ptr, len);
  }

  void set_span_id(const uint8_t *ptr, size_t len) {
    otlp__set_bytes(&this->span_id, ptr, len);
  }

  const otlp_profiles_dev_link *get() const { return this; }
};

class ProfilesLine : private otlp_profiles_dev_line {
public:
  ProfilesLine() { otlp_profiles_dev_line_init(this); }

  void set_function_index(int32_t value) { this->function_index = value; }

  void set_line(int64_t value) { this->line = value; }

  void set_column(int64_t value) { this->column = value; }

  const otlp_profiles_dev_line *get() const { return this; }
};

class ProfilesLocation : private otlp_profiles_dev_location {
public:
  ProfilesLocation() { otlp_profiles_dev_location_init(this); }

  ProfilesLocation(const ProfilesLocation &other) {
    otlp_profiles_dev_location_init(this);
    otlp_profiles_dev_location_copy(this, other.get());
  }

  ProfilesLocation &operator=(const ProfilesLocation &other) {
    if (this != &other) {
      otlp_profiles_dev_location_destroy(this);
      otlp_profiles_dev_location_init(this);
      otlp_profiles_dev_location_copy(this, other.get());
    }
    return *this;
  }

  ~ProfilesLocation() { otlp_profiles_dev_location_destroy(this); }

  void set_mapping_index(int32_t value) { this->mapping_index = value; }

  void set_address(uint64_t value) { this->address = value; }

  void append_line(const ProfilesLine &line) {
    otlp_profiles_dev_line copy = *line.get();
    otlp__append_primitive((void **)&this->lines.ptr, &this->lines.len,
                           &this->lines.cap, sizeof(copy), &copy);
  }

  void append_attribute_index(int32_t value) {
    otlp__append_primitive((void **)&this->attribute_indices.ptr,
                           &this->attribute_indices.len,
                           &this->attribute_indices.cap, sizeof(value),
                           &value);
  }

  const otlp_profiles_dev_location *get() const { return this; }
};

class ProfilesMapping : private otlp_profiles_dev_mapping {
public:
  ProfilesMapping() { otlp_profiles_dev_mapping_init(this); }

  ProfilesMapping(const ProfilesMapping &other) {
    otlp_profiles_dev_mapping_init(this);
    otlp_profiles_dev_mapping_copy(this, other.get());
  }

  ProfilesMapping &operator=(const ProfilesMapping &other) {
    if (this != &other) {
      otlp_profiles_dev_mapping_destroy(this);
      otlp_profiles_dev_mapping_init(this);
      otlp_profiles_dev_mapping_copy(this, other.get());
    }
    return *this;
  }

  ~ProfilesMapping() { otlp_profiles_dev_mapping_destroy(this); }

  void set_memory_start(uint64_t value) { this->memory_start = value; }

  void set_memory_limit(uint64_t value) { this->memory_limit = value; }

  void set_file_offset(uint64_t value) { this->file_offset = value; }

  void set_filename_strindex(int32_t value) {
    this->filename_strindex = value;
  }

  void append_attribute_index(int32_t value) {
    otlp__append_primitive((void **)&this->attribute_indices.ptr,
                           &this->attribute_indices.len,
                           &this->attribute_indices.cap, sizeof(value),
                           &value);
  }

  const otlp_profiles_dev_mapping *get() const { return this; }
};

class ProfilesFunction : private otlp_profiles_dev_function {
public:
  ProfilesFunction() { otlp_profiles_dev_function_init(this); }

  void set_name_strindex(int32_t value) { this->name_strindex = value; }

  void set_system_name_strindex(int32_t value) {
    this->system_name_strindex = value;
  }

  void set_filename_strindex(int32_t value) {
    this->filename_strindex = value;
  }

  void set_start_line(int64_t value) { this->start_line = value; }

  const otlp_profiles_dev_function *get() const { return this; }
};

class ProfilesKeyValueAndUnit
    : private otlp_profiles_dev_key_value_and_unit {
public:
  ProfilesKeyValueAndUnit() {
    otlp_profiles_dev_key_value_and_unit_init(this);
  }

  ProfilesKeyValueAndUnit(const ProfilesKeyValueAndUnit &other) {
    otlp_profiles_dev_key_value_and_unit_init(this);
    otlp_profiles_dev_key_value_and_unit_copy(this, other.get());
  }

  ProfilesKeyValueAndUnit &
  operator=(const ProfilesKeyValueAndUnit &other) {
    if (this != &other) {
      otlp_profiles_dev_key_value_and_unit_destroy(this);
      otlp_profiles_dev_key_value_and_unit_init(this);
      otlp_profiles_dev_key_value_and_unit_copy(this, other.get());
    }
    return *this;
  }

  ~ProfilesKeyValueAndUnit() {
    otlp_profiles_dev_key_value_and_unit_destroy(this);
  }

  void set_key_strindex(int32_t value) { this->key_strindex = value; }

  void set_value(const AnyValue &value) {
    otlp_any_value_copy(&this->value, value.get());
  }

  void set_unit_strindex(int32_t value) { this->unit_strindex = value; }

  const otlp_profiles_dev_key_value_and_unit *get() const { return this; }
};

class ProfilesStack : private otlp_profiles_dev_stack {
public:
  ProfilesStack() { otlp_profiles_dev_stack_init(this); }

  ProfilesStack(const ProfilesStack &other) {
    otlp_profiles_dev_stack_init(this);
    otlp_profiles_dev_stack_copy(this, other.get());
  }

  ProfilesStack &operator=(const ProfilesStack &other) {
    if (this != &other) {
      otlp_profiles_dev_stack_destroy(this);
      otlp_profiles_dev_stack_init(this);
      otlp_profiles_dev_stack_copy(this, other.get());
    }
    return *this;
  }

  ~ProfilesStack() { otlp_profiles_dev_stack_destroy(this); }

  void append_location_index(int32_t value) {
    otlp__append_primitive((void **)&this->location_indices.ptr,
                           &this->location_indices.len,
                           &this->location_indices.cap, sizeof(value),
                           &value);
  }

  const otlp_profiles_dev_stack *get() const { return this; }
};

class ProfilesSample : private otlp_profiles_dev_sample {
public:
  ProfilesSample() { otlp_profiles_dev_sample_init(this); }

  ProfilesSample(const ProfilesSample &other) {
    otlp_profiles_dev_sample_init(this);
    otlp_profiles_dev_sample_copy(this, other.get());
  }

  ProfilesSample &operator=(const ProfilesSample &other) {
    if (this != &other) {
      otlp_profiles_dev_sample_destroy(this);
      otlp_profiles_dev_sample_init(this);
      otlp_profiles_dev_sample_copy(this, other.get());
    }
    return *this;
  }

  ~ProfilesSample() { otlp_profiles_dev_sample_destroy(this); }

  void set_stack_index(int32_t value) { this->stack_index = value; }

  void set_link_index(int32_t value) { this->link_index = value; }

  void append_attribute_index(int32_t value) {
    otlp__append_primitive((void **)&this->attribute_indices.ptr,
                           &this->attribute_indices.len,
                           &this->attribute_indices.cap, sizeof(value),
                           &value);
  }

  void append_value(int64_t value) {
    otlp_profiles_dev_sample_append_value(this, value);
  }

  void append_timestamp_unix_nano(uint64_t value) {
    otlp__append_primitive((void **)&this->timestamps_unix_nano.ptr,
                           &this->timestamps_unix_nano.len,
                           &this->timestamps_unix_nano.cap, sizeof(value),
                           &value);
  }

  const otlp_profiles_dev_sample *get() const { return this; }
};

class ProfilesProfile : private otlp_profiles_dev_profile {
public:
  ProfilesProfile() { otlp_profiles_dev_profile_init(this); }

  ProfilesProfile(const ProfilesProfile &other) {
    otlp_profiles_dev_profile_init(this);
    otlp_profiles_dev_profile_copy(this, other.get());
  }

  ProfilesProfile &operator=(const ProfilesProfile &other) {
    if (this != &other) {
      otlp_profiles_dev_profile_destroy(this);
      otlp_profiles_dev_profile_init(this);
      otlp_profiles_dev_profile_copy(this, other.get());
    }
    return *this;
  }

  ~ProfilesProfile() { otlp_profiles_dev_profile_destroy(this); }

  void set_sample_type(int32_t type_strindex, int32_t unit_strindex) {
    this->has_sample_type = 1;
    this->sample_type.type_strindex = type_strindex;
    this->sample_type.unit_strindex = unit_strindex;
  }

  void set_sample_type(const ProfilesValueType &value) {
    this->has_sample_type = 1;
    this->sample_type = *value.get();
  }

  void set_time_unix_nano(uint64_t value) { this->time_unix_nano = value; }

  void set_duration_nano(uint64_t value) { this->duration_nano = value; }

  void set_period_type(int32_t type_strindex, int32_t unit_strindex) {
    this->period_type.type_strindex = type_strindex;
    this->period_type.unit_strindex = unit_strindex;
  }

  void set_period_type(const ProfilesValueType &value) {
    this->period_type = *value.get();
  }

  void set_period(int64_t value) { this->period = value; }

  void set_profile_id(const uint8_t *ptr, size_t len) {
    otlp__set_bytes(&this->profile_id, ptr, len);
  }

  void set_dropped_attributes_count(uint32_t value) {
    this->dropped_attributes_count = value;
  }

  void set_original_payload_format(const char *value) {
    otlp__set_string(&this->original_payload_format, value,
                     std::strlen(value));
  }

  void set_original_payload(const uint8_t *ptr, size_t len) {
    otlp__set_bytes(&this->original_payload, ptr, len);
  }

  void append_attribute_index(int32_t value) {
    otlp__append_primitive((void **)&this->attribute_indices.ptr,
                           &this->attribute_indices.len,
                           &this->attribute_indices.cap, sizeof(value),
                           &value);
  }

  void append_sample(const ProfilesSample &sample) {
    otlp_profiles_dev_profile_append_sample(this, sample.get());
  }

  const otlp_profiles_dev_profile *get() const { return this; }
};

class ProfilesScopeProfiles : private otlp_profiles_dev_scope_profiles {
public:
  ProfilesScopeProfiles() { otlp_profiles_dev_scope_profiles_init(this); }

  ProfilesScopeProfiles(const ProfilesScopeProfiles &other) {
    otlp_profiles_dev_scope_profiles_init(this);
    otlp_profiles_dev_scope_profiles_copy(this, other.get());
  }

  ProfilesScopeProfiles &operator=(const ProfilesScopeProfiles &other) {
    if (this != &other) {
      otlp_profiles_dev_scope_profiles_destroy(this);
      otlp_profiles_dev_scope_profiles_init(this);
      otlp_profiles_dev_scope_profiles_copy(this, other.get());
    }
    return *this;
  }

  ~ProfilesScopeProfiles() { otlp_profiles_dev_scope_profiles_destroy(this); }

  void append_profile(const ProfilesProfile &profile) {
    otlp_profiles_dev_scope_profiles_append_profile(this, profile.get());
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

  const otlp_profiles_dev_scope_profiles *get() const { return this; }
};

class ProfilesResourceProfiles : private otlp_profiles_dev_resource_profiles {
public:
  ProfilesResourceProfiles() { otlp_profiles_dev_resource_profiles_init(this); }

  ProfilesResourceProfiles(const ProfilesResourceProfiles &other) {
    otlp_profiles_dev_resource_profiles_init(this);
    otlp_profiles_dev_resource_profiles_copy(this, other.get());
  }

  ProfilesResourceProfiles &operator=(const ProfilesResourceProfiles &other) {
    if (this != &other) {
      otlp_profiles_dev_resource_profiles_destroy(this);
      otlp_profiles_dev_resource_profiles_init(this);
      otlp_profiles_dev_resource_profiles_copy(this, other.get());
    }
    return *this;
  }

  ~ProfilesResourceProfiles() {
    otlp_profiles_dev_resource_profiles_destroy(this);
  }

  void append_scope_profiles(const ProfilesScopeProfiles &scope_profiles) {
    otlp_profiles_dev_resource_profiles_append_scope_profiles(
        this, scope_profiles.get());
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

  const otlp_profiles_dev_resource_profiles *get() const { return this; }
};

class ProfilesDictionary : private otlp_profiles_dev_profiles_dictionary {
public:
  ProfilesDictionary() { otlp_profiles_dev_profiles_dictionary_init(this); }

  ProfilesDictionary(const ProfilesDictionary &other) = delete;
  ProfilesDictionary &operator=(const ProfilesDictionary &other) = delete;

  ~ProfilesDictionary() { otlp_profiles_dev_profiles_dictionary_destroy(this); }

  void init_default_zero_entries() {
    otlp_profiles_dev_profiles_dictionary_init_default_zero_entries(this);
  }

  otlp_status validate_zero_entries() const {
    return otlp_profiles_dev_profiles_dictionary_validate_zero_entries(this);
  }

  void append_mapping(const otlp_profiles_dev_mapping &value) {
    otlp_profiles_dev_mapping copy;
    otlp_profiles_dev_mapping_init(&copy);
    otlp_profiles_dev_mapping_copy(&copy, &value);
    otlp__append_primitive((void **)&this->mapping_table.ptr,
                           &this->mapping_table.len,
                           &this->mapping_table.cap, sizeof(copy), &copy);
  }

  void append_mapping(const ProfilesMapping &value) {
    append_mapping(*value.get());
  }

  void append_location(const otlp_profiles_dev_location &value) {
    otlp_profiles_dev_location copy;
    otlp_profiles_dev_location_init(&copy);
    otlp_profiles_dev_location_copy(&copy, &value);
    otlp__append_primitive((void **)&this->location_table.ptr,
                           &this->location_table.len,
                           &this->location_table.cap, sizeof(copy), &copy);
  }

  void append_location(const ProfilesLocation &value) {
    append_location(*value.get());
  }

  void append_function(const otlp_profiles_dev_function &value) {
    otlp_profiles_dev_function copy;
    otlp_profiles_dev_function_init(&copy);
    otlp_profiles_dev_function_copy(&copy, &value);
    otlp__append_primitive((void **)&this->function_table.ptr,
                           &this->function_table.len,
                           &this->function_table.cap, sizeof(copy), &copy);
  }

  void append_function(const ProfilesFunction &value) {
    append_function(*value.get());
  }

  void append_link(const otlp_profiles_dev_link &value) {
    otlp_profiles_dev_link copy;
    otlp_profiles_dev_link_init(&copy);
    otlp_profiles_dev_link_copy(&copy, &value);
    otlp__append_primitive((void **)&this->link_table.ptr,
                           &this->link_table.len, &this->link_table.cap,
                           sizeof(copy), &copy);
  }

  void append_link(const ProfilesLink &value) { append_link(*value.get()); }

  void append_string(const char *value) {
    otlp_string copy;
    otlp__zero(&copy, sizeof(copy));
    otlp__set_string(&copy, value, std::strlen(value));
    otlp__append_primitive((void **)&this->string_table.ptr,
                           &this->string_table.len, &this->string_table.cap,
                           sizeof(copy), &copy);
  }

  void append_attribute(const otlp_profiles_dev_key_value_and_unit &value) {
    otlp_profiles_dev_key_value_and_unit copy;
    otlp_profiles_dev_key_value_and_unit_init(&copy);
    otlp_profiles_dev_key_value_and_unit_copy(&copy, &value);
    otlp__append_primitive((void **)&this->attribute_table.ptr,
                           &this->attribute_table.len,
                           &this->attribute_table.cap, sizeof(copy), &copy);
  }

  void append_attribute(const ProfilesKeyValueAndUnit &value) {
    append_attribute(*value.get());
  }

  void append_stack(const otlp_profiles_dev_stack &value) {
    otlp_profiles_dev_stack copy;
    otlp_profiles_dev_stack_init(&copy);
    otlp_profiles_dev_stack_copy(&copy, &value);
    otlp__append_primitive((void **)&this->stack_table.ptr,
                           &this->stack_table.len, &this->stack_table.cap,
                           sizeof(copy), &copy);
  }

  void append_stack(const ProfilesStack &value) { append_stack(*value.get()); }

  const otlp_profiles_dev_profiles_dictionary *get() const { return this; }
};

class ProfilesExportRequest
    : private otlp_profiles_dev_export_profiles_service_request {
public:
  ProfilesExportRequest() {
    otlp_profiles_dev_export_profiles_service_request_init(this);
    otlp_profiles_dev_profiles_dictionary_init_default_zero_entries(
        &this->otlp_profiles_dev_export_profiles_service_request::dictionary);
  }

  ProfilesExportRequest(const ProfilesExportRequest &other) {
    otlp_profiles_dev_export_profiles_service_request_init(this);
    size_t size =
        otlp_profiles_dev_export_profiles_service_request_encoded_size(
            other.get());
    uint8_t *buffer = size ? new uint8_t[size] : NULL;
    if (size && buffer != NULL &&
        otlp_profiles_dev_export_profiles_service_request_encode(
            other.get(), buffer, size) == OTLP_STATUS_OK) {
      otlp_profiles_dev_export_profiles_service_request_decode(this, buffer,
                                                               size);
    }
    delete[] buffer;
  }

  ProfilesExportRequest &operator=(const ProfilesExportRequest &other) {
    if (this != &other) {
      otlp_profiles_dev_export_profiles_service_request_destroy(this);
      otlp_profiles_dev_export_profiles_service_request_init(this);
      otlp_profiles_dev_profiles_dictionary_init_default_zero_entries(
          &this->otlp_profiles_dev_export_profiles_service_request::dictionary);
      size_t size =
          otlp_profiles_dev_export_profiles_service_request_encoded_size(
              other.get());
      uint8_t *buffer = size ? new uint8_t[size] : NULL;
      if (size && buffer != NULL &&
          otlp_profiles_dev_export_profiles_service_request_encode(
              other.get(), buffer, size) == OTLP_STATUS_OK) {
        otlp_profiles_dev_export_profiles_service_request_decode(this, buffer,
                                                                 size);
      }
      delete[] buffer;
    }
    return *this;
  }

  ~ProfilesExportRequest() {
    otlp_profiles_dev_export_profiles_service_request_destroy(this);
  }

  ProfilesDictionary &dictionary() {
    return *reinterpret_cast<ProfilesDictionary *>(
        &this->otlp_profiles_dev_export_profiles_service_request::dictionary);
  }

  void
  append_resource_profiles(const ProfilesResourceProfiles &resource_profiles) {
    otlp_profiles_dev_export_profiles_service_request_append_resource_profiles(
        this, resource_profiles.get());
  }

  size_t encoded_size() const {
    return otlp_profiles_dev_export_profiles_service_request_encoded_size(this);
  }

  otlp_status encode(uint8_t *out, size_t len) const {
    return otlp_profiles_dev_export_profiles_service_request_encode(this, out,
                                                                    len);
  }

  otlp_status decode(const uint8_t *data, size_t len) {
    return otlp_profiles_dev_export_profiles_service_request_decode(this, data,
                                                                    len);
  }

  const otlp_profiles_dev_export_profiles_service_request *get() const {
    return this;
  }
};

class ProfilesData : private otlp_profiles_dev_profiles_data {
public:
  ProfilesData() {
    otlp_profiles_dev_profiles_data_init(this);
    otlp_profiles_dev_profiles_dictionary_init_default_zero_entries(
        &this->otlp_profiles_dev_profiles_data::dictionary);
  }

  ProfilesData(const ProfilesData &other) {
    otlp_profiles_dev_profiles_data_init(this);
    otlp_profiles_dev_profiles_dictionary_init_default_zero_entries(
        &this->otlp_profiles_dev_profiles_data::dictionary);
    size_t size = otlp_profiles_dev_profiles_data_encoded_size(other.get());
    uint8_t *buffer = size ? new uint8_t[size] : NULL;
    if (size && buffer != NULL &&
        otlp_profiles_dev_profiles_data_encode(other.get(), buffer, size) ==
            OTLP_STATUS_OK) {
      otlp_profiles_dev_profiles_data_decode(this, buffer, size);
    }
    delete[] buffer;
  }

  ProfilesData &operator=(const ProfilesData &other) {
    if (this != &other) {
      otlp_profiles_dev_profiles_data_destroy(this);
      otlp_profiles_dev_profiles_data_init(this);
      otlp_profiles_dev_profiles_dictionary_init_default_zero_entries(
          &this->otlp_profiles_dev_profiles_data::dictionary);
      size_t size = otlp_profiles_dev_profiles_data_encoded_size(other.get());
      uint8_t *buffer = size ? new uint8_t[size] : NULL;
      if (size && buffer != NULL &&
          otlp_profiles_dev_profiles_data_encode(other.get(), buffer, size) ==
              OTLP_STATUS_OK) {
        otlp_profiles_dev_profiles_data_decode(this, buffer, size);
      }
      delete[] buffer;
    }
    return *this;
  }

  ~ProfilesData() { otlp_profiles_dev_profiles_data_destroy(this); }

  ProfilesDictionary &dictionary() {
    return *reinterpret_cast<ProfilesDictionary *>(
        &this->otlp_profiles_dev_profiles_data::dictionary);
  }

  void append_resource_profiles(const ProfilesResourceProfiles &resource_profiles) {
    otlp_profiles_dev_resource_profiles copy;
    otlp_profiles_dev_resource_profiles_init(&copy);
    otlp_profiles_dev_resource_profiles_copy(&copy, resource_profiles.get());
    otlp__append_primitive((void **)&this->resource_profiles.ptr,
                           &this->resource_profiles.len,
                           &this->resource_profiles.cap, sizeof(copy), &copy);
  }

  size_t encoded_size() const {
    return otlp_profiles_dev_profiles_data_encoded_size(this);
  }

  otlp_status encode(uint8_t *out, size_t len) const {
    return otlp_profiles_dev_profiles_data_encode(this, out, len);
  }

  otlp_status decode(const uint8_t *data, size_t len) {
    return otlp_profiles_dev_profiles_data_decode(this, data, len);
  }

  const otlp_profiles_dev_profiles_data *get() const { return this; }
};

class ProfilesExportResponse
    : private otlp_profiles_dev_export_profiles_service_response {
public:
  ProfilesExportResponse() {
    otlp_profiles_dev_export_profiles_service_response_init(this);
  }

  ProfilesExportResponse(const ProfilesExportResponse &other) {
    otlp_profiles_dev_export_profiles_service_response_init(this);
    if (other.get()->has_partial_success) {
      this->has_partial_success = 1;
      this->partial_success.rejected_profiles =
          other.get()->partial_success.rejected_profiles;
      otlp__set_string(&this->partial_success.error_message,
                       other.get()->partial_success.error_message.ptr,
                       other.get()->partial_success.error_message.len);
    }
  }

  ProfilesExportResponse &operator=(const ProfilesExportResponse &other) {
    if (this != &other) {
      otlp_profiles_dev_export_profiles_service_response_destroy(this);
      otlp_profiles_dev_export_profiles_service_response_init(this);
      if (other.get()->has_partial_success) {
        this->has_partial_success = 1;
        this->partial_success.rejected_profiles =
            other.get()->partial_success.rejected_profiles;
        otlp__set_string(&this->partial_success.error_message,
                         other.get()->partial_success.error_message.ptr,
                         other.get()->partial_success.error_message.len);
      }
    }
    return *this;
  }

  ~ProfilesExportResponse() {
    otlp_profiles_dev_export_profiles_service_response_destroy(this);
  }

  void set_partial_success(int64_t rejected_profiles,
                           const char *error_message) {
    this->has_partial_success = 1;
    this->partial_success.rejected_profiles = rejected_profiles;
    otlp__set_string(&this->partial_success.error_message, error_message,
                     std::strlen(error_message));
  }

  size_t encoded_size() const {
    return otlp_profiles_dev_export_profiles_service_response_encoded_size(this);
  }

  otlp_status encode(uint8_t *out, size_t len) const {
    return otlp_profiles_dev_export_profiles_service_response_encode(this, out,
                                                                     len);
  }

  otlp_status decode(const uint8_t *data, size_t len) {
    return otlp_profiles_dev_export_profiles_service_response_decode(this, data,
                                                                     len);
  }

  const otlp_profiles_dev_export_profiles_service_response *get() const {
    return this;
  }
};

} /* namespace otlp */
#endif

#endif /* OTLP_FORMAT_PROFILES_HPP */
