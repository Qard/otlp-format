#include "test_helpers.h"

#if defined(OTLP_FORMAT_ENABLE_PROFILES)

static void test_profiles_proto_wire_compatibility(void) {
  otlp_profiles_dev_profiles_dictionary dictionary;
  otlp_profiles_dev_export_profiles_service_request invalid_request;
  otlp_profiles_dev_profiles_data invalid_data;
  otlp_profiles_dev_export_profiles_service_request decoded_invalid_request;
  otlp_profiles_dev_profiles_data decoded_invalid_data;
  otlp_profiles_dev_value_type value_type;
  otlp_profiles_dev_line line;
  otlp_profiles_dev_sample sample;
  otlp_profiles_dev_profile profile;
  otlp_profiles_dev_export_profiles_partial_success partial_success;
  otlp_profiles_dev_export_profiles_service_response response;
  otlp__writer writer;
  uint8_t encoded_profile[64];
  uint8_t encoded_response[64];
  uint8_t encoded_dictionary[64];
  const uint8_t line_bytes[] = {
    0x10, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01,
    0x18, 0x2a
  };
  const uint8_t sample_packed_bytes[] = {
    0x12, 0x02, 0x01, 0x02,
    0x22, 0x02, 0x01, 0x02,
    0x2a, 0x10,
    0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  const uint8_t period_type_field[] = { 0x2a, 0x04, 0x08, 0x01, 0x10, 0x02 };
  const uint8_t rejected_minus_one[] = {
    0x08, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01
  };
  const uint8_t empty_attribute_table_entry[] = { 0x32, 0x00 };
  const uint8_t nonzero_attribute_table_entry[] = { 0x32, 0x02, 0x12, 0x00 };
  const uint8_t value_type_minus_one[] = {
    0x08, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01
  };
  const uint8_t invalid_empty_request_dictionary[] = { 0x12, 0x00 };
  const uint8_t invalid_empty_data_dictionary[] = { 0x12, 0x00 };
  const uint8_t truncated_profile_sample[] = { 0x12, 0x02, 0x08 };
  const uint8_t negative_profile_period[] = {
    0x30, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01
  };
  const uint8_t negative_sample_values_packed[] = {
    0x22, 0x14,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01,
    0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01
  };
  size_t dictionary_size;
  size_t value_type_size;
  size_t profile_size;
  size_t response_size;

  otlp_profiles_dev_profiles_dictionary_init(&dictionary);
  CHECK(otlp_profiles_dev_profiles_dictionary_validate_zero_entries(&dictionary) == OTLP_STATUS_INVALID_ARGUMENT);
  CHECK(otlp_profiles_dev_profiles_dictionary_init_default_zero_entries(&dictionary) == OTLP_STATUS_OK);
  CHECK(otlp_profiles_dev_profiles_dictionary_validate_zero_entries(&dictionary) == OTLP_STATUS_OK);
  dictionary_size = otlp_profiles_dev_profiles_dictionary_encoded_size_inner(&dictionary);
  otlp__writer_init(&writer, encoded_dictionary, sizeof(encoded_dictionary));
  otlp_profiles_dev_profiles_dictionary_encode_field(&writer, 1, &dictionary);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, dictionary_size));
  CHECK(otlp_test_contains_bytes(encoded_dictionary, writer.off, empty_attribute_table_entry, sizeof(empty_attribute_table_entry)));
  CHECK(!otlp_test_contains_bytes(encoded_dictionary, writer.off, nonzero_attribute_table_entry, sizeof(nonzero_attribute_table_entry)));
  otlp_profiles_dev_profiles_dictionary_destroy(&dictionary);

  otlp_profiles_dev_export_profiles_service_request_init(&invalid_request);
  CHECK(otlp_profiles_dev_export_profiles_service_request_encode(
             &invalid_request, encoded_dictionary, sizeof(encoded_dictionary)) ==
         OTLP_STATUS_INVALID_ARGUMENT);
  otlp_profiles_dev_export_profiles_service_request_destroy(&invalid_request);

  otlp_profiles_dev_profiles_data_init(&invalid_data);
  CHECK(otlp_profiles_dev_profiles_data_encode(
             &invalid_data, encoded_dictionary, sizeof(encoded_dictionary)) ==
         OTLP_STATUS_INVALID_ARGUMENT);
  otlp_profiles_dev_profiles_data_destroy(&invalid_data);

  otlp_profiles_dev_export_profiles_service_request_init(&decoded_invalid_request);
  CHECK(otlp_profiles_dev_export_profiles_service_request_decode(
             &decoded_invalid_request, invalid_empty_request_dictionary,
             sizeof(invalid_empty_request_dictionary)) ==
         OTLP_STATUS_DECODE_ERROR);
  otlp_profiles_dev_export_profiles_service_request_destroy(&decoded_invalid_request);

  otlp_profiles_dev_profiles_data_init(&decoded_invalid_data);
  CHECK(otlp_profiles_dev_profiles_data_decode(
             &decoded_invalid_data, invalid_empty_data_dictionary,
             sizeof(invalid_empty_data_dictionary)) ==
         OTLP_STATUS_DECODE_ERROR);
  otlp_profiles_dev_profiles_data_destroy(&decoded_invalid_data);

  otlp__zero(&value_type, sizeof(value_type));
  value_type.type_strindex = -1;
  value_type_size = otlp_profiles_dev_value_type_encoded_size_inner(&value_type);
  otlp__writer_init(&writer, encoded_profile, sizeof(encoded_profile));
  otlp_profiles_dev_value_type_encode_field(&writer, 1, &value_type);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, value_type_size));
  CHECK(otlp_test_contains_bytes(encoded_profile, writer.off,
                                  value_type_minus_one,
                                  sizeof(value_type_minus_one)));

  otlp_profiles_dev_line_init(&line);
  CHECK(otlp_profiles_dev_line_decode_inner(&line, line_bytes, sizeof(line_bytes)) == OTLP_STATUS_OK);
  CHECK(line.line == -1);
  CHECK(line.column == 42);

  otlp_profiles_dev_sample_init(&sample);
  CHECK(otlp_profiles_dev_sample_decode_inner(&sample, sample_packed_bytes, sizeof(sample_packed_bytes)) == OTLP_STATUS_OK);
  CHECK(sample.attribute_indices.len == 2);
  CHECK(sample.attribute_indices.ptr[0] == 1);
  CHECK(sample.values.len == 2);
  CHECK(sample.values.ptr[1] == 2);
  CHECK(sample.timestamps_unix_nano.len == 2);
  CHECK(sample.timestamps_unix_nano.ptr[1] == 4);
  otlp_profiles_dev_sample_destroy(&sample);

  otlp_profiles_dev_profile_init(&profile);
  profile.has_sample_type = 1;
  profile.period_type.type_strindex = 1;
  profile.period_type.unit_strindex = 2;
  profile.period = -1;
  profile_size = otlp_profiles_dev_profile_encoded_size_inner(&profile);
  otlp__writer_init(&writer, encoded_profile, sizeof(encoded_profile));
  otlp_profiles_dev_profile_encode_field(&writer, 1, &profile);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, profile_size));
  CHECK(otlp_test_contains_bytes(encoded_profile, writer.off, period_type_field, sizeof(period_type_field)));
  otlp_profiles_dev_profile_destroy(&profile);

  otlp_profiles_dev_profile_init(&profile);
  CHECK(otlp_profiles_dev_profile_encoded_size_inner(&profile) == 0);
  CHECK(otlp_profiles_dev_profile_decode_inner(
             &profile, truncated_profile_sample,
             sizeof(truncated_profile_sample)) != OTLP_STATUS_OK);
  otlp_profiles_dev_profile_destroy(&profile);

  otlp_profiles_dev_profile_init(&profile);
  CHECK(otlp_profiles_dev_profile_decode_inner(&profile, encoded_profile + 2, profile_size) == OTLP_STATUS_OK);
  CHECK(profile.period_type.type_strindex == 1);
  CHECK(profile.period_type.unit_strindex == 2);
  CHECK(profile.period == -1);
  otlp_profiles_dev_profile_destroy(&profile);

  otlp_profiles_dev_export_profiles_partial_success_init(&partial_success);
  partial_success.rejected_profiles = -1;
  response_size = otlp_profiles_dev_export_profiles_partial_success_encoded_size_inner(&partial_success);
  otlp__writer_init(&writer, encoded_response, sizeof(encoded_response));
  otlp_profiles_dev_export_profiles_partial_success_encode_field(&writer, 1, &partial_success);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, response_size));
  CHECK(otlp_test_contains_bytes(encoded_response, writer.off, rejected_minus_one, sizeof(rejected_minus_one)));

  otlp_profiles_dev_export_profiles_service_response_init(&response);
  CHECK(otlp_profiles_dev_export_profiles_service_response_decode(&response, encoded_response, writer.off) == OTLP_STATUS_OK);
  CHECK(response.has_partial_success == 1);
  CHECK(response.partial_success.rejected_profiles == -1);
  otlp_profiles_dev_export_profiles_service_response_destroy(&response);
  otlp_profiles_dev_export_profiles_partial_success_destroy(&partial_success);

  otlp_profiles_dev_profile_init(&profile);
  tap_equal(g_current_tap,
            otlp_profiles_dev_profile_decode_inner(&profile,
                                                  negative_profile_period,
                                                  sizeof(negative_profile_period)),
            OTLP_STATUS_OK, "decode negative profile period");
  tap_equal(g_current_tap, profile.period, (int64_t)-1,
            "negative profile period round-trips as signed int64");
  otlp_profiles_dev_profile_destroy(&profile);

  otlp_profiles_dev_sample_init(&sample);
  tap_equal(g_current_tap,
            otlp_profiles_dev_sample_decode_inner(&sample,
                                                 negative_sample_values_packed,
                                                 sizeof(negative_sample_values_packed)),
            OTLP_STATUS_OK, "decode negative packed sample values");
  tap_equal(g_current_tap, sample.values.len, (size_t)2,
            "decoded two negative packed sample values");
  tap_equal(g_current_tap, sample.values.ptr[0], (int64_t)-1,
            "first packed sample value round-trips as signed int64");
  tap_equal(g_current_tap, sample.values.ptr[1], (int64_t)-2,
            "second packed sample value round-trips as signed int64");
  otlp_profiles_dev_sample_destroy(&sample);

  otlp_test_ok("profiles proto wire compatibility");
}

