#include "test_helpers.hpp"

#if defined(__cplusplus) && (__cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900))

namespace {

static void test_cpp_common_wrappers(Tap &t) {
#if defined(OTLP_FORMAT_ENABLE_LOGS) || defined(OTLP_FORMAT_ENABLE_PROFILES) || \
    defined(OTLP_FORMAT_ENABLE_TRACES) || defined(OTLP_FORMAT_ENABLE_METRICS)
  const std::uint8_t common_bytes[] = {1, 2, 3};

  otlp::AnyValue nested = otlp::AnyValue::string("nested");
  otlp::AnyValue bool_value = otlp::AnyValue::bool_value(true);
  otlp::AnyValue int_value = otlp::AnyValue::int_value(-7);
  otlp::AnyValue double_value = otlp::AnyValue::double_value(2.5);
  otlp::AnyValue string_index_value =
      otlp::AnyValue::string_value_strindex(11);
  otlp::AnyValue bytes_value =
      otlp::AnyValue::bytes_value(common_bytes, sizeof(common_bytes));

  otlp::KeyValue kv;
  kv.set_key("key");
  kv.set_value(nested);

  otlp::KeyValue indexed_kv;
  indexed_kv.set_key_strindex(4);
  indexed_kv.set_value(string_index_value);

  otlp::KeyValueList kvlist;
  kvlist.append_value(kv);

  otlp::AnyValue kvlist_value;
  kvlist_value.set_kvlist_value(kvlist);

  otlp::ArrayValue array;
  array.append_value(nested);
  array.append_value(kvlist_value);

  otlp::AnyValue array_value;
  array_value.set_array_value(array);

  otlp::InstrumentationScope common_scope;
  common_scope.set_name("scope");
  common_scope.set_version("1.0");
  common_scope.append_attribute(kv);
  common_scope.set_dropped_attributes_count(2);

  otlp::EntityRef entity_ref;
  entity_ref.set_schema_url("entity-schema");
  entity_ref.set_type("service.instance");
  entity_ref.append_id_key("service.instance.id");
  entity_ref.append_description_key("entity");

  otlp::Resource common_resource;
  common_resource.append_attribute(kv);
  common_resource.append_entity_ref(entity_ref);
  common_resource.set_dropped_attributes_count(3);

  check_equal(t, array_value.get()->kind, OTLP_ANY_VALUE_ARRAY,
              "array any value kind");
  check_equal(t, bool_value.get()->kind, OTLP_ANY_VALUE_BOOL,
              "bool any value kind");
  check_equal(t, int_value.get()->kind, OTLP_ANY_VALUE_INT,
              "int any value kind");
  check_equal(t, double_value.get()->kind, OTLP_ANY_VALUE_DOUBLE,
              "double any value kind");
  check_equal(t, string_index_value.get()->kind,
              OTLP_ANY_VALUE_STRING_STRINDEX,
              "string index any value kind");
  check_equal(t, string_index_value.get()->value.string_value_strindex,
              static_cast<int32_t>(11), "string index value");
  check_equal(t, bytes_value.get()->kind, OTLP_ANY_VALUE_BYTES,
              "bytes any value kind");
  check_equal(t, indexed_kv.get()->key_strindex, static_cast<int32_t>(4),
              "indexed key uses profiling key index");
  check_equal(t, common_scope.get()->dropped_attributes_count,
              static_cast<std::uint32_t>(2),
              "scope dropped attributes count");
  check_equal(t, common_resource.get()->attributes.len,
              static_cast<size_t>(1), "resource attribute count");
  check_equal(t, common_resource.get()->entity_refs.len,
              static_cast<size_t>(1), "resource entity refs count");
  check_equal(t, common_resource.get()->dropped_attributes_count,
              static_cast<std::uint32_t>(3),
              "resource dropped attributes count");
#else
  t.pass("common wrappers skipped with no enabled signals");
#endif
}

} // namespace

#endif

void test_common_cpp(Tap &t) {
#if defined(__cplusplus) && (__cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900))
#if defined(OTLP_FORMAT_ENABLE_LOGS) || defined(OTLP_FORMAT_ENABLE_PROFILES) || \
    defined(OTLP_FORMAT_ENABLE_TRACES) || defined(OTLP_FORMAT_ENABLE_METRICS)
  t.test("cpp common wrappers", [](Tap &sub) {
    test_cpp_common_wrappers(sub);
  });
#endif
#else
  (void)t;
#endif
}

int test_common_cpp_count(void) {
#if defined(OTLP_FORMAT_ENABLE_LOGS) || defined(OTLP_FORMAT_ENABLE_PROFILES) || \
    defined(OTLP_FORMAT_ENABLE_TRACES) || defined(OTLP_FORMAT_ENABLE_METRICS)
  return 1;
#else
  return 0;
#endif
}
