#include "test_helpers.h"

#if defined(OTLP_FORMAT_ENABLE_TRACES)

static void test_traces_proto_wire_compatibility(void) {
  otlp_span span;
  otlp_span decoded_span;
  otlp_span_event event;
  otlp_export_trace_partial_success partial_success;
  otlp_export_trace_service_response response;
  otlp__writer writer;
  uint8_t encoded_span[32];
  uint8_t encoded_event[32];
  uint8_t encoded_response[64];
  const uint8_t empty_status_field[] = { 0x7a, 0x00 };
  const uint8_t rejected_minus_one[] = {
    0x08, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01
  };
  const uint8_t split_status[] = {
    0x7a, 0x02, 0x18, 0x02,
    0x7a, 0x03, 0x12, 0x01, 'x'
  };
  const uint8_t split_partial_success[] = {
    0x0a, 0x02, 0x08, 0x01,
    0x0a, 0x03, 0x12, 0x01, 'x'
  };
  size_t span_size;
  size_t event_size;
  size_t response_size;

  otlp_span_init(&span);
  span_size = otlp_span_encoded_size_inner(&span);
  otlp__writer_init(&writer, encoded_span, sizeof(encoded_span));
  otlp_span_encode_field(&writer, 1, &span);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, span_size));
  otlp_span_destroy(&span);

  otlp_span_init(&span);
  span.has_status = 1;
  span_size = otlp_span_encoded_size_inner(&span);
  otlp__writer_init(&writer, encoded_span, sizeof(encoded_span));
  otlp_span_encode_field(&writer, 1, &span);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, span_size));
  CHECK(otlp_test_contains_bytes(encoded_span, writer.off, empty_status_field, sizeof(empty_status_field)));
  otlp_span_destroy(&span);

  otlp_span_init(&decoded_span);
  CHECK(otlp_span_decode_inner(&decoded_span, split_status,
                                sizeof(split_status)) == OTLP_STATUS_OK);
  CHECK(decoded_span.has_status == 1);
  CHECK(decoded_span.status.code == OTLP_TRACE_STATUS_CODE_ERROR);
  CHECK(decoded_span.status.message.len == 1);
  otlp_span_destroy(&decoded_span);

  otlp_span_event_init(&event);
  event_size = otlp_span_event_encoded_size_inner(&event);
  otlp__writer_init(&writer, encoded_event, sizeof(encoded_event));
  otlp_span_event_encode_field(&writer, 1, &event);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, event_size));
  otlp_span_event_destroy(&event);

  otlp_export_trace_partial_success_init(&partial_success);
  partial_success.rejected_spans = -1;
  response_size = otlp_export_trace_partial_success_encoded_size_inner(&partial_success);
  otlp__writer_init(&writer, encoded_response, sizeof(encoded_response));
  otlp_export_trace_partial_success_encode_field(&writer, 1, &partial_success);
  CHECK(writer.status == OTLP_STATUS_OK);
  CHECK(writer.off == otlp__message_field_size(1, response_size));
  CHECK(otlp_test_contains_bytes(encoded_response, writer.off, rejected_minus_one, sizeof(rejected_minus_one)));

  otlp_export_trace_service_response_init(&response);
  CHECK(otlp_export_trace_service_response_decode(&response, encoded_response, writer.off) == OTLP_STATUS_OK);
  CHECK(response.has_partial_success == 1);
  CHECK(response.partial_success.rejected_spans == -1);
  otlp_export_trace_service_response_destroy(&response);

  otlp_export_trace_service_response_init(&response);
  CHECK(otlp_export_trace_service_response_decode(
             &response, split_partial_success,
             sizeof(split_partial_success)) == OTLP_STATUS_OK);
  CHECK(response.has_partial_success == 1);
  CHECK(response.partial_success.rejected_spans == 1);
  CHECK(response.partial_success.error_message.len == 1);
  otlp_export_trace_service_response_destroy(&response);
  otlp_export_trace_partial_success_destroy(&partial_success);
  otlp_test_ok("traces proto wire compatibility");
}

