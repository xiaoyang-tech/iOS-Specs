//
//  XYMeasurementConfig.h
//  MeasurementSDK
//
//  SDK 初始化配置。通过 XYMeasurementManager.setConfig: 传入。
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * 测量 SDK 配置项。
 *
 * 使用流程：
 * 1. `initWithAppId:sdkKey:` 创建实例
 * 2. 设置测量指标、时长、服务地址等
 * 3. 交给 `-[XYMeasurementManager setConfig:]`
 */
@interface XYMeasurementConfig : NSObject

/// 平台分配的 App ID（鉴权必填）
@property (nonatomic, copy) NSString *appId;

/// 平台分配的 SDK Key（鉴权必填）
@property (nonatomic, copy) NSString *sdkKey;

/**
 * 目标测量时长（秒），默认 30。
 * 测量结束需同时满足：累计有效时长 ≥ 本值（与远程 min_measurement_duration 取较大值），
 * 且有效帧数 ≥ 远程 min_frames_cnt。
 */
@property (nonatomic, assign) NSInteger measurementDuration;

/**
 * 阶段性 partial 报告的最小发送间隔（秒），默认 6。
 * 远程 `min_chunk_timespan`（毫秒）优先；未配置时本值 × 1000。
 */
@property (nonatomic, assign) NSInteger minChunkTimeSpan;

/**
 * SignalR 测量服务地址（对应远程 measurement_url）。
 * 示例：https://measurement-health.xymind.cn/measurement/feature
 * App 配置优先于远程 ClientSdkConfig。
 */
@property (nonatomic, copy, nullable) NSString *measurementUrl;

/**
 * 鉴权 Token 地址（对应远程 auth_url），SDK 原样使用。
 * 示例：https://measurement-auth.xymind.cn/connect/sdk/token
 */
@property (nonatomic, copy, nullable) NSString *authUrl;

/**
 * 需要测量的指标列表，元素为 `MeasurementCategory` 的 rawValue（NSNumber）。
 * 枚举定义见 Category.pbobjc.h，常用值：
 * - MeasurementCategory_All = 1727（全部指标）
 * - MeasurementCategory_Physiology = 15（生理组合）
 * - MeasurementCategory_Emotion = 1713（情绪组合）
 */
@property (nonatomic, copy) NSArray<NSNumber *> *measurementCategoryValues;

/// 是否打印 SDK 内部日志，默认 YES。也可通过 XYLogConfig.enableLog 全局控制。
@property (nonatomic, assign) BOOL enableLog;

/**
 * 创建配置实例。
 * @param appId  平台 App ID
 * @param sdkKey 平台 SDK Key
 */
- (instancetype)initWithAppId:(NSString *)appId sdkKey:(NSString *)sdkKey;

@end

NS_ASSUME_NONNULL_END
