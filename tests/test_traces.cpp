#include "test_helpers.hpp"

#if defined(__cplusplus) && (__cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900))

#if defined(OTLP_FORMAT_ENABLE_TRACES)
#include <vector>

namespace {

static void test_cpp_traces_wrappers(Tap &t) {
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

  otlp::TracesExportRequest traces_request;
  otlp::Span span;
  std::uint8_t trace_id[16] = {1, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 2};
  std::uint8_t span_id[8] = {3, 0, 0, 0, 0, 0, 0, 4};
  span.set_trace_id(trace_id, sizeof(trace_id));
  span.set_span_id(span_id, sizeof(span_id));
  span.set_name("cpp-trace");
  span.set_kind(OTLP_SPAN_KIND_SERVER);
  span.set_trace_state("state");
  span.set_parent_span_id(span_id, sizeof(span_id));
  span.set_start_time_unix_nano(100);
  span.set_end_time_unix_nano(200);
  span.append_attribute(kv);
  span.set_flags(OTLP_SPAN_FLAGS_TRACE_FLAGS_MASK);

  otlp::SpanEvent span_event;
  span_event.set_name("event");
  span_event.set_time_unix_nano(150);
  span_event.append_attribute(kv);
  span.append_event(span_event);

  otlp::SpanLink span_link;
  span_link.set_trace_id(trace_id, sizeof(trace_id));
  span_link.set_span_id(span_id, sizeof(span_id));
  span_link.set_flags(OTLP_SPAN_FLAGS_CONTEXT_HAS_IS_REMOTE_MASK);
  span.append_link(span_link);

  otlp::TraceStatus trace_status;
  trace_status.set_code(OTLP_TRACE_STATUS_CODE_ERROR);
  trace_status.set_message("status");
  span.set_status(trace_status);

  otlp::ScopeSpans trace_scope;
  trace_scope.set_scope(common_scope);
  trace_scope.set_schema_url("trace-scope-schema");
  trace_scope.append_span(span);

  otlp::ResourceSpans trace_resource;
  trace_resource.set_resource(common_resource);
  trace_resource.set_schema_url("trace-resource-schema");
  trace_resource.append_scope_spans(trace_scope);

  traces_request.append_resource_spans(trace_resource);

  std::vector<std::uint8_t> encoded_traces(traces_request.encoded_size());
  t.not_ok(encoded_traces.empty(), "traces request buffer allocated");
  check_equal(t,
              traces_request.encode(encoded_traces.data(), encoded_traces.size()),
              OTLP_STATUS_OK, "traces request encode");

  otlp::TracesExportRequest decoded_traces;
  check_equal(t,
              decoded_traces.decode(encoded_traces.data(), encoded_traces.size()),
              OTLP_STATUS_OK, "traces request decode");
  check_equal(t, decoded_traces.get()->resource_spans.len,
              static_cast<size_t>(1), "decoded resource spans count");
  check_equal(
      t,
      decoded_traces.get()->resource_spans.ptr[0].scope_spans.ptr[0].spans.ptr[0]
          .events.len,
      static_cast<size_t>(1), "decoded span event count");
  check_equal(
      t,
      decoded_traces.get()->resource_spans.ptr[0].scope_spans.ptr[0].spans.ptr[0]
          .links.len,
      static_cast<size_t>(1), "decoded span link count");
  check_equal(
      t,
      decoded_traces.get()->resource_spans.ptr[0].scope_spans.ptr[0].spans.ptr[0]
          .has_status,
      1, "decoded span status presence");
}

static void test_cpp_traces_copy_independence(Tap &t) {
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

  otlp::TracesExportRequest traces_request;
  otlp::Span span;
  std::uint8_t trace_id[16] = {1, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 2};
  std::uint8_t span_id[8] = {3, 0, 0, 0, 0, 0, 0, 4};
  span.set_trace_id(trace_id, sizeof(trace_id));
  span.set_span_id(span_id, sizeof(span_id));
  span.set_name("cpp-trace");
  span.set_kind(OTLP_SPAN_KIND_SERVER);

  otlp::ScopeSpans trace_scope;
  trace_scope.set_scope(common_scope);
  trace_scope.set_schema_url("trace-scope-schema");
  trace_scope.append_span(span);

  otlp::ResourceSpans trace_resource;
  trace_resource.set_resource(common_resource);
  trace_resource.set_schema_url("trace-resource-schema");
  trace_resource.append_scope_spans(trace_scope);

  traces_request.append_resource_spans(trace_resource);

  otlp::TracesExportRequest copied_traces = traces_request;
  trace_scope.append_span(span);

  check_equal(t, copied_traces.get()->resource_spans.len,
              static_cast<size_t>(1),
              "copied traces resource count");
  check_equal(
      t,
      copied_traces.get()->resource_spans.ptr[0].scope_spans.ptr[0].spans.len,
      static_cast<size_t>(1), "copied traces span count");
  check_equal(t, copied_traces.get()->resource_spans.ptr[0].schema_url.len,
              static_cast<size_t>(21),
              "copied traces resource schema length");
  check_equal(
      t,
      copied_traces.get()->resource_spans.ptr[0].scope_spans.ptr[0].schema_url
          .len,
      static_cast<size_t>(18), "copied traces scope schema length");
}

static void test_cpp_traces_data_and_response(Tap &t) {
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

  otlp::TracesData traces_data;
  otlp::ResourceSpans trace_resource;
  trace_resource.set_resource(common_resource);
  trace_resource.set_schema_url("trace-resource-schema");
  traces_data.append_resource_spans(trace_resource);

  std::vector<std::uint8_t> encoded_traces_data(traces_data.encoded_size());
  check_equal(t,
              traces_data.encode(encoded_traces_data.data(),
                                 encoded_traces_data.size()),
              OTLP_STATUS_OK, "traces data encode");

  otlp::TracesData decoded_traces_data;
  check_equal(t,
              decoded_traces_data.decode(encoded_traces_data.data(),
                                         encoded_traces_data.size()),
              OTLP_STATUS_OK, "traces data decode");

  otlp::TracesExportResponse traces_response;
  traces_response.set_partial_success(4, "trace-partial");
  std::vector<std::uint8_t> encoded_traces_response(
      traces_response.encoded_size());
  check_equal(t,
              traces_response.encode(encoded_traces_response.data(),
                                     encoded_traces_response.size()),
              OTLP_STATUS_OK, "traces response encode");

  otlp::TracesExportResponse decoded_traces_response;
  check_equal(t,
              decoded_traces_response.decode(encoded_traces_response.data(),
                                             encoded_traces_response.size()),
              OTLP_STATUS_OK, "traces response decode");

  check_equal(t, decoded_traces_data.get()->resource_spans.len,
              static_cast<size_t>(1),
              "decoded traces data resource count");
  check_equal(t, decoded_traces_response.get()->has_partial_success, 1,
              "decoded traces response partial success");
}

} // namespace

#endif /* OTLP_FORMAT_ENABLE_TRACES */

#endif /* __cplusplus */

void test_traces_cpp(Tap &t) {
#if defined(__cplusplus) && (__cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900))
#if defined(OTLP_FORMAT_ENABLE_TRACES)
  t.test("cpp traces wrappers", [](Tap &sub) {
    test_cpp_traces_wrappers(sub);
  });
  t.test("cpp traces copy independence", [](Tap &sub) {
    test_cpp_traces_copy_independence(sub);
  });
  t.test("cpp traces data and response", [](Tap &sub) {
    test_cpp_traces_data_and_response(sub);
  });
#else
  (void)t;
#endif
#else
  (void)t;
#endif
}

int test_traces_cpp_count(void) {
#if defined(OTLP_FORMAT_ENABLE_TRACES)
  return 3;
#else
  return 0;
#endif
}