static void test_traces_round_trip(void) {
  otlp_key_value attribute;
  otlp_any_value attr_value;
  otlp_span span;
  otlp_scope_spans scope_spans;
  otlp_resource_spans resource_spans;
  otlp_export_trace_service_request request;
  otlp_export_trace_service_request decoded;
  uint8_t trace_id[16] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 };
  uint8_t span_id[8] = { 3, 0, 0, 0, 0, 0, 0, 4 };
  size_t encoded_size;
  uint8_t *encoded;

  CHECK(otlp_any_value_init_string(&attr_value, "server", 6) == OTLP_STATUS_OK);
  otlp_key_value_init(&attribute);
  CHECK(otlp__set_string(&attribute.key, "component", 9) == OTLP_STATUS_OK);
  CHECK(otlp_any_value_copy(&attribute.value, &attr_value) == OTLP_STATUS_OK);
  attribute.has_value = 1;

  otlp_span_init(&span);
  CHECK(otlp__set_bytes(&span.trace_id, trace_id, sizeof(trace_id)) == OTLP_STATUS_OK);
  CHECK(otlp__set_bytes(&span.span_id, span_id, sizeof(span_id)) == OTLP_STATUS_OK);
  CHECK(otlp__set_string(&span.name, "http.request", 12) == OTLP_STATUS_OK);
  span.kind = OTLP_SPAN_KIND_SERVER;
  span.start_time_unix_nano = 100;
  span.end_time_unix_nano = 200;
  span.flags = 1;
  CHECK(otlp__append_primitive((void **) &span.attributes.ptr, &span.attributes.len, &span.attributes.cap, sizeof(attribute), &attribute) == OTLP_STATUS_OK);
  attribute.key.ptr = NULL;
  attribute.key.len = 0;
  otlp_any_value_init(&attribute.value);

  otlp_scope_spans_init(&scope_spans);
  CHECK(otlp_scope_spans_append_span(&scope_spans, &span) == OTLP_STATUS_OK);
  otlp_resource_spans_init(&resource_spans);
  CHECK(otlp_resource_spans_append_scope_spans(&resource_spans, &scope_spans) == OTLP_STATUS_OK);
  otlp_export_trace_service_request_init(&request);
  CHECK(otlp_export_trace_service_request_append_resource_spans(&request, &resource_spans) == OTLP_STATUS_OK);

  encoded_size = otlp_export_trace_service_request_encoded_size(&request);
  encoded = (uint8_t *) malloc(encoded_size);
  CHECK(encoded != NULL);
  CHECK(otlp_export_trace_service_request_encode(&request, encoded, encoded_size) == OTLP_STATUS_OK);

  otlp_export_trace_service_request_init(&decoded);
  CHECK(otlp_export_trace_service_request_decode(&decoded, encoded, encoded_size) == OTLP_STATUS_OK);
  CHECK(decoded.resource_spans.len == 1);
  CHECK(decoded.resource_spans.ptr[0].scope_spans.len == 1);
  CHECK(decoded.resource_spans.ptr[0].scope_spans.ptr[0].spans.len == 1);
  CHECK(decoded.resource_spans.ptr[0].scope_spans.ptr[0].spans.ptr[0].kind == OTLP_SPAN_KIND_SERVER);
  CHECK(decoded.resource_spans.ptr[0].scope_spans.ptr[0].spans.ptr[0].attributes.len == 1);
  CHECK(decoded.resource_spans.ptr[0].scope_spans.ptr[0].spans.ptr[0].trace_id.len == 16);

  otlp_export_trace_service_request_destroy(&decoded);
  otlp_export_trace_service_request_destroy(&request);
  otlp_resource_spans_destroy(&resource_spans);
  otlp_scope_spans_destroy(&scope_spans);
  otlp_span_destroy(&span);
  otlp_key_value_destroy(&attribute);
  otlp_any_value_destroy(&attr_value);
  free(encoded);
  otlp_test_ok("traces round trip");
}

