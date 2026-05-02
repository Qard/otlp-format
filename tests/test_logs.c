#include "test_helpers.h"

#if defined(OTLP_FORMAT_ENABLE_LOGS)

static void test_logs_proto_wire_compatibility(void) {
  otlp_log_record record;
  otlp_log_record decoded_record;
  otlp_export_logs_partial_success partial_success;
  otlp_export_logs_service_response response;
  otlp__writer writer;
  uint8_t encoded_record[32];
  uint8_t encoded_response[64];
  const uint8_t event_name_field[] = { 0x62, 0x03, 'e', 'v', 't' };
  const uint8_t reserved_event_name_field[] = { 0x22, 0x03, 'e', 'v', 't' };
  const uint8_t rejected_minus_one[] = {
    0x08, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01
  };
  const uint8_t split_body[] = {
    0x2a, 0x07, 0x2a, 0x05, 0x0a, 0x03, 0x0a, 0x01, 'a',
    0x2a, 0x07, 0x2a, 0x05, 0x0a, 0x03, 0x0a, 0x01, 'b'
  };
  const uint8_t split_partial_success[] = {
    0x0a, 0x02, 0x08, 0x01,
    0x0a, 0x03, 0x12, 0x01, 'x'
  };
  size_t record_size;
  size_t response_size;

  CHECK(OTLP_LOG_RECORD_FLAGS_DO_NOT_USE == 0U);
  CHECK(OTLP_LOG_RECORD_FLAGS_TRACE_FLAGS_MASK == 0x000000FFU);

  otlp_log_record_init(&record);
  CHECK(otlp__set_string(&record.event_name, "evt", 3) == OTLP_STATUS_OK);
  record_size = otlp_log_record_encoded_size_inner(&record);
  otlp__writer_init(&writer, encoded_record, sizeof(encoded_record));
  otlp_log_record_encode_field(&writer, 1, &record);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, record_size));
  CHECK(otlp_test_contains_bytes(encoded_record, writer.off, event_name_field, sizeof(event_name_field)));
  CHECK(!otlp_test_contains_bytes(encoded_record, writer.off, reserved_event_name_field, sizeof(reserved_event_name_field)));
  otlp_log_record_destroy(&record);

  otlp_log_record_init(&decoded_record);
  CHECK(otlp_log_record_decode_inner(&decoded_record, split_body,
                                      sizeof(split_body)) == OTLP_STATUS_OK);
  CHECK(decoded_record.has_body == 1);
  CHECK(decoded_record.body.kind == OTLP_ANY_VALUE_ARRAY);
  CHECK(decoded_record.body.value.array_value->values.len == 2);
  otlp_log_record_destroy(&decoded_record);

  otlp_export_logs_partial_success_init(&partial_success);
  partial_success.rejected_log_records = -1;
  response_size = otlp_export_logs_partial_success_encoded_size_inner(&partial_success);
  otlp__writer_init(&writer, encoded_response, sizeof(encoded_response));
  otlp_export_logs_partial_success_encode_field(&writer, 1, &partial_success);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, response_size));
  CHECK(otlp_test_contains_bytes(encoded_response, writer.off, rejected_minus_one, sizeof(rejected_minus_one)));

  otlp_export_logs_service_response_init(&response);
  CHECK(otlp_export_logs_service_response_decode(&response, encoded_response, writer.off) == OTLP_STATUS_OK);
  CHECK(response.has_partial_success == 1);
  CHECK(response.partial_success.rejected_log_records == -1);
  otlp_export_logs_service_response_destroy(&response);

  otlp_export_logs_service_response_init(&response);
  CHECK(otlp_export_logs_service_response_decode(
             &response, split_partial_success,
             sizeof(split_partial_success)) == OTLP_STATUS_OK);
  CHECK(response.has_partial_success == 1);
  CHECK(response.partial_success.rejected_log_records == 1);
  CHECK(response.partial_success.error_message.len == 1);
  otlp_export_logs_service_response_destroy(&response);
  otlp_export_logs_partial_success_destroy(&partial_success);
  otlp_test_ok("logs proto wire compatibility");
}

