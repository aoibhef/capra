#pragma once

#define SPDLOG_HEADER_ONLY
#if !defined(SPDLOG_ACTIVE_LEVEL)
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#endif
#include "fmt/ranges.h"  // Allow logging ranges (vector, etc.)
#include "spdlog/spdlog.h"
#include "spdlog/sinks/ansicolor_sink.h"
#include <memory>

namespace capra {

std::shared_ptr<spdlog::logger> logger();

} // namespace capra

#define CAPRA_LOG_TRACE(...)    SPDLOG_LOGGER_TRACE(capra::logger(), __VA_ARGS__)
#define CAPRA_LOG_DEBUG(...)    SPDLOG_LOGGER_DEBUG(capra::logger(), __VA_ARGS__)
#define CAPRA_LOG_INFO(...)     SPDLOG_LOGGER_INFO(capra::logger(), __VA_ARGS__)
#define CAPRA_LOG_WARN(...)     SPDLOG_LOGGER_WARN(capra::logger(), __VA_ARGS__)
#define CAPRA_LOG_ERROR(...)    SPDLOG_LOGGER_ERROR(capra::logger(), __VA_ARGS__)
#define CAPRA_LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(capra::logger(), __VA_ARGS__)
