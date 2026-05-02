#include "test_helpers.h"

static void test_common_proto_wire_compatibility(void) {
  otlp_any_value value;
  otlp_key_value kv;
  otlp_entity_ref entity_ref;
  otlp_instrumentation_scope scope;
  otlp_resource resource;
  otlp_any_value decoded_value;
  otlp_entity_ref decoded_entity_ref;
  otlp__writer writer;
  uint8_t encoded_value[16];
  uint8_t encoded_kv[32];
  uint8_t encoded_entity_ref[64];
  const uint8_t int_minus_one[] = {
    0x18, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01
  };
  const uint8_t string_value_strindex[] = { 0x40, 0x09 };
  const uint8_t string_value_strindex_minus_one[] = {
    0x40, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01
  };
  const uint8_t key_strindex[] = { 0x18, 0x07 };
  const uint8_t key_strindex_minus_one[] = {
    0x18, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01
  };
  const uint8_t desc_one[] = { 0x22, 0x04, 'o', 'n', 'e', '1' };
  const uint8_t desc_two[] = { 0x22, 0x04, 't', 'w', 'o', '2' };
  const uint8_t empty_string_value[] = { 0x0a, 0x00 };
  const uint8_t empty_bytes_value[] = { 0x3a, 0x00 };
  const uint8_t oneof_last_wins[] = { 0x0a, 0x05, 'f', 'i', 'r', 's', 't', 0x10, 0x01 };
  const uint8_t split_array_value[] = {
    0x2a, 0x05, 0x0a, 0x03, 0x0a, 0x01, 'a',
    0x2a, 0x05, 0x0a, 0x03, 0x0a, 0x01, 'b'
  };
  const uint8_t split_kv_value[] = {
    0x12, 0x07, 0x2a, 0x05, 0x0a, 0x03, 0x0a, 0x01, 'a',
    0x12, 0x07, 0x2a, 0x05, 0x0a, 0x03, 0x0a, 0x01, 'b'
  };
  const uint8_t empty_any_value_field[] = { 0x12, 0x00 };
  const uint8_t empty_entity_ref_strings[] = { 0x1a, 0x00, 0x22, 0x00 };
  const uint8_t scope_name[] = { 0x0a, 0x01, 'n' };
  const uint8_t scope_version[] = { 0x12, 0x01, 'v' };
  const uint8_t resource_dropped_count[] = { 0x10, 0x03 };
  size_t value_size;
  size_t kv_size;
  size_t entity_ref_size;
  otlp_string description_key;

  otlp_any_value_init(&value);
  value.kind = OTLP_ANY_VALUE_INT;
  value.value.int_value = -1;
  value_size = otlp_any_value_encoded_size_inner(&value);
  otlp__writer_init(&writer, encoded_value, sizeof(encoded_value));
  otlp_any_value_encode_inner(&writer, &value);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == value_size);
  CHECK(value_size == sizeof(int_minus_one));
  CHECK(memcmp(encoded_value, int_minus_one, sizeof(int_minus_one)) == 0);

  otlp_any_value_init(&decoded_value);
  CHECK(otlp_any_value_decode_inner(&decoded_value, int_minus_one, sizeof(int_minus_one)) == OTLP_STATUS_OK);
  CHECK(decoded_value.kind == OTLP_ANY_VALUE_INT);
  CHECK(decoded_value.value.int_value == -1);
  CHECK(otlp_any_value_decode_inner(&decoded_value, string_value_strindex, sizeof(string_value_strindex)) == OTLP_STATUS_OK);
  CHECK(decoded_value.kind == OTLP_ANY_VALUE_STRING_STRINDEX);
  CHECK(decoded_value.value.string_value_strindex == 9);
  otlp_any_value_destroy(&decoded_value);

  otlp_any_value_init(&value);
  value.kind = OTLP_ANY_VALUE_STRING_STRINDEX;
  value.value.string_value_strindex = 9;
  value_size = otlp_any_value_encoded_size_inner(&value);
  otlp__writer_init(&writer, encoded_value, sizeof(encoded_value));
  otlp_any_value_encode_inner(&writer, &value);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == value_size);
  CHECK(value_size == sizeof(string_value_strindex));
  CHECK(memcmp(encoded_value, string_value_strindex, sizeof(string_value_strindex)) == 0);

  otlp_any_value_init(&value);
  value.kind = OTLP_ANY_VALUE_STRING_STRINDEX;
  value.value.string_value_strindex = -1;
  value_size = otlp_any_value_encoded_size_inner(&value);
  otlp__writer_init(&writer, encoded_value, sizeof(encoded_value));
  otlp_any_value_encode_inner(&writer, &value);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == value_size);
  CHECK(value_size == sizeof(string_value_strindex_minus_one));
  CHECK(memcmp(encoded_value, string_value_strindex_minus_one,
                sizeof(string_value_strindex_minus_one)) == 0);

  otlp_any_value_init(&value);
  CHECK(otlp_any_value_init_string(&value, "", 0) == OTLP_STATUS_OK);
  value_size = otlp_any_value_encoded_size_inner(&value);
  otlp__writer_init(&writer, encoded_value, sizeof(encoded_value));
  otlp_any_value_encode_inner(&writer, &value);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == value_size);
  CHECK(value_size == sizeof(empty_string_value));
  CHECK(memcmp(encoded_value, empty_string_value, sizeof(empty_string_value)) == 0);
  otlp_any_value_destroy(&value);

  otlp_any_value_init(&value);
  value.kind = OTLP_ANY_VALUE_BYTES;
  value_size = otlp_any_value_encoded_size_inner(&value);
  otlp__writer_init(&writer, encoded_value, sizeof(encoded_value));
  otlp_any_value_encode_inner(&writer, &value);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == value_size);
  CHECK(value_size == sizeof(empty_bytes_value));
  CHECK(memcmp(encoded_value, empty_bytes_value, sizeof(empty_bytes_value)) == 0);
  otlp_any_value_destroy(&value);

  otlp_any_value_init(&decoded_value);
  CHECK(otlp_any_value_decode_inner(&decoded_value, oneof_last_wins, sizeof(oneof_last_wins)) == OTLP_STATUS_OK);
  CHECK(decoded_value.kind == OTLP_ANY_VALUE_BOOL);
  CHECK(decoded_value.value.bool_value == 1);
  otlp_any_value_destroy(&decoded_value);

  otlp_any_value_init(&decoded_value);
  CHECK(otlp_any_value_decode_inner(&decoded_value, split_array_value,
                                     sizeof(split_array_value)) ==
         OTLP_STATUS_OK);
  CHECK(decoded_value.kind == OTLP_ANY_VALUE_ARRAY);
  CHECK(decoded_value.value.array_value->values.len == 2);
  CHECK(decoded_value.value.array_value->values.ptr[0].kind ==
         OTLP_ANY_VALUE_STRING);
  CHECK(decoded_value.value.array_value->values.ptr[1].kind ==
         OTLP_ANY_VALUE_STRING);
  otlp_any_value_destroy(&decoded_value);

  otlp_key_value_init(&kv);
  CHECK(otlp__set_string(&kv.key, "k", 1) == OTLP_STATUS_OK);
  CHECK(otlp_any_value_init_string(&kv.value, "v", 1) == OTLP_STATUS_OK);
  kv.has_value = 1;
  kv_size = otlp_key_value_encoded_size_inner(&kv);
  otlp__writer_init(&writer, encoded_kv, sizeof(encoded_kv));
  otlp_key_value_encode_field(&writer, 1, &kv);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, kv_size));
  CHECK(!otlp_test_contains_bytes(encoded_kv, writer.off, key_strindex, sizeof(key_strindex)));
  otlp_key_value_destroy(&kv);

  otlp_key_value_init(&kv);
  CHECK(otlp__set_string(&kv.key, "k", 1) == OTLP_STATUS_OK);
  kv.key_strindex = 7;
  kv_size = otlp_key_value_encoded_size_inner(&kv);
  otlp__writer_init(&writer, encoded_kv, sizeof(encoded_kv));
  otlp_key_value_encode_field(&writer, 1, &kv);
  CHECK(writer.status == OTLP_STATUS_INVALID_ARGUMENT);
  otlp_key_value_destroy(&kv);

  otlp_key_value_init(&kv);
  kv.key_strindex = 7;
  kv_size = otlp_key_value_encoded_size_inner(&kv);
  otlp__writer_init(&writer, encoded_kv, sizeof(encoded_kv));
  otlp_key_value_encode_field(&writer, 1, &kv);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, kv_size));
  CHECK(otlp_test_contains_bytes(encoded_kv, writer.off, key_strindex, sizeof(key_strindex)));
  otlp_key_value_destroy(&kv);

  otlp_key_value_init(&kv);
  kv.key_strindex = -1;
  kv_size = otlp_key_value_encoded_size_inner(&kv);
  otlp__writer_init(&writer, encoded_kv, sizeof(encoded_kv));
  otlp_key_value_encode_field(&writer, 1, &kv);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, kv_size));
  CHECK(otlp_test_contains_bytes(encoded_kv, writer.off,
                                  key_strindex_minus_one,
                                  sizeof(key_strindex_minus_one)));
  otlp_key_value_destroy(&kv);

  otlp_key_value_init(&kv);
  CHECK(otlp_key_value_decode_inner(&kv, key_strindex, sizeof(key_strindex)) == OTLP_STATUS_OK);
  CHECK(kv.key_strindex == 7);
  otlp_key_value_destroy(&kv);

  otlp_key_value_init(&kv);
  CHECK(otlp_key_value_decode_inner(&kv, split_kv_value,
                                     sizeof(split_kv_value)) == OTLP_STATUS_OK);
  CHECK(kv.has_value == 1);
  CHECK(kv.value.kind == OTLP_ANY_VALUE_ARRAY);
  CHECK(kv.value.value.array_value->values.len == 2);
  otlp_key_value_destroy(&kv);

  otlp_key_value_init(&kv);
  kv_size = otlp_key_value_encoded_size_inner(&kv);
  otlp__writer_init(&writer, encoded_kv, sizeof(encoded_kv));
  otlp_key_value_encode_field(&writer, 1, &kv);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, kv_size));
  CHECK(!otlp_test_contains_bytes(encoded_kv, writer.off, empty_any_value_field, sizeof(empty_any_value_field)));
  otlp_key_value_destroy(&kv);

  otlp_key_value_init(&kv);
  kv.has_value = 1;
  kv_size = otlp_key_value_encoded_size_inner(&kv);
  otlp__writer_init(&writer, encoded_kv, sizeof(encoded_kv));
  otlp_key_value_encode_field(&writer, 1, &kv);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, kv_size));
  CHECK(otlp_test_contains_bytes(encoded_kv, writer.off, empty_any_value_field, sizeof(empty_any_value_field)));
  otlp_key_value_destroy(&kv);

  otlp_entity_ref_init(&entity_ref);
  CHECK(otlp__set_string(&entity_ref.schema_url, "schema", 6) == OTLP_STATUS_OK);
  CHECK(otlp__set_string(&entity_ref.type, "service", 7) == OTLP_STATUS_OK);
  otlp__zero(&description_key, sizeof(description_key));
  CHECK(otlp__set_string(&description_key, "one1", 4) == OTLP_STATUS_OK);
  CHECK(otlp__append_primitive((void **)&entity_ref.description_keys.ptr,
                                &entity_ref.description_keys.len,
                                &entity_ref.description_keys.cap,
                                sizeof(description_key),
                                &description_key) == OTLP_STATUS_OK);
  otlp__zero(&description_key, sizeof(description_key));
  CHECK(otlp__set_string(&description_key, "two2", 4) == OTLP_STATUS_OK);
  CHECK(otlp__append_primitive((void **)&entity_ref.description_keys.ptr,
                                &entity_ref.description_keys.len,
                                &entity_ref.description_keys.cap,
                                sizeof(description_key),
                                &description_key) == OTLP_STATUS_OK);
  entity_ref_size = otlp_entity_ref_encoded_size_inner(&entity_ref);
  otlp__writer_init(&writer, encoded_entity_ref, sizeof(encoded_entity_ref));
  otlp_entity_ref_encode_field(&writer, 1, &entity_ref);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, entity_ref_size));
  CHECK(otlp_test_contains_bytes(encoded_entity_ref, writer.off, desc_one, sizeof(desc_one)));
  CHECK(otlp_test_contains_bytes(encoded_entity_ref, writer.off, desc_two, sizeof(desc_two)));

  otlp_entity_ref_init(&decoded_entity_ref);
  CHECK(otlp_entity_ref_decode_inner(&decoded_entity_ref, encoded_entity_ref + 2, entity_ref_size) == OTLP_STATUS_OK);
  CHECK(decoded_entity_ref.description_keys.len == 2);
  CHECK(decoded_entity_ref.description_keys.ptr[0].len == 4);
  CHECK(decoded_entity_ref.description_keys.ptr[1].len == 4);

  otlp_entity_ref_destroy(&decoded_entity_ref);
  otlp_entity_ref_destroy(&entity_ref);
  otlp_entity_ref_init(&entity_ref);
  otlp__zero(&description_key, sizeof(description_key));
  CHECK(otlp__append_primitive((void **)&entity_ref.id_keys.ptr,
                                &entity_ref.id_keys.len,
                                &entity_ref.id_keys.cap,
                                sizeof(description_key),
                                &description_key) == OTLP_STATUS_OK);
  otlp__zero(&description_key, sizeof(description_key));
  CHECK(otlp__append_primitive((void **)&entity_ref.description_keys.ptr,
                                &entity_ref.description_keys.len,
                                &entity_ref.description_keys.cap,
                                sizeof(description_key),
                                &description_key) == OTLP_STATUS_OK);
  entity_ref_size = otlp_entity_ref_encoded_size_inner(&entity_ref);
  otlp__writer_init(&writer, encoded_entity_ref, sizeof(encoded_entity_ref));
  otlp_entity_ref_encode_field(&writer, 1, &entity_ref);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, entity_ref_size));
  CHECK(otlp_test_contains_bytes(encoded_entity_ref, writer.off, empty_entity_ref_strings, sizeof(empty_entity_ref_strings)));

  otlp_entity_ref_destroy(&entity_ref);

  otlp_instrumentation_scope_init(&scope);
  CHECK(otlp_instrumentation_scope_decode_inner(&scope, scope_name,
                                                 sizeof(scope_name)) ==
         OTLP_STATUS_OK);
  CHECK(otlp_instrumentation_scope_decode_inner(&scope, scope_version,
                                                 sizeof(scope_version)) ==
         OTLP_STATUS_OK);
  CHECK(scope.name.len == 1);
  CHECK(scope.version.len == 1);
  otlp_instrumentation_scope_destroy(&scope);

  otlp_resource_init(&resource);
  CHECK(otlp_resource_decode_inner(&resource, resource_dropped_count,
                                    sizeof(resource_dropped_count)) ==
         OTLP_STATUS_OK);
  CHECK(otlp_resource_decode_inner(&resource, empty_entity_ref_strings,
                                    sizeof(empty_entity_ref_strings)) ==
         OTLP_STATUS_OK);
  CHECK(resource.dropped_attributes_count == 3);
  CHECK(resource.entity_refs.len == 1);
  otlp_resource_destroy(&resource);

  otlp_test_ok("common proto wire compatibility");
}

