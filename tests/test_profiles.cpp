#include "test_helpers.hpp"

#if defined(__cplusplus) && (__cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900))

#if defined(OTLP_FORMAT_ENABLE_PROFILES)
#include <vector>

namespace {

static void test_cpp_profiles_wrappers(Tap &t) {
  const std::uint8_t common_bytes[] = {1, 2, 3};

  otlp::AnyValue nested = otlp::AnyValue::string("nested");
  otlp::KeyValue kv;
  kv.set_key("key");
  kv.set_value(nested);

  otlp::InstrumentationScope common_scope;
  common_scope.set_name("scope");
  common_scope.set_version("1.0");
  common_scope.append_attribute(kv);

  otlp::EntityRef entity_ref;
  entity_ref.set_schema_url("entity-schema");
  entity_ref.set_type("service.instance");
  entity_ref.append_id_key("service.instance.id");
  entity_ref.append_description_key("entity");

  otlp::Resource common_resource;
  common_resource.append_attribute(kv);
  common_resource.append_entity_ref(entity_ref);

  otlp::ProfilesExportRequest default_profiles_request;
  std::vector<std::uint8_t> encoded_default_profiles(
      default_profiles_request.encoded_size());
  t.not_ok(encoded_default_profiles.empty(),
           "default profiles request buffer allocated");
  check_equal(t,
              default_profiles_request.encode(encoded_default_profiles.data(),
                                              encoded_default_profiles.size()),
              OTLP_STATUS_OK, "default profiles request encode");

  otlp::ProfilesExportRequest profiles_request;

  otlp::ProfilesValueType sample_type;
  sample_type.set_type_strindex(1);
  sample_type.set_unit_strindex(2);

  otlp::ProfilesMapping mapping;
  mapping.set_memory_start(100);
  mapping.set_memory_limit(200);
  mapping.set_file_offset(8);
  mapping.set_filename_strindex(3);
  mapping.append_attribute_index(1);
  profiles_request.dictionary().append_mapping(mapping);

  otlp::ProfilesFunction function_value;
  function_value.set_name_strindex(4);
  function_value.set_system_name_strindex(5);
  function_value.set_filename_strindex(3);
  function_value.set_start_line(10);
  profiles_request.dictionary().append_function(function_value);

  otlp::ProfilesLine line;
  line.set_function_index(1);
  line.set_line(10);
  line.set_column(2);

  otlp::ProfilesLocation location;
  location.set_mapping_index(1);
  location.set_address(0x1000);
  location.append_line(line);
  location.append_attribute_index(1);
  profiles_request.dictionary().append_location(location);

  otlp::ProfilesLink profile_link;
  profile_link.set_trace_id(common_bytes, sizeof(common_bytes));
  profile_link.set_span_id(common_bytes, sizeof(common_bytes));
  profiles_request.dictionary().append_link(profile_link);

  profiles_request.dictionary().append_string("cpu");
  profiles_request.dictionary().append_string("ns");
  profiles_request.dictionary().append_string("file.cc");
  profiles_request.dictionary().append_string("function");
  profiles_request.dictionary().append_string("system_function");

  otlp::ProfilesKeyValueAndUnit profile_attribute;
  profile_attribute.set_key_strindex(1);
  profile_attribute.set_value(otlp::AnyValue::string("prod"));
  profile_attribute.set_unit_strindex(2);
  profiles_request.dictionary().append_attribute(profile_attribute);

  otlp::ProfilesStack stack;
  stack.append_location_index(1);
  profiles_request.dictionary().append_stack(stack);

  otlp::ProfilesSample sample;
  sample.set_stack_index(1);
  sample.set_link_index(1);
  sample.append_attribute_index(1);
  sample.append_value(7);
  sample.append_timestamp_unix_nano(123);

  otlp::ProfilesProfile profile;
  profile.set_sample_type(sample_type);
  profile.set_time_unix_nano(1000);
  profile.set_duration_nano(2000);
  profile.set_period_type(sample_type);
  profile.set_period(99);
  profile.set_profile_id(common_bytes, sizeof(common_bytes));
  profile.set_dropped_attributes_count(1);
  profile.set_original_payload_format("pprof");
  profile.set_original_payload(common_bytes, sizeof(common_bytes));
  profile.append_attribute_index(1);
  profile.append_sample(sample);

  otlp::ProfilesScopeProfiles scope_profiles;
  scope_profiles.set_scope(common_scope);
  scope_profiles.set_schema_url("profiles-scope-schema");
  scope_profiles.append_profile(profile);

  otlp::ProfilesResourceProfiles resource_profiles;
  resource_profiles.set_resource(common_resource);
  resource_profiles.set_schema_url("profiles-resource-schema");
  resource_profiles.append_scope_profiles(scope_profiles);
  profiles_request.append_resource_profiles(resource_profiles);

  std::vector<std::uint8_t> encoded_profiles(profiles_request.encoded_size());
  t.not_ok(encoded_profiles.empty(), "profiles request buffer allocated");
  check_equal(t,
              profiles_request.encode(encoded_profiles.data(),
                                      encoded_profiles.size()),
              OTLP_STATUS_OK, "profiles request encode");

  otlp::ProfilesExportRequest decoded_profiles;
  check_equal(t,
              decoded_profiles.decode(encoded_profiles.data(),
                                      encoded_profiles.size()),
              OTLP_STATUS_OK, "profiles request decode");
  check_equal(t, decoded_profiles.get()->resource_profiles.len,
              static_cast<size_t>(1), "decoded profiles resource count");
  check_equal(t, decoded_profiles.get()->dictionary.mapping_table.len,
              static_cast<size_t>(2), "decoded mapping table size");
  check_equal(t, decoded_profiles.get()->dictionary.location_table.len,
              static_cast<size_t>(2), "decoded location table size");
  check_equal(t, decoded_profiles.get()->dictionary.function_table.len,
              static_cast<size_t>(2), "decoded function table size");
  check_equal(t, decoded_profiles.get()->dictionary.link_table.len,
              static_cast<size_t>(2), "decoded link table size");
  check_equal(t, decoded_profiles.get()->dictionary.string_table.len,
              static_cast<size_t>(6), "decoded string table size");
  check_equal(t, decoded_profiles.get()->dictionary.attribute_table.len,
              static_cast<size_t>(2), "decoded attribute table size");
  check_equal(t, decoded_profiles.get()->dictionary.stack_table.len,
              static_cast<size_t>(2), "decoded stack table size");
  check_equal(t, decoded_profiles.get()->resource_profiles.ptr[0].schema_url.len,
              static_cast<size_t>(24),
              "decoded profiles resource schema length");
  check_equal(
      t,
      decoded_profiles.get()->resource_profiles.ptr[0]
          .scope_profiles.ptr[0]
          .schema_url.len,
      static_cast<size_t>(21), "decoded profiles scope schema length");
  check_equal(
      t,
      decoded_profiles.get()->resource_profiles.ptr[0]
          .scope_profiles.ptr[0]
          .profiles.ptr[0]
          .profile_id.len,
      sizeof(common_bytes), "decoded profile id length");
  check_equal(
      t,
      decoded_profiles.get()->resource_profiles.ptr[0]
          .scope_profiles.ptr[0]
          .profiles.ptr[0]
          .samples.ptr[0]
          .timestamps_unix_nano.len,
      static_cast<size_t>(1), "decoded sample timestamp count");
}

static void test_cpp_profiles_copy_independence(Tap &t) {
  const std::uint8_t common_bytes[] = {1, 2, 3};

  otlp::AnyValue nested = otlp::AnyValue::string("nested");
  otlp::KeyValue kv;
  kv.set_key("key");
  kv.set_value(nested);

  otlp::InstrumentationScope common_scope;
  common_scope.set_name("scope");
  common_scope.set_version("1.0");
  common_scope.append_attribute(kv);

  otlp::EntityRef entity_ref;
  entity_ref.set_schema_url("entity-schema");
  entity_ref.set_type("service.instance");
  entity_ref.append_id_key("service.instance.id");
  entity_ref.append_description_key("entity");

  otlp::Resource common_resource;
  common_resource.append_attribute(kv);
  common_resource.append_entity_ref(entity_ref);

  otlp::ProfilesExportRequest profiles_request;

  otlp::ProfilesValueType sample_type;
  sample_type.set_type_strindex(1);
  sample_type.set_unit_strindex(2);

  profiles_request.dictionary().append_string("cpu");
  profiles_request.dictionary().append_string("ns");

  otlp::ProfilesSample sample;
  sample.set_stack_index(1);
  sample.append_value(7);
  sample.append_timestamp_unix_nano(123);

  otlp::ProfilesProfile profile;
  profile.set_sample_type(sample_type);
  profile.set_profile_id(common_bytes, sizeof(common_bytes));
  profile.append_sample(sample);

  otlp::ProfilesScopeProfiles scope_profiles;
  scope_profiles.set_scope(common_scope);
  scope_profiles.set_schema_url("profiles-scope-schema");
  scope_profiles.append_profile(profile);

  otlp::ProfilesResourceProfiles resource_profiles;
  resource_profiles.set_resource(common_resource);
  resource_profiles.set_schema_url("profiles-resource-schema");
  resource_profiles.append_scope_profiles(scope_profiles);

  profiles_request.append_resource_profiles(resource_profiles);

  otlp::ProfilesExportRequest copied_profiles = profiles_request;
  profiles_request.append_resource_profiles(resource_profiles);

  check_equal(t, copied_profiles.get()->resource_profiles.len,
              static_cast<size_t>(1),
              "copied profiles request resource count");
  check_equal(t, profiles_request.get()->resource_profiles.len,
              static_cast<size_t>(2),
              "mutated original profiles request resource count");
}

static void test_cpp_profiles_data_and_response(Tap &t) {
  otlp::AnyValue nested = otlp::AnyValue::string("nested");
  otlp::KeyValue kv;
  kv.set_key("key");
  kv.set_value(nested);

  otlp::InstrumentationScope common_scope;
  common_scope.set_name("scope");
  common_scope.set_version("1.0");
  common_scope.append_attribute(kv);

  otlp::EntityRef entity_ref;
  entity_ref.set_schema_url("entity-schema");
  entity_ref.set_type("service.instance");
  entity_ref.append_id_key("service.instance.id");
  entity_ref.append_description_key("entity");

  otlp::Resource common_resource;
  common_resource.append_attribute(kv);
  common_resource.append_entity_ref(entity_ref);

  otlp::ProfilesResourceProfiles resource_profiles;
  resource_profiles.set_resource(common_resource);
  resource_profiles.set_schema_url("profiles-resource-schema");

  otlp::ProfilesData profiles_data;
  profiles_data.append_resource_profiles(resource_profiles);
  std::vector<std::uint8_t> encoded_profiles_data(
      profiles_data.encoded_size());
  check_equal(t,
              profiles_data.encode(encoded_profiles_data.data(),
                                   encoded_profiles_data.size()),
              OTLP_STATUS_OK, "profiles data encode");

  otlp::ProfilesData decoded_profiles_data;
  check_equal(t,
              decoded_profiles_data.decode(encoded_profiles_data.data(),
                                           encoded_profiles_data.size()),
              OTLP_STATUS_OK, "profiles data decode");

  otlp::ProfilesExportResponse profiles_response;
  profiles_response.set_partial_success(3, "profile-partial");
  std::vector<std::uint8_t> encoded_profiles_response(
      profiles_response.encoded_size());
  check_equal(t,
              profiles_response.encode(encoded_profiles_response.data(),
                                       encoded_profiles_response.size()),
              OTLP_STATUS_OK, "profiles response encode");

  otlp::ProfilesExportResponse decoded_profiles_response;
  check_equal(t,
              decoded_profiles_response.decode(encoded_profiles_response.data(),
                                               encoded_profiles_response.size()),
              OTLP_STATUS_OK, "profiles response decode");

  check_equal(t, decoded_profiles_data.get()->resource_profiles.len,
              static_cast<size_t>(1),
              "decoded profiles data resource count");
  check_equal(t, decoded_profiles_response.get()->has_partial_success, 1,
              "decoded profiles response partial success");
}

} // namespace

#endif /* OTLP_FORMAT_ENABLE_PROFILES */

#endif /* __cplusplus */

void test_profiles_cpp(Tap &t) {
#if defined(__cplusplus) && (__cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900))
#if defined(OTLP_FORMAT_ENABLE_PROFILES)
  t.test("cpp profiles wrappers", [](Tap &sub) {
    test_cpp_profiles_wrappers(sub);
  });
  t.test("cpp profiles copy independence", [](Tap &sub) {
    test_cpp_profiles_copy_independence(sub);
  });
  t.test("cpp profiles data and response", [](Tap &sub) {
    test_cpp_profiles_data_and_response(sub);
  });
#else
  (void)t;
#endif
#else
  (void)t;
#endif
}

int test_profiles_cpp_count(void) {
#if defined(OTLP_FORMAT_ENABLE_PROFILES)
  return 3;
#else
  return 0;
#endif
}
