#pragma once

/**
 * @file error_codes.hpp
 * @brief Centralized error/status constants for the Measurement Client SDK.
 *
 * The client SDK does not produce numeric error codes in its output buffer;
 * errors are reported via the logging system (see logger.hpp).
 * These constants are provided for consumers who want to identify SDK
 * log messages programmatically.
 */

namespace measurement {
namespace ErrorCode {

/// Output buffer too small to hold the Base64-encoded result.
constexpr int BUFFER_TOO_SMALL   = 1;

/// Input image could not be decoded (corrupt or unsupported format).
constexpr int IMAGE_DECODE_ERROR = 2;

/// A landmark index referenced by the triangle table is out of range.
constexpr int LANDMARK_OOB       = 3;

}  // namespace ErrorCode
}  // namespace measurement
