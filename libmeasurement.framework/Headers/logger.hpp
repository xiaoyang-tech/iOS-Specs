#pragma once

#include <cstdarg>
#include <cstdio>
#include <mutex>
#include <string>

/**
 * @file logger.hpp
 * @brief Lightweight, header-only logging system for the Measurement SDK.
 *
 * Usage inside the SDK (internal code only):
 * @code
 *   SDK_LOG_ERROR("model load failed: %s", e.what());
 *   SDK_LOG_WARN("signal too short (%d frames)", length);
 *   SDK_LOG_INFO("report generated");
 *   SDK_LOG_DEBUG("peak count: %d", peaksLen);
 * @endcode
 *
 * Consumer control (via public C API):
 * @code
 *   MeasurementSetLogLevel(3);          // suppress everything below ERROR
 *   MeasurementSetLogCallback(myFunc);  // redirect to application logger
 * @endcode
 */

namespace measurement {

/// Log severity levels. Values are intentionally contiguous so that
/// integer comparisons work: a message is emitted when its level >= the
/// configured minimum level.
enum class LogLevel {
    _DEBUG = 0,
    INFO  = 1,
    WARN  = 2,
    ERROR = 3,
    NONE  = 4   ///< Suppress all output
};

/// Callback type for consumer-supplied log sinks.
/// @param level   Numeric log level (0=DEBUG … 3=ERROR)
/// @param message Null-terminated, fully-formatted message string
using LogCallback = void (*)(int level, const char* message);

/// Thread-safe singleton logger.
class Logger {
public:
    static Logger& instance() {
        static Logger inst;
        return inst;
    }

    void setLevel(LogLevel level) {
        std::lock_guard<std::mutex> lk(mu_);
        level_ = level;
    }

    /// Set a consumer-supplied callback.  Pass @c nullptr to restore the
    /// default behaviour (write to @c stderr).
    void setCallback(LogCallback cb) {
        std::lock_guard<std::mutex> lk(mu_);
        cb_ = cb;
    }

    void log(LogLevel level, const char* fmt, ...) const {
        std::lock_guard<std::mutex> lk(mu_);
        if (level < level_) return;

        static const char* const kLabels[] = {"DEBUG", "INFO", "WARN", "ERROR"};
        const char* label = (static_cast<int>(level) < 4) ? kLabels[static_cast<int>(level)] : "LOG";

        // Format the message
        char buf[2048];
        char msg[2100];  // extra room for "[MeasurementSDK][ERROR] " prefix
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(buf, sizeof(buf), fmt, ap);
        va_end(ap);
        snprintf(msg, sizeof(msg), "[MeasurementSDK][%s] %s", label, buf);

        if (cb_) {
            cb_(static_cast<int>(level), msg);
        } else {
            fprintf(stderr, "%s\n", msg);
        }
    }

private:
    Logger()                         = default;
    Logger(const Logger&)            = delete;
    Logger& operator=(const Logger&) = delete;

    mutable std::mutex mu_;
    LogLevel           level_ = LogLevel::INFO;
    LogCallback        cb_    = nullptr;
};

}  // namespace measurement

// ──────────────────────────────────────────────────────────────────────────────
// Convenience macros.  When MEASUREMENT_ENABLE_LOGGING is 0, all macros
// expand to nothing so the compiler can elide them entirely.
// ──────────────────────────────────────────────────────────────────────────────

#ifndef MEASUREMENT_ENABLE_LOGGING
#define MEASUREMENT_ENABLE_LOGGING 1
#endif

#if MEASUREMENT_ENABLE_LOGGING
#define SDK_LOG_DEBUG(fmt, ...) \
    measurement::Logger::instance().log(measurement::LogLevel::DEBUG, fmt, ##__VA_ARGS__)
#define SDK_LOG_INFO(fmt, ...) \
    measurement::Logger::instance().log(measurement::LogLevel::INFO, fmt, ##__VA_ARGS__)
#define SDK_LOG_WARN(fmt, ...) \
    measurement::Logger::instance().log(measurement::LogLevel::WARN, fmt, ##__VA_ARGS__)
#define SDK_LOG_ERROR(fmt, ...) \
    measurement::Logger::instance().log(measurement::LogLevel::ERROR, fmt, ##__VA_ARGS__)
#else
#define SDK_LOG_DEBUG(fmt, ...) (void)0
#define SDK_LOG_INFO(fmt, ...)  (void)0
#define SDK_LOG_WARN(fmt, ...)  (void)0
#define SDK_LOG_ERROR(fmt, ...) (void)0
#endif