static void test_profiles_size_matches_encode_exactly(void) {
  otlp_profiles_dev_export_profiles_service_request request;
  otlp_profiles_dev_resource_profiles resource_profiles;
  otlp_profiles_dev_scope_profiles scope_profiles;
  otlp_profiles_dev_profile profile;
  otlp_profiles_dev_sample sample;
  otlp_profiles_dev_value_type sample_type;
  uint8_t *encoded;
  size_t size;
  size_t actual;

  otlp_profiles_dev_export_profiles_service_request_init(&request);
  otlp_profiles_dev_resource_profiles_init(&resource_profiles);
  otlp_profiles_dev_scope_profiles_init(&scope_profiles);
  otlp_profiles_dev_profile_init(&profile);
  otlp_profiles_dev_sample_init(&sample);
  otlp_profiles_dev_profiles_dictionary_init(&request.dictionary);

  tap_equal(g_current_tap,
            otlp_profiles_dev_profiles_dictionary_init_default_zero_entries(
                &request.dictionary),
            OTLP_STATUS_OK, "initialize default profiles dictionary zero entries");

  sample.stack_index = 1;
  tap_equal(g_current_tap, otlp_profiles_dev_sample_append_value(&sample, (int64_t)-1),
            OTLP_STATUS_OK, "append negative sample value");
  tap_equal(g_current_tap,
            otlp__append_primitive((void **)&sample.timestamps_unix_nano.ptr,
                                   &sample.timestamps_unix_nano.len,
                                   &sample.timestamps_unix_nano.cap,
                                   sizeof(uint64_t),
                                   &(uint64_t){123}),
            OTLP_STATUS_OK, "append sample timestamp");

  sample_type.type_strindex = 1;
  sample_type.unit_strindex = 2;
  profile.has_sample_type = 1;
  profile.sample_type = sample_type;
  profile.period = -1;
  profile.duration_nano = 99;
  profile.time_unix_nano = 77;
  tap_equal(g_current_tap, otlp_profiles_dev_profile_append_sample(&profile, &sample),
            OTLP_STATUS_OK, "append profile sample");

  tap_equal(g_current_tap,
            otlp_profiles_dev_scope_profiles_append_profile(&scope_profiles, &profile),
            OTLP_STATUS_OK, "append scope profile");
  tap_equal(g_current_tap,
            otlp_profiles_dev_resource_profiles_append_scope_profiles(
                &resource_profiles, &scope_profiles),
            OTLP_STATUS_OK, "append resource profiles");
  tap_equal(g_current_tap,
            otlp_profiles_dev_export_profiles_service_request_append_resource_profiles(
                &request, &resource_profiles),
            OTLP_STATUS_OK, "append export resource profiles");

  size = otlp_profiles_dev_export_profiles_service_request_encoded_size(&request);
  encoded = (uint8_t *)malloc(size);
  tap_ok(g_current_tap, encoded != NULL, "allocated profiles encode buffer");
  if (encoded == NULL) {
    exit(1);
  }

  tap_equal(g_current_tap,
            otlp_profiles_dev_export_profiles_service_request_encode(&request,
                                                                    encoded,
                                                                    size),
            OTLP_STATUS_OK, "profiles request encode succeeded");

  {
    otlp__writer writer;
    size_t i;
    otlp__writer_init(&writer, encoded, size);
    for (i = 0; i < request.resource_profiles.len; ++i) {
      otlp_profiles_dev_resource_profiles_encode_field(
          &writer, 1, &request.resource_profiles.ptr[i]);
    }
    otlp_profiles_dev_profiles_dictionary_encode_field(&writer, 2,
                                                       &request.dictionary);
    tap_equal(g_current_tap, writer.status, OTLP_STATUS_OK, "manual profiles encode succeeded");
    actual = writer.off;
  }
  tap_equal(g_current_tap, actual, size, "profiles encoded size matches manual writer offset");

  free(encoded);
  otlp_profiles_dev_sample_destroy(&sample);
  otlp_profiles_dev_profile_destroy(&profile);
  otlp_profiles_dev_scope_profiles_destroy(&scope_profiles);
  otlp_profiles_dev_resource_profiles_destroy(&resource_profiles);
  otlp_profiles_dev_export_profiles_service_request_destroy(&request);

  otlp_test_ok("profiles size matches exact encode");
}