static void test_logs_round_trip(void) {
  otlp_any_value body;
  otlp_any_value_init_string(&body, "hello", 5);

  otlp_log_record log_record;
  otlp_log_record_init(&log_record);
  otlp_log_record_set_time_unix_nano(&log_record, 123);
  otlp_log_record_set_severity_number(&log_record, OTLP_SEVERITY_NUMBER_INFO);
  otlp_log_record_set_body(&log_record, &body);

  otlp_scope_logs scope_logs;
  otlp_scope_logs_init(&scope_logs);
  otlp_scope_logs_append_log_record(&scope_logs, &log_record);

  otlp_resource_logs resource_logs;
  otlp_resource_logs_init(&resource_logs);
  otlp_resource_logs_append_scope_logs(&resource_logs, &scope_logs);

  otlp_export_logs_service_request request;
  otlp_export_logs_service_request_init(&request);
  otlp_export_logs_service_request_append_resource_logs(&request, &resource_logs);

  size_t encoded_size = otlp_export_logs_service_request_encoded_size(&request);
  CHECK(encoded_size > 0);

  uint8_t *encoded = (uint8_t *) malloc(encoded_size);
  CHECK(encoded != NULL);

  CHECK(otlp_export_logs_service_request_encode(&request, encoded, encoded_size) == OTLP_STATUS_OK);

  otlp_export_logs_service_request decoded;
  otlp_export_logs_service_request_init(&decoded);
  CHECK(otlp_export_logs_service_request_decode(&decoded, encoded, encoded_size) == OTLP_STATUS_OK);
  CHECK(decoded.resource_logs.len == 1);
  CHECK(decoded.resource_logs.ptr[0].scope_logs.len == 1);
  CHECK(decoded.resource_logs.ptr[0].scope_logs.ptr[0].log_records.len == 1);

  otlp_export_logs_service_request_destroy(&decoded);
  otlp_export_logs_service_request_destroy(&request);
  otlp_resource_logs_destroy(&resource_logs);
  otlp_scope_logs_destroy(&scope_logs);
  otlp_log_record_destroy(&log_record);
  otlp_any_value_destroy(&body);
  free(encoded);
  otlp_test_ok("logs round trip");
}

static void test_logs_size_matches_encode_exactly(void) {
  otlp_export_logs_service_request request;
  otlp_resource_logs resource_logs;
  otlp_scope_logs scope_logs;
  otlp_log_record record;
  otlp_any_value body;
  uint8_t *encoded;
  size_t size;
  otlp_status status;
  size_t actual;

  otlp_export_logs_service_request_init(&request);
  otlp_resource_logs_init(&resource_logs);
  otlp_scope_logs_init(&scope_logs);
  otlp_log_record_init(&record);
  otlp_any_value_init(&body);

  record.time_unix_nano = 123;
  record.observed_time_unix_nano = 456;
  record.severity_number = OTLP_SEVERITY_NUMBER_WARN;
  tap_equal(g_current_tap, otlp__set_string(&record.severity_text, "warn", 4),
            OTLP_STATUS_OK, "set severity_text");
  tap_equal(g_current_tap, otlp__set_string(&record.event_name, "evt", 3),
            OTLP_STATUS_OK, "set event_name");
  tap_equal(g_current_tap, otlp_any_value_init_string(&body, "hello", 5),
            OTLP_STATUS_OK, "init body string");
  tap_equal(g_current_tap, otlp_log_record_set_body(&record, &body),
            OTLP_STATUS_OK, "set log body");
  record.flags = 3;

  tap_equal(g_current_tap, otlp_scope_logs_append_log_record(&scope_logs, &record),
            OTLP_STATUS_OK, "append log record");
  tap_equal(g_current_tap, otlp_resource_logs_append_scope_logs(&resource_logs, &scope_logs),
            OTLP_STATUS_OK, "append scope logs");
  tap_equal(g_current_tap,
            otlp_export_logs_service_request_append_resource_logs(&request, &resource_logs),
            OTLP_STATUS_OK, "append resource logs");

  size = otlp_export_logs_service_request_encoded_size(&request);
  encoded = (uint8_t *)malloc(size);
  tap_ok(g_current_tap, encoded != NULL, "allocated logs encode buffer");
  if (encoded == NULL) {
    exit(1);
  }
  status = otlp_export_logs_service_request_encode(&request, encoded, size);
  tap_equal(g_current_tap, status, OTLP_STATUS_OK, "logs request encode succeeded");

  {
    otlp__writer writer;
    otlp__writer_init(&writer, encoded, size);
    actual = 0;
    if (request.resource_logs.len > 0) {
      size_t i;
      for (i = 0; i < request.resource_logs.len; ++i) {
        otlp_resource_logs_encode_field(&writer, 1, &request.resource_logs.ptr[i]);
      }
      actual = writer.off;
      tap_equal(g_current_tap, writer.status, OTLP_STATUS_OK, "manual logs encode succeeded");
    }
  }
  tap_equal(g_current_tap, actual, size, "logs encoded size matches manual writer offset");

  free(encoded);
  otlp_any_value_destroy(&body);
  otlp_log_record_destroy(&record);
  otlp_scope_logs_destroy(&scope_logs);
  otlp_resource_logs_destroy(&resource_logs);
  otlp_export_logs_service_request_destroy(&request);

  otlp_test_ok("logs size matches exact encode");
}

