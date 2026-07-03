//
//  XYMeasurementDelegate.h
//  MeasurementSDK
//
//  测量过程回调协议。宿主实现后赋给 XYMeasurementManager.delegate。
//

#import <Foundation/Foundation.h>

@class HrReport;
@class Report;

NS_ASSUME_NONNULL_BEGIN

/**
 * 测量生命周期与结果回调（OC 公开契约）。
 *
 * 所有方法均为 @optional；未实现的方法会被 SDK 忽略。
 * 报告类型 HrReport / Report 为 Protobuf 生成类，见 Report.pbobjc.h。
 */
@protocol XYMeasurementDelegate <NSObject>
@optional

/// 测量已成功启动。`measurementId` 由服务端分配，后续报告回调会携带同一 ID。
- (void)onStartedWithMeasurementId:(NSString *)measurementId;

/// 测量状态或人脸引导提示更新。`message` 可直接展示在 UI（如「请靠近一些」）。
- (void)onStateUpdateWithCode:(NSInteger)code message:(NSString *)message;

/// 测量被中断（人脸丢失、姿态异常等）。`reason` 为可读说明，可提示用户重新测量。
- (void)onInterruptedWithReason:(NSString *)reason;

/// 视频帧采集已完成，SDK 正在等待完整健康报告。
- (void)onCollected;

/// 阶段性心率报告（partial）。可用于实时更新 UI 上的心率数值。
/// @param hrReport Protobuf 心率报告，常用字段：hrReport.data_p.hrBpm
- (void)onChunkReportGeneratedWithMeasurementId:(NSString *)measurementId
                                       hrReport:(HrReport *)hrReport;

/// 完整健康报告已生成。`report` 包含生理、情绪等全部已请求指标。
- (void)onWholeReportGeneratedWithMeasurementId:(NSString *)measurementId
                                         report:(Report *)report;

/// 测量过程发生不可恢复错误（鉴权失败、网络异常等）。
- (void)onCrashedWithCode:(NSInteger)code message:(NSString *)message;

@end

NS_ASSUME_NONNULL_END