static void test_profiles_round_trip(void) {
  otlp_profiles_dev_export_profiles_service_request request;
  otlp_profiles_dev_export_profiles_service_request_init(&request);

  otlp_profiles_dev_profiles_dictionary_init_default_zero_entries(&request.dictionary);

  otlp_profiles_dev_resource_profiles resource_profiles;
  otlp_profiles_dev_resource_profiles_init(&resource_profiles);

  otlp_profiles_dev_scope_profiles scope_profiles;
  otlp_profiles_dev_scope_profiles_init(&scope_profiles);

  otlp_profiles_dev_profile profile;
  otlp_profiles_dev_profile_init(&profile);
  profile.sample_type.type_strindex = 0;
  profile.sample_type.unit_strindex = 0;

  otlp_profiles_dev_sample sample;
  otlp_profiles_dev_sample_init(&sample);
  otlp_profiles_dev_sample_append_value(&sample, 7);
  otlp_profiles_dev_profile_append_sample(&profile, &sample);

  otlp_profiles_dev_scope_profiles_append_profile(&scope_profiles, &profile);
  otlp_profiles_dev_resource_profiles_append_scope_profiles(&resource_profiles, &scope_profiles);
  otlp_profiles_dev_export_profiles_service_request_append_resource_profiles(&request, &resource_profiles);

  size_t encoded_size = otlp_profiles_dev_export_profiles_service_request_encoded_size(&request);
  CHECK(encoded_size > 0);

  uint8_t *encoded = (uint8_t *) malloc(encoded_size);
  CHECK(encoded != NULL);

  CHECK(otlp_profiles_dev_export_profiles_service_request_encode(&request, encoded, encoded_size) == OTLP_STATUS_OK);

  otlp_profiles_dev_export_profiles_service_request decoded;
  otlp_profiles_dev_export_profiles_service_request_init(&decoded);
  CHECK(otlp_profiles_dev_export_profiles_service_request_decode(&decoded, encoded, encoded_size) == OTLP_STATUS_OK);
  CHECK(decoded.resource_profiles.len == 1);
  CHECK(decoded.dictionary.string_table.len >= 1);

  otlp_profiles_dev_export_profiles_service_request_destroy(&decoded);
  otlp_profiles_dev_export_profiles_service_request_destroy(&request);
  otlp_profiles_dev_resource_profiles_destroy(&resource_profiles);
  otlp_profiles_dev_scope_profiles_destroy(&scope_profiles);
  otlp_profiles_dev_profile_destroy(&profile);
  otlp_profiles_dev_sample_destroy(&sample);
  free(encoded);
  otlp_test_ok("profiles round trip");
}