static void test_logs_round_trip_full_fields(void) {
  otlp_any_value body;
  otlp_any_value attr_value;
  otlp_key_value attribute;
  otlp_log_record log_record;
  otlp_scope_logs scope_logs;
  otlp_resource_logs resource_logs;
  otlp_export_logs_service_request request;
  otlp_export_logs_service_request decoded;
  uint8_t trace_id_bytes[16] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
  };
  uint8_t span_id_bytes[8] = { 20, 21, 22, 23, 24, 25, 26, 27 };
  size_t encoded_size;
  uint8_t *encoded;

  CHECK(otlp_any_value_init_string(&body, "body", 4) == OTLP_STATUS_OK);
  CHECK(otlp_any_value_init_string(&attr_value, "component-a", 11) == OTLP_STATUS_OK);

  otlp_key_value_init(&attribute);
  CHECK(otlp__set_string(&attribute.key, "component", 9) == OTLP_STATUS_OK);
  CHECK(otlp_any_value_copy(&attribute.value, &attr_value) == OTLP_STATUS_OK);
  attribute.has_value = 1;

  otlp_log_record_init(&log_record);
  otlp_log_record_set_time_unix_nano(&log_record, 123);
  log_record.observed_time_unix_nano = 456;
  otlp_log_record_set_severity_number(&log_record, OTLP_SEVERITY_NUMBER_ERROR);
  CHECK(otlp__set_string(&log_record.severity_text, "ERROR", 5) == OTLP_STATUS_OK);
  CHECK(otlp_log_record_set_body(&log_record, &body) == OTLP_STATUS_OK);
  CHECK(
    otlp__append_primitive(
      (void **) &log_record.attributes.ptr,
      &log_record.attributes.len,
      &log_record.attributes.cap,
      sizeof(attribute),
      &attribute
    ) == OTLP_STATUS_OK
  );
  attribute.key.ptr = NULL;
  attribute.key.len = 0;
  otlp_any_value_init(&attribute.value);
  log_record.dropped_attributes_count = 3;
  log_record.flags = 0xAB;
  CHECK(otlp__set_bytes(&log_record.trace_id, trace_id_bytes, sizeof(trace_id_bytes)) == OTLP_STATUS_OK);
  CHECK(otlp__set_bytes(&log_record.span_id, span_id_bytes, sizeof(span_id_bytes)) == OTLP_STATUS_OK);

  otlp_scope_logs_init(&scope_logs);
  CHECK(otlp__set_string(&scope_logs.schema_url, "scope-schema", 12) == OTLP_STATUS_OK);
  CHECK(otlp_scope_logs_append_log_record(&scope_logs, &log_record) == OTLP_STATUS_OK);

  otlp_resource_logs_init(&resource_logs);
  CHECK(otlp__set_string(&resource_logs.schema_url, "resource-schema", 15) == OTLP_STATUS_OK);
  CHECK(otlp_resource_logs_append_scope_logs(&resource_logs, &scope_logs) == OTLP_STATUS_OK);

  otlp_export_logs_service_request_init(&request);
  CHECK(otlp_export_logs_service_request_append_resource_logs(&request, &resource_logs) == OTLP_STATUS_OK);

  encoded_size = otlp_export_logs_service_request_encoded_size(&request);
  encoded = (uint8_t *) malloc(encoded_size);
  CHECK(encoded != NULL);
  CHECK(otlp_export_logs_service_request_encode(&request, encoded, encoded_size) == OTLP_STATUS_OK);

  otlp_export_logs_service_request_init(&decoded);
  CHECK(otlp_export_logs_service_request_decode(&decoded, encoded, encoded_size) == OTLP_STATUS_OK);
  CHECK(decoded.resource_logs.len == 1);
  CHECK(decoded.resource_logs.ptr[0].scope_logs.len == 1);
  CHECK(decoded.resource_logs.ptr[0].scope_logs.ptr[0].log_records.len == 1);
  CHECK(decoded.resource_logs.ptr[0].scope_logs.ptr[0].log_records.ptr[0].observed_time_unix_nano == 456);
  CHECK(decoded.resource_logs.ptr[0].scope_logs.ptr[0].log_records.ptr[0].severity_number == OTLP_SEVERITY_NUMBER_ERROR);
  CHECK(decoded.resource_logs.ptr[0].scope_logs.ptr[0].log_records.ptr[0].severity_text.len == 5);
  CHECK(decoded.resource_logs.ptr[0].scope_logs.ptr[0].log_records.ptr[0].has_body == 1);
  CHECK(decoded.resource_logs.ptr[0].scope_logs.ptr[0].log_records.ptr[0].attributes.len == 1);
  CHECK(decoded.resource_logs.ptr[0].scope_logs.ptr[0].log_records.ptr[0].dropped_attributes_count == 3);
  CHECK(decoded.resource_logs.ptr[0].scope_logs.ptr[0].log_records.ptr[0].flags == 0xAB);
  CHECK(decoded.resource_logs.ptr[0].scope_logs.ptr[0].log_records.ptr[0].trace_id.len == 16);
  CHECK(decoded.resource_logs.ptr[0].scope_logs.ptr[0].log_records.ptr[0].span_id.len == 8);
  CHECK(decoded.resource_logs.ptr[0].schema_url.len == 15);
  CHECK(decoded.resource_logs.ptr[0].scope_logs.ptr[0].schema_url.len == 12);

  otlp_export_logs_service_request_destroy(&decoded);
  otlp_export_logs_service_request_destroy(&request);
  otlp_resource_logs_destroy(&resource_logs);
  otlp_scope_logs_destroy(&scope_logs);
  otlp_log_record_destroy(&log_record);
  otlp_key_value_destroy(&attribute);
  otlp_any_value_destroy(&attr_value);
  otlp_any_value_destroy(&body);
  free(encoded);
  otlp_test_ok("logs round trip full fields");
}