static void test_traces_size_matches_encode_exactly(void) {
  otlp_export_trace_service_request request;
  otlp_resource_spans resource_spans;
  otlp_scope_spans scope_spans;
  otlp_span span;
  uint8_t trace_id[16];
  uint8_t span_id[8];
  uint8_t *encoded;
  size_t size;
  size_t actual;
  size_t i;

  for (i = 0; i < 16; ++i) trace_id[i] = (uint8_t)(i + 1U);
  for (i = 0; i < 8; ++i) span_id[i] = (uint8_t)(i + 1U);

  otlp_export_trace_service_request_init(&request);
  otlp_resource_spans_init(&resource_spans);
  otlp_scope_spans_init(&scope_spans);
  otlp_span_init(&span);

  tap_equal(g_current_tap, otlp__set_bytes(&span.trace_id, trace_id, sizeof(trace_id)),
            OTLP_STATUS_OK, "set trace_id");
  tap_equal(g_current_tap, otlp__set_bytes(&span.span_id, span_id, sizeof(span_id)),
            OTLP_STATUS_OK, "set span_id");
  tap_equal(g_current_tap, otlp__set_string(&span.name, "span", 4),
            OTLP_STATUS_OK, "set span name");
  span.kind = OTLP_SPAN_KIND_SERVER;
  span.start_time_unix_nano = 10;
  span.end_time_unix_nano = 20;
  span.flags = OTLP_SPAN_FLAGS_CONTEXT_HAS_IS_REMOTE_MASK;

  tap_equal(g_current_tap, otlp_scope_spans_append_span(&scope_spans, &span),
            OTLP_STATUS_OK, "append span");
  tap_equal(g_current_tap, otlp_resource_spans_append_scope_spans(&resource_spans, &scope_spans),
            OTLP_STATUS_OK, "append scope spans");
  tap_equal(g_current_tap,
            otlp_export_trace_service_request_append_resource_spans(&request, &resource_spans),
            OTLP_STATUS_OK, "append resource spans");

  size = otlp_export_trace_service_request_encoded_size(&request);
  encoded = (uint8_t *)malloc(size);
  tap_ok(g_current_tap, encoded != NULL, "allocated traces encode buffer");
  if (encoded == NULL) {
    exit(1);
  }

  tap_equal(g_current_tap,
            otlp_export_trace_service_request_encode(&request, encoded, size),
            OTLP_STATUS_OK, "traces request encode succeeded");

  {
    otlp__writer writer;
    otlp__writer_init(&writer, encoded, size);
    for (i = 0; i < request.resource_spans.len; ++i) {
      otlp_resource_spans_encode_field(&writer, 1, &request.resource_spans.ptr[i]);
    }
    tap_equal(g_current_tap, writer.status, OTLP_STATUS_OK, "manual traces encode succeeded");
    actual = writer.off;
  }
  tap_equal(g_current_tap, actual, size, "traces encoded size matches manual writer offset");

  free(encoded);
  otlp_span_destroy(&span);
  otlp_scope_spans_destroy(&scope_spans);
  otlp_resource_spans_destroy(&resource_spans);
  otlp_export_trace_service_request_destroy(&request);

  otlp_test_ok("traces size matches exact encode");
}