static void test_profiles_round_trip_dictionary_content(void) {
  otlp_profiles_dev_export_profiles_service_request request;
  otlp_profiles_dev_export_profiles_service_request decoded;
  otlp_profiles_dev_resource_profiles resource_profiles;
  otlp_profiles_dev_scope_profiles scope_profiles;
  otlp_profiles_dev_profile profile;
  otlp_profiles_dev_sample sample;
  otlp_profiles_dev_mapping mapping;
  otlp_profiles_dev_location location;
  otlp_profiles_dev_function function_value;
  otlp_profiles_dev_key_value_and_unit attribute;
  otlp_profiles_dev_stack stack;
  otlp_profiles_dev_link link;
  otlp_any_value attr_value;
  otlp_string string_value;
  int32_t idx;
  size_t encoded_size;
  uint8_t *encoded;

  otlp_profiles_dev_export_profiles_service_request_init(&request);
  CHECK(otlp_profiles_dev_profiles_dictionary_init_default_zero_entries(&request.dictionary) == OTLP_STATUS_OK);

  otlp__zero(&string_value, sizeof(string_value));
  CHECK(otlp__set_string(&string_value, "cpu", 3) == OTLP_STATUS_OK);
  CHECK(otlp__append_primitive(
    (void **) &request.dictionary.string_table.ptr,
    &request.dictionary.string_table.len,
    &request.dictionary.string_table.cap,
    sizeof(string_value),
    &string_value
  ) == OTLP_STATUS_OK);
  string_value.ptr = NULL;
  string_value.len = 0;

  CHECK(otlp__set_string(&string_value, "ns", 2) == OTLP_STATUS_OK);
  CHECK(otlp__append_primitive(
    (void **) &request.dictionary.string_table.ptr,
    &request.dictionary.string_table.len,
    &request.dictionary.string_table.cap,
    sizeof(string_value),
    &string_value
  ) == OTLP_STATUS_OK);
  string_value.ptr = NULL;
  string_value.len = 0;

  CHECK(otlp__set_string(&string_value, "binary.so", 9) == OTLP_STATUS_OK);
  CHECK(otlp__append_primitive(
    (void **) &request.dictionary.string_table.ptr,
    &request.dictionary.string_table.len,
    &request.dictionary.string_table.cap,
    sizeof(string_value),
    &string_value
  ) == OTLP_STATUS_OK);
  string_value.ptr = NULL;
  string_value.len = 0;

  CHECK(otlp__set_string(&string_value, "main", 4) == OTLP_STATUS_OK);
  CHECK(otlp__append_primitive(
    (void **) &request.dictionary.string_table.ptr,
    &request.dictionary.string_table.len,
    &request.dictionary.string_table.cap,
    sizeof(string_value),
    &string_value
  ) == OTLP_STATUS_OK);
  string_value.ptr = NULL;
  string_value.len = 0;

  otlp_profiles_dev_mapping_init(&mapping);
  mapping.memory_start = 100;
  mapping.memory_limit = 200;
  mapping.file_offset = 7;
  mapping.filename_strindex = 3;
  idx = 1;
  CHECK(otlp__append_primitive(
    (void **) &mapping.attribute_indices.ptr,
    &mapping.attribute_indices.len,
    &mapping.attribute_indices.cap,
    sizeof(idx),
    &idx
  ) == OTLP_STATUS_OK);
  CHECK(otlp__append_primitive(
    (void **) &request.dictionary.mapping_table.ptr,
    &request.dictionary.mapping_table.len,
    &request.dictionary.mapping_table.cap,
    sizeof(mapping),
    &mapping
  ) == OTLP_STATUS_OK);
  mapping.attribute_indices.ptr = NULL;
  mapping.attribute_indices.len = 0;
  mapping.attribute_indices.cap = 0;

  otlp_profiles_dev_function_init(&function_value);
  function_value.name_strindex = 4;
  function_value.system_name_strindex = 4;
  function_value.filename_strindex = 3;
  function_value.start_line = 55;
  CHECK(otlp__append_primitive(
    (void **) &request.dictionary.function_table.ptr,
    &request.dictionary.function_table.len,
    &request.dictionary.function_table.cap,
    sizeof(function_value),
    &function_value
  ) == OTLP_STATUS_OK);

  otlp_profiles_dev_location_init(&location);
  location.mapping_index = 1;
  location.address = 1234;
  {
    otlp_profiles_dev_line line;
    otlp_profiles_dev_line_init(&line);
    line.function_index = 1;
    line.line = 77;
    CHECK(otlp__append_primitive(
      (void **) &location.lines.ptr,
      &location.lines.len,
      &location.lines.cap,
      sizeof(line),
      &line
    ) == OTLP_STATUS_OK);
  }
  idx = 1;
  CHECK(otlp__append_primitive(
    (void **) &location.attribute_indices.ptr,
    &location.attribute_indices.len,
    &location.attribute_indices.cap,
    sizeof(idx),
    &idx
  ) == OTLP_STATUS_OK);
  CHECK(otlp__append_primitive(
    (void **) &request.dictionary.location_table.ptr,
    &request.dictionary.location_table.len,
    &request.dictionary.location_table.cap,
    sizeof(location),
    &location
  ) == OTLP_STATUS_OK);
  location.lines.ptr = NULL;
  location.lines.len = 0;
  location.lines.cap = 0;
  location.attribute_indices.ptr = NULL;
  location.attribute_indices.len = 0;
  location.attribute_indices.cap = 0;

  otlp_profiles_dev_stack_init(&stack);
  idx = 1;
  CHECK(otlp__append_primitive(
    (void **) &stack.location_indices.ptr,
    &stack.location_indices.len,
    &stack.location_indices.cap,
    sizeof(idx),
    &idx
  ) == OTLP_STATUS_OK);
  CHECK(otlp__append_primitive(
    (void **) &request.dictionary.stack_table.ptr,
    &request.dictionary.stack_table.len,
    &request.dictionary.stack_table.cap,
    sizeof(stack),
    &stack
  ) == OTLP_STATUS_OK);
  stack.location_indices.ptr = NULL;
  stack.location_indices.len = 0;
  stack.location_indices.cap = 0;

  otlp_profiles_dev_link_init(&link);
  {
    uint8_t trace_id_bytes[16] = { 1 };
    uint8_t span_id_bytes[8] = { 2 };
    CHECK(otlp__set_bytes(&link.trace_id, trace_id_bytes, sizeof(trace_id_bytes)) == OTLP_STATUS_OK);
    CHECK(otlp__set_bytes(&link.span_id, span_id_bytes, sizeof(span_id_bytes)) == OTLP_STATUS_OK);
  }
  CHECK(otlp__append_primitive(
    (void **) &request.dictionary.link_table.ptr,
    &request.dictionary.link_table.len,
    &request.dictionary.link_table.cap,
    sizeof(link),
    &link
  ) == OTLP_STATUS_OK);
  link.trace_id.ptr = NULL;
  link.trace_id.len = 0;
  link.span_id.ptr = NULL;
  link.span_id.len = 0;

  CHECK(otlp_any_value_init_string(&attr_value, "prod", 4) == OTLP_STATUS_OK);
  otlp_profiles_dev_key_value_and_unit_init(&attribute);
  attribute.key_strindex = 4;
  attribute.unit_strindex = 2;
  CHECK(otlp_any_value_copy(&attribute.value, &attr_value) == OTLP_STATUS_OK);
  CHECK(otlp__append_primitive(
    (void **) &request.dictionary.attribute_table.ptr,
    &request.dictionary.attribute_table.len,
    &request.dictionary.attribute_table.cap,
    sizeof(attribute),
    &attribute
  ) == OTLP_STATUS_OK);
  otlp_profiles_dev_key_value_and_unit_init(&attribute);

  otlp_profiles_dev_resource_profiles_init(&resource_profiles);
  otlp_profiles_dev_scope_profiles_init(&scope_profiles);
  otlp_profiles_dev_profile_init(&profile);
  profile.sample_type.type_strindex = 1;
  profile.sample_type.unit_strindex = 2;
  profile.period_type.type_strindex = 1;
  profile.period_type.unit_strindex = 2;
  profile.period = 10;
  profile.time_unix_nano = 999;
  idx = 1;
  CHECK(otlp__append_primitive(
    (void **) &profile.attribute_indices.ptr,
    &profile.attribute_indices.len,
    &profile.attribute_indices.cap,
    sizeof(idx),
    &idx
  ) == OTLP_STATUS_OK);

  otlp_profiles_dev_sample_init(&sample);
  sample.stack_index = 1;
  sample.link_index = 1;
  idx = 1;
  CHECK(otlp__append_primitive(
    (void **) &sample.attribute_indices.ptr,
    &sample.attribute_indices.len,
    &sample.attribute_indices.cap,
    sizeof(idx),
    &idx
  ) == OTLP_STATUS_OK);
  CHECK(otlp_profiles_dev_sample_append_value(&sample, 7) == OTLP_STATUS_OK);
  {
    uint64_t ts = 1111;
    CHECK(otlp__append_primitive(
      (void **) &sample.timestamps_unix_nano.ptr,
      &sample.timestamps_unix_nano.len,
      &sample.timestamps_unix_nano.cap,
      sizeof(ts),
      &ts
    ) == OTLP_STATUS_OK);
  }
  CHECK(otlp_profiles_dev_profile_append_sample(&profile, &sample) == OTLP_STATUS_OK);
  CHECK(otlp_profiles_dev_scope_profiles_append_profile(&scope_profiles, &profile) == OTLP_STATUS_OK);
  CHECK(otlp_profiles_dev_resource_profiles_append_scope_profiles(&resource_profiles, &scope_profiles) == OTLP_STATUS_OK);
  CHECK(otlp_profiles_dev_export_profiles_service_request_append_resource_profiles(&request, &resource_profiles) == OTLP_STATUS_OK);

  encoded_size = otlp_profiles_dev_export_profiles_service_request_encoded_size(&request);
  encoded = (uint8_t *) malloc(encoded_size);
  CHECK(encoded != NULL);
  CHECK(otlp_profiles_dev_export_profiles_service_request_encode(&request, encoded, encoded_size) == OTLP_STATUS_OK);

  otlp_profiles_dev_export_profiles_service_request_init(&decoded);
  CHECK(otlp_profiles_dev_export_profiles_service_request_decode(&decoded, encoded, encoded_size) == OTLP_STATUS_OK);
  CHECK(decoded.dictionary.mapping_table.len == 2);
  CHECK(decoded.dictionary.mapping_table.ptr[1].memory_start == 100);
  CHECK(decoded.dictionary.mapping_table.ptr[1].filename_strindex == 3);
  CHECK(decoded.dictionary.function_table.len == 2);
  CHECK(decoded.dictionary.function_table.ptr[1].start_line == 55);
  CHECK(decoded.dictionary.location_table.len == 2);
  CHECK(decoded.dictionary.location_table.ptr[1].mapping_index == 1);
  CHECK(decoded.dictionary.location_table.ptr[1].lines.len == 1);
  CHECK(decoded.dictionary.location_table.ptr[1].lines.ptr[0].function_index == 1);
  CHECK(decoded.dictionary.attribute_table.len == 2);
  CHECK(decoded.dictionary.attribute_table.ptr[1].key_strindex == 4);
  CHECK(decoded.dictionary.attribute_table.ptr[1].unit_strindex == 2);
  CHECK(decoded.dictionary.stack_table.len == 2);
  CHECK(decoded.dictionary.stack_table.ptr[1].location_indices.len == 1);
  CHECK(decoded.dictionary.stack_table.ptr[1].location_indices.ptr[0] == 1);
  CHECK(decoded.resource_profiles.len == 1);
  CHECK(decoded.resource_profiles.ptr[0].scope_profiles.len == 1);
  CHECK(decoded.resource_profiles.ptr[0].scope_profiles.ptr[0].profiles.len == 1);
  CHECK(decoded.resource_profiles.ptr[0].scope_profiles.ptr[0].profiles.ptr[0].samples.len == 1);
  CHECK(decoded.resource_profiles.ptr[0].scope_profiles.ptr[0].profiles.ptr[0].samples.ptr[0].stack_index == 1);

  otlp_profiles_dev_export_profiles_service_request_destroy(&decoded);
  otlp_profiles_dev_export_profiles_service_request_destroy(&request);
  otlp_profiles_dev_resource_profiles_destroy(&resource_profiles);
  otlp_profiles_dev_scope_profiles_destroy(&scope_profiles);
  otlp_profiles_dev_profile_destroy(&profile);
  otlp_profiles_dev_sample_destroy(&sample);
  otlp_profiles_dev_mapping_destroy(&mapping);
  otlp_profiles_dev_location_destroy(&location);
  otlp_profiles_dev_function_destroy(&function_value);
  otlp_profiles_dev_key_value_and_unit_destroy(&attribute);
  otlp_profiles_dev_stack_destroy(&stack);
  otlp_profiles_dev_link_destroy(&link);
  otlp_any_value_destroy(&attr_value);
  free(encoded);
  otlp_test_ok("profiles round trip dictionary content");
}