static void test_logs_decode_unknown_and_truncated(void) {
  otlp_any_value body;
  otlp_log_record log_record;
  otlp_scope_logs scope_logs;
  otlp_resource_logs resource_logs;
  otlp_export_logs_service_request request;
  otlp_export_logs_service_request decoded;
  size_t encoded_size;
  size_t extended_size;
  uint8_t *encoded;
  uint8_t *extended;

  CHECK(otlp_any_value_init_string(&body, "hello", 5) == OTLP_STATUS_OK);
  otlp_log_record_init(&log_record);
  CHECK(otlp_log_record_set_body(&log_record, &body) == OTLP_STATUS_OK);
  otlp_scope_logs_init(&scope_logs);
  CHECK(otlp_scope_logs_append_log_record(&scope_logs, &log_record) == OTLP_STATUS_OK);
  otlp_resource_logs_init(&resource_logs);
  CHECK(otlp_resource_logs_append_scope_logs(&resource_logs, &scope_logs) == OTLP_STATUS_OK);
  otlp_export_logs_service_request_init(&request);
  CHECK(otlp_export_logs_service_request_append_resource_logs(&request, &resource_logs) == OTLP_STATUS_OK);

  encoded_size = otlp_export_logs_service_request_encoded_size(&request);
  encoded = (uint8_t *) malloc(encoded_size);
  CHECK(encoded != NULL);
  CHECK(otlp_export_logs_service_request_encode(&request, encoded, encoded_size) == OTLP_STATUS_OK);

  extended = otlp_test_append_unknown_varint_field(encoded, encoded_size, 99, 7, &extended_size);
  otlp_export_logs_service_request_init(&decoded);
  CHECK(otlp_export_logs_service_request_decode(&decoded, extended, extended_size) == OTLP_STATUS_OK);
  CHECK(decoded.resource_logs.len == 1);
  otlp_export_logs_service_request_destroy(&decoded);

  otlp_export_logs_service_request_init(&decoded);
  CHECK(
    otlp_export_logs_service_request_decode(&decoded, encoded, encoded_size - 1) ==
    OTLP_STATUS_DECODE_ERROR
  );
  otlp_export_logs_service_request_destroy(&decoded);

  free(extended);
  free(encoded);
  otlp_export_logs_service_request_destroy(&request);
  otlp_resource_logs_destroy(&resource_logs);
  otlp_scope_logs_destroy(&scope_logs);
  otlp_log_record_destroy(&log_record);
  otlp_any_value_destroy(&body);
  otlp_test_ok("logs decode unknown and truncated");
}

