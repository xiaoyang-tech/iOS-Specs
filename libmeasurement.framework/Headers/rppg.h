/**
 * @file rppg.h
 * @brief Public C API for the Measurement Client SDK.
 *
 * 从视频帧中提取 rPPG（远程光电体积描记）信号，供心率、血氧等生理参数计算使用。
 * 所有函数均采用 C 链接，可直接从 Android JNI、iOS Objective-C、Python ctypes
 * 或其他 C 兼容层调用。
 *
 * ### Quick start
 * @code
 * // 可选：将 SDK 日志接入应用日志系统
 * measurementSetLogCallback([](int level, const char* msg) {
 *     my_app_log(level, msg);
 * });
 * measurementSetLogLevel(2);   // 只输出 WARN 及以上
 *
 * // 零拷贝接口（推荐）
 * char result[65536] = {};
 * processFrameBytes(pixels, width, height, step,
 *                   landmarks, blendshapes, 1, result, sizeof(result));
 * @endcode
 */

#pragma once

// ── Version ───────────────────────────────────────────────────────────────────
#define MEASUREMENT_CLIENT_SDK_VERSION_MAJOR 2
#define MEASUREMENT_CLIENT_SDK_VERSION_MINOR 2
#define MEASUREMENT_CLIENT_SDK_VERSION_PATCH 5
#define MEASUREMENT_CLIENT_SDK_VERSION       "2.2.5"

#if defined(__GNUC__)
#  if defined(measurement_EXPORTS) || defined(MEASUREMENT_BUILDING_LIBRARY)
#    define MEASUREMENT_API __attribute__((visibility("default")))
#  else
#    define MEASUREMENT_API
#  endif
#else
#  define MEASUREMENT_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ── Logging control ───────────────────────────────────────────────────────────

/**
 * @brief 设置 SDK 最低日志输出级别。
 *
 * 低于该级别的消息将被静默丢弃。
 *
 * @param level  0=DEBUG, 1=INFO（默认）, 2=WARN, 3=ERROR, 4=NONE（完全静默）
 */
MEASUREMENT_API void measurementSetLogLevel(int level);

/**
 * @brief 将 SDK 日志重定向到调用方提供的回调函数。
 *
 * 默认输出到 @c stderr。传入 @c nullptr 可恢复默认行为。
 *
 * @param callback  签名为 `void(int level, const char* message)` 的函数指针。
 */
typedef void (*MeasurementLogCallback)(int level, const char* message);
MEASUREMENT_API void measurementSetLogCallback(MeasurementLogCallback callback);

// ── Measurement API ───────────────────────────────────────────────────────────

/**
 * @brief 处理原始像素数据的视频帧（零拷贝接口）。
 *
 * 直接包装调用方内存，不进行任何像素数据复制。
 *
 * @param imageData       图像原始 BGR 像素数据指针（不会被修改）
 * @param width           图像宽度（像素）
 * @param height          图像高度（像素）
 * @param step            图像行步长（字节数，通常为 width * 3）
 * @param faceLandmarks   人脸关键点坐标数组，格式为 [x0,y0, x1,y1, ...]（像素坐标）
 *                        MNN 模式：106 点 → 212 个浮点数
 *                        MediaPipe 模式：468 点 → 936 个浮点数
 * @param faceBlendShapes 面部表情融合权重数组（MediaPipe 模式下 52 个值，MNN 模式下忽略）
 * @param featureExtractor 特征提取器类型：0=MNN（106点），1=MediaPipe（468点）
 * @param frameResult     输出缓冲区，写入 Base64 编码的序列化 Protobuf Feature 消息
 * @param frameResultSize 输出缓冲区字节大小（建议 64 KB）
 */
MEASUREMENT_API void processFrameBytes(unsigned char* imageData,
                       int width,
                       int height,
                       int step,
                       float* faceLandmarks,
                       float* faceBlendShapes,
                       int featureExtractor,
                       char* frameResult,
                       int frameResultSize);

#ifdef __cplusplus
}
#endif