static void test_profiles_payload_fields_and_unknown(void) {
  otlp_profiles_dev_export_profiles_service_request request;
  otlp_profiles_dev_export_profiles_service_request decoded;
  otlp_profiles_dev_resource_profiles resource_profiles;
  otlp_profiles_dev_scope_profiles scope_profiles;
  otlp_profiles_dev_profile profile;
  otlp_profiles_dev_sample sample;
  size_t encoded_size;
  size_t extended_size;
  uint8_t *encoded;
  uint8_t *extended;
  uint8_t profile_id[16] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6 };
  uint8_t payload[4] = { 1, 3, 3, 7 };

  otlp_profiles_dev_export_profiles_service_request_init(&request);
  CHECK(otlp_profiles_dev_profiles_dictionary_init_default_zero_entries(&request.dictionary) == OTLP_STATUS_OK);

  otlp_profiles_dev_resource_profiles_init(&resource_profiles);
  CHECK(otlp__set_string(&resource_profiles.schema_url, "profiles-resource", 17) == OTLP_STATUS_OK);
  otlp_profiles_dev_scope_profiles_init(&scope_profiles);
  CHECK(otlp__set_string(&scope_profiles.schema_url, "profiles-scope", 14) == OTLP_STATUS_OK);
  otlp_profiles_dev_profile_init(&profile);
  profile.sample_type.type_strindex = 0;
  profile.sample_type.unit_strindex = 0;
  profile.time_unix_nano = 2000;
  profile.duration_nano = 4000;
  profile.period = 10;
  profile.dropped_attributes_count = 2;
  CHECK(otlp__set_bytes(&profile.profile_id, profile_id, sizeof(profile_id)) == OTLP_STATUS_OK);
  CHECK(otlp__set_string(&profile.original_payload_format, "pprof", 5) == OTLP_STATUS_OK);
  CHECK(otlp__set_bytes(&profile.original_payload, payload, sizeof(payload)) == OTLP_STATUS_OK);

  otlp_profiles_dev_sample_init(&sample);
  sample.link_index = 0;
  sample.stack_index = 0;
  CHECK(otlp_profiles_dev_sample_append_value(&sample, 10) == OTLP_STATUS_OK);
  CHECK(otlp_profiles_dev_sample_append_value(&sample, 11) == OTLP_STATUS_OK);
  {
    uint64_t ts = 100;
    CHECK(otlp__append_primitive((void **) &sample.timestamps_unix_nano.ptr, &sample.timestamps_unix_nano.len, &sample.timestamps_unix_nano.cap, sizeof(ts), &ts) == OTLP_STATUS_OK);
    ts = 200;
    CHECK(otlp__append_primitive((void **) &sample.timestamps_unix_nano.ptr, &sample.timestamps_unix_nano.len, &sample.timestamps_unix_nano.cap, sizeof(ts), &ts) == OTLP_STATUS_OK);
  }
  CHECK(otlp_profiles_dev_profile_append_sample(&profile, &sample) == OTLP_STATUS_OK);
  CHECK(otlp_profiles_dev_scope_profiles_append_profile(&scope_profiles, &profile) == OTLP_STATUS_OK);
  CHECK(otlp_profiles_dev_resource_profiles_append_scope_profiles(&resource_profiles, &scope_profiles) == OTLP_STATUS_OK);
  CHECK(otlp_profiles_dev_export_profiles_service_request_append_resource_profiles(&request, &resource_profiles) == OTLP_STATUS_OK);
  CHECK(otlp_profiles_dev_export_profiles_service_request_append_resource_profiles(&request, &resource_profiles) == OTLP_STATUS_OK);

  encoded_size = otlp_profiles_dev_export_profiles_service_request_encoded_size(&request);
  encoded = (uint8_t *) malloc(encoded_size);
  CHECK(encoded != NULL);
  CHECK(otlp_profiles_dev_export_profiles_service_request_encode(&request, encoded, encoded_size) == OTLP_STATUS_OK);
  CHECK(
    otlp_profiles_dev_export_profiles_service_request_encode(&request, encoded, encoded_size - 1) ==
    OTLP_STATUS_BUFFER_TOO_SMALL
  );

  extended = otlp_test_append_unknown_varint_field(encoded, encoded_size, 77, 1, &extended_size);
  otlp_profiles_dev_export_profiles_service_request_init(&decoded);
  CHECK(otlp_profiles_dev_export_profiles_service_request_decode(&decoded, extended, extended_size) == OTLP_STATUS_OK);
  CHECK(decoded.resource_profiles.len == 2);
  CHECK(decoded.resource_profiles.ptr[0].schema_url.len == 17);
  CHECK(decoded.resource_profiles.ptr[0].scope_profiles.ptr[0].schema_url.len == 14);
  CHECK(decoded.resource_profiles.ptr[0].scope_profiles.ptr[0].profiles.ptr[0].profile_id.len == 16);
  CHECK(decoded.resource_profiles.ptr[0].scope_profiles.ptr[0].profiles.ptr[0].duration_nano == 4000);
  CHECK(decoded.resource_profiles.ptr[0].scope_profiles.ptr[0].profiles.ptr[0].dropped_attributes_count == 2);
  CHECK(decoded.resource_profiles.ptr[0].scope_profiles.ptr[0].profiles.ptr[0].original_payload_format.len == 5);
  CHECK(decoded.resource_profiles.ptr[0].scope_profiles.ptr[0].profiles.ptr[0].original_payload.len == 4);
  CHECK(decoded.resource_profiles.ptr[0].scope_profiles.ptr[0].profiles.ptr[0].samples.ptr[0].values.len == 2);
  CHECK(decoded.resource_profiles.ptr[0].scope_profiles.ptr[0].profiles.ptr[0].samples.ptr[0].timestamps_unix_nano.len == 2);

  otlp_profiles_dev_export_profiles_service_request_destroy(&decoded);
  free(extended);
  free(encoded);
  otlp_profiles_dev_export_profiles_service_request_destroy(&request);
  otlp_profiles_dev_resource_profiles_destroy(&resource_profiles);
  otlp_profiles_dev_scope_profiles_destroy(&scope_profiles);
  otlp_profiles_dev_profile_destroy(&profile);
  otlp_profiles_dev_sample_destroy(&sample);
  otlp_test_ok("profiles payload fields and unknown");
}