static void test_traces_round_trip_full_fields(void) {
  otlp_any_value attr_value;
  otlp_key_value attribute;
  otlp_span_event event;
  otlp_span_link link;
  otlp_span span;
  otlp_scope_spans scope_spans;
  otlp_resource_spans resource_spans;
  otlp_export_trace_service_request request;
  otlp_export_trace_service_request decoded;
  size_t encoded_size;
  size_t extended_size;
  uint8_t *encoded;
  uint8_t *extended;
  uint8_t trace_id[16] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
  uint8_t span_id[8] = { 2, 2, 2, 2, 2, 2, 2, 2 };
  uint8_t parent_span_id[8] = { 3, 3, 3, 3, 3, 3, 3, 3 };

  CHECK(otlp_any_value_init_string(&attr_value, "value", 5) == OTLP_STATUS_OK);
  otlp_key_value_init(&attribute);
  CHECK(otlp__set_string(&attribute.key, "key", 3) == OTLP_STATUS_OK);
  CHECK(otlp_any_value_copy(&attribute.value, &attr_value) == OTLP_STATUS_OK);
  attribute.has_value = 1;

  otlp_span_event_init(&event);
  event.time_unix_nano = 50;
  event.dropped_attributes_count = 4;
  CHECK(otlp__set_string(&event.name, "exception", 9) == OTLP_STATUS_OK);
  CHECK(otlp__append_primitive((void **) &event.attributes.ptr, &event.attributes.len, &event.attributes.cap, sizeof(attribute), &attribute) == OTLP_STATUS_OK);
  attribute.key.ptr = NULL;
  attribute.key.len = 0;
  otlp_any_value_init(&attribute.value);

  otlp_key_value_init(&attribute);
  CHECK(otlp__set_string(&attribute.key, "link", 4) == OTLP_STATUS_OK);
  CHECK(otlp_any_value_copy(&attribute.value, &attr_value) == OTLP_STATUS_OK);
  attribute.has_value = 1;
  otlp_span_link_init(&link);
  link.dropped_attributes_count = 5;
  link.flags = 0xCC;
  CHECK(otlp__set_bytes(&link.trace_id, trace_id, sizeof(trace_id)) == OTLP_STATUS_OK);
  CHECK(otlp__set_bytes(&link.span_id, span_id, sizeof(span_id)) == OTLP_STATUS_OK);
  CHECK(otlp__set_string(&link.trace_state, "state", 5) == OTLP_STATUS_OK);
  CHECK(otlp__append_primitive((void **) &link.attributes.ptr, &link.attributes.len, &link.attributes.cap, sizeof(attribute), &attribute) == OTLP_STATUS_OK);
  attribute.key.ptr = NULL;
  attribute.key.len = 0;
  otlp_any_value_init(&attribute.value);

  otlp_key_value_init(&attribute);
  CHECK(otlp__set_string(&attribute.key, "span", 4) == OTLP_STATUS_OK);
  CHECK(otlp_any_value_copy(&attribute.value, &attr_value) == OTLP_STATUS_OK);
  attribute.has_value = 1;
  otlp_span_init(&span);
  span.kind = OTLP_SPAN_KIND_SERVER;
  span.flags = 1;
  span.start_time_unix_nano = 100;
  span.end_time_unix_nano = 200;
  span.dropped_attributes_count = 1;
  span.dropped_events_count = 2;
  span.dropped_links_count = 3;
  span.has_status = 1;
  span.status.code = OTLP_TRACE_STATUS_CODE_ERROR;
  CHECK(otlp__set_string(&span.status.message, "failed", 6) == OTLP_STATUS_OK);
  CHECK(otlp__set_bytes(&span.trace_id, trace_id, sizeof(trace_id)) == OTLP_STATUS_OK);
  CHECK(otlp__set_bytes(&span.span_id, span_id, sizeof(span_id)) == OTLP_STATUS_OK);
  CHECK(otlp__set_bytes(&span.parent_span_id, parent_span_id, sizeof(parent_span_id)) == OTLP_STATUS_OK);
  CHECK(otlp__set_string(&span.trace_state, "trace", 5) == OTLP_STATUS_OK);
  CHECK(otlp__set_string(&span.name, "request", 7) == OTLP_STATUS_OK);
  CHECK(otlp__append_primitive((void **) &span.attributes.ptr, &span.attributes.len, &span.attributes.cap, sizeof(attribute), &attribute) == OTLP_STATUS_OK);
  attribute.key.ptr = NULL;
  attribute.key.len = 0;
  otlp_any_value_init(&attribute.value);
  CHECK(otlp__append_primitive((void **) &span.events.ptr, &span.events.len, &span.events.cap, sizeof(event), &event) == OTLP_STATUS_OK);
  otlp_span_event_init(&event);
  CHECK(otlp__append_primitive((void **) &span.links.ptr, &span.links.len, &span.links.cap, sizeof(link), &link) == OTLP_STATUS_OK);
  otlp_span_link_init(&link);

  otlp_scope_spans_init(&scope_spans);
  CHECK(otlp__set_string(&scope_spans.schema_url, "trace-scope", 11) == OTLP_STATUS_OK);
  CHECK(otlp_scope_spans_append_span(&scope_spans, &span) == OTLP_STATUS_OK);
  otlp_resource_spans_init(&resource_spans);
  CHECK(otlp__set_string(&resource_spans.schema_url, "trace-resource", 14) == OTLP_STATUS_OK);
  CHECK(otlp_resource_spans_append_scope_spans(&resource_spans, &scope_spans) == OTLP_STATUS_OK);
  otlp_export_trace_service_request_init(&request);
  CHECK(otlp_export_trace_service_request_append_resource_spans(&request, &resource_spans) == OTLP_STATUS_OK);

  encoded_size = otlp_export_trace_service_request_encoded_size(&request);
  encoded = (uint8_t *) malloc(encoded_size);
  CHECK(encoded != NULL);
  CHECK(otlp_export_trace_service_request_encode(&request, encoded, encoded_size) == OTLP_STATUS_OK);
  CHECK(
    otlp_export_trace_service_request_encode(&request, encoded, encoded_size - 1) ==
    OTLP_STATUS_BUFFER_TOO_SMALL
  );

  extended = otlp_test_append_unknown_varint_field(encoded, encoded_size, 88, 9, &extended_size);
  otlp_export_trace_service_request_init(&decoded);
  CHECK(otlp_export_trace_service_request_decode(&decoded, extended, extended_size) == OTLP_STATUS_OK);
  CHECK(decoded.resource_spans.len == 1);
  CHECK(decoded.resource_spans.ptr[0].schema_url.len == 14);
  CHECK(decoded.resource_spans.ptr[0].scope_spans.ptr[0].schema_url.len == 11);
  CHECK(decoded.resource_spans.ptr[0].scope_spans.ptr[0].spans.ptr[0].trace_state.len == 5);
  CHECK(decoded.resource_spans.ptr[0].scope_spans.ptr[0].spans.ptr[0].parent_span_id.len == 8);
  CHECK(decoded.resource_spans.ptr[0].scope_spans.ptr[0].spans.ptr[0].events.len == 1);
  CHECK(decoded.resource_spans.ptr[0].scope_spans.ptr[0].spans.ptr[0].events.ptr[0].attributes.len == 1);
  CHECK(decoded.resource_spans.ptr[0].scope_spans.ptr[0].spans.ptr[0].links.len == 1);
  CHECK(decoded.resource_spans.ptr[0].scope_spans.ptr[0].spans.ptr[0].links.ptr[0].flags == 0xCC);
  CHECK(decoded.resource_spans.ptr[0].scope_spans.ptr[0].spans.ptr[0].has_status == 1);
  CHECK(decoded.resource_spans.ptr[0].scope_spans.ptr[0].spans.ptr[0].status.code == OTLP_TRACE_STATUS_CODE_ERROR);
  CHECK(decoded.resource_spans.ptr[0].scope_spans.ptr[0].spans.ptr[0].dropped_links_count == 3);

  otlp_export_trace_service_request_destroy(&decoded);
  free(extended);
  free(encoded);
  otlp_export_trace_service_request_destroy(&request);
  otlp_resource_spans_destroy(&resource_spans);
  otlp_scope_spans_destroy(&scope_spans);
  otlp_span_destroy(&span);
  otlp_span_link_destroy(&link);
  otlp_span_event_destroy(&event);
  otlp_key_value_destroy(&attribute);
  otlp_any_value_destroy(&attr_value);
  otlp_test_ok("traces round trip full fields");
}