static void test_size_matches_encode_exactly_common_regressions(void) {
  otlp_any_value value;
  otlp_key_value kv;
  uint8_t *encoded;
  size_t size;
  otlp_status status;

  otlp_any_value_init(&value);
  value.kind = OTLP_ANY_VALUE_STRING_STRINDEX;
  value.value.string_value_strindex = -1;
  size = otlp_any_value_encoded_size_inner(&value);
  encoded = (uint8_t *)malloc(size);
  tap_ok(g_current_tap, encoded != NULL, "allocated buffer for AnyValue string_value_strindex");
  if (encoded == NULL) {
    exit(1);
  }
  {
    otlp__writer writer;
    otlp__writer_init(&writer, encoded, size);
    otlp_any_value_encode_inner(&writer, &value);
    tap_equal(g_current_tap, writer.status, OTLP_STATUS_OK, "AnyValue encode succeeded");
    tap_equal(g_current_tap, writer.off, size, "AnyValue encoded size matches writer offset");
  }
  free(encoded);
  otlp_any_value_destroy(&value);

  otlp_key_value_init(&kv);
  kv.has_value = 1;
  size = otlp_key_value_encoded_size_inner(&kv);
  encoded = (uint8_t *)malloc(otlp__message_field_size(1, size));
  tap_ok(g_current_tap, encoded != NULL, "allocated buffer for empty KeyValue message");
  if (encoded == NULL) {
    exit(1);
  }
  status = OTLP_STATUS_OK;
  {
    otlp__writer writer;
    otlp__writer_init(&writer, encoded, otlp__message_field_size(1, size));
    otlp_key_value_encode_field(&writer, 1, &kv);
    status = writer.status;
    tap_equal(g_current_tap, writer.off, otlp__message_field_size(1, size),
              "empty KeyValue field size matches writer offset");
  }
  tap_equal(g_current_tap, status, OTLP_STATUS_OK, "empty KeyValue field encode succeeded");
  free(encoded);
  otlp_key_value_destroy(&kv);

  otlp_test_ok("common size matches exact encode");
}

OTLP_DEFINE_SUBTEST(test_common_proto_wire_compatibility)
OTLP_DEFINE_SUBTEST(test_size_matches_encode_exactly_common_regressions)

int test_common_count(void) {
  return 2;
}

void test_common_run(tap_t *t) {
  tap_test(t, "common proto wire compatibility", test_common_proto_wire_compatibility_subtest, NULL);
  tap_test(t, "common size matches exact encode", test_size_matches_encode_exactly_common_regressions_subtest, NULL);
}
