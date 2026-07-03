//
//  XYLogConfig.h
//  MeasurementSDK
//
//  SDK 日志全局开关。
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * SDK 日志配置。
 *
 * 开启后，SDK 内部调试信息会输出到控制台（NSLog）。
 * 也可在 XYMeasurementConfig.enableLog 中按配置实例设置，二者效果一致。
 */
@interface XYLogConfig : NSObject

/// 是否启用 SDK 日志，默认 YES
@property (class, nonatomic, assign) BOOL enableLog;

@end

NS_ASSUME_NONNULL_END