static void test_traces_data_and_response_round_trip(void) {
  otlp_span span;
  otlp_scope_spans scope_spans;
  otlp_resource_spans resource_spans;
  otlp_traces_data data;
  otlp_traces_data decoded_data;
  otlp_export_trace_service_response response;
  otlp_export_trace_service_response decoded_response;
  uint8_t trace_id[16] = {0};
  uint8_t span_id[8] = {0};
  size_t data_size;
  size_t response_size;
  uint8_t *data_bytes;
  uint8_t *response_bytes;

  trace_id[0] = 1;
  span_id[0] = 2;
  otlp_span_init(&span);
  CHECK(otlp__set_bytes(&span.trace_id, trace_id, sizeof(trace_id)) == OTLP_STATUS_OK);
  CHECK(otlp__set_bytes(&span.span_id, span_id, sizeof(span_id)) == OTLP_STATUS_OK);
  CHECK(otlp__set_string(&span.name, "trace", 5) == OTLP_STATUS_OK);
  span.kind = OTLP_SPAN_KIND_CLIENT;

  otlp_scope_spans_init(&scope_spans);
  scope_spans.has_scope = 1;
  CHECK(otlp__set_string(&scope_spans.scope.name, "scope", 5) == OTLP_STATUS_OK);
  CHECK(otlp_scope_spans_append_span(&scope_spans, &span) == OTLP_STATUS_OK);

  otlp_resource_spans_init(&resource_spans);
  resource_spans.has_resource = 1;
  resource_spans.resource.dropped_attributes_count = 4;
  CHECK(otlp_resource_spans_append_scope_spans(&resource_spans, &scope_spans) == OTLP_STATUS_OK);

  otlp_traces_data_init(&data);
  CHECK(otlp__append_primitive((void **)&data.resource_spans.ptr,
                                &data.resource_spans.len,
                                &data.resource_spans.cap,
                                sizeof(resource_spans),
                                &resource_spans) == OTLP_STATUS_OK);
  otlp_resource_spans_init(&resource_spans);
  data_size = otlp_traces_data_encoded_size(&data);
  data_bytes = (uint8_t *)malloc(data_size);
  CHECK(data_bytes != NULL);
  CHECK(otlp_traces_data_encode(&data, data_bytes, data_size) == OTLP_STATUS_OK);
  otlp_traces_data_init(&decoded_data);
  CHECK(otlp_traces_data_decode(&decoded_data, data_bytes, data_size) == OTLP_STATUS_OK);
  CHECK(decoded_data.resource_spans.len == 1);
  CHECK(decoded_data.resource_spans.ptr[0].has_resource == 1);
  CHECK(decoded_data.resource_spans.ptr[0].scope_spans.ptr[0].has_scope == 1);

  otlp_export_trace_service_response_init(&response);
  response.has_partial_success = 1;
  response.partial_success.rejected_spans = 2;
  CHECK(otlp__set_string(&response.partial_success.error_message, "trace-partial", 13) == OTLP_STATUS_OK);
  response_size = otlp_export_trace_service_response_encoded_size(&response);
  response_bytes = (uint8_t *)malloc(response_size);
  CHECK(response_bytes != NULL);
  CHECK(otlp_export_trace_service_response_encode(&response, response_bytes, response_size) == OTLP_STATUS_OK);
  otlp_export_trace_service_response_init(&decoded_response);
  CHECK(otlp_export_trace_service_response_decode(&decoded_response, response_bytes, response_size) == OTLP_STATUS_OK);
  CHECK(decoded_response.has_partial_success == 1);
  CHECK(decoded_response.partial_success.rejected_spans == 2);

  otlp_export_trace_service_response_destroy(&decoded_response);
  otlp_export_trace_service_response_destroy(&response);
  otlp_traces_data_destroy(&decoded_data);
  otlp_traces_data_destroy(&data);
  otlp_resource_spans_destroy(&resource_spans);
  otlp_scope_spans_destroy(&scope_spans);
  otlp_span_destroy(&span);
  free(data_bytes);
  free(response_bytes);
  otlp_test_ok("traces data and response round trip");
}