static void test_profiles_data_and_response_round_trip(void) {
  otlp_profiles_dev_profile profile;
  otlp_profiles_dev_scope_profiles scope_profiles;
  otlp_profiles_dev_resource_profiles resource_profiles;
  otlp_profiles_dev_profiles_data data;
  otlp_profiles_dev_profiles_data decoded_data;
  otlp_profiles_dev_export_profiles_service_response response;
  otlp_profiles_dev_export_profiles_service_response decoded_response;
  size_t data_size;
  size_t response_size;
  uint8_t *data_bytes;
  uint8_t *response_bytes;

  otlp_profiles_dev_profile_init(&profile);
  profile.sample_type.type_strindex = 0;
  profile.sample_type.unit_strindex = 0;

  otlp_profiles_dev_scope_profiles_init(&scope_profiles);
  scope_profiles.has_scope = 1;
  CHECK(otlp__set_string(&scope_profiles.scope.name, "profiles", 8) == OTLP_STATUS_OK);
  CHECK(otlp_profiles_dev_scope_profiles_append_profile(&scope_profiles, &profile) == OTLP_STATUS_OK);

  otlp_profiles_dev_resource_profiles_init(&resource_profiles);
  resource_profiles.has_resource = 1;
  CHECK(otlp_profiles_dev_resource_profiles_append_scope_profiles(&resource_profiles, &scope_profiles) == OTLP_STATUS_OK);

  otlp_profiles_dev_profiles_data_init(&data);
  CHECK(otlp_profiles_dev_profiles_dictionary_init_default_zero_entries(&data.dictionary) == OTLP_STATUS_OK);
  CHECK(otlp__append_primitive((void **)&data.resource_profiles.ptr,
                                &data.resource_profiles.len,
                                &data.resource_profiles.cap,
                                sizeof(resource_profiles),
                                &resource_profiles) == OTLP_STATUS_OK);
  otlp_profiles_dev_resource_profiles_init(&resource_profiles);
  data_size = otlp_profiles_dev_profiles_data_encoded_size(&data);
  data_bytes = (uint8_t *)malloc(data_size);
  CHECK(data_bytes != NULL);
  CHECK(otlp_profiles_dev_profiles_data_encode(&data, data_bytes, data_size) == OTLP_STATUS_OK);
  otlp_profiles_dev_profiles_data_init(&decoded_data);
  CHECK(otlp_profiles_dev_profiles_data_decode(&decoded_data, data_bytes, data_size) == OTLP_STATUS_OK);
  CHECK(decoded_data.resource_profiles.len == 1);
  CHECK(decoded_data.resource_profiles.ptr[0].has_resource == 1);
  CHECK(decoded_data.resource_profiles.ptr[0].scope_profiles.ptr[0].has_scope == 1);

  otlp_profiles_dev_export_profiles_service_response_init(&response);
  response.has_partial_success = 1;
  response.partial_success.rejected_profiles = 4;
  CHECK(otlp__set_string(&response.partial_success.error_message, "profile-partial", 15) == OTLP_STATUS_OK);
  response_size = otlp_profiles_dev_export_profiles_service_response_encoded_size(&response);
  response_bytes = (uint8_t *)malloc(response_size);
  CHECK(response_bytes != NULL);
  CHECK(otlp_profiles_dev_export_profiles_service_response_encode(&response, response_bytes, response_size) == OTLP_STATUS_OK);
  otlp_profiles_dev_export_profiles_service_response_init(&decoded_response);
  CHECK(otlp_profiles_dev_export_profiles_service_response_decode(&decoded_response, response_bytes, response_size) == OTLP_STATUS_OK);
  CHECK(decoded_response.has_partial_success == 1);
  CHECK(decoded_response.partial_success.rejected_profiles == 4);

  otlp_profiles_dev_export_profiles_service_response_destroy(&decoded_response);
  otlp_profiles_dev_export_profiles_service_response_destroy(&response);
  otlp_profiles_dev_profiles_data_destroy(&decoded_data);
  otlp_profiles_dev_profiles_data_destroy(&data);
  otlp_profiles_dev_resource_profiles_destroy(&resource_profiles);
  otlp_profiles_dev_scope_profiles_destroy(&scope_profiles);
  otlp_profiles_dev_profile_destroy(&profile);
  free(data_bytes);
  free(response_bytes);
  otlp_test_ok("profiles data and response round trip");
}