static void test_logs_encode_buffer_too_small(void) {
  otlp_any_value body;
  otlp_log_record log_record;
  otlp_scope_logs scope_logs;
  otlp_resource_logs resource_logs;
  otlp_export_logs_service_request request;
  size_t encoded_size;
  uint8_t *encoded;

  CHECK(otlp_any_value_init_string(&body, "hello", 5) == OTLP_STATUS_OK);
  otlp_log_record_init(&log_record);
  CHECK(otlp_log_record_set_body(&log_record, &body) == OTLP_STATUS_OK);
  otlp_scope_logs_init(&scope_logs);
  CHECK(otlp_scope_logs_append_log_record(&scope_logs, &log_record) == OTLP_STATUS_OK);
  otlp_resource_logs_init(&resource_logs);
  CHECK(otlp_resource_logs_append_scope_logs(&resource_logs, &scope_logs) == OTLP_STATUS_OK);
  otlp_export_logs_service_request_init(&request);
  CHECK(otlp_export_logs_service_request_append_resource_logs(&request, &resource_logs) == OTLP_STATUS_OK);

  encoded_size = otlp_export_logs_service_request_encoded_size(&request);
  encoded = (uint8_t *) malloc(encoded_size - 1);
  CHECK(encoded != NULL);
  CHECK(otlp_export_logs_service_request_encode(&request, encoded, encoded_size - 1) == OTLP_STATUS_BUFFER_TOO_SMALL);

  free(encoded);
  otlp_export_logs_service_request_destroy(&request);
  otlp_resource_logs_destroy(&resource_logs);
  otlp_scope_logs_destroy(&scope_logs);
  otlp_log_record_destroy(&log_record);
  otlp_any_value_destroy(&body);
  otlp_test_ok("logs encode buffer too small");
}

