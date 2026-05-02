#ifndef OTLP_FORMAT_H
#define OTLP_FORMAT_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "common/common.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(OTLP_FORMAT_ENABLE_LOGS)
#include "logs/logs.h"
#endif

#if defined(OTLP_FORMAT_ENABLE_PROFILES)
#include "profiles/profiles.h"
#endif

#if defined(OTLP_FORMAT_ENABLE_TRACES)
#include "traces/traces.h"
#endif

#if defined(OTLP_FORMAT_ENABLE_METRICS)
#include "metrics/metrics.h"
#endif

#ifdef __cplusplus
} /* extern "C" */

#include "common/common.hpp"

#if defined(OTLP_FORMAT_ENABLE_LOGS)
#include "logs/logs.hpp"
#endif

#if defined(OTLP_FORMAT_ENABLE_PROFILES)
#include "profiles/profiles.hpp"
#endif

#if defined(OTLP_FORMAT_ENABLE_TRACES)
#include "traces/traces.hpp"
#endif

#if defined(OTLP_FORMAT_ENABLE_METRICS)
#include "metrics/metrics.hpp"
#endif

#endif

#endif /* OTLP_FORMAT_H */