OTLP_DEFINE_SUBTEST(test_profiles_proto_wire_compatibility)
OTLP_DEFINE_SUBTEST(test_profiles_size_matches_encode_exactly)
OTLP_DEFINE_SUBTEST(test_profiles_round_trip)
OTLP_DEFINE_SUBTEST(test_profiles_round_trip_dictionary_content)
OTLP_DEFINE_SUBTEST(test_profiles_payload_fields_and_unknown)
OTLP_DEFINE_SUBTEST(test_profiles_data_and_response_round_trip)

#endif /* OTLP_FORMAT_ENABLE_PROFILES */

int test_profiles_count(void) {
#if defined(OTLP_FORMAT_ENABLE_PROFILES)
  return 6;
#else
  return 0;
#endif
}

void test_profiles_run(tap_t *t) {
#if defined(OTLP_FORMAT_ENABLE_PROFILES)
  tap_test(t, "profiles proto wire compatibility", test_profiles_proto_wire_compatibility_subtest, NULL);
  tap_test(t, "profiles size matches exact encode", test_profiles_size_matches_encode_exactly_subtest, NULL);
  tap_test(t, "profiles round trip", test_profiles_round_trip_subtest, NULL);
  tap_test(t, "profiles round trip dictionary content", test_profiles_round_trip_dictionary_content_subtest, NULL);
  tap_test(t, "profiles payload fields and unknown", test_profiles_payload_fields_and_unknown_subtest, NULL);
  tap_test(t, "profiles data and response round trip", test_profiles_data_and_response_round_trip_subtest, NULL);
#else
  (void)t;
#endif
}