static void test_logs_data_and_response_round_trip(void) {
  otlp_any_value string_item;
  otlp_any_value nested_value;
  otlp_any_value resource_value;
  otlp_any_value kvlist_value;
  otlp_any_value body;
  otlp_key_value nested_kv;
  otlp_key_value resource_attr;
  otlp_entity_ref entity_ref;
  otlp_log_record record;
  otlp_scope_logs scope_logs;
  otlp_resource_logs resource_logs;
  otlp_logs_data data;
  otlp_logs_data decoded_data;
  otlp_export_logs_service_response response;
  otlp_export_logs_service_response decoded_response;
  size_t data_size;
  size_t response_size;
  uint8_t *data_bytes;
  uint8_t *response_bytes;

  CHECK(otlp_any_value_init_string(&string_item, "nested", 6) == OTLP_STATUS_OK);
  CHECK(otlp_any_value_init_string(&nested_value, "nested-value", 12) == OTLP_STATUS_OK);
  CHECK(otlp_any_value_init_string(&resource_value, "resource-value", 14) == OTLP_STATUS_OK);
  otlp_key_value_init(&nested_kv);
  CHECK(otlp__set_string(&nested_kv.key, "nested-key", 10) == OTLP_STATUS_OK);
  CHECK(otlp_any_value_copy(&nested_kv.value, &nested_value) == OTLP_STATUS_OK);
  nested_kv.has_value = 1;
  otlp_any_value_init(&kvlist_value);
  kvlist_value.kind = OTLP_ANY_VALUE_KVLIST;
  kvlist_value.value.kvlist_value =
      (otlp_key_value_list *)malloc(sizeof(*kvlist_value.value.kvlist_value));
  CHECK(kvlist_value.value.kvlist_value != NULL);
  otlp_key_value_list_init(kvlist_value.value.kvlist_value);
  CHECK(otlp_key_value_list_append_value(kvlist_value.value.kvlist_value, &nested_kv) == OTLP_STATUS_OK);
  otlp_any_value_init(&body);
  body.kind = OTLP_ANY_VALUE_ARRAY;
  body.value.array_value =
      (otlp_array_value *)malloc(sizeof(*body.value.array_value));
  CHECK(body.value.array_value != NULL);
  otlp_array_value_init(body.value.array_value);
  CHECK(otlp_array_value_append_value(body.value.array_value, &string_item) == OTLP_STATUS_OK);
  CHECK(otlp_array_value_append_value(body.value.array_value, &kvlist_value) == OTLP_STATUS_OK);

  otlp_key_value_init(&resource_attr);
  CHECK(otlp__set_string(&resource_attr.key, "resource-key", 12) == OTLP_STATUS_OK);
  CHECK(otlp_any_value_copy(&resource_attr.value, &resource_value) == OTLP_STATUS_OK);
  resource_attr.has_value = 1;

  otlp_entity_ref_init(&entity_ref);
  CHECK(otlp__set_string(&entity_ref.schema_url, "entity-schema", 13) == OTLP_STATUS_OK);
  CHECK(otlp__set_string(&entity_ref.type, "service.instance", 16) == OTLP_STATUS_OK);
  {
    otlp_string description_key;
    otlp__zero(&description_key, sizeof(description_key));
    CHECK(otlp__set_string(&description_key, "service.name", 12) == OTLP_STATUS_OK);
    CHECK(otlp__append_primitive((void **)&entity_ref.description_keys.ptr,
                                  &entity_ref.description_keys.len,
                                  &entity_ref.description_keys.cap,
                                  sizeof(description_key),
                                  &description_key) == OTLP_STATUS_OK);
  }
  {
    otlp_string id_key;
    otlp__zero(&id_key, sizeof(id_key));
    CHECK(otlp__set_string(&id_key, "service.instance.id", 19) == OTLP_STATUS_OK);
    CHECK(otlp__append_primitive((void **)&entity_ref.id_keys.ptr,
                                  &entity_ref.id_keys.len,
                                  &entity_ref.id_keys.cap, sizeof(id_key),
                                  &id_key) == OTLP_STATUS_OK);
  }

  otlp_log_record_init(&record);
  record.time_unix_nano = 101;
  record.observed_time_unix_nano = 202;
  record.severity_number = OTLP_SEVERITY_NUMBER_INFO;
  CHECK(otlp__set_string(&record.severity_text, "INFO", 4) == OTLP_STATUS_OK);
  CHECK(otlp__set_string(&record.event_name, "app.event", 9) == OTLP_STATUS_OK);
  CHECK(otlp_log_record_set_body(&record, &body) == OTLP_STATUS_OK);

  otlp_scope_logs_init(&scope_logs);
  scope_logs.has_scope = 1;
  CHECK(otlp__set_string(&scope_logs.scope.name, "logger", 6) == OTLP_STATUS_OK);
  CHECK(otlp__set_string(&scope_logs.scope.version, "1.2.3", 5) == OTLP_STATUS_OK);
  scope_logs.scope.dropped_attributes_count = 1;
  CHECK(otlp_scope_logs_append_log_record(&scope_logs, &record) == OTLP_STATUS_OK);

  otlp_resource_logs_init(&resource_logs);
  resource_logs.has_resource = 1;
  resource_logs.resource.dropped_attributes_count = 2;
  CHECK(otlp__append_primitive((void **)&resource_logs.resource.attributes.ptr,
                                &resource_logs.resource.attributes.len,
                                &resource_logs.resource.attributes.cap,
                                sizeof(resource_attr),
                                &resource_attr) == OTLP_STATUS_OK);
  otlp_key_value_init(&resource_attr);
  CHECK(otlp__append_primitive((void **)&resource_logs.resource.entity_refs.ptr,
                                &resource_logs.resource.entity_refs.len,
                                &resource_logs.resource.entity_refs.cap,
                                sizeof(entity_ref),
                                &entity_ref) == OTLP_STATUS_OK);
  otlp_entity_ref_init(&entity_ref);
  CHECK(otlp_resource_logs_append_scope_logs(&resource_logs, &scope_logs) == OTLP_STATUS_OK);

  otlp_logs_data_init(&data);
  CHECK(otlp__append_primitive((void **)&data.resource_logs.ptr,
                                &data.resource_logs.len,
                                &data.resource_logs.cap,
                                sizeof(resource_logs),
                                &resource_logs) == OTLP_STATUS_OK);
  otlp_resource_logs_init(&resource_logs);
  data_size = otlp_logs_data_encoded_size(&data);
  data_bytes = (uint8_t *)malloc(data_size);
  CHECK(data_bytes != NULL);
  CHECK(otlp_logs_data_encode(&data, data_bytes, data_size) == OTLP_STATUS_OK);
  otlp_logs_data_init(&decoded_data);
  CHECK(otlp_logs_data_decode(&decoded_data, data_bytes, data_size) == OTLP_STATUS_OK);
  CHECK(decoded_data.resource_logs.len == 1);
  CHECK(decoded_data.resource_logs.ptr[0].has_resource == 1);
  CHECK(decoded_data.resource_logs.ptr[0].resource.attributes.len == 1);
  CHECK(decoded_data.resource_logs.ptr[0].resource.attributes.ptr[0].key.len == 12);
  CHECK(decoded_data.resource_logs.ptr[0].resource.attributes.ptr[0].value.kind == OTLP_ANY_VALUE_STRING);
  CHECK(decoded_data.resource_logs.ptr[0].resource.entity_refs.len == 1);
  CHECK(decoded_data.resource_logs.ptr[0].resource.entity_refs.ptr[0].description_keys.len == 1);
  CHECK(decoded_data.resource_logs.ptr[0].scope_logs.ptr[0].has_scope == 1);
  CHECK(decoded_data.resource_logs.ptr[0].scope_logs.ptr[0].scope.name.len == 6);
  CHECK(decoded_data.resource_logs.ptr[0].scope_logs.ptr[0].log_records.ptr[0].event_name.len == 9);
  CHECK(decoded_data.resource_logs.ptr[0].scope_logs.ptr[0].log_records.ptr[0].body.kind == OTLP_ANY_VALUE_ARRAY);
  CHECK(decoded_data.resource_logs.ptr[0].scope_logs.ptr[0].log_records.ptr[0].body.value.array_value->values.len == 2);
  CHECK(decoded_data.resource_logs.ptr[0].scope_logs.ptr[0].log_records.ptr[0].body.value.array_value->values.ptr[1].kind == OTLP_ANY_VALUE_KVLIST);

  otlp_export_logs_service_response_init(&response);
  response.has_partial_success = 1;
  response.partial_success.rejected_log_records = 5;
  CHECK(otlp__set_string(&response.partial_success.error_message, "partial", 7) == OTLP_STATUS_OK);
  response_size = otlp_export_logs_service_response_encoded_size(&response);
  response_bytes = (uint8_t *)malloc(response_size);
  CHECK(response_bytes != NULL);
  CHECK(otlp_export_logs_service_response_encode(&response, response_bytes, response_size) == OTLP_STATUS_OK);
  otlp_export_logs_service_response_init(&decoded_response);
  CHECK(otlp_export_logs_service_response_decode(&decoded_response, response_bytes, response_size) == OTLP_STATUS_OK);
  CHECK(decoded_response.has_partial_success == 1);
  CHECK(decoded_response.partial_success.rejected_log_records == 5);
  CHECK(decoded_response.partial_success.error_message.len == 7);

  otlp_export_logs_service_response_destroy(&decoded_response);
  otlp_export_logs_service_response_destroy(&response);
  otlp_logs_data_destroy(&decoded_data);
  otlp_logs_data_destroy(&data);
  otlp_resource_logs_destroy(&resource_logs);
  otlp_scope_logs_destroy(&scope_logs);
  otlp_log_record_destroy(&record);
  otlp_entity_ref_destroy(&entity_ref);
  otlp_key_value_destroy(&resource_attr);
  otlp_key_value_destroy(&nested_kv);
  otlp_any_value_destroy(&body);
  otlp_any_value_destroy(&kvlist_value);
  otlp_any_value_destroy(&resource_value);
  otlp_any_value_destroy(&nested_value);
  otlp_any_value_destroy(&string_item);
  free(data_bytes);
  free(response_bytes);
  otlp_test_ok("logs data and response round trip");
}

