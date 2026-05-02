# otlp-format C

A zero-dependency C/C++ library for encoding and decoding the [OpenTelemetry Protocol (OTLP)](https://opentelemetry.io/docs/specs/otlp/) wire format. All types, encoders, and decoders are delivered as a single header file — no build system integration required.

## Usage

Two bundled headers are provided:

- **`otlp_format_bundled.h`** — C-only, no C++ wrappers
- **`otlp_format_bundled.hpp`** — self-contained C++ (includes C types and C++ wrappers)

C projects:
```c
#include "otlp_format_bundled.h"
```

C++ projects:
```cpp
#include "otlp_format_bundled.hpp"
```

All four signals are enabled by default. To include only specific signals, define the signals you want before any system-wide defaults kick in:

```c
#define OTLP_FORMAT_ENABLE_LOGS 1
#include "otlp_format_bundled.h"
```

Available feature flags:

| Flag | Signal |
|---|---|
| `OTLP_FORMAT_ENABLE_LOGS` | Logs |
| `OTLP_FORMAT_ENABLE_TRACES` | Traces |
| `OTLP_FORMAT_ENABLE_METRICS` | Metrics |
| `OTLP_FORMAT_ENABLE_PROFILES` | Profiles |

### Modular headers

Alternatively, add `include/` to your include path and use the modular entry point:

```c
#include "otlp_format/otlp_format.h"
```

Individual signal headers can be included directly (`otlp_format/logs/logs.h`, etc.) and each is self-contained.

## Example

```c
#include "otlp_format_bundled.h"

int main(void) {
  otlp_export_logs_service_request req;
  otlp_resource_logs rl;
  otlp_scope_logs sl;
  otlp_log_record record;

  otlp_export_logs_service_request_init(&req);
  otlp_resource_logs_init(&rl);
  otlp_scope_logs_init(&sl);
  otlp_log_record_init(&record);

  record.time_unix_nano = 1234567890ULL;
  record.severity_number = OTLP_SEVERITY_NUMBER_INFO;
  otlp_any_value body;
  otlp_any_value_init_string(&body, "hello", 5);
  otlp_log_record_set_body(&record, &body);
  otlp_any_value_destroy(&body);

  otlp_scope_logs_append_log_record(&sl, &record);
  otlp_resource_logs_append_scope_logs(&rl, &sl);
  otlp_export_logs_service_request_append_resource_logs(&req, &rl);

  size_t size = otlp_export_logs_service_request_encoded_size(&req);
  uint8_t *buf = malloc(size);
  otlp_export_logs_service_request_encode(&req, buf, size);
  /* send buf ... */
  free(buf);

  otlp_log_record_destroy(&record);
  otlp_scope_logs_destroy(&sl);
  otlp_resource_logs_destroy(&rl);
  otlp_export_logs_service_request_destroy(&req);
  return 0;
}
```

### C++ example

```cpp
#include "otlp_format_bundled.hpp"
#include <vector>

int main() {
  otlp::AnyValue body = otlp::AnyValue::string("hello");

  otlp::LogRecord record;
  record.set_time_unix_nano(1234567890ULL);
  record.set_severity_number(OTLP_SEVERITY_NUMBER_INFO);
  record.set_body(body);

  otlp::ScopeLogs scope;
  scope.append_log_record(record);

  otlp::ResourceLogs resource;
  resource.append_scope_logs(scope);

  otlp::LogsExportRequest request;
  request.append_resource_logs(resource);

  std::vector<std::uint8_t> buf(request.encoded_size());
  request.encode(buf.data(), buf.size());
  /* send buf ... */

  return 0;
}
```

C++ wrappers manage memory automatically via RAII — no explicit init or destroy calls needed.

## API

Each signal follows the same pattern:

- **Init/destroy**: `otlp_<type>_init`, `otlp_<type>_destroy`
- **Encode**: `otlp_<type>_encoded_size`, `otlp_<type>_encode`
- **Decode**: `otlp_<type>_decode`
- **Append**: `otlp_<container>_append_<item>`

Return values use `otlp_status`:

| Value | Meaning |
|---|---|
| `OTLP_STATUS_OK` | Success |
| `OTLP_STATUS_INVALID_ARGUMENT` | Bad argument |
| `OTLP_STATUS_OUT_OF_MEMORY` | Allocation failed |
| `OTLP_STATUS_BUFFER_TOO_SMALL` | Encode buffer too small |
| `OTLP_STATUS_DECODE_ERROR` | Malformed input |

## Building and testing

```sh
make test           # full suite: all signals, bundled + modular headers
make check-header   # verify otlp_format_bundled.h is up to date
make merge-header   # regenerate otlp_format_bundled.h from modular sources
```

Per-signal builds:

```sh
make test-logs-only
make test-traces-only
make test-metrics-only
make test-profiles-only
```

Requirements: a C11 compiler and a C++11 compiler (defaults to `cc` and `c++`).

## License

MIT
