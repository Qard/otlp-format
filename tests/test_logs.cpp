#include "test_helpers.hpp"

#if defined(__cplusplus) && (__cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900))

#if defined(OTLP_FORMAT_ENABLE_LOGS)
#include <vector>

namespace {

static void test_cpp_logs_wrappers(Tap &t) {
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

  otlp::LogsExportRequest request;
  otlp::LogRecord record;
  record.set_time_unix_nano(456);
  record.set_observed_time_unix_nano(789);
  record.set_severity_number(OTLP_SEVERITY_NUMBER_WARN);
  record.set_severity_text("warn");
  record.set_body(otlp::AnyValue::string("cpp"));
  record.append_attribute(kv);
  record.set_dropped_attributes_count(1);
  record.set_flags(OTLP_LOG_RECORD_FLAGS_TRACE_FLAGS_MASK);
  record.set_trace_id(common_bytes, sizeof(common_bytes));
  record.set_span_id(common_bytes, sizeof(common_bytes));
  record.set_event_name("evt");

  otlp::ScopeLogs scope;
  scope.set_scope(common_scope);
  scope.set_schema_url("logs-scope-schema");
  scope.append_log_record(record);

  otlp::ResourceLogs resource;
  resource.set_resource(common_resource);
  resource.set_schema_url("logs-resource-schema");
  resource.append_scope_logs(scope);

  request.append_resource_logs(resource);

  std::vector<std::uint8_t> encoded(request.encoded_size());
  t.not_ok(encoded.empty(), "logs request buffer allocated");
  check_equal(t, request.encode(encoded.data(), encoded.size()),
              OTLP_STATUS_OK, "logs request encode");

  otlp::LogsExportRequest decoded;
  check_equal(t, decoded.decode(encoded.data(), encoded.size()),
              OTLP_STATUS_OK, "logs request decode");
  check_equal(t, decoded.get()->resource_logs.len, static_cast<size_t>(1),
              "decoded resource logs count");

  otlp::AnyValue copy_body = otlp::AnyValue::string("copy");
  otlp::AnyValue assigned_body;
  assigned_body = copy_body;

  otlp::LogRecord copied_record = record;
  otlp::LogRecord assigned_record;
  assigned_record = record;

  otlp::ScopeLogs copied_scope = scope;
  otlp::ScopeLogs assigned_scope;
  assigned_scope = scope;

  otlp::ResourceLogs copied_resource = resource;
  otlp::ResourceLogs assigned_resource;
  assigned_resource = resource;

  otlp::LogsExportRequest copied_request = request;
  otlp::LogsExportRequest assigned_request;
  assigned_request = request;

  check_equal(t, copy_body.get()->kind, OTLP_ANY_VALUE_STRING,
              "copied body kind");
  check_equal(t, assigned_body.get()->kind, OTLP_ANY_VALUE_STRING,
              "assigned body kind");
  check_equal(t, copied_record.get()->has_body, 1,
              "copied log record body presence");
  check_equal(t, copied_record.get()->observed_time_unix_nano,
              static_cast<std::uint64_t>(789), "copied observed timestamp");
  check_equal(t, copied_record.get()->severity_text.len,
              static_cast<size_t>(4), "copied severity text length");
  check_equal(t, copied_record.get()->attributes.len,
              static_cast<size_t>(1), "copied attribute count");
  check_equal(t, copied_record.get()->dropped_attributes_count,
              static_cast<std::uint32_t>(1),
              "copied dropped attributes count");
  check_equal(t, copied_record.get()->flags,
              static_cast<std::uint32_t>(
                  OTLP_LOG_RECORD_FLAGS_TRACE_FLAGS_MASK),
              "copied log flags");
  check_equal(t, copied_record.get()->trace_id.len, sizeof(common_bytes),
              "copied trace id length");
  check_equal(t, copied_record.get()->span_id.len, sizeof(common_bytes),
              "copied span id length");
  check_equal(t, assigned_record.get()->has_body, 1,
              "assigned log record body presence");
  check_equal(t, copied_scope.get()->log_records.len, static_cast<size_t>(1),
              "copied scope log count");
  check_equal(t, copied_scope.get()->schema_url.len,
              static_cast<size_t>(17),
              "copied scope schema url length");
  check_equal(t, assigned_scope.get()->log_records.len, static_cast<size_t>(1),
              "assigned scope log count");
  check_equal(t, copied_resource.get()->scope_logs.len,
              static_cast<size_t>(1), "copied resource scope count");
  check_equal(t, copied_resource.get()->schema_url.len,
              static_cast<size_t>(20),
              "copied resource schema url length");
  check_equal(t, assigned_resource.get()->scope_logs.len,
              static_cast<size_t>(1), "assigned resource scope count");
  check_equal(t, copied_request.get()->resource_logs.len,
              static_cast<size_t>(1), "copied request resource count");
  check_equal(t, assigned_request.get()->resource_logs.len,
              static_cast<size_t>(1), "assigned request resource count");
}

static void test_cpp_logs_data_and_response(Tap &t) {
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

  otlp::LogRecord record;
  record.set_time_unix_nano(456);
  record.set_observed_time_unix_nano(789);
  record.set_severity_number(OTLP_SEVERITY_NUMBER_WARN);
  record.set_severity_text("warn");
  record.set_body(otlp::AnyValue::string("cpp"));
  record.append_attribute(kv);
  record.set_event_name("evt");
  record.set_trace_id(common_bytes, sizeof(common_bytes));
  record.set_span_id(common_bytes, sizeof(common_bytes));

  otlp::ScopeLogs scope;
  scope.set_scope(common_scope);
  scope.set_schema_url("logs-scope-schema");
  scope.append_log_record(record);

  otlp::ResourceLogs resource;
  resource.set_resource(common_resource);
  resource.set_schema_url("logs-resource-schema");
  resource.append_scope_logs(scope);

  otlp::LogsData logs_data;
  logs_data.append_resource_logs(resource);
  std::vector<std::uint8_t> encoded_logs_data(logs_data.encoded_size());
  check_equal(t,
              logs_data.encode(encoded_logs_data.data(),
                               encoded_logs_data.size()),
              OTLP_STATUS_OK, "logs data encode");

  otlp::LogsData decoded_logs_data;
  check_equal(t,
              decoded_logs_data.decode(encoded_logs_data.data(),
                                       encoded_logs_data.size()),
              OTLP_STATUS_OK, "logs data decode");

  otlp::LogsExportResponse logs_response;
  logs_response.set_partial_success(2, "partial");
  std::vector<std::uint8_t> encoded_logs_response(
      logs_response.encoded_size());
  check_equal(t,
              logs_response.encode(encoded_logs_response.data(),
                                   encoded_logs_response.size()),
              OTLP_STATUS_OK, "logs response encode");

  otlp::LogsExportResponse decoded_logs_response;
  check_equal(t,
              decoded_logs_response.decode(encoded_logs_response.data(),
                                           encoded_logs_response.size()),
              OTLP_STATUS_OK, "logs response decode");

  check_equal(t, decoded_logs_data.get()->resource_logs.len,
              static_cast<size_t>(1),
              "decoded logs data resource count");
  check_equal(t, decoded_logs_response.get()->has_partial_success, 1,
              "decoded logs response partial success");
}

} // namespace

#endif /* OTLP_FORMAT_ENABLE_LOGS */

#endif /* __cplusplus */

void test_logs_cpp(Tap &t) {
#if defined(__cplusplus) && (__cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900))
#if defined(OTLP_FORMAT_ENABLE_LOGS)
  t.test("cpp logs wrappers", [](Tap &sub) {
    test_cpp_logs_wrappers(sub);
  });
  t.test("cpp logs data and response", [](Tap &sub) {
    test_cpp_logs_data_and_response(sub);
  });
#else
  (void)t;
#endif
#else
  (void)t;
#endif
}

int test_logs_cpp_count(void) {
#if defined(OTLP_FORMAT_ENABLE_LOGS)
  return 2;
#else
  return 0;
#endif
}