OTLP_DEFINE_SUBTEST(test_logs_proto_wire_compatibility)
OTLP_DEFINE_SUBTEST(test_logs_round_trip)
OTLP_DEFINE_SUBTEST(test_logs_size_matches_encode_exactly)
OTLP_DEFINE_SUBTEST(test_logs_round_trip_full_fields)
OTLP_DEFINE_SUBTEST(test_logs_decode_unknown_and_truncated)
OTLP_DEFINE_SUBTEST(test_logs_encode_buffer_too_small)
OTLP_DEFINE_SUBTEST(test_logs_data_and_response_round_trip)

#endif /* OTLP_FORMAT_ENABLE_LOGS */

int test_logs_count(void) {
#if defined(OTLP_FORMAT_ENABLE_LOGS)
  return 7;
#else
  return 0;
#endif
}

void test_logs_run(tap_t *t) {
#if defined(OTLP_FORMAT_ENABLE_LOGS)
  tap_test(t, "logs proto wire compatibility", test_logs_proto_wire_compatibility_subtest, NULL);
  tap_test(t, "logs round trip", test_logs_round_trip_subtest, NULL);
  tap_test(t, "logs size matches exact encode", test_logs_size_matches_encode_exactly_subtest, NULL);
  tap_test(t, "logs round trip full fields", test_logs_round_trip_full_fields_subtest, NULL);
  tap_test(t, "logs encode buffer too small", test_logs_encode_buffer_too_small_subtest, NULL);
  tap_test(t, "logs decode unknown and truncated", test_logs_decode_unknown_and_truncated_subtest, NULL);
  tap_test(t, "logs data and response round trip", test_logs_data_and_response_round_trip_subtest, NULL);
#else
  (void)t;
#endif
}