OTLP_DEFINE_SUBTEST(test_traces_proto_wire_compatibility)
OTLP_DEFINE_SUBTEST(test_traces_round_trip)
OTLP_DEFINE_SUBTEST(test_traces_size_matches_encode_exactly)
OTLP_DEFINE_SUBTEST(test_traces_round_trip_full_fields)
OTLP_DEFINE_SUBTEST(test_traces_data_and_response_round_trip)

#endif /* OTLP_FORMAT_ENABLE_TRACES */

int test_traces_count(void) {
#if defined(OTLP_FORMAT_ENABLE_TRACES)
  return 5;
#else
  return 0;
#endif
}

void test_traces_run(tap_t *t) {
#if defined(OTLP_FORMAT_ENABLE_TRACES)
  tap_test(t, "traces proto wire compatibility", test_traces_proto_wire_compatibility_subtest, NULL);
  tap_test(t, "traces round trip", test_traces_round_trip_subtest, NULL);
  tap_test(t, "traces size matches exact encode", test_traces_size_matches_encode_exactly_subtest, NULL);
  tap_test(t, "traces round trip full fields", test_traces_round_trip_full_fields_subtest, NULL);
  tap_test(t, "traces data and response round trip", test_traces_data_and_response_round_trip_subtest, NULL);
#else
  (void)t;
#endif
}
